/**
 * UndoManager.cpp
 * 回退管理器实现
 */

#include "UndoManager.h"

USING_NS_CC;

UndoManager::UndoManager()
    : _undoModel(nullptr)
    , _gameModel(nullptr)
    , _gameView(nullptr)
{
}

UndoManager::~UndoManager()
{
    CC_SAFE_DELETE(_undoModel);
}

bool UndoManager::init(GameModel* gameModel, GameView* gameView)
{
    if (!gameModel || !gameView) {
        return false;
    }
    
    _gameModel = gameModel;
    _gameView = gameView;
    
    // 创建回退数据模型
    _undoModel = new UndoModel();
    
    return true;
}

void UndoManager::recordPlayfieldToTrayOperation(int cardId, const Vec2& fromPos, 
                                                const Vec2& toPos, int prevTrayCardId)
{
    if (!_undoModel) {
        return;
    }
    
    // 创建操作记录
    OperationRecord record;
    record.type = OT_PLAYFIELD_TO_TRAY;
    record.cardId = cardId;
    record.fromPos = fromPos;
    record.toPos = toPos;
    record.prevTrayCardId = prevTrayCardId;
    
    // 添加到回退模型
    _undoModel->addOperationRecord(record);
    
    // 更新UI状态
    if (_gameView) {
        _gameView->setUndoButtonEnabled(true);
    }
}

void UndoManager::recordStackToTrayOperation(int cardId, int prevTrayCardId)
{
    if (!_undoModel) {
        return;
    }
    
    // 创建操作记录
    OperationRecord record;
    record.type = OT_STACK_TO_TRAY;
    record.cardId = cardId;
    record.prevTrayCardId = prevTrayCardId;
    
    // 添加到回退模型
    _undoModel->addOperationRecord(record);
    
    // 更新UI状态
    if (_gameView) {
        _gameView->setUndoButtonEnabled(true);
    }
}

bool UndoManager::undo()
{
    if (!_undoModel || !_gameModel || !_gameView) {
        return false;
    }
    
    if (!_undoModel->canUndo()) {
        return false;
    }
    
    // 获取最后一条记录
    OperationRecord record = _undoModel->getLastRecord();
    
    // 根据操作类型执行不同的撤销操作
    bool result = false;
    switch (record.type) {
        case OT_PLAYFIELD_TO_TRAY:
            result = undoPlayfieldToTrayOperation(record);
            break;
        case OT_STACK_TO_TRAY:
            result = undoStackToTrayOperation(record);
            break;
        default:
            break;
    }
    
    // 如果操作成功，移除该记录
    if (result) {
        _undoModel->removeLastRecord();
    }
    
    // 如果没有更多的撤销记录，禁用撤销按钮
    if (!_undoModel->canUndo()) {
        _gameView->setUndoButtonEnabled(false);
    }
    
    return result;
}

bool UndoManager::undoPlayfieldToTrayOperation(const OperationRecord& record)
{
    // 1. 获取当前手牌区顶部卡牌
    CardModel* currentTrayCard = _gameModel->getTrayTopCard();
    if (!currentTrayCard || currentTrayCard->getCardId() != record.cardId) {
        return false;
    }
    
    // 添加锁定机制，防止动画过程中再次触发回退
    static bool isUndoing = false;
    if (isUndoing) return false;
    isUndoing = true;
    
    // 2. 创建一个新的卡牌模型放回到主牌区，确保使用原始的位置
    CardModel* newCard = new CardModel(record.cardId, currentTrayCard->getFace(), 
                                       currentTrayCard->getSuit(), record.fromPos);
    
    // 3. 添加卡牌到主牌区
    _gameModel->getPlayfieldCards().push_back(newCard);
    
    // 4. 先清除当前手牌区顶部卡牌，防止后续被重新生成
    _gameModel->setTrayTopCardDirectly(nullptr);
    if (_gameView->_trayTopCardView) {
        _gameView->_trayLayer->removeChild(_gameView->_trayTopCardView);
        _gameView->_trayTopCardView = nullptr;
    }
    
    // 5. 恢复之前的手牌区顶部卡牌（被覆盖的卡牌）
    CardModel* previousTrayCard = _gameModel->restorePreviousTrayCard();
    if (previousTrayCard) {
        // 直接设置手牌区顶部卡牌，不保存当前卡牌
        _gameModel->setTrayTopCardDirectly(previousTrayCard);
        
        // 立即更新手牌区显示（先显示原被覆盖的卡牌）
        _gameView->updateTrayTopCard(previousTrayCard);
    } else if (record.prevTrayCardId >= 0) {
        // 如果无法从栈中恢复（兼容旧版本），尝试从记录中恢复
        CardModel* prevTrayCard = nullptr;
        for (auto card : _gameModel->getStackCards()) {
            if (card->getCardId() == record.prevTrayCardId) {
                prevTrayCard = card;
                break;
            }
        }
        
        if (prevTrayCard) {
            _gameModel->setTrayTopCard(prevTrayCard);
            
            // 立即更新手牌区显示
            _gameView->updateTrayTopCard(prevTrayCard);
        }
    }
    
    // 6. 播放动画：从手牌区移回到主牌区的原始位置
    // 这里我们创建一个临时卡牌视图，从手牌区位置开始动画
    auto tempCardView = CardView::create(newCard);
    if (tempCardView) {
        // 设置初始位置为手牌区（下方）
        tempCardView->setPosition(_gameView->_trayLayer->getPosition());
        _gameView->addChild(tempCardView, 100); // 添加到顶层确保可见
        
        // 创建一个从下到上的路径
        Vec2 startPos = tempCardView->getPosition();
        Vec2 targetPos = record.fromPos;
        
        // 创建贝塞尔曲线参数
        ccBezierConfig bezier;
        bezier.controlPoint_1 = Vec2(startPos.x, startPos.y + 100); // 控制点1：稍微向上
        bezier.controlPoint_2 = Vec2(targetPos.x, targetPos.y - 50); // 控制点2：目标下方
        bezier.endPosition = targetPos; // 终点
        
        // 创建贝塞尔曲线动画
        auto bezierTo = BezierTo::create(0.3f, bezier);
        
        // 播放动画
        auto callFunc = CallFunc::create([this, newCard, tempCardView]() {
            // 动画完成后，移除临时视图，创建实际的卡牌视图
            _gameView->removeChild(tempCardView);
            
            // 在原位置创建正式的卡牌视图
            auto cardView = CardView::create(newCard);
            if (cardView) {
                cardView->setTouchEnabled(true);
                // 使用GameView保存的回调函数
                cardView->setOnClickCallback(_gameView->getCardClickCallback());
                _gameView->_playfieldLayer->addChild(cardView);
                _gameView->_playfieldCardViews[newCard->getCardId()] = cardView;
            }
            
            // 解锁
            isUndoing = false;
        });
        
        auto sequence = Sequence::create(bezierTo, callFunc, nullptr);
        tempCardView->runAction(sequence);
    }
    
    return true;
}

bool UndoManager::undoStackToTrayOperation(const OperationRecord& record)
{
    // 1. 获取当前手牌区顶部卡牌
    CardModel* currentTrayCard = _gameModel->getTrayTopCard();
    if (!currentTrayCard || currentTrayCard->getCardId() != record.cardId) {
        return false;
    }
    
    // 添加锁定机制，防止动画过程中再次触发回退
    static bool isUndoing = false;
    if (isUndoing) return false;
    isUndoing = true;
    
    // 2. 将当前手牌区顶部卡牌放回备用牌堆
    _gameModel->getStackCards().push_back(currentTrayCard);
    
    // 3. 先清除当前手牌区顶部卡牌，防止后续被重新生成
    _gameModel->setTrayTopCardDirectly(nullptr);
    if (_gameView->_trayTopCardView) {
        _gameView->_trayLayer->removeChild(_gameView->_trayTopCardView);
        _gameView->_trayTopCardView = nullptr;
    }
    
    // 4. 恢复之前的手牌区顶部卡牌（被覆盖的卡牌）
    CardModel* previousTrayCard = _gameModel->restorePreviousTrayCard();
    if (previousTrayCard) {
        // 直接设置手牌区顶部卡牌，不保存当前卡牌
        _gameModel->setTrayTopCardDirectly(previousTrayCard);
        
        // 立即更新手牌区显示（先显示原被覆盖的卡牌）
        _gameView->updateTrayTopCard(previousTrayCard);
    } else if (record.prevTrayCardId >= 0) {
        // 如果无法从栈中恢复（兼容旧版本），尝试从记录中恢复
        CardModel* prevTrayCard = nullptr;
        for (auto card : _gameModel->getStackCards()) {
            if (card->getCardId() == record.prevTrayCardId) {
                prevTrayCard = card;
                break;
            }
        }
        
        if (prevTrayCard) {
            // 从备用牌堆移除
            auto it = std::find(_gameModel->getStackCards().begin(), 
                               _gameModel->getStackCards().end(), prevTrayCard);
            if (it != _gameModel->getStackCards().end()) {
                _gameModel->getStackCards().erase(it);
            }
            
            _gameModel->setTrayTopCardDirectly(prevTrayCard);
            _gameView->updateTrayTopCard(prevTrayCard);
        }
    } else {
        // 没有之前的手牌区顶部卡牌，设置为nullptr
        _gameModel->setTrayTopCardDirectly(nullptr);
        // 清空手牌区显示
        if (_gameView->_trayTopCardView) {
            _gameView->_trayLayer->removeChild(_gameView->_trayTopCardView);
            _gameView->_trayTopCardView = nullptr;
        }
    }
    
    // 5. 播放动画：从手牌区移回到备用牌堆
    // 创建一个临时卡牌视图，从手牌区位置开始动画
    auto tempCardView = CardView::create(currentTrayCard);
    if (tempCardView) {
        // 设置初始位置为手牌区
        tempCardView->setPosition(_gameView->_trayLayer->getPosition());
        _gameView->addChild(tempCardView, 100); // 添加到顶层确保可见
        
        // 获取备用牌堆位置
        Vec2 stackPosition = _gameView->_stackLayer->getPosition();
        
        // 播放移动动画回到备用牌堆
        tempCardView->playMoveAnimation(stackPosition, 0.3f, [this, tempCardView]() {
            // 动画完成后，移除临时视图
            _gameView->removeChild(tempCardView);
            
            // 更新备用牌堆显示
            auto countLabel = static_cast<Label*>(_gameView->_stackNode->getChildByName("count_label"));
            if (countLabel) {
                countLabel->setString(StringUtils::format("%d", static_cast<int>(_gameModel->getStackCards().size())));
            }
            
            // 使用现有方法更新游戏状态
            _gameView->setPlayfieldCardsInteractive(true);
            
            // 动画完成，解除锁定
            isUndoing = false;
        });
    }
    
    return true;
}

bool UndoManager::canUndo() const
{
    return _undoModel && _undoModel->canUndo();
}

void UndoManager::clearAllUndoRecords()
{
    if (_undoModel) {
        _undoModel->clearAllRecords();
    }
    
    if (_gameView) {
        _gameView->setUndoButtonEnabled(false);
    }
}
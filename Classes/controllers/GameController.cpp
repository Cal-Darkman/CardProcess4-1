/**
 * GameController.cpp
 * 游戏控制器实现
 */

#include "GameController.h"
#include "../configs/loaders/LevelConfigLoader.h"
#include "../services/GameModelFromLevelGenerator.h"

USING_NS_CC;

GameController::GameController()
    : _gameModel(nullptr)
    , _gameView(nullptr)
    , _undoManager(nullptr)
{
}

GameController::~GameController()
{
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_undoManager);
    // _gameView是autorelease对象，不需要手动删除
}

bool GameController::init(int levelId, Node* parent)
{
    // 初始化游戏数据模型
    if (!initGameModel(levelId)) {
        return false;
    }
    
    // 初始化游戏视图
    if (!initGameView(parent)) {
        return false;
    }
    
    // 初始化回退管理器
    if (!initUndoManager()) {
        return false;
    }
    
    // 初始化事件处理
    initEventHandlers();
    
    return true;
}

bool GameController::initGameModel(int levelId)
{
    // 加载关卡配置
    auto levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (!levelConfig) {
        return false;
    }
    
    // 生成游戏模型
    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    
    // 释放关卡配置
    CC_SAFE_DELETE(levelConfig);
    
    return _gameModel != nullptr;
}

bool GameController::initGameView(Node* parent)
{
    if (!_gameModel || !parent) {
        return false;
    }
    
    // 创建游戏视图
    _gameView = GameView::create(_gameModel);
    if (!_gameView) {
        return false;
    }
    
    // 设置游戏视图的控制器引用
    _gameView->setGameController(this);
    
    // 添加到父节点
    parent->addChild(_gameView);
    
    return true;
}

bool GameController::initUndoManager()
{
    if (!_gameModel || !_gameView) {
        return false;
    }
    
    // 创建回退管理器
    _undoManager = new UndoManager();
    if (!_undoManager) {
        return false;
    }
    
    // 初始化回退管理器
    return _undoManager->init(_gameModel, _gameView);
}

void GameController::initEventHandlers()
{
    if (!_gameView) {
        return;
    }
    
    // 设置主牌区卡牌点击回调
    _gameView->setOnPlayfieldCardClickCallback(CC_CALLBACK_1(GameController::handlePlayfieldCardClick, this));
    
    // 设置备用牌堆点击回调
    _gameView->setOnStackClickCallback(CC_CALLBACK_0(GameController::handleStackClick, this));
    
    // 设置回退按钮点击回调
    _gameView->setOnUndoClickCallback(CC_CALLBACK_0(GameController::handleUndoClick, this));
}

bool GameController::handlePlayfieldCardClick(int cardId)
{
    if (!_gameModel || !_gameView || !_undoManager) {
        return false;
    }
    
    // 获取卡牌模型
    CardModel* card = _gameModel->getPlayfieldCardById(cardId);
    if (!card) {
        return false;
    }
    
    // 获取当前手牌区顶部卡牌
    CardModel* trayTopCard = _gameModel->getTrayTopCard();
    int prevTrayCardId = trayTopCard ? trayTopCard->getCardId() : -1;
    
    // 检查是否可以移动卡牌
    if (trayTopCard && !card->canMatch(trayTopCard)) {
        return false;
    }
    
    // 记录操作
    _undoManager->recordPlayfieldToTrayOperation(cardId, card->getPosition(), 
                                                Vec2::ZERO, prevTrayCardId);
    
    // 判断是否是匹配的牌（差值为1的牌）
    bool isMatchingCard = trayTopCard && card->canMatch(trayTopCard);
    
    if (isMatchingCard) {
        // 如果是匹配的牌，使用直接覆盖动画
        _gameView->playDirectCoverAnimation(cardId);
    } else {
        // 否则使用普通的移动到手牌区动画
        _gameView->playCardMoveToTrayAnimation(cardId);
    }
    
    // 更新游戏模型
    _gameModel->moveCardFromPlayfieldToTray(cardId);
    
    // 检查游戏结束
    checkGameOver();
    
    return true;
}

bool GameController::handleStackClick()
{
    if (!_gameModel || !_gameView || !_undoManager) {
        return false;
    }
    
    // 获取当前手牌区顶部卡牌
    CardModel* trayTopCard = _gameModel->getTrayTopCard();
    int prevTrayCardId = trayTopCard ? trayTopCard->getCardId() : -1;
    
    // 从备用牌堆抽一张牌
    if (!_gameModel->drawCardFromStack()) {
        return false;
    }
    
    // 获取新的手牌区顶部卡牌
    CardModel* newTrayTopCard = _gameModel->getTrayTopCard();
    if (!newTrayTopCard) {
        return false;
    }
    
    // 记录操作
    _undoManager->recordStackToTrayOperation(newTrayTopCard->getCardId(), prevTrayCardId);
    
    // 播放动画
    _gameView->playStackToTrayAnimation();
    
    // 检查游戏结束
    checkGameOver();
    
    return true;
}

bool GameController::handleUndoClick()
{
    if (!_undoManager) {
        return false;
    }
    
    // 执行撤销操作
    return _undoManager->undo();
}

void GameController::checkGameOver()
{
    if (!_gameModel || !_gameView) {
        return;
    }
    
    // 检查游戏是否结束
    if (_gameModel->isGameOver()) {
        // 游戏结束，显示结果
        if (_gameModel->isGameWon()) {
            // 赢了
            auto winLabel = Label::createWithTTF("You Win!", "fonts/Marker Felt.ttf", 80);
            winLabel->setPosition(Vec2(_gameView->getContentSize().width/2, _gameView->getContentSize().height/2));
            _gameView->addChild(winLabel, 100);
        } else {
            // 输了
            auto loseLabel = Label::createWithTTF("Game Over!", "fonts/Marker Felt.ttf", 80);
            loseLabel->setPosition(Vec2(_gameView->getContentSize().width/2, _gameView->getContentSize().height/2));
            _gameView->addChild(loseLabel, 100);
        }
        
        // 禁用所有交互
        _gameView->setPlayfieldCardsInteractive(false);
        _gameView->setStackInteractive(false);
        _gameView->setUndoButtonEnabled(false);
    }
}

void GameController::resetGameState()
{
    // 注意：删除了对不存在的updateValidMoves方法的调用
    
    // 更新游戏视图的交互状态
    _gameView->setPlayfieldCardsInteractive(true);
    
    // 更新牌堆交互状态
    if (_gameModel->getStackCards().size() > 0) {
        _gameView->setStackInteractive(true);
    } else {
        _gameView->setStackInteractive(false);
    }
    
    // 检查游戏结束条件
    checkGameOver();
}
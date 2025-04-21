/**
 * GameView.cpp
 * 游戏视图实现
 */

#include "GameView.h"
#include "ui/CocosGUI.h"
#include "../controllers/GameController.h"

USING_NS_CC;
using namespace cocos2d::ui;

GameView* GameView::create(const GameModel* model)
{
    GameView* view = new (std::nothrow) GameView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool GameView::init(const GameModel* model)
{
    if (!Node::init()) {
        return false;
    }
    
    _model = model;
    _trayTopCardView = nullptr;
    _gameController = nullptr;
    
    // 初始化游戏区域
    initGameAreas();
    
    // 初始化UI控件
    initUI();
    
    // 初始化主牌区卡牌
    initPlayfieldCards();
    
    // 初始化手牌区
    initTray();
    
    // 初始化备用牌堆
    initStack();
    
    return true;
}

void GameView::initGameAreas()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 设定游戏区域尺寸
    Size deskSize(1080, 1500); // 主牌区尺寸
    Size handSize(1080, 580);  // 堆牌区尺寸
    
    // 计算垂直偏移，确保游戏区域在屏幕中央
    float verticalOffset = (visibleSize.height - (deskSize.height + handSize.height)) / 2;
    if (verticalOffset < 0) verticalOffset = 0;
    
    // 计算水平偏移，确保游戏区域在屏幕中央
    float horizontalOffset = (visibleSize.width - deskSize.width) / 2;
    if (horizontalOffset < 0) horizontalOffset = 0;
    
    // 创建主牌区层 - 放在上方区域
    _playfieldLayer = Node::create();
    _playfieldLayer->setContentSize(deskSize);
    _playfieldLayer->setPosition(Vec2(origin.x + horizontalOffset, origin.y + verticalOffset + handSize.height));
    
    // 添加主牌区背景
    auto deskBg = Sprite::create("res/desk_scene.png");
    if (deskBg) {
        deskBg->setPosition(Vec2(deskSize.width/2, deskSize.height/2));
        deskBg->setScale(
            deskSize.width / deskBg->getContentSize().width,
            deskSize.height / deskBg->getContentSize().height
        );
        _playfieldLayer->addChild(deskBg, -1); // 背景层级为-1
    } else {
        // 如果图片加载失败，创建一个纯色背景
        auto colorBg = LayerColor::create(Color4B(87, 49, 105, 255), deskSize.width, deskSize.height); // 紫色背景
        _playfieldLayer->addChild(colorBg, -1);
    }
    
    this->addChild(_playfieldLayer);
    
    // 创建手牌区层 - 放在下方区域
    _trayLayer = Node::create();
    _trayLayer->setContentSize(Size(handSize.width, handSize.height));
    _trayLayer->setPosition(Vec2(origin.x + horizontalOffset + handSize.width/2, 
                                origin.y + verticalOffset + handSize.height/2));
    
    // 添加手牌区背景
    auto handBg = Sprite::create("res/hand_scene.png");
    if (handBg) {
        handBg->setPosition(Vec2(0, 0)); // 相对于_trayLayer的位置
        handBg->setScale(
            handSize.width / handBg->getContentSize().width,
            handSize.height / handBg->getContentSize().height
        );
        _trayLayer->addChild(handBg, -1); // 背景层级为-1
    } else {
        // 如果图片加载失败，创建一个纯色背景
        auto colorBg = LayerColor::create(Color4B(159, 110, 74, 255), handSize.width, handSize.height); // 棕色背景
        colorBg->setPosition(Vec2(-handSize.width/2, -handSize.height/2)); // LayerColor需要调整位置
        _trayLayer->addChild(colorBg, -1);
    }
    
    this->addChild(_trayLayer);
    
    // 创建备用牌堆层 - 放在左下区域
    _stackLayer = Node::create();
    _stackLayer->setPosition(Vec2(200, 290)); // 放在左下区域
    this->addChild(_stackLayer);
}

void GameView::initPlayfieldCards()
{
    if (!_model) return;
    
    // 创建主牌区卡牌视图
    for (const auto& card : _model->getPlayfieldCards()) {
        auto cardView = CardView::create(card);
        if (cardView) {
            cardView->setTouchEnabled(true);
            _playfieldLayer->addChild(cardView);
            _playfieldCardViews[card->getCardId()] = cardView;
        }
    }
}

void GameView::initTray()
{
    if (!_model) return;
    
    // 初始化手牌区顶部卡牌
    const CardModel* trayTopCard = _model->getTrayTopCard();
    if (trayTopCard) {
        updateTrayTopCard(trayTopCard);
    }
}

void GameView::initStack()
{
    if (!_model) return;
    
    // 创建备用牌堆UI
    _stackNode = Node::create();
    
    // 添加堆牌背景
    auto stackBg = Sprite::create("res/card_general.png");
    if (!stackBg) {
        // 如果找不到图片，创建一个空精灵
        stackBg = Sprite::create();
        stackBg->setTextureRect(Rect(0, 0, 182, 282));
        stackBg->setColor(Color3B(50, 50, 50));
    }
    
    // 调整大小以匹配卡牌尺寸
    Size cardSize = Size(182, 282);  // 修改卡牌尺寸为 182x282
    stackBg->setScale(cardSize.width / stackBg->getContentSize().width,
                      cardSize.height / stackBg->getContentSize().height);
    _stackNode->addChild(stackBg);
    
    // 添加卡牌数量标签
    auto countLabel = Label::createWithTTF(
        StringUtils::format("%d", static_cast<int>(_model->getStackCards().size())),
        "fonts/Marker Felt.ttf", 30);
    countLabel->setPosition(Vec2(cardSize.width/2, cardSize.height/2));
    _stackNode->setContentSize(cardSize);
    _stackNode->addChild(countLabel, 1, "count_label");
    
    _stackLayer->addChild(_stackNode);
    
    // 添加触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        Vec2 locationInNode = _stackNode->convertToNodeSpace(touch->getLocation());
        Size s = _stackNode->getContentSize();
        Rect rect = Rect(-s.width/2, -s.height/2, s.width, s.height);
        
        if (rect.containsPoint(locationInNode)) {
            _stackNode->setScale(0.95f);
            return true;
        }
        return false;
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        _stackNode->setScale(1.0f);
        
        Vec2 locationInNode = _stackNode->convertToNodeSpace(touch->getLocation());
        Size s = _stackNode->getContentSize();
        Rect rect = Rect(-s.width/2, -s.height/2, s.width, s.height);
        
        if (rect.containsPoint(locationInNode)) {
            // 触发备用牌堆点击回调
            if (_model->getStackCards().size() > 0) {
                auto callback = std::bind(&GameController::handleStackClick, _gameController);
                callback();
            }
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event) {
        _stackNode->setScale(1.0f);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _stackNode);
}

void GameView::initUI()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建回退按钮
    _undoButton = Button::create();
    _undoButton->setTitleText("回退");
    _undoButton->setTitleFontSize(70);
    _undoButton->setTitleColor(Color3B::WHITE);
    
    // 设置按钮背景
    // auto btnBg = Sprite::create("res/card_general.png");
    // if (btnBg) {
    //     btnBg->setScale(0.3f, 0.15f);
    //     btnBg->setColor(Color3B(70, 130, 180)); // 钢蓝色
    //     _undoButton->addChild(btnBg, -10, 999);  // 使用999作为标签而不是-1
    // }
    
    // 设置按钮位置，放在屏幕右下角
    _undoButton->setPosition(Vec2(origin.x + visibleSize.width-200 , 290));
    this->addChild(_undoButton);
    
    // 默认禁用回退按钮
    _undoButton->setEnabled(false);
    _undoButton->setOpacity(150); // 禁用状态设置半透明
}

void GameView::setOnPlayfieldCardClickCallback(const std::function<void(int)>& callback)
{
    // 保存回调函数
    _cardClickCallback = callback;
    
    // 设置主牌区卡牌点击回调
    for (auto& pair : _playfieldCardViews) {
        pair.second->setOnClickCallback(callback);
    }
}

std::function<void(int)> GameView::getCardClickCallback() const
{
    return _cardClickCallback;
}

void GameView::setOnStackClickCallback(const std::function<void()>& callback)
{
    // 备用牌堆点击回调在initStack中已经设置
}

void GameView::setOnUndoClickCallback(const std::function<void()>& callback)
{
    // 直接设置触摸事件监听器，Cocos2d-x会自动覆盖之前的监听器
    _undoButton->addTouchEventListener([callback, this](Ref* sender, Widget::TouchEventType type) {
        auto btn = static_cast<Button*>(sender);
        switch (type) {
            case Widget::TouchEventType::BEGAN:
                btn->setScale(0.95f);
                break;
            case Widget::TouchEventType::ENDED:
                btn->setScale(1.0f);
                // 触发回退回调
                if (callback) {
                    callback();
                }
                break;
            case Widget::TouchEventType::CANCELED:
                btn->setScale(1.0f);
                break;
            default:
                break;
        }
    });
}

void GameView::updateTrayTopCard(const CardModel* card)
{
    // 总是先清除现有的卡牌视图
    // if (_trayTopCardView) {
    //     _trayLayer->removeChild(_trayTopCardView);
    //     _trayTopCardView = nullptr;
    // }
    
    // 如果没有新卡牌，就直接返回
    if (!card) return;
    
    // 创建新的卡牌视图
    _trayTopCardView = CardView::create(card);
    if (_trayTopCardView) {
        _trayLayer->addChild(_trayTopCardView);
    }
}

void GameView::playCardMoveToTrayAnimation(int cardId)
{
    auto it = _playfieldCardViews.find(cardId);
    if (it == _playfieldCardViews.end() || !_trayLayer) {
        return;
    }
    
    CardView* cardView = it->second;
    cardView->setTouchEnabled(false);
    
    // 计算目标位置 - 手牌区位置（在下方）
    Vec2 targetPos = _trayLayer->getPosition();
    
    // 播放从主牌区（上方）向手牌区（下方）的移动动画
    cardView->playMoveAnimation(targetPos, 0.3f, [this, cardId, cardView]() {
        // 动画完成后，移除原卡牌视图并更新手牌区
        _playfieldLayer->removeChild(cardView, false);
        _playfieldCardViews.erase(cardId);
        setTrayTopCardView(cardView);
    });
}

// 直接覆盖手牌的动画
void GameView::playDirectCoverAnimation(int cardId)
{
    auto it = _playfieldCardViews.find(cardId);
    if (it == _playfieldCardViews.end() || !_trayLayer || !_trayTopCardView) {
        return;
    }
    
    CardView* cardView = it->second;
    cardView->setTouchEnabled(false);
    
    // 获取手牌区顶部卡牌的位置（在下方）
    Vec2 targetPos = _trayLayer->getPosition();
    
    // 使用从上到下的动画方法
    cardView->playMoveDownAnimation(targetPos, 0.3f, [this, cardId, cardView]() {
        // 动画完成后，移除原卡牌视图并更新手牌区
        _playfieldLayer->removeChild(cardView, false);
        _playfieldCardViews.erase(cardId);
        setTrayTopCardView(cardView);
    });
}

void GameView::playStackToTrayAnimation()
{
    if (!_model || !_trayLayer) {
        return;
    }
    
    // 在备用牌堆位置创建一个临时卡牌视图
    const CardModel* trayTopCard = _model->getTrayTopCard();
    if (!trayTopCard) {
        return;
    }
    
    auto tempCardView = CardView::create(trayTopCard);
    if (!tempCardView) {
        return;
    }
    
    tempCardView->setPosition(_stackLayer->getPosition());
    this->addChild(tempCardView);
    
    // 计算目标位置
    Vec2 targetPos = _trayLayer->getPosition();
    
    // 播放移动动画
    tempCardView->playMoveAnimation(targetPos, 0.3f, [this, tempCardView]() {
        // 动画完成后，移除临时卡牌视图并更新手牌区
        this->removeChild(tempCardView, false);
        updateTrayTopCard(_model->getTrayTopCard());
        
        // 更新备用牌堆显示
        auto countLabel = static_cast<Label*>(_stackNode->getChildByName("count_label"));
        if (countLabel) {
            countLabel->setString(StringUtils::format("%d", static_cast<int>(_model->getStackCards().size())));
        }
    });
}

void GameView::playTrayToPositionAnimation(const Vec2& targetPos, const std::function<void()>& callback)
{
    if (!_trayTopCardView) {
        return;
    }
    
    // 播放手牌区卡牌移动动画
    _trayTopCardView->playMoveAnimation(targetPos, 0.3f, callback);
}

void GameView::removePlayfieldCard(int cardId)
{
    auto it = _playfieldCardViews.find(cardId);
    if (it != _playfieldCardViews.end()) {
        _playfieldLayer->removeChild(it->second);
        _playfieldCardViews.erase(it);
    }
}

void GameView::setTrayTopCardView(CardView* cardView)
{
    // 移除旧的卡牌视图
    // if (_trayTopCardView) {
    //     _trayLayer->removeChild(_trayTopCardView);
    // }
    
    _trayTopCardView = cardView;
    
    if (_trayTopCardView) {
        _trayTopCardView->setPosition(Vec2::ZERO);
        _trayLayer->addChild(_trayTopCardView);
    }
}

void GameView::setPlayfieldCardsInteractive(bool enabled)
{
    for (auto& pair : _playfieldCardViews) {
        pair.second->setTouchEnabled(enabled);
    }
}

void GameView::setStackInteractive(bool enabled)
{
    _stackNode->setVisible(enabled);
}

void GameView::setUndoButtonEnabled(bool enabled)
{
    _undoButton->setEnabled(enabled);
    
    // 添加更清晰的视觉反馈
    if (enabled) {
        _undoButton->setOpacity(255); // 启用状态设置完全不透明
        _undoButton->setTitleColor(Color3B::WHITE);
        
        // 找到按钮背景并设置颜色
        auto btnBg = _undoButton->getChildByTag(999);
        if (btnBg) {
            btnBg->setColor(Color3B(70, 130, 180)); // 钢蓝色
        }
    } else {
        _undoButton->setOpacity(150); // 禁用状态设置半透明
        _undoButton->setTitleColor(Color3B(200, 200, 200)); // 灰色文字
        
        // 找到按钮背景并设置颜色
        auto btnBg = _undoButton->getChildByTag(999);
        if (btnBg) {
            btnBg->setColor(Color3B(100, 100, 100)); // 灰色背景
        }
    }
}
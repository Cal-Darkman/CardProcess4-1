/**
 * CardView.cpp
 * 卡牌视图实现
 */

#include "CardView.h"
#include "../configs/models/CardResConfig.h"

USING_NS_CC;

CardView* CardView::create(const CardModel* model)
{
    CardView* view = new (std::nothrow) CardView();
    if (view && view->init(model)) {
        view->autorelease();
        return view;
    }
    CC_SAFE_DELETE(view);
    return nullptr;
}

bool CardView::init(const CardModel* model)
{
    if (!Node::init()) {
        return false;
    }
    
    _model = model;
    _touchEnabled = false;
    
    if (!_model) {
        return false;
    }
    
    // 设置卡牌尺寸
    Size cardSize = Size(182, 282);  // 修改卡牌尺寸为 182x282
    this->setContentSize(cardSize);
    
    // 创建卡牌基础精灵
    std::string cardImagePath = CardResConfig::getCardFaceImagePath(_model->getFace(), _model->getSuit());
    _cardSprite = Sprite::create(cardImagePath);
    if (!_cardSprite) {
        // //CCLOG("Failed to load card base image: %s", cardImagePath.c_str());
        return false;
    }
    
    // 调整基础卡牌大小以适应节点
    _cardSprite->setScale(cardSize.width / _cardSprite->getContentSize().width,
                          cardSize.height / _cardSprite->getContentSize().height);
    //_cardSprite->setPosition(Vec2(cardSize.width/2, -cardSize.height/2));
    _cardSprite->setPosition(Vec2(0,0));
    this->addChild(_cardSprite);
    
    // 判断卡牌是否为红色
    bool isRed = (_model->getSuit() == CST_HEARTS || _model->getSuit() == CST_DIAMONDS);
    
    // 添加花色精灵
    std::string suitImagePath = CardResConfig::getCardSuitImagePath(_model->getSuit());
    auto suitSprite = Sprite::create(suitImagePath);
    if (suitSprite) {
        // 调整花色位置和大小
        suitSprite->setScale(1.0f);
        suitSprite->setPosition(Vec2(cardSize.width*0.8f-cardSize.width/2, cardSize.height*0.85f-cardSize.height/2));
        this->addChild(suitSprite);
    } else {
        // //CCLOG("Failed to load suit image: %s", suitImagePath.c_str());
    }
    
    // 添加大数字精灵在中央
    std::string bigNumberImagePath = CardResConfig::getCardNumberImagePath(_model->getFace(), isRed, false);
    auto bigNumberSprite = Sprite::create(bigNumberImagePath);
    if (bigNumberSprite) {
        // 调整数字位置和大小
        bigNumberSprite->setScale(1.0f);
        bigNumberSprite->setPosition(Vec2(cardSize.width*0.5f-cardSize.width/2, cardSize.height*0.5f-cardSize.height/2));
        this->addChild(bigNumberSprite);
    } else {
        // //CCLOG("Failed to load big number image: %s", bigNumberImagePath.c_str());
    }
    
    // 添加小数字精灵在左上角和右下角
    std::string smallNumberImagePath = CardResConfig::getCardNumberImagePath(_model->getFace(), isRed, true);
    auto smallNumberTopSprite = Sprite::create(smallNumberImagePath);
    if (smallNumberTopSprite) {
        // 左上角小数字
        smallNumberTopSprite->setScale(1.0f);
        smallNumberTopSprite->setPosition(Vec2(cardSize.width*0.2f-cardSize.width/2, cardSize.height*0.85f-cardSize.height/2));
        this->addChild(smallNumberTopSprite);
        
        // 右下角小数字（翻转）
        // auto smallNumberBottomSprite = Sprite::create(smallNumberImagePath);
        // smallNumberBottomSprite->setScale(0.3f);
        // smallNumberBottomSprite->setRotation(180);
        // smallNumberBottomSprite->setPosition(Vec2(cardSize.width*0.85f, cardSize.height*0.15f));
        // this->addChild(smallNumberBottomSprite);
    } else {
        // //CCLOG("Failed to load small number image: %s", smallNumberImagePath.c_str());
    }
    
    // 设置卡牌位置
    this->setPosition(_model->getPosition());
    
    // 设置触摸事件
    setupTouchEvents();
    
    return true;
}

int CardView::getCardId() const
{
    return _model ? _model->getCardId() : -1;
}

void CardView::playMoveAnimation(const Vec2& targetPos, float duration, const std::function<void()>& callback)
{
    // 创建移动动画
    auto moveTo = MoveTo::create(duration, targetPos);
    
    if (callback) {
        // 如果有回调，添加回调动作
        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(moveTo, callFunc, nullptr);
        this->runAction(sequence);
    } else {
        this->runAction(moveTo);
    }
}

void CardView::playMoveDownAnimation(const Vec2& targetPos, float duration, const std::function<void()>& callback)
{
    // 创建一个路径，确保卡牌是从上到下移动
    // 获取起始和目标位置
    Vec2 startPos = this->getPosition();
    
    // 创建一个从上到下的路径
    ccBezierConfig bezier;
    bezier.controlPoint_1 = Vec2(startPos.x, startPos.y - 50); // 控制点1：稍微向下
    bezier.controlPoint_2 = Vec2(targetPos.x, targetPos.y + 100); // 控制点2：目标上方
    bezier.endPosition = targetPos; // 终点
    
    // 创建贝塞尔曲线动画
    auto bezierTo = BezierTo::create(duration, bezier);
    
    if (callback) {
        // 如果有回调，添加回调动作
        auto callFunc = CallFunc::create(callback);
        auto sequence = Sequence::create(bezierTo, callFunc, nullptr);
        this->runAction(sequence);
    } else {
        this->runAction(bezierTo);
    }
}

void CardView::setTouchEnabled(bool enabled)
{
    _touchEnabled = enabled;
}

void CardView::setOnClickCallback(const std::function<void(int)>& callback)
{
    _onClickCallback = callback;
}

void CardView::setupTouchEvents()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = [this](Touch* touch, Event* event) -> bool {
        if (!_touchEnabled) {
            return false;
        }
        
        // 检查触摸点是否在卡牌上
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size s = this->getContentSize();
        //Rect rect = Rect(0, 0, s.width, s.height);
        Rect rect = Rect(-s.width/2, -s.height/2, s.width, s.height);
        
        if (rect.containsPoint(locationInNode)) {
            // 卡牌被点击，可以添加点击效果
            this->setScale(0.95f);
            return true;
        }
        return false;
    };
    
    listener->onTouchEnded = [this](Touch* touch, Event* event) {
        // 恢复卡牌大小
        this->setScale(1.0f);
        
        // 检查触摸释放是否在卡牌上
        Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
        Size s = this->getContentSize();
        //Rect rect = Rect(0, 0, s.width, s.height);
        Rect rect = Rect(-s.width/2, -s.height/2, s.width, s.height);
        
        if (rect.containsPoint(locationInNode) && _onClickCallback) {
            // 触发点击回调
            _onClickCallback(_model->getCardId());
        }
    };
    
    listener->onTouchCancelled = [this](Touch* touch, Event* event) {
        // 恢复卡牌大小
        this->setScale(1.0f);
    };
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
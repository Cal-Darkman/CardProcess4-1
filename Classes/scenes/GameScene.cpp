/**
 * GameScene.cpp
 * 游戏场景实现
 */

#include "GameScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace cocos2d::ui;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" method you need to initialize your instance
bool GameScene::init()
{
    // 1. super init first
    if (!Scene::init()) {
        return false;
    }
    
    // 初始化背景
    initBackground();
    
    // 初始化游戏控制器
    if (!initGameController()) {
        return false;
    }
    
    return true;
}

void GameScene::initBackground()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // 创建背景
    auto bg = Sprite::create("res/card_general.png");
    if (!bg) {
        // 如果找不到背景图片，创建一个纯色背景
        bg = Sprite::create();
        bg->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
        bg->setColor(Color3B(0, 100, 0));  // 深绿色背景
    } else {
        // 调整尺寸以填充屏幕
        bg->setScale(visibleSize.width / bg->getContentSize().width * 1.05f, 
                     visibleSize.height / bg->getContentSize().height * 1.05f);
        bg->setColor(Color3B(0, 100, 0));  // 给基础卡牌添加绿色
        bg->setOpacity(100);  // 降低透明度
    }
    
    bg->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
    this->addChild(bg, 0);
    
    // 创建顶部标题
    auto titleLabel = Label::createWithTTF("Card Game", "fonts/Marker Felt.ttf", 36);
    titleLabel->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height - 50 + origin.y));
    this->addChild(titleLabel, 1);
    
    // 创建返回按钮 - 使用通用卡牌图片创建一个简单按钮
    auto backButton = Button::create();
    backButton->setTitleText("Back");
    backButton->setTitleFontSize(30);
    
    backButton->setPosition(Vec2(origin.x + 100, visibleSize.height - 50 + origin.y));
    backButton->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
        if (type == Widget::TouchEventType::ENDED) {
            // 返回主菜单或上一个场景
            Director::getInstance()->popScene();
        }
    });
    this->addChild(backButton, 1);
}

bool GameScene::initGameController()
{
    // //CCLOG("Starting to initialize game controller");
    
    // 直接创建 GameController 实例，而不是使用 create 方法
    _gameController = new GameController();
    if (!_gameController) {
        // //CCLOG("Failed to create GameController instance");
        return false;
    }
    
    // 传递参数给 init 方法：levelId 和 parent 节点
    if (!_gameController->init(1, this)) {  // 使用关卡 ID 1 和当前场景作为父节点
        // //CCLOG("Failed to initialize GameController");
        CC_SAFE_DELETE(_gameController);
        return false;
    }
    
    // //CCLOG("Successfully initialized game controller");
    
    return true;
}
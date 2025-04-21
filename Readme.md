# Cocos2d-x 卡牌游戏项目结构说明文档

## 目录结构

```
Classes/
    ├── AppDelegate.cpp/h                    // 应用程序入口
    ├── configs/                             // 配置相关
    │   ├── loaders/
    │   │   └── LevelConfigLoader.cpp/h      // 关卡配置加载器
    │   ├── models/
    │   │   ├── CardResConfig.cpp/h          // 卡牌资源配置
    │   │   └── LevelConfig.cpp/h            // 关卡配置数据结构
    ├── controllers/
    │   └── GameController.cpp/h             // 游戏控制器
    ├── managers/
    │   └── UndoManager.cpp/h                // 回退操作管理器
    ├── models/                              // 数据模型
    │   ├── CardModel.cpp/h                  // 卡牌数据模型
    │   ├── GameModel.cpp/h                  // 游戏数据模型
    │   └── UndoModel.cpp/h                  // 回退数据模型
    ├── scenes/
    │   └── GameScene.cpp/h                  // 游戏场景
    ├── services/
    │   └── GameModelFromLevelGenerator.cpp/h // 游戏模型生成器
    └── views/                               // 视图层
        ├── CardView.cpp/h                   // 卡牌视图
        └── GameView.cpp/h                   // 游戏视图
```

## 功能模块说明

### 1. 应用程序入口

#### AppDelegate (应用程序委托)

| 方法 | 描述 |
|------|------|
| `AppDelegate()` | 构造函数 |
| `~AppDelegate()` | 析构函数 |
| `initGLContextAttrs()` | 初始化OpenGL上下文属性 |
| `applicationDidFinishLaunching()` | 应用程序启动完成后调用 |
| `applicationDidEnterBackground()` | 应用程序进入后台时调用 |
| `applicationWillEnterForeground()` | 应用程序将要进入前台时调用 |

### 2. 配置模块

#### LevelConfigLoader (关卡配置加载器)

| 方法 | 描述 |
|------|------|
| `loadLevelConfig(int levelId)` | 加载指定ID的关卡配置 |
| `parseFromJson(const std::string& jsonString)` | 从JSON字符串解析关卡配置 |

#### LevelConfig (关卡配置)

| 方法 | 描述 |
|------|------|
| `LevelConfig()` | 构造函数 |
| `getPlayfieldCards()` | 获取主牌区卡牌配置 |
| `setPlayfieldCards(const std::vector<CardConfig>& cards)` | 设置主牌区卡牌配置 |
| `getStackCards()` | 获取备用牌堆卡牌配置 |
| `setStackCards(const std::vector<CardConfig>& cards)` | 设置备用牌堆卡牌配置 |

#### CardResConfig (卡牌资源配置)

| 方法 | 描述 |
|------|------|
| `getCardFaceImagePath(CardFaceType face, CardSuitType suit)` | 获取卡牌正面图片路径 |
| `getCardBackImagePath()` | 获取卡牌背面图片路径 |
| `getCardFaceString(CardFaceType face)` | 获取卡牌面值字符串 |
| `getCardSuitString(CardSuitType suit)` | 获取卡牌花色字符串 |
| `getCardSuitImagePath(CardSuitType suit)` | 获取花色图片路径 |
| `getCardNumberImagePath(CardFaceType face, bool isRed, bool isSmall)` | 获取数字图片路径 |

### 3. 模型层

#### CardModel (卡牌模型)

| 方法 | 描述 |
|------|------|
| `CardModel(int cardId, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position)` | 构造函数 |
| `getCardId()` | 获取卡牌ID |
| `getFace()` | 获取卡牌面值 |
| `getSuit()` | 获取卡牌花色 |
| `getPosition()` | 获取卡牌位置 |
| `setPosition(const cocos2d::Vec2& position)` | 设置卡牌位置 |
| `getValue()` | 获取卡牌数值 |
| `canMatch(const CardModel* targetCard)` | 检查是否可以与目标卡牌匹配 |

#### GameModel (游戏模型)

| 方法 | 描述 |
|------|------|
| `GameModel()` | 构造函数 |
| `~GameModel()` | 析构函数 |
| `init(const std::vector<CardModel*>& playfieldCards, const std::vector<CardModel*>& stackCards)` | 初始化游戏模型 |
| `getPlayfieldCardById(int cardId)` | 获取指定ID的主牌区卡牌 |
| `drawCardFromStack()` | 从备用牌堆抽取一张卡牌 |
| `moveCardFromPlayfieldToTray(int cardId)` | 将卡牌从主牌区移动到手牌区 |
| `setTrayTopCard(CardModel* card)` | 设置手牌区顶部卡牌 |
| `savePreviousTrayCard(CardModel* prevCard)` | 保存之前的手牌区顶部卡牌 |
| `restorePreviousTrayCard()` | 恢复之前的手牌区顶部卡牌 |
| `hasPreviousTrayCard()` | 是否有之前的手牌区顶部卡牌 |
| `removePlayfieldCard(int cardId)` | 移除主牌区卡牌 |
| `isGameOver()` | 游戏是否结束 |
| `isGameWon()` | 游戏是否胜利 |
| `setTrayTopCardDirectly(CardModel* card)` | 直接设置手牌区顶部卡牌 |

#### UndoModel (回退模型)

回退操作的数据模型，存储游戏中的操作记录以支持撤销功能。

### 4. 视图层

#### CardView (卡牌视图)

卡牌的可视化表示，负责卡牌的渲染和交互。

#### GameView (游戏视图)

| 方法 | 描述 |
|------|------|
| `create(const GameModel* model)` | 创建游戏视图 |
| `init(const GameModel* model)` | 初始化游戏视图 |
| `initGameAreas()` | 初始化游戏区域 |
| `initPlayfieldCards()` | 初始化主牌区卡牌 |
| `initTray()` | 初始化手牌区 |
| `initStack()` | 初始化备用牌堆 |
| `initUI()` | 初始化UI控件 |
| `setOnPlayfieldCardClickCallback(const std::function<void(int)>& callback)` | 设置主牌区卡牌点击回调 |
| `getCardClickCallback()` | 获取卡牌点击回调 |
| `setOnStackClickCallback(const std::function<void()>& callback)` | 设置备用牌堆点击回调 |
| `setOnUndoClickCallback(const std::function<void()>& callback)` | 设置回退按钮点击回调 |
| `updateTrayTopCard(const CardModel* card)` | 更新手牌区顶部卡牌 |
| `playCardMoveToTrayAnimation(int cardId)` | 播放卡牌移动到手牌区动画 |
| `playDirectCoverAnimation(int cardId)` | 播放直接覆盖手牌动画 |
| `playStackToTrayAnimation()` | 播放从备用牌堆到手牌区的动画 |
| `playTrayToPositionAnimation(const Vec2& targetPos, const std::function<void()>& callback)` | 播放手牌区到指定位置的动画 |
| `removePlayfieldCard(int cardId)` | 移除主牌区卡牌 |
| `setTrayTopCardView(CardView* cardView)` | 设置手牌区顶部卡牌视图 |
| `setPlayfieldCardsInteractive(bool enabled)` | 设置主牌区卡牌是否可交互 |
| `setStackInteractive(bool enabled)` | 设置备用牌堆是否可交互 |

### 5. 控制器层

#### GameController (游戏控制器)

| 方法 | 描述 |
|------|------|
| `GameController()` | 构造函数 |
| `~GameController()` | 析构函数 |
| `init(int levelId, cocos2d::Node* parent)` | 初始化游戏控制器 |
| `handlePlayfieldCardClick(int cardId)` | 处理主牌区卡牌点击事件 |
| `handleStackClick()` | 处理备用牌堆点击事件 |
| `handleUndoClick()` | 处理回退按钮点击事件 |
| `resetGameState()` | 重置游戏状态 |
| `initGameModel(int levelId)` | 初始化游戏数据模型 |
| `initGameView(cocos2d::Node* parent)` | 初始化游戏视图 |
| `initUndoManager()` | 初始化回退管理器 |
| `initEventHandlers()` | 初始化事件处理器 |
| `checkGameOver()` | 检查游戏是否结束 |

### 6. 管理器

#### UndoManager (回退管理器)

| 方法 | 描述 |
|------|------|
| `UndoManager()` | 构造函数 |
| `~UndoManager()` | 析构函数 |
| `init(GameModel* gameModel, GameView* gameView)` | 初始化回退管理器 |
| `recordPlayfieldToTrayOperation(int cardId, const cocos2d::Vec2& fromPos, const cocos2d::Vec2& toPos, int prevTrayCardId)` | 记录从主牌区到手牌区的操作 |
| `recordStackToTrayOperation(int cardId, int prevTrayCardId)` | 记录从备用牌堆到手牌区的操作 |
| `undo()` | 撤销最后一次操作 |
| `canUndo()` | 是否可以撤销 |
| `clearAllUndoRecords()` | 清空所有撤销记录 |
| `undoPlayfieldToTrayOperation(const OperationRecord& record)` | 撤销从主牌区到手牌区的操作 |
| `undoStackToTrayOperation(const OperationRecord& record)` | 撤销从备用牌堆到手牌区的操作 |

### 7. 服务

#### GameModelFromLevelGenerator (游戏模型生成器)

从关卡配置生成游戏模型的服务类。

### 8. 场景

#### GameScene (游戏场景)

游戏的主场景，管理整个游戏界面的显示和交互。

## 游戏流程

1. 应用启动时，`AppDelegate`初始化游戏环境并创建`GameScene`
2. `GameScene`创建`GameController`并初始化游戏
3. `GameController`加载关卡配置并创建`GameModel`和`GameView`
4. 玩家与游戏界面交互，点击卡牌或按钮
5. 交互事件由`GameView`捕获并传递给`GameController`处理
6. `GameController`更新`GameModel`，然后通知`GameView`更新显示
7. 撤销操作由`UndoManager`处理，恢复游戏状态

## 游戏规则

- 玩家需要将主牌区的所有卡牌移动到手牌区
- 卡牌可以匹配的条件是数值相差1
- 当主牌区卡牌全部移除，且备用牌堆为空时，玩家获胜
- 玩家可以使用撤销功能恢复之前的操作

## 设计模式

- **MVC模式**：将游戏逻辑分为模型(Model)、视图(View)和控制器(Controller)
- **观察者模式**：通过回调函数实现视图对模型的响应
- **命令模式**：在`UndoManager`中实现撤销操作
- **工厂模式**：使用静态工厂方法创建游戏对象
/**
 * GameController.h
 * 游戏控制器，管理整个游戏流程
 */

#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../managers/UndoManager.h"

/**
 * 游戏控制器类，管理游戏逻辑
 */
class GameController
{
public:
    /**
     * 构造函数
     */
    GameController();
    
    /**
     * 析构函数
     */
    ~GameController();
    
    /**
     * 初始化游戏控制器
     * @param levelId 关卡ID
     * @param parent 父节点
     * @return 是否初始化成功
     */
    bool init(int levelId, cocos2d::Node* parent);
    
    /**
     * 处理主牌区卡牌点击事件
     * @param cardId 卡牌ID
     * @return 是否处理成功
     */
    bool handlePlayfieldCardClick(int cardId);
    
    /**
     * 处理备用牌堆点击事件
     * @return 是否处理成功
     */
    bool handleStackClick();
    
    /**
     * 处理回退按钮点击事件
     * @return 是否处理成功
     */
    bool handleUndoClick();
    
    /**
     * 重置游戏状态，重新计算有效移动
     */
    void resetGameState();
    
private:
    GameModel* _gameModel;        // 游戏数据模型
    GameView* _gameView;          // 游戏视图
    UndoManager* _undoManager;    // 回退管理器
    
    /**
     * 初始化游戏数据模型
     * @param levelId 关卡ID
     * @return 是否初始化成功
     */
    bool initGameModel(int levelId);
    
    /**
     * 初始化游戏视图
     * @param parent 父节点
     * @return 是否初始化成功
     */
    bool initGameView(cocos2d::Node* parent);
    
    /**
     * 初始化回退管理器
     * @return 是否初始化成功
     */
    bool initUndoManager();
    
    /**
     * 初始化事件处理
     */
    void initEventHandlers();
    
    /**
     * 检查游戏结束
     */
    void checkGameOver();
};

#endif // __GAME_CONTROLLER_H__
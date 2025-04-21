/**
 * GameScene.h
 * 游戏场景类，作为游戏的主场景
 */

#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "../controllers/GameController.h"

/**
 * 游戏场景类，包含游戏的所有内容
 */
class GameScene : public cocos2d::Scene
{
public:
    /**
     * 创建游戏场景
     * @return 游戏场景
     */
    static cocos2d::Scene* createScene();
    
    /**
     * 初始化场景
     * @return 是否初始化成功
     */
    virtual bool init();
    
    // 实现create()静态方法
    CREATE_FUNC(GameScene);
    
private:
    GameController* _gameController;  // 游戏控制器
    
    /**
     * 初始化背景
     */
    void initBackground();
    
    /**
     * 初始化游戏控制器
     * @return 是否初始化成功
     */
    bool initGameController();
};

#endif // __GAME_SCENE_H__ 
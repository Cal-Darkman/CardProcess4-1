/**
 * GameModelFromLevelGenerator.h
 * 游戏模型生成器，将关卡配置转换为游戏模型
 */

#ifndef __GAME_MODEL_FROM_LEVEL_GENERATOR_H__
#define __GAME_MODEL_FROM_LEVEL_GENERATOR_H__

#include "cocos2d.h"
#include "../models/GameModel.h"
#include "../configs/models/LevelConfig.h"

/**
 * 游戏模型生成器类，将关卡配置转换为游戏模型
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * 从关卡配置生成游戏模型
     * @param levelConfig 关卡配置
     * @return 游戏模型
     */
    static GameModel* generateGameModel(const LevelConfig* levelConfig);
    
private:
    /**
     * 生成主牌区卡牌
     * @param cardConfigs 卡牌配置列表
     * @return 卡牌模型列表
     */
    static std::vector<CardModel*> generatePlayfieldCards(const std::vector<CardConfig>& cardConfigs);
    
    /**
     * 生成备用牌堆卡牌
     * @param cardConfigs 卡牌配置列表
     * @return 卡牌模型列表
     */
    static std::vector<CardModel*> generateStackCards(const std::vector<CardConfig>& cardConfigs);
};

#endif // __GAME_MODEL_FROM_LEVEL_GENERATOR_H__ 
/**
 * LevelConfig.h
 * 关卡配置数据结构
 */

#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include "cocos2d.h"
#include <vector>

/**
 * 卡牌配置结构体
 */
struct CardConfig
{
    int cardFace;                  // 卡牌面值
    int cardSuit;                  // 卡牌花色
    cocos2d::Vec2 position;        // 卡牌位置
    
    CardConfig() 
        : cardFace(0)
        , cardSuit(0)
        , position(cocos2d::Vec2::ZERO)
    {}
};

/**
 * 关卡配置类，存储关卡的配置数据
 */
class LevelConfig
{
public:
    /**
     * 创建关卡配置
     */
    LevelConfig();
    
    /**
     * 获取主牌区卡牌配置
     * @return 主牌区卡牌配置列表
     */
    const std::vector<CardConfig>& getPlayfieldCards() const { return _playfieldCards; }
    
    /**
     * 设置主牌区卡牌配置
     * @param cards 主牌区卡牌配置列表
     */
    void setPlayfieldCards(const std::vector<CardConfig>& cards) { _playfieldCards = cards; }
    
    /**
     * 获取备用牌堆卡牌配置
     * @return 备用牌堆卡牌配置列表
     */
    const std::vector<CardConfig>& getStackCards() const { return _stackCards; }
    
    /**
     * 设置备用牌堆卡牌配置
     * @param cards 备用牌堆卡牌配置列表
     */
    void setStackCards(const std::vector<CardConfig>& cards) { _stackCards = cards; }
    
private:
    std::vector<CardConfig> _playfieldCards;      // 主牌区卡牌配置
    std::vector<CardConfig> _stackCards;          // 备用牌堆卡牌配置
};

#endif // __LEVEL_CONFIG_H__ 
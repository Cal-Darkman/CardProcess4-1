/**
 * CardModel.h
 * 卡牌数据模型，存储卡牌的数据和状态
 */

#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "cocos2d.h"

/**
 * 花色类型
 */
enum CardSuitType
{
    CST_NONE = -1,
    CST_CLUBS,      // 梅花
    CST_DIAMONDS,   // 方块
    CST_HEARTS,     // 红桃
    CST_SPADES,     // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * 正面类型
 */
enum CardFaceType
{
    CFT_NONE = -1,
    CFT_ACE,
    CFT_TWO,
    CFT_THREE,
    CFT_FOUR,
    CFT_FIVE,
    CFT_SIX,
    CFT_SEVEN,
    CFT_EIGHT,
    CFT_NINE,
    CFT_TEN,
    CFT_JACK,
    CFT_QUEEN,
    CFT_KING,
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * 卡牌模型类，保存卡牌的基本属性
 */
class CardModel
{
public:
    /** 
     * 创建卡牌模型
     * @param cardId 卡牌唯一ID
     * @param face 卡牌面值
     * @param suit 卡牌花色
     * @param position 卡牌位置
     */
    CardModel(int cardId, CardFaceType face, CardSuitType suit, const cocos2d::Vec2& position);
    
    /**
     * 获取卡牌ID
     * @return 卡牌ID
     */
    int getCardId() const { return _cardId; }
    
    /**
     * 获取卡牌面值
     * @return 卡牌面值
     */
    CardFaceType getFace() const { return _face; }
    
    /**
     * 获取卡牌花色
     * @return 卡牌花色
     */
    CardSuitType getSuit() const { return _suit; }
    
    /**
     * 获取卡牌位置
     * @return 卡牌位置
     */
    cocos2d::Vec2 getPosition() const { return _position; }
    
    /**
     * 设置卡牌位置
     * @param position 卡牌位置
     */
    void setPosition(const cocos2d::Vec2& position) { _position = position; }
    
    /**
     * 获取卡牌数值（1-13）
     * @return 卡牌数值
     */
    int getValue() const { return static_cast<int>(_face) + 1; }
    
    /**
     * 检查是否可以与目标卡牌匹配
     * @param targetCard 目标卡牌
     * @return 是否可以匹配
     */
    bool canMatch(const CardModel* targetCard) const;
    
private:
    int _cardId;                 // 卡牌唯一ID
    CardFaceType _face;          // 卡牌面值
    CardSuitType _suit;          // 卡牌花色
    cocos2d::Vec2 _position;     // 卡牌位置
};

#endif // __CARD_MODEL_H__ 
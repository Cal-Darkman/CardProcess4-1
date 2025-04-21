/**
 * CardResConfig.h
 * 卡牌资源配置
 */

#ifndef __CARD_RES_CONFIG_H__
#define __CARD_RES_CONFIG_H__

#include <string>
#include "models/CardModel.h"

/**
 * 卡牌资源配置类，管理卡牌的UI资源
 */
class CardResConfig
{
public:
    /**
     * 获取卡牌正面图片路径
     * @param face 卡牌面值
     * @param suit 卡牌花色
     * @return 图片路径
     */
    static std::string getCardFaceImagePath(CardFaceType face, CardSuitType suit);
    
    /**
     * 获取卡牌背面图片路径
     * @return 图片路径
     */
    static std::string getCardBackImagePath();
    
    /**
     * 获取卡牌面值字符串
     * @param face 卡牌面值
     * @return 面值字符串
     */
    static std::string getCardFaceString(CardFaceType face);
    
    /**
     * 获取卡牌花色字符串
     * @param suit 卡牌花色
     * @return 花色字符串
     */
    static std::string getCardSuitString(CardSuitType suit);
    
    /**
     * 获取花色图片路径
     * @param suit 卡牌花色
     * @return 图片路径
     */
    static std::string getCardSuitImagePath(CardSuitType suit);
    
    /**
     * 获取数字图片路径
     * @param face 卡牌面值
     * @param isRed 是否为红色
     * @param isSmall 是否为小数字
     * @return 图片路径
     */
    static std::string getCardNumberImagePath(CardFaceType face, bool isRed, bool isSmall);
};

#endif // __CARD_RES_CONFIG_H__ 
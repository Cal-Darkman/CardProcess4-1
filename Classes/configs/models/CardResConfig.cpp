/**
 * CardResConfig.cpp
 * 卡牌资源配置实现
 */

#include "CardResConfig.h"
#include "../../models/CardModel.h"

USING_NS_CC;

std::string CardResConfig::getCardFaceImagePath(CardFaceType face, CardSuitType suit)
{
    // 由于资源结构不同，我们实际上在这里不返回组合的图片路径
    // 而是在CardView中组合基本卡牌、花色和数字
    return "res/card_general.png";
}

std::string CardResConfig::getCardBackImagePath()
{
    // 卡牌背面图片，如果没有特定的背面图片，可以使用基本卡牌
    return "res/card_general.png";
}

std::string CardResConfig::getCardFaceString(CardFaceType face)
{
    switch (face) {
        case CFT_ACE:    return "A";
        case CFT_TWO:    return "2";
        case CFT_THREE:  return "3";
        case CFT_FOUR:   return "4";
        case CFT_FIVE:   return "5";
        case CFT_SIX:    return "6";
        case CFT_SEVEN:  return "7";
        case CFT_EIGHT:  return "8";
        case CFT_NINE:   return "9";
        case CFT_TEN:    return "10";
        case CFT_JACK:   return "J";
        case CFT_QUEEN:  return "Q";
        case CFT_KING:   return "K";
        default:         return "";
    }
}

std::string CardResConfig::getCardSuitString(CardSuitType suit)
{
    switch (suit) {
        case CST_CLUBS:      return "clubs";
        case CST_DIAMONDS:   return "diamonds";
        case CST_HEARTS:     return "hearts";
        case CST_SPADES:     return "spades";
        default:             return "";
    }
}

// 获取花色图片路径
std::string CardResConfig::getCardSuitImagePath(CardSuitType suit)
{
    switch (suit) {
        case CST_CLUBS:      return "res/suits/clubs.png";
        case CST_DIAMONDS:   return "res/suits/diamonds.png";
        case CST_HEARTS:     return "res/suits/hearts.png";
        case CST_SPADES:     return "res/suits/spades.png";
        default:             return "";
    }
}

// 获取数字图片路径，根据当前文件命名规则修改
std::string CardResConfig::getCardNumberImagePath(CardFaceType face, bool isRed, bool isSmall)
{
    std::string color = isRed ? "red" : "black";
    std::string size = isSmall ? "small" : "big";
    std::string faceStr = getCardFaceString(face);
    
    return StringUtils::format("res/number/%s_%s_%s.png", size.c_str(), color.c_str(), faceStr.c_str());
} 
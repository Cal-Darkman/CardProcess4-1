/**
 * CardModel.cpp
 * 卡牌数据模型实现
 */

#include "CardModel.h"

USING_NS_CC;

CardModel::CardModel(int cardId, CardFaceType face, CardSuitType suit, const Vec2& position)
    : _cardId(cardId)
    , _face(face)
    , _suit(suit)
    , _position(position)
{
}

bool CardModel::canMatch(const CardModel* targetCard) const
{
    if (!targetCard) return false;
    
    // 数字相差1即可匹配
    int valueDiff = abs(this->getValue() - targetCard->getValue());
    return valueDiff == 1;
} 
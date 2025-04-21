/**
 * GameModel.cpp
 * 游戏数据模型实现
 */

#include "GameModel.h"

USING_NS_CC;

GameModel::GameModel()
    : _trayTopCard(nullptr)
{
}

GameModel::~GameModel()
{
    // 清理所有卡牌内存
    for (auto card : _playfieldCards) {
        CC_SAFE_DELETE(card);
    }
    _playfieldCards.clear();
    
    for (auto card : _stackCards) {
        CC_SAFE_DELETE(card);
    }
    _stackCards.clear();
    
    // 不需要删除_trayTopCard，因为它是从_playfieldCards或_stackCards中移除的
}

bool GameModel::init(const std::vector<CardModel*>& playfieldCards, const std::vector<CardModel*>& stackCards)
{
    if (playfieldCards.empty() || stackCards.empty()) {
        return false;
    }
    
    _playfieldCards = playfieldCards;
    _stackCards = stackCards;
    
    // 初始化手牌区顶部卡牌
    if (!_stackCards.empty()) {
        _trayTopCard = _stackCards.back();
        _stackCards.pop_back();
    }
    
    return true;
}

CardModel* GameModel::getPlayfieldCardById(int cardId) const
{
    for (auto card : _playfieldCards) {
        if (card->getCardId() == cardId) {
            return card;
        }
    }
    return nullptr;
}

bool GameModel::drawCardFromStack()
{
    if (_stackCards.empty()) {
        return false;
    }
    
    _trayTopCard = _stackCards.back();
    _stackCards.pop_back();
    
    return true;
}

bool GameModel::moveCardFromPlayfieldToTray(int cardId)
{
    CardModel* card = getPlayfieldCardById(cardId);
    if (!card) {
        return false;
    }
    
    // 检查卡牌是否可以与手牌区顶部卡牌匹配
    if (_trayTopCard && !card->canMatch(_trayTopCard)) {
        return false;
    }
    
    // 从主牌区移除卡牌
    removePlayfieldCard(cardId);
    
    // 设置为手牌区顶部卡牌
    setTrayTopCard(card);
    
    return true;
}

void GameModel::setTrayTopCard(CardModel* card)
{
    // 保存当前的手牌顶部卡牌
    if (_trayTopCard != nullptr) {
        savePreviousTrayCard(_trayTopCard);
    }
    
    // 设置新的手牌顶部卡牌
    _trayTopCard = card;
}

void GameModel::savePreviousTrayCard(CardModel* prevCard)
{
    if (prevCard != nullptr) {
        _previousTrayCards.push(prevCard);
    }
}

CardModel* GameModel::restorePreviousTrayCard()
{
    if (_previousTrayCards.empty()) {
        return nullptr;
    }
    
    CardModel* prevCard = _previousTrayCards.top();
    _previousTrayCards.pop();
    return prevCard;
}

bool GameModel::hasPreviousTrayCard() const
{
    return !_previousTrayCards.empty();
}

CardModel* GameModel::removePlayfieldCard(int cardId)
{
    for (auto it = _playfieldCards.begin(); it != _playfieldCards.end(); ++it) {
        if ((*it)->getCardId() == cardId) {
            CardModel* card = *it;
            _playfieldCards.erase(it);
            return card;
        }
    }
    return nullptr;
}

bool GameModel::isGameOver() const
{
    // 游戏结束条件：主牌区和备用牌堆都为空
    return _playfieldCards.empty() && _stackCards.empty();
}

bool GameModel::isGameWon() const
{
    // 赢得游戏条件：主牌区为空
    return _playfieldCards.empty();
}

void GameModel::setTrayTopCardDirectly(CardModel* card)
{
    // 直接设置手牌区顶部卡牌，不保存当前卡牌
    _trayTopCard = card;
} 
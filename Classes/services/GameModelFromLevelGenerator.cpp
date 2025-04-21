/**
 * GameModelFromLevelGenerator.cpp
 * 游戏模型生成器实现
 */

#include "GameModelFromLevelGenerator.h"

USING_NS_CC;

GameModel* GameModelFromLevelGenerator::generateGameModel(const LevelConfig* levelConfig)
{
    if (!levelConfig) {
        return nullptr;
    }
    
    // 创建游戏模型
    GameModel* gameModel = new GameModel();
    
    // 生成主牌区卡牌
    auto playfieldCards = generatePlayfieldCards(levelConfig->getPlayfieldCards());
    
    // 生成备用牌堆卡牌
    auto stackCards = generateStackCards(levelConfig->getStackCards());
    
    // 初始化游戏模型
    if (!gameModel->init(playfieldCards, stackCards)) {
        CC_SAFE_DELETE(gameModel);
        return nullptr;
    }
    
    return gameModel;
}

std::vector<CardModel*> GameModelFromLevelGenerator::generatePlayfieldCards(const std::vector<CardConfig>& cardConfigs)
{
    std::vector<CardModel*> cards;
    int cardId = 0;
    
    for (const auto& config : cardConfigs) {
        // 创建卡牌模型
        CardFaceType face = static_cast<CardFaceType>(config.cardFace);
        CardSuitType suit = static_cast<CardSuitType>(config.cardSuit);
        
        CardModel* card = new CardModel(cardId++, face, suit, config.position);
        cards.push_back(card);
    }
    
    return cards;
}

std::vector<CardModel*> GameModelFromLevelGenerator::generateStackCards(const std::vector<CardConfig>& cardConfigs)
{
    std::vector<CardModel*> cards;
    // 主牌区卡牌ID从0开始，备用牌堆ID从100开始，避免冲突
    int cardId = 100;
    
    for (const auto& config : cardConfigs) {
        // 创建卡牌模型
        CardFaceType face = static_cast<CardFaceType>(config.cardFace);
        CardSuitType suit = static_cast<CardSuitType>(config.cardSuit);
        
        CardModel* card = new CardModel(cardId++, face, suit, Vec2::ZERO);
        cards.push_back(card);
    }
    
    return cards;
} 
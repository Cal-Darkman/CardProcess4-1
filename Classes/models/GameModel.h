/**
 * GameModel.h
 * 游戏数据模型，存储游戏状态和所有卡牌数据
 */

#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include "cocos2d.h"
#include "CardModel.h"
#include <vector>
#include <stack>

/**
 * 游戏模型类，管理游戏数据和状态
 */
class GameModel
{
public:
    /**
     * 创建游戏模型
     */
    GameModel();
    
    /**
     * 析构函数
     */
    ~GameModel();
    
    /**
     * 初始化游戏
     * @param playfieldCards 主牌区卡牌
     * @param stackCards 备用牌堆卡牌
     * @return 是否初始化成功
     */
    bool init(const std::vector<CardModel*>& playfieldCards, const std::vector<CardModel*>& stackCards);
    
    /**
     * 获取主牌区卡牌
     * @return 主牌区卡牌列表
     */
    const std::vector<CardModel*>& getPlayfieldCards() const { return _playfieldCards; }
    
    /**
     * 获取主牌区卡牌（可修改版本）
     * @return 主牌区卡牌列表
     */
    std::vector<CardModel*>& getPlayfieldCards() { return _playfieldCards; }
    
    /**
     * 获取备用牌堆卡牌
     * @return 备用牌堆卡牌列表
     */
    const std::vector<CardModel*>& getStackCards() const { return _stackCards; }
    
    /**
     * 获取备用牌堆卡牌（可修改版本）
     * @return 备用牌堆卡牌列表
     */
    std::vector<CardModel*>& getStackCards() { return _stackCards; }
    
    /**
     * 获取手牌区顶部卡牌
     * @return 手牌区顶部卡牌
     */
    CardModel* getTrayTopCard() const { return _trayTopCard; }
    
    /**
     * 根据卡牌ID获取主牌区卡牌
     * @param cardId 卡牌ID
     * @return 卡牌模型，未找到则返回nullptr
     */
    CardModel* getPlayfieldCardById(int cardId) const;
    
    /**
     * 从备用牌堆抽一张牌到手牌区
     * @return 是否成功抽牌
     */
    bool drawCardFromStack();
    
    /**
     * 把主牌区的牌移动到手牌区
     * @param cardId 要移动的卡牌ID
     * @return 是否成功移动
     */
    bool moveCardFromPlayfieldToTray(int cardId);
    
    /**
     * 把手牌区的牌设置为指定的牌
     * @param card 要设置的卡牌
     */
    void setTrayTopCard(CardModel* card);
    
    /**
     * 把手牌区的牌直接设置为指定的牌（不保存之前的卡牌）
     * @param card 要设置的卡牌
     */
    void setTrayTopCardDirectly(CardModel* card);
    
    /**
     * 从主牌区移除指定ID的卡牌
     * @param cardId 要移除的卡牌ID
     * @return 移除的卡牌，未找到则返回nullptr
     */
    CardModel* removePlayfieldCard(int cardId);
    
    /**
     * 检查游戏是否结束
     * @return 游戏是否结束
     */
    bool isGameOver() const;
    
    /**
     * 检查是否赢得游戏
     * @return 是否赢得游戏
     */
    bool isGameWon() const;
    
    /**
     * 在设置新的手牌顶部卡牌时保存之前的手牌
     * @param prevCard 上一张手牌顶部卡牌
     */
    void savePreviousTrayCard(CardModel* prevCard);
    
    /**
     * 恢复上一张手牌顶部卡牌
     * @return 之前的手牌顶部卡牌
     */
    CardModel* restorePreviousTrayCard();
    
    /**
     * 检查是否有上一张手牌可以恢复
     * @return 是否有上一张手牌
     */
    bool hasPreviousTrayCard() const;
    
private:
    std::vector<CardModel*> _playfieldCards;   // 主牌区卡牌
    std::vector<CardModel*> _stackCards;       // 备用牌堆卡牌
    CardModel* _trayTopCard;                   // 手牌区顶部卡牌
    std::stack<CardModel*> _previousTrayCards; // 之前的手牌区顶部卡牌栈
};

#endif // __GAME_MODEL_H__ 
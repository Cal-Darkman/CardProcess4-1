/**
 * UndoManager.h
 * 回退管理器，处理游戏中的撤销操作
 */

#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include "cocos2d.h"
#include "../models/UndoModel.h"
#include "../models/GameModel.h"
#include "../views/GameView.h"
#include "../views/CardView.h"

/**
 * 回退管理器类，处理游戏中的撤销操作
 */
class UndoManager
{
public:
    /**
     * 构造函数
     */
    UndoManager();
    
    /**
     * 析构函数
     */
    ~UndoManager();
    
    /**
     * 初始化回退管理器
     * @param gameModel 游戏模型
     * @param gameView 游戏视图
     * @return 是否初始化成功
     */
    bool init(GameModel* gameModel, GameView* gameView);
    
    /**
     * 记录从主牌区到手牌区的操作
     * @param cardId 卡牌ID
     * @param fromPos 起始位置
     * @param toPos 目标位置
     * @param prevTrayCardId 之前的手牌区顶部卡牌ID
     */
    void recordPlayfieldToTrayOperation(int cardId, const cocos2d::Vec2& fromPos, 
                                        const cocos2d::Vec2& toPos, int prevTrayCardId);
    
    /**
     * 记录从备用牌堆到手牌区的操作
     * @param cardId 卡牌ID
     * @param prevTrayCardId 之前的手牌区顶部卡牌ID
     */
    void recordStackToTrayOperation(int cardId, int prevTrayCardId);
    
    /**
     * 撤销最后一次操作
     * @return 是否成功撤销
     */
    bool undo();
    
    /**
     * 检查是否可以撤销
     * @return 是否可以撤销
     */
    bool canUndo() const;
    
    /**
     * 清空所有撤销记录
     */
    void clearAllUndoRecords();
    
private:
    UndoModel* _undoModel;       // 回退数据模型
    GameModel* _gameModel;       // 游戏数据模型
    GameView* _gameView;         // 游戏视图
    
    /**
     * 撤销从主牌区到手牌区的操作
     * @param record 操作记录
     * @return 是否成功撤销
     */
    bool undoPlayfieldToTrayOperation(const OperationRecord& record);
    
    /**
     * 撤销从备用牌堆到手牌区的操作
     * @param record 操作记录
     * @return 是否成功撤销
     */
    bool undoStackToTrayOperation(const OperationRecord& record);
};

#endif // __UNDO_MANAGER_H__ 
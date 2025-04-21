/**
 * UndoModel.h
 * 回退操作数据模型，用于存储可回退的操作
 */

#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"
#include "models/CardModel.h"
#include <vector>

/**
 * 操作类型
 */
enum OperationType 
{
    OT_NONE = 0,
    OT_PLAYFIELD_TO_TRAY,  // 主牌区到手牌区
    OT_STACK_TO_TRAY       // 备用牌堆到手牌区
};

/**
 * 操作记录结构体
 */
struct OperationRecord 
{
    OperationType type;       // 操作类型
    int cardId;               // 卡牌ID
    cocos2d::Vec2 fromPos;    // 起始位置
    cocos2d::Vec2 toPos;      // 目标位置
    int prevTrayCardId;       // 之前的手牌区顶部卡牌ID
    
    OperationRecord() 
        : type(OT_NONE)
        , cardId(-1)
        , fromPos(cocos2d::Vec2::ZERO)
        , toPos(cocos2d::Vec2::ZERO)
        , prevTrayCardId(-1)
    {}
};

/**
 * 回退模型类，存储回退操作数据
 */
class UndoModel 
{
public:
    /**
     * 创建回退模型
     */
    UndoModel();
    ~UndoModel(); // 确保声明了析构函数
    
    /**
     * 添加操作记录
     * @param record 操作记录
     */
    void addOperationRecord(const OperationRecord& record);
    
    /**
     * 获取最后一条操作记录
     * @return 操作记录
     */
    OperationRecord getLastRecord() const;
    
    /**
     * 移除最后一条操作记录
     */
    void removeLastRecord();
    
    /**
     * 清空所有操作记录
     */
    void clearAllRecords();
    
    /**
     * 检查是否有可回退的操作
     * @return 是否有可回退的操作
     */
    bool canUndo() const;
    
private:
    std::vector<OperationRecord> _operationRecords;   // 操作记录列表
};

#endif // __UNDO_MODEL_H__
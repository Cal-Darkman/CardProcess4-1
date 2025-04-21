/**
 * UndoModel.cpp
 * 回退操作数据模型实现
 */

#include "UndoModel.h"

USING_NS_CC;

UndoModel::UndoModel()
{
}

// 添加析构函数实现
UndoModel::~UndoModel()
{
    // 如果需要清理资源，在这里添加相应代码
    // 例如，如果有动态分配的内存需要释放
    clearAllRecords();
}

void UndoModel::addOperationRecord(const OperationRecord& record)
{
    _operationRecords.push_back(record);
}

// 添加获取最后一条记录的方法
OperationRecord UndoModel::getLastRecord() const
{
    if (_operationRecords.empty()) {
        return OperationRecord();
    }
    
    return _operationRecords.back();
}

// 添加移除最后一条记录的方法
void UndoModel::removeLastRecord()
{
    if (!_operationRecords.empty()) {
        _operationRecords.pop_back();
    }
}

void UndoModel::clearAllRecords()
{
    _operationRecords.clear();
}

bool UndoModel::canUndo() const
{
    return !_operationRecords.empty();
}
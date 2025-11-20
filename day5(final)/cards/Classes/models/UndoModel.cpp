#include "UndoModel.h"

USING_NS_CC;

UndoModel* UndoModel::create()
{
    auto ret = new (std::nothrow) UndoModel();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool UndoModel::init()
{
    return true;
}

void UndoModel::pushRecord(const UndoRecord& record)
{
    _records.push_back(record);
}

bool UndoModel::popRecord(UndoRecord& outRecord)
{
    if (_records.empty())
        return false;

    outRecord = _records.back();
    _records.pop_back();
    return true;
}

bool UndoModel::isEmpty() const
{
    return _records.empty();
}

void UndoModel::clear()
{
    _records.clear();
}
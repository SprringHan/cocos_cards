#pragma once

#include "cocos2d.h"
#include <vector>

class CardModel;

// 一次可撤销操作的类型
enum class UndoOpType
{
    None = 0,
    // 手牌区 -> 顶部牌
    StackToTop,
    // 主牌区 -> 顶部牌
    PlayfieldToTop
};

// 一条撤销记录
struct UndoRecord
{
    UndoOpType type = UndoOpType::None;

    // 被移动到顶部的那张牌
    CardModel* movedCard = nullptr;

    // 操作前的顶部牌
    CardModel* prevTopCard = nullptr;

    // 操作前卡牌在其原父节点中的坐标
    cocos2d::Vec2 fromPos;

    // 操作前是否在 Stack 区
    bool fromStack = false;
};

// 用来保存所有撤销记录的 Model
class UndoModel : public cocos2d::Ref
{
public:
    static UndoModel* create();
    bool init();

    void pushRecord(const UndoRecord& record);
    bool popRecord(UndoRecord& outRecord);
    bool isEmpty() const;
    void clear();

private:
    UndoModel() = default;
    ~UndoModel() = default;

    std::vector<UndoRecord> _records;
};
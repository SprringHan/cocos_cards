#pragma once
#include "cocos2d.h"
#include "CardModel.h"
#include <vector>

class GameModel : public cocos2d::Ref {
public:
    static GameModel* create();
    bool init();


    // 1. 顶牌管理（当前显示的底牌）
    void setTopCard(CardModel* card);
    CardModel* getTopCard() const { return _topCard; }

    // 2. 主牌区管理
    void addPlayfieldCard(CardModel* card);
    void removePlayfieldCard(CardModel* card);
    bool isCardInPlayfield(CardModel* card) const;

    // 3. 备用牌堆管理（Stack）
    void addStackCard(CardModel* card);
    // 获取备用堆的下一张牌（用于检查点击是否合法）
    CardModel* getNextStackCard() const;
    // 从备用堆移除并返回该牌（翻牌时调用）
    CardModel* popStackCard();
    bool isStackEmpty() const;

private:
    GameModel() = default;
    ~GameModel();

    // 当前用于匹配的底牌
    CardModel* _topCard = nullptr;

    // 主牌区所有卡牌
    cocos2d::Vector<CardModel*> _playfieldCards;

    // 备用牌堆
    cocos2d::Vector<CardModel*> _stackCards;
};
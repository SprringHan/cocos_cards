#pragma once

#include "cocos2d.h"
#include "../configs/GameConfig.h"
//卡牌模型
class CardModel:public cocos2d::Ref
{
public:
	static CardModel* create(int id, CardFaceType face, CardSuitType suit);
	virtual bool init(int id, CardFaceType face, CardSuitType suit);

	//获取只读数据
	int getCardId() const { return _cardId; }
	CardFaceType getFace() const { return _face; }
	CardSuitType getSuit() const { return _suit; }

private:
	CardModel() = default;
	~CardModel() = default;
	int _cardId;
	CardFaceType _face;
	CardSuitType _suit;
};


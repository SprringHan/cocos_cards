#pragma once

#include "cocos2d.h"
#include "../models/CardModel.h"
class CardView:public cocos2d::Node
{
public:
	static CardView* create(CardModel* model);
	CardView() = default;
	~CardView();

	bool init(CardModel* model);
	//获取只读模型
	const CardModel* getModel() const { return _model; }
private:

	CardModel* _model = nullptr;
};


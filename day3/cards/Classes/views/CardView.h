#pragma once

#include "cocos2d.h"
#include "../models/CardModel.h"
#include <functional>

class CardView:public cocos2d::Node
{
public:
	static CardView* create(CardModel* model);
	CardView() = default;
	~CardView();

	bool init(CardModel* model);
	//获取只读模型
	const CardModel* getModel() const { return _model; }

	using ClickCallback = std::function<void(CardView*)>;
	void setClickCallback(const ClickCallback& cb) { _clickCallback = cb; }
private:

	CardModel* _model = nullptr;

	cocos2d::Sprite* _bgSprite = nullptr;
	cocos2d::Sprite* _bigFaceSprite = nullptr;
	cocos2d::Sprite* _smallFaceSprite = nullptr;
	cocos2d::Sprite* _suitSprite = nullptr;

	ClickCallback _clickCallback;

	bool onTouchBegan(cocos2d::Touch* touch,cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
};


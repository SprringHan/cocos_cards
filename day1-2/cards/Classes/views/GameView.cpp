#include "GameView.h"
#include "CardView.h"
#include "../models/CardModel.h"

USING_NS_CC;

bool GameView::init() {
	if (!Node::init()) { return false; }

	//将view的尺寸设为整个屏幕
	auto visibleSize = Director::getInstance()->getVisibleSize();
	this->setContentSize(visibleSize);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setPosition(visibleSize / 2);

	//创建主牌区
	float playfieldHeight = 1500.0f;
	auto playfieldLayer = LayerColor::create(Color4B(50, 50, 50, 255), visibleSize.width, playfieldHeight);
	playfieldLayer->setIgnoreAnchorPointForPosition(false);
	playfieldLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	playfieldLayer->setPosition(Vec2(visibleSize.width / 2, visibleSize.height));
	
	this->addChild(playfieldLayer);
	_playfieldLayer = playfieldLayer;

	//创建底牌区
	float stackHeight = visibleSize.height - playfieldHeight;
	auto stackLayer = LayerColor::create(Color4B(30, 30, 30, 255), visibleSize.width, stackHeight);
	stackLayer->setIgnoreAnchorPointForPosition(false);
	stackLayer->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	stackLayer->setPosition(Vec2(visibleSize.width / 2, 0));

	this->addChild(stackLayer);
	_stackLayer = stackLayer;

	auto model1 = CardModel::create(0, CFT_FIVE, CST_HEARTS);
	auto view1 = CardView::create(model1);
	_playfieldLayer->addChild(view1);

	view1->setPosition(Vec2(_playfieldLayer->getContentSize().width / 2,_playfieldLayer->getContentSize().height / 2));

	return true;
}
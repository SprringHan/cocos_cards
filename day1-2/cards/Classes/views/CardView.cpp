#include "CardView.h"

USING_NS_CC;

CardView::~CardView() {
	CC_SAFE_RELEASE(_model);
}

CardView* CardView::create(CardModel* model) {
	auto ret = new (std::nothrow) CardView();
	if (ret && ret->init(model)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

bool CardView::init(CardModel* model) {
	if (!Node::init()) {
		return false;
	}

	_model = model;
	CC_SAFE_RETAIN(_model);

	cocos2d::Size cardSize(120.0f, 160.0f);
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	this->setContentSize(cardSize);

	auto draw = DrawNode::create();

	Vec2 rect[4] = {
		Vec2(0,0),
		Vec2(cardSize.width,0),
		Vec2(cardSize.width,cardSize.height),
		Vec2(0,cardSize.height)
	};

	draw->drawSolidPoly(rect, 4, Color4F(1, 1, 1, 1));
	draw->drawPoly(rect, 4, true, Color4F(0, 0, 0, 1));

	draw->setPosition(Vec2::ZERO);
	this->addChild(draw);

	return true;
}

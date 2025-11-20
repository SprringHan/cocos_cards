#include "CardView.h"
#include "../configs/models/GameConfig.h"
#include "../utils/CardTextUtil.h"

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

	//1.加载底牌背景
	_bgSprite = Sprite::create("res/card_general.png");
	if (!_bgSprite) {
		CCLOG("CardView:无法加载背景图片");
		return false;
	}
	this->addChild(_bgSprite);

	//由于卡牌太小，这里对其进行放大处理
	auto rawSize = _bgSprite->getContentSize();

	float targetWidth = 180.0f;
	float targetScale = targetWidth / rawSize.width;

	_bgSprite->setScale(targetScale);

	this->setContentSize(Size(rawSize.width * targetScale, rawSize.height * targetScale));
	this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	_bgSprite->setPosition(this->getContentSize() / 2);

	//获取卡牌资源
	std::string bigFace = bigFaceTexturePath(_model->getFace(), _model->getSuit());
	std::string smallFace = smallFaceTexturePath(_model->getFace(), _model->getSuit());
	std::string suitPath = suitToTexturePath(_model->getSuit());

	auto size = this->getContentSize();

	//设置牌边距，画图更美观
	float marginX = 0.23f * size.width;
	float marginY = 0.15f * size.height;

	//将三张图片贴在底牌当中
	_bigFaceSprite = Sprite::create(bigFace);
	_smallFaceSprite = Sprite::create(smallFace);
	_suitSprite = Sprite::create(suitPath);
	_bigFaceSprite->setScale(targetScale);
	_smallFaceSprite->setScale(targetScale);
	_suitSprite->setScale(targetScale);
	if (!_bigFaceSprite) {
		CCLOG("CardView:无法加载大数字图片");
	}else{
		this->addChild(_bigFaceSprite);
		_bigFaceSprite->setPosition(Vec2(size.width * 0.5, size.height * 0.4));
	}
	if (!_smallFaceSprite) {
		CCLOG("CardView:无法加载小数字图片");
	}
	else {
		this->addChild(_smallFaceSprite);
		_smallFaceSprite->setPosition(Vec2(marginX, size.height - marginY));
	}
	if (!_suitSprite) {
		CCLOG("CardView:无法加载花色图片");
	}
	else {
		this->addChild(_suitSprite);
		_suitSprite->setPosition(Vec2(size.width - marginX, size.height - marginY));
	}

	//注册卡牌监听
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(CardView::onTouchEnded, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

//实现点击判断逻辑
bool CardView::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event) {
	//将触摸的世界坐标转化为自身坐标
	cocos2d::Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

	Size size = getContentSize();
	cocos2d::Rect rect(0, 0, size.width, size.height);

	//判断是否命中
	if (!rect.containsPoint(locationInNode)) {
		return false;
	}
	return true;
}

//防止出现按下时在牌上，松开时离开牌面
void CardView::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event) {
	cocos2d::Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());

	Size size = getContentSize();
	cocos2d::Rect rect(0, 0, size.width, size.height);

	if (!rect.containsPoint(locationInNode)) {
		return;
	}


	//执行触摸回调函数
	if (_clickCallback) {
		CCLOG("CardView:成功调用触摸回调函数");
		_clickCallback(this);
	}
}
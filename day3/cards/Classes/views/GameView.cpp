#include "GameView.h"
#include "CardView.h"
#include "models/CardModel.h"
#include "configs/loaders/LevelConfigLoader.h"

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

	if (!LevelConfigLoader::loadLevelConfig("1", _levelConfig)) {
		CCLOG("GameView:无法加载场景");
		return false;
	}

	initCardsFromLevelConfig();

	return true;
}

void GameView::initCardsFromLevelConfig() {
	if (!_playfieldLayer && !_stackLayer) return;
	
	float playfieldHeight = _playfieldLayer->getContentSize().height;
	float stackHeight = _stackLayer->getContentSize().height;

	//记录当前卡牌编号
	int nextId = 0;

	//遍历实例化每一张主牌区卡牌
	for (const auto& cfg : _levelConfig.getPlayfieldCards()) {
		auto model = CardModel::create(nextId++, cfg.face, cfg.suit);
		auto view = CardView::create(model);

		_playfieldLayer->addChild(view);

		cocos2d::Vec2 localPos(cfg.position.x, cfg.position.y);
		view->setPosition(localPos);
	}

	//遍历实例化每一张手牌区卡牌
	for (const auto& cfg : _levelConfig.getStackCards()) {
		auto model = CardModel::create(nextId++, cfg.face, cfg.suit);
		auto view = CardView::create(model);

		_stackLayer->addChild(view);
		cocos2d::Vec2 localPos(cfg.position.x + nextId * view->getContentSize().width, cfg.position.y+view->getContentSize().height/2);
		view->setPosition(localPos);
	}
}

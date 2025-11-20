#include "GameView.h"
#include "CardView.h"
#include "models/CardModel.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "controllers/GameController.h"

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



	return true;
}

void GameView::addCardView(CardModel* model, cocos2d::Vec2 pos, bool isStack) {
    auto cardView = CardView::create(model);
    cardView->setPosition(pos);

    if (isStack) {
        _stackLayer->addChild(cardView);
        _stackCardViews.push_back(cardView);

        // 简单的顶牌判断逻辑可以保留在 View，或者由 Controller 指定
        _stackTopView = cardView;
        _stackTopPos = pos;
    }
    else {
        _playfieldLayer->addChild(cardView);
        _playfieldCardViews.push_back(cardView);
    }

    // 绑定点击事件
    cardView->setClickCallback([this](CardView* v) {
        if (_controller) _controller->onCardClicked(v);
        });
}

void GameView::runMoveToTopAnim(CardView* target, const std::function<void()>& onComplete) {
    if (!target) return;

    // 1. 防止从父节点移除时被自动释放
    target->retain();

    // 2. 获取世界坐标
    Vec2 worldPos = target->getParent()->convertToWorldSpace(target->getPosition());

    // 3. 从原父节点移除
    target->removeFromParent();

    // 4. 加到底牌区
    _stackLayer->addChild(target);

    // 5. 世界坐标转成 stackLayer 的本地坐标
    Vec2 startPosInStack = _stackLayer->convertToNodeSpace(worldPos);
    target->setPosition(startPosInStack);

    // 6. 释放引用
    target->release();

    // 7. 保证在底牌区最上面
    target->setLocalZOrder(100);

    // 8. 创建移动动作
    auto moveAction = MoveTo::create(0.3f, _stackTopPos);

    auto seq = Sequence::create(
        moveAction,
        CallFunc::create([this, target, onComplete]() {
            target->setPosition(_stackTopPos);
            this->updateStackTopView(target);
            if (onComplete) onComplete();
            }),
        nullptr
    );

    target->runAction(seq);
}
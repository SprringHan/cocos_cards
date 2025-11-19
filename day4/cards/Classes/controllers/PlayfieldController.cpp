#include "PlayfieldController.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "../views/GameView.h"
#include "../views/CardView.h"

void PlayfieldController::init(GameModel* model, GameView* view) {
    _model = model;
    _view = view;
}

bool PlayfieldController::isMatch(CardModel* cardA, CardModel* cardB) {
    if (!cardA || !cardB) return false;

    int faceA = (int)cardA->getFace();
    int faceB = (int)cardB->getFace();

    // 规则：点数差的绝对值为 1
    int diff = abs(faceA - faceB);

    // 基础匹配
    if (diff == 1) return true;

    // 可选规则：K 和 A 是否相连？
    // if ((faceA == CFT_ACE && faceB == CFT_KING) || (faceA == CFT_KING && faceB == CFT_ACE)) return true;

    return false;
}

void PlayfieldController::onCardClicked(CardView* cardView) {
    if (!_model || !_view || !cardView) return;

    CardModel* clickedCard = const_cast<CardModel*>(cardView->getModel());
    CardModel* topCard = _model->getTopCard();

    if (!topCard) return;

    // 1. 逻辑判断：是否匹配
    if (isMatch(clickedCard, topCard)) {
        CCLOG("Playfield: Match Success! Face %d vs Top %d", clickedCard->getFace(), topCard->getFace());

        // 2. 执行动画 (View层)
        // 注意：这里假设 GameView 实现了 runMoveToTopAnim 接口
        _view->runMoveToTopAnim(cardView, [this, clickedCard]() {

            // 3. 动画结束回调：更新数据 (Model层)
            // 旧的顶牌实际上被覆盖了（进入弃牌堆逻辑，这里简化为直接替换）

            // 从主牌区移除
            _model->removePlayfieldCard(clickedCard);
            // 设置为新的顶牌
            _model->setTopCard(clickedCard);
            });
    }
    else {
        CCLOG("Playfield: Match Failed.");
    }
}
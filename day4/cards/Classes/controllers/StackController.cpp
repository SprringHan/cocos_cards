#include "StackController.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "../views/GameView.h"
#include "../views/CardView.h"

void StackController::init(GameModel* model, GameView* view) {
    _model = model;
    _view = view;
}

void StackController::onCardClicked(CardView* cardView) {
    if (!_model || !_view || !cardView) return;

    CardModel* clickedModel = const_cast<CardModel*>(cardView->getModel());

    // 1. 验证合法性：必须是备用堆最上面那张
    CardModel* nextStackCard = _model->getNextStackCard();

    if (clickedModel != nextStackCard) {
        CCLOG("Stack: Clicked card is not the top of stack.");
        return;
    }

    CCLOG("Stack: Flip new card.");

    // 2. 执行动画
    _view->runMoveToTopAnim(cardView, [this, clickedModel]() {

        // 3. 更新数据
        // 从备用堆弹出
        _model->popStackCard();
        // 设为新的顶牌
        _model->setTopCard(clickedModel);
        });
}
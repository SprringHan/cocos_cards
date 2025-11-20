#include "StackController.h"
#include "../models/GameModel.h"
#include "../models/CardModel.h"
#include "../views/GameView.h"
#include "../views/CardView.h"
#include "../managers/UndoManager.h"

void StackController::init(GameModel* model, GameView* view, UndoManager* undoManager) {
    _model = model;
    _view = view;
    _undoManager = undoManager;
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

    // 先记录 Undo，再真正执行动画和 Model 更新
    if (_undoManager) {
        _undoManager->pushStackToTop(cardView);
    }

    // 2. 执行动画
    _view->runMoveToTopAnim(cardView, [this, clickedModel]() {
        _model->popStackCard();
        _model->setTopCard(clickedModel);
        });
}
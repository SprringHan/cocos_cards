#include "GameController.h"
#include "models/GameModel.h"
#include "views/CardView.h"
#include "views/GameView.h"
#include "configs/loaders/LevelConfigLoader.h"

USING_NS_CC;

GameController* GameController::create(GameView* view, GameModel* model) {
    auto ret = new(std::nothrow) GameController();
    if (ret && ret->init(view, model)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool GameController::init(GameView* view, GameModel* model) {
    _view = view;
    _model = model;

    _undoManager.init(model, view);

    _playfieldController.init(model, view, &_undoManager);
    _stackController.init(model, view, &_undoManager);

    _view->setController(this);
    _view->setController(this);

    LevelConfig levelConfig;
    if (!LevelConfigLoader::loadLevelConfig("1", levelConfig)) {
        return false;
    }

    int nextId = 0;

    for (const auto& cfg : levelConfig.getPlayfieldCards()) {
        // A. 创建数据 
        auto cardModel = CardModel::create(nextId++, cfg.face, cfg.suit);
        _model->addPlayfieldCard(cardModel); // 存入 GameModel

        // B. 创建视图
        _view->addCardView(cardModel, cfg.position, false);
    }

    nextId = 1000;

    for (const auto& cfg : levelConfig.getStackCards()) {
        // A. 创建数据
        auto cardModel = CardModel::create(nextId++, cfg.face, cfg.suit);
        _model->addStackCard(cardModel); // 存入 GameModel

        // B. 创建视图
        _view->addCardView(cardModel, cfg.position, true);
    }

    auto topModel = _model->getNextStackCard();
    if (topModel) {
        // 模型层设置
        _model->popStackCard();
        _model->setTopCard(topModel);
    }
    return true;
}

void GameController::onCardClicked(CardView* cardView) {
    if (!cardView || !cardView->getModel()) return;

    auto model = const_cast<CardModel*>(cardView->getModel());
    int cardId = model->getCardId();

    // 根据 ID 范围分发给不同的控制器
    // 假设 ID < 1000 是主牌区，>= 1000 是手牌区
    if (cardId < 1000) {
        _playfieldController.onCardClicked(cardView);
    }
    else {
        _stackController.onCardClicked(cardView);
    }
}
void GameController::onUndoClicked() {
    _undoManager.undo();
}
#include "GameScene.h"
#include "views/GameView.h"
#include "models/GameModel.h"
#include "controllers/GameController.h"

USING_NS_CC;

Scene* GameScene::createScene() {
    return GameScene::create();
}

bool GameScene::init() {
    if (!Scene::init()) return false;

    // 1. 创建数据模型 (Model)
    _gameModel = GameModel::create();
    _gameModel->retain(); // Scene持有数据，防止被自动释放

    // 2. 创建视图 (View)
    _gameView = GameView::create();
    this->addChild(_gameView, 0);

    // 3. 创建控制器 (Controller)
    // Controller 初始化时会自动调用 startGame 加载数据并填充 View
    _gameController = GameController::create(_gameView, _gameModel);
    _gameController->retain(); // 【关键】Controller不是Node，不会被addChild持有，必须手动retain！

    return true;
}

void GameScene::onExit() {
    // 释放内存，防止内存泄漏
    CC_SAFE_RELEASE(_gameController);
    CC_SAFE_RELEASE(_gameModel);
    Scene::onExit();
}
#include "GameScene.h"
#include "views/GameView.h"

USING_NS_CC;

Scene* GameScene::createScene() {
	return GameScene::create();
}

bool GameScene::init() {
	if (!Scene::init()) return false;

	_gameView = GameView::create();
	this->addChild(_gameView, 0);

	return true;
}
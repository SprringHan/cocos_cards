#pragma once
#include "cocos2d.h"

class GameView;
class GameController;
class GameModel;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    // 必须重写 onExit 以释放内存
    virtual void onExit() override;

    CREATE_FUNC(GameScene);

private:
    GameView* _gameView = nullptr;
    GameModel* _gameModel = nullptr;
    GameController* _gameController = nullptr;
};
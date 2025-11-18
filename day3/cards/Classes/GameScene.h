#pragma once
#include "cocos2d.h"
class GameView;

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GameScene);

private:
    GameView* _gameView;
};

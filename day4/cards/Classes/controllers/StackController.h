#pragma once
#include "cocos2d.h"

class GameModel;
class GameView;
class CardView;

class StackController {
public:
    void init(GameModel* model, GameView* view);
    void onCardClicked(CardView* cardView);

private:
    GameModel* _model = nullptr;
    GameView* _view = nullptr;
};
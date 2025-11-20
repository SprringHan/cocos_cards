#pragma once
#include "cocos2d.h"

class GameModel;
class GameView;
class CardView;
class UndoManager;

class StackController {
public:
    void init(GameModel* model, GameView* view, UndoManager* undoManager);
    void onCardClicked(CardView* cardView);

private:
    GameModel* _model = nullptr;
    GameView* _view = nullptr;
    UndoManager* _undoManager = nullptr;
};
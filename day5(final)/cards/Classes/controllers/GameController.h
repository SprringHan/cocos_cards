#pragma once
#include <cocos2d.h>
#include "PlayfieldController.h" // 包含子控制器
#include "StackController.h"     // 包含子控制器
#include "managers/UndoManager.h" //提供Undo接口

class CardView;
class GameView;
class GameModel;

class GameController : public cocos2d::Ref
{
public:
    static GameController* create(GameView* view, GameModel* model);
    void onCardClicked(CardView* cardview);
    void onUndoClicked();
private:
    GameController() = default;
    bool init(GameView* view, GameModel* model);

    GameView* _view = nullptr;
    GameModel* _model = nullptr;

    // 子控制器实例
    PlayfieldController _playfieldController;
    StackController _stackController;
    UndoManager _undoManager;
};
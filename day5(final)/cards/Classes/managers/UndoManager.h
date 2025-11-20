#pragma once

#include "cocos2d.h"
#include "../models/UndoModel.h"

class GameModel;
class GameView;
class CardView;
class CardModel;

// 管理 UndoModel，并负责真正执行“撤销”
class UndoManager
{
public:
    UndoManager();
    ~UndoManager();

    // 在 GameController 初始化时调用
    void init(GameModel* model, GameView* view);

    // 在“动作真正发生之前”由各个 Controller 调用，压入撤销记录
    void pushStackToTop(CardView* sourceView);
    void pushPlayfieldToTop(CardView* sourceView);

    bool canUndo() const;
    void undo();

private:
    GameModel* _gameModel = nullptr;
    GameView* _gameView = nullptr;
    UndoModel* _undoModel = nullptr;

    CardView* findCardViewForModel(CardModel* model) const;

    void undoStackToTop(const UndoRecord& rec);
    void undoPlayfieldToTop(const UndoRecord& rec);
};
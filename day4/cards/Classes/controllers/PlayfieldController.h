#pragma once
#include "cocos2d.h"
// 前置声明，减少编译依赖
class GameModel;
class GameView;
class CardView;
class CardModel;

class PlayfieldController {
public:
    void init(GameModel* model, GameView* view);
    void onCardClicked(CardView* cardView);

private:
    GameModel* _model = nullptr;
    GameView* _view = nullptr;

    // 核心算法：判断两张牌是否匹配
    bool isMatch(CardModel* cardA, CardModel* cardB);
};
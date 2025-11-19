#pragma once

#include "cocos2d.h"
#include "configs/models/LevelConfig.h"

// 这两个是标准库类型，需要头文件
#include <vector>
#include <functional>

// 前置声明：告诉编译器“这个名字是个类，具体长什么样以后再说”
class GameController;
class CardModel;
class CardView;

class GameView : public cocos2d::Node
{
public:
    virtual bool init();
    CREATE_FUNC(GameView);

    // 提供 Controller 接口调用这些区域
    cocos2d::Node* getPlayFieldLayer() const { return _playfieldLayer; }
    cocos2d::Node* getStackLayer() const { return _stackLayer; }

    // 提供给 GameController 接口
    void setController(GameController* controller) { _controller = controller; }

    // isStack: true 表示放底牌区，false 表示放主牌区
    void addCardView(CardModel* model, cocos2d::Vec2 pos, bool isStack);

    // 顶牌视图：直接在类内写小函数没问题
    void updateStackTopView(CardView* newTop) { _stackTopView = newTop; }

    // 这里要用 std::function<void()>，所以必须 <functional> + 前置声明 CardView
    void runMoveToTopAnim(CardView* target, const std::function<void()>& onComplete);

private:
    cocos2d::Node* _playfieldLayer = nullptr;  // 主牌区
    cocos2d::Node* _stackLayer = nullptr;      // 底牌区

    // 在视图上的管理
    std::vector<CardView*> _playfieldCardViews;
    std::vector<CardView*> _stackCardViews;

    CardView* _stackTopView = nullptr; // 当前顶牌
    cocos2d::Vec2 _stackTopPos;

    GameController* _controller = nullptr; // 控制器接口
};

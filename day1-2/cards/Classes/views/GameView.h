#pragma once

#include "cocos2d.h"

class GameView: public cocos2d::Node
{
public:
	virtual bool init();
	CREATE_FUNC(GameView);

	//提供Controller接口调用这些区域
	cocos2d::Node* getPlayFieldLayer() const { return _playfieldLayer; }
	cocos2d::Node* getStackLayer() const { return _stackLayer; }

	//提供Controller获取手牌坐标
	cocos2d::Vec2 getTopCardPos() const { return _topCardPos; }
private:
	cocos2d::Node* _playfieldLayer = nullptr;  //主牌区
	cocos2d::Node* _stackLayer = nullptr;  //底牌区
	cocos2d::Vec2 _topCardPos;  //手牌坐标
};


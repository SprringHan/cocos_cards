#pragma once

#include "cocos2d.h"
#include "configs/models/LevelConfig.h"

class GameView: public cocos2d::Node
{
public:
	virtual bool init();
	CREATE_FUNC(GameView);

	//提供Controller接口调用这些区域
	cocos2d::Node* getPlayFieldLayer() const { return _playfieldLayer; }
	cocos2d::Node* getStackLayer() const { return _stackLayer; }


private:
	cocos2d::Node* _playfieldLayer = nullptr;  //主牌区
	cocos2d::Node* _stackLayer = nullptr;  //底牌区

	LevelConfig _levelConfig;

	void initCardsFromLevelConfig();

};


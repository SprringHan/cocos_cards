#pragma once

#include<vector>
#include "cocos2d.h"
#include "GameConfig.h"

struct LevelCardConfig {
	CardSuitType suit;
	CardFaceType face;
	cocos2d::Vec2 position;
};

class LevelConfig {
public:
	const std::vector<LevelCardConfig> getPlayfieldCards() { return _playfieldCards; }
	const std::vector<LevelCardConfig> getStackCards() { return _stackCards; }


	std::vector<LevelCardConfig> _playfieldCards;
	std::vector<LevelCardConfig> _stackCards;
};
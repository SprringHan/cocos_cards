#pragma once

#include <string>
#include "configs/models/LevelConfig.h"
class LevelConfigLoader
{
public:
	static bool loadLevelConfig(const std::string& levelId,LevelConfig& outConfig);
};


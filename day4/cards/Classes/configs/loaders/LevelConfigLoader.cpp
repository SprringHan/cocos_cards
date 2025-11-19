#include "LevelConfigLoader.h"
#include "cocos2d.h"
#include "json/document.h"
#include "utils/StackLayoutUtil.h"

USING_NS_CC;
//若使用下方命名空间，Value会冲突
using namespace rapidjson;

//获取关卡文件信息 
std::string makeLevelFilePath(const std::string levelId) {
	return "configs/levels/level"+levelId+".json";
}

bool LevelConfigLoader::loadLevelConfig(const std::string& levelId, LevelConfig& outConfig) {
	std::string path = makeLevelFilePath(levelId);

	std::string jsonStr = FileUtils::getInstance()->getStringFromFile(path);
    if (jsonStr.empty()) {
        CCLOG("LevelConfigLoader: failed to read file: %s", path.c_str());
        return false;
    }

    Document doc;
    doc.Parse(jsonStr.c_str());
    if (doc.HasParseError() || !doc.IsObject()) {
        CCLOG("LevelConfigLoader: JSON parse error: %s", path.c_str());
        return false;
    }

    outConfig._playfieldCards.clear();
    outConfig._stackCards.clear();

    // 解析 Playfield 数组
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& arr = doc["Playfield"];
        for (SizeType i = 0; i < arr.Size(); ++i) {
            const rapidjson::Value& obj = arr[i];
            if (!obj.IsObject()) continue;

            LevelCardConfig cfg;

            cfg.face = static_cast<CardFaceType>(obj["CardFace"].GetInt());
            cfg.suit = static_cast<CardSuitType>(obj["CardSuit"].GetInt());

            const rapidjson::Value& pos = obj["Position"];
            cfg.position.x = pos["x"].GetFloat();
            cfg.position.y = pos["y"].GetFloat();

            outConfig._playfieldCards.push_back(cfg);
        }
    }

    // 解析 Stack 数组
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& arr = doc["Stack"];
        for (SizeType i = 0; i < arr.Size(); ++i) {
            const rapidjson::Value& obj = arr[i];
            if (!obj.IsObject()) continue;

            LevelCardConfig cfg;

            cfg.face = static_cast<CardFaceType>(obj["CardFace"].GetInt());
            cfg.suit = static_cast<CardSuitType>(obj["CardSuit"].GetInt());

            if (obj.HasMember("Position") && obj["Position"].IsObject()) {
                const rapidjson::Value& pos = obj["Position"];
                cfg.position.x = pos["x"].GetFloat();
                cfg.position.y = pos["y"].GetFloat();
            }
            else {
                cfg.position.x = 0.0f;
                cfg.position.y = 0.0f;
            }

            outConfig._stackCards.push_back(cfg);
        }

        StackLayoutUtil::applyStackLayout(outConfig._stackCards);
    }

    return true;
}
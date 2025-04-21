/**
 * LevelConfigLoader.cpp
 * 关卡配置加载器实现
 */

#include "LevelConfigLoader.h"
#include "json/document.h"
#include "json/stringbuffer.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId)
{
    // 构建关卡配置文件路径，从 Resources 目录加载
    std::string levelFile = StringUtils::format("res/levels/level_%d.json", levelId);
    //CCLOG("LevelConfigLoader::loadLevelConfig - Trying to load file: %s", levelFile.c_str());
    
    // 加载JSON文件内容
    std::string jsonContent = FileUtils::getInstance()->getStringFromFile(levelFile);
    if (jsonContent.empty()) {
        // 如果找不到指定关卡，加载默认关卡
        //CCLOG("LevelConfigLoader::loadLevelConfig - File not found, trying default level");
        levelFile = "res/levels/default_level.json";
        jsonContent = FileUtils::getInstance()->getStringFromFile(levelFile);
        
        // 如果默认关卡也找不到，尝试不同的路径
        if (jsonContent.empty()) {
            levelFile = "Resources/res/levels/default_level.json";
            jsonContent = FileUtils::getInstance()->getStringFromFile(levelFile);
        }
        
        if (jsonContent.empty()) {
            //CCLOG("LevelConfigLoader::loadLevelConfig - Failed to load level config: %s and default level", levelFile.c_str());
            
            // 尝试打印资源路径
            auto searchPaths = FileUtils::getInstance()->getSearchPaths();
            //CCLOG("Search paths:");
            for (const auto& path : searchPaths) {
                //CCLOG("  %s", path.c_str());
            }
            
            return nullptr;
        }
        //CCLOG("LevelConfigLoader::loadLevelConfig - Loaded default level successfully");
    } else {
        //CCLOG("LevelConfigLoader::loadLevelConfig - Loaded level file successfully");
    }
    
    return parseFromJson(jsonContent);
}

LevelConfig* LevelConfigLoader::parseFromJson(const std::string& jsonString)
{
    rapidjson::Document doc;
    doc.Parse(jsonString.c_str());
    
    if (doc.HasParseError() || !doc.IsObject()) {
        //CCLOG("Failed to parse level config JSON");
        return nullptr;
    }
    
    LevelConfig* config = new LevelConfig();
    
    // 解析主牌区卡牌配置
    if (doc.HasMember("Playfield") && doc["Playfield"].IsArray()) {
        const rapidjson::Value& playfield = doc["Playfield"];
        std::vector<CardConfig> playfieldCards;
        
        for (rapidjson::SizeType i = 0; i < playfield.Size(); i++) {
            const rapidjson::Value& card = playfield[i];
            if (card.IsObject() && card.HasMember("CardFace") && card.HasMember("CardSuit") && card.HasMember("Position")) {
                CardConfig cardConfig;
                cardConfig.cardFace = card["CardFace"].GetInt();
                cardConfig.cardSuit = card["CardSuit"].GetInt();
                
                if (card["Position"].IsObject() && card["Position"].HasMember("x") && card["Position"].HasMember("y")) {
                    cardConfig.position.x = card["Position"]["x"].GetFloat();
                    cardConfig.position.y = card["Position"]["y"].GetFloat();
                }
                
                playfieldCards.push_back(cardConfig);
            }
        }
        
        config->setPlayfieldCards(playfieldCards);
    }
    
    // 解析备用牌堆卡牌配置
    if (doc.HasMember("Stack") && doc["Stack"].IsArray()) {
        const rapidjson::Value& stack = doc["Stack"];
        std::vector<CardConfig> stackCards;
        
        for (rapidjson::SizeType i = 0; i < stack.Size(); i++) {
            const rapidjson::Value& card = stack[i];
            if (card.IsObject() && card.HasMember("CardFace") && card.HasMember("CardSuit")) {
                CardConfig cardConfig;
                cardConfig.cardFace = card["CardFace"].GetInt();
                cardConfig.cardSuit = card["CardSuit"].GetInt();
                
                // 堆牌位置可能不重要，但仍然处理
                if (card.HasMember("Position") && card["Position"].IsObject() && 
                    card["Position"].HasMember("x") && card["Position"].HasMember("y")) {
                    cardConfig.position.x = card["Position"]["x"].GetFloat();
                    cardConfig.position.y = card["Position"]["y"].GetFloat();
                }
                
                stackCards.push_back(cardConfig);
            }
        }
        
        config->setStackCards(stackCards);
    }
    
    return config;
}
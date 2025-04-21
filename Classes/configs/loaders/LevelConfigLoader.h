/**
 * LevelConfigLoader.h
 * 关卡配置加载器，负责加载和解析关卡配置文件
 */

#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "cocos2d.h"
#include "../models/LevelConfig.h"

/**
 * 关卡配置加载器类，负责加载关卡配置
 */
class LevelConfigLoader
{
public:
    /**
     * 加载关卡配置
     * @param levelId 关卡ID
     * @return 关卡配置对象
     */
    static LevelConfig* loadLevelConfig(int levelId);
    
private:
    /**
     * 从JSON文件中解析关卡配置
     * @param jsonString JSON字符串
     * @return 关卡配置对象
     */
    static LevelConfig* parseFromJson(const std::string& jsonString);
};

#endif // __LEVEL_CONFIG_LOADER_H__ 
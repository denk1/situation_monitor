#ifndef CONFIG_MONITOR_H
#define CONFIG_MONITOR_H
#include <map>
#include "OgreRoot.h"

struct ConfigMonitor: public Ogre::Singleton<ConfigMonitor> {
public:
    ConfigMonitor() = default;
    void setConfig(Ogre::String cf);
    std::map<std::string, std::string> mConfigItems;
};


#endif
#include <iostream>
#include "ConfigMonitor.h"
#include <OgreConfigFile.h>

using namespace Ogre;

template <>
ConfigMonitor *Ogre::Singleton<ConfigMonitor>::msSingleton = 0;

void ConfigMonitor::setConfig(String str_conf_file) {
    Ogre::ConfigFile cf;
    try {
        cf.load(str_conf_file);
    } catch(Exception e) {
        std::cout << "the error of openning the network configuration file:" << " " << e.what() << std::endl;
    }
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            mConfigItems[i->first] = i->second;
            //Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
            //    archName, typeName, secName);
        }
    }
}
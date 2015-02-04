#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <map>
#include <stdexcept>
using namespace std;

#include <SDL/SDL.h>

#include "Configuration.hpp"
#include "HelperFunctions.hpp"

/**
 * Constructs a configuration object.
 */
Configuration::Configuration()
{
    // determine the configuration direcory
    determineConfigDir();

    // ensure the directory exists
    try
    {
        isAccessible(configurationDirectory);
    } catch(runtime_error &e) {
#ifdef _WIN32
        system(("MD " + configurationDirectory).c_str());
#else
        system(("mkdir " + configurationDirectory).c_str());
#endif

        // we just issued this command so give it some time to execute
        SDL_Delay(100);

        // ensure that it exists
        try
        {
            isAccessible(configurationDirectory);
        } catch(runtime_error &e) {
            throw runtime_error("Unable to create the missing configuration directory.");
        }
    }

    // determine the configuration filename
    determineConfigFile();

    // now load the default configuration
    defaultConfiguration();

    // lastly load the user's configuration
    loadSettings();

    // NOTE: by working in this order we can ensure the user's configuration is loaded
    // and at the same time parameters that were omitted ascertain values anyway
}

/**
 * Sets the configuration settings in memory to their default values.
 */
void Configuration::defaultConfiguration()
{
    theSettings["PlaylistEnabled"].BoolSetting = false;
    theSettings["CdromEnabled"].BoolSetting = false;
    theSettings["Sound3DEnabled"].BoolSetting = false;
    theSettings["DisplayFPS"].BoolSetting = true;
    theSettings["ScreenMode"].ModeSetting = MODE2;
    theSettings["SkillLevel"].SkillSetting = NORMAL;
    theSettings["EffectVolume"].IntegerSetting = 5;
    theSettings["MusicVolume"].IntegerSetting = 5;
    theSettings["UseCdromNumber"].IntegerSetting = 0;
    theSettings["UseJoystickNumber"].IntegerSetting = 0;
    theSettings["PlaylistFilename"].StringSetting = "NoPlaylist";
    theSettings["ProfileName"].StringSetting = "NoName";
    theSettings["DataDirectory"].StringSetting = "DataDirectoryHere"; // TODO: make better assumption
}

/**
 * Loads settings from the application's configuration file to the system's memory.
 */
void Configuration::loadSettings()
{
    // open file handle/descriptor
    ifstream configurationData(configurationFilename.c_str());

    // if the file is openable...
    string setting;
    string settingName;
    string settingValue;
    int firstBlank = 0;
    int firstNewline = 0;

    if (configurationData.is_open())
    {
        while (!configurationData.eof())
        {
            // ...load all the configuration data
            getline(configurationData, setting);
            firstBlank = setting.find_first_of(' ', 0);
            settingName = setting.substr(0, firstBlank);
            firstNewline = setting.find_first_of('\n', firstBlank);
            settingValue = setting.substr(firstBlank + 1, firstNewline);

            // assign configuration data
            if (settingName == "PlaylistEnabled" || settingName == "CdromEnabled" || settingName == "Sound3DEnabled")
            {
                theSettings[settingName.c_str()].BoolSetting = atoi(settingValue.c_str()) ? true : false;
            }
            else if (settingName == "PlaylistFilename" || settingName == "ProfileName" || settingName == "DataDirectory")
            {
                theSettings[settingName.c_str()].StringSetting = settingValue.c_str();
            }
            else if (settingName == "ScreenMode")
            {
                theSettings[settingName.c_str()].ModeSetting = static_cast<ScreenMode>(atoi(settingValue.c_str()));
            }
            else if (settingName == "SkillLevel")
            {
                theSettings[settingName.c_str()].SkillSetting = static_cast<SkillLevel>(atoi(settingValue.c_str()));
            }
            else
            {
                theSettings[settingName.c_str()].IntegerSetting = atoi(settingValue.c_str());
            }
        }

        // ...close file handle/descriptor
        configurationData.close();
    }
}

/**
 * Saves the settings that are in memory to the application's
 * configuration file.
 */
void Configuration::saveSettings() const
{
    // open file handle/descriptor
    ofstream configurationData(configurationFilename.c_str());

    // if the file is openable...
    if (configurationData.is_open())
    {
        // save all the configuration data
        for(ConfigMap::const_iterator i = theSettings.begin(); i != theSettings.end(); ++i )
        {
            if (i->first == "PlaylistEnabled" || i->first == "CdromEnabled" || i->first == "Sound3DEnabled")
            {
                configurationData << i->first << " " << (i->second).BoolSetting << endl;
            }
            else if (i->first == "PlaylistFilename" || i->first == "ProfileName" || i->first == "DataDirectory")
            {
                configurationData << i->first << " " << (i->second).StringSetting << endl;
            }
            else if (i->first == "ScreenMode")
            {
                configurationData << i->first << " " << (i->second).ModeSetting << endl;
            }
            else if (i->first == "SkillLevel")
            {
                configurationData << i->first << " " << (i->second).SkillSetting << endl;
            }
            else if (i->first == "EffectVolume" || i->first == "MusicVolume" || i->first == "UseCdromNumber" || i->first == "UseJoystickNumber")
            {
                configurationData << i->first << " " << (i->second).IntegerSetting << endl;
            }
        }

        // close file handle/descriptor
        configurationData.close();
    }
}

/**
 * Determined what directory is used to store configuration settings.
 */
void Configuration::determineConfigDir()
{
#ifdef _WIN32
    configurationDirectory = getenv("HOMEPATH");
    configurationDirectory += "\.mysticduels.dat";
#elif macintosh
    configurationDirectory = getenv("HOMEPATH");
    configurationDirectory += "\.mysticduels.cfg";
#else
    configurationDirectory = getenv("HOME");
    configurationDirectory += "/.mysticduels";
#endif
}

/**
 * Determined what file is used to store configuration settings.
 */
void Configuration::determineConfigFile()
{
#ifdef _WIN32
    configurationFilename = configurationDirectory + "\settings.dat";
#else
    configurationFilename = configurationDirectory + "/settings.conf";
#endif
}

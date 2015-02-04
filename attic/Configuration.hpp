#ifndef CONFIGURATION_HPP
#define	CONFIGURATION_HPP

#include <string>
#include <map>

#include "Singleton.hpp"

/**
 * Symbolizes all the user-defined configuration data.
 */
class Configuration: public Singleton<Configuration>
{
    public:
        /** Available screen modes for the application */
        enum ScreenMode {
            MODE1,  /**< 640 x 480 */
            MODE2,  /**< 800 x 600 */
            MODE3,  /**< 1024 x 768 */
            MODE4   /**< 1280 x 960 */
        };

        /** Available skill levels for the application*/
        enum SkillLevel {
            CHICKEN,    /**< unbelievably easy skill level */
            EASY,       /**< moderately easy skill level */
            NORMAL,     /**< average difficulty skill level */
            HARD,       /**< hard skill level */
            MASTER      /**< very hard skill level */
        };

        /** A union that symbolizes a single setting */
        struct SingleSetting {
            std::string StringSetting;        /**< stores a setting value that is a string format */
            bool BoolSetting;           /**< stores a setting value that is a boolean format */
            float FloatSetting;         /**< stores a setting value that is a float format */
            int IntegerSetting;         /**< stores a setting value that is a integer format */
            ScreenMode ModeSetting;     /**< Saves a value related to the screen's resolution */
            SkillLevel SkillSetting;    /**< Saves a value related to the difficulty level */
        };

        typedef std::map<std::string, SingleSetting> ConfigMap;

	public:
		Configuration();
		~Configuration()
        {
            saveSettings();
        }

        // accessor methods
		ConfigMap* getSettings()
			{ return &theSettings; }

    protected:
        void defaultConfiguration();
		void loadSettings();
        void saveSettings() const;
        void determineConfigDir();
        void determineConfigFile();

	private:
		ConfigMap theSettings;              /**< map containing all the settings used by the game */
        std::string configurationFilename;  /**< filename of the configuration file */
        std::string configurationDirectory; /** directory all configuration info is stored */
};

#endif	/* CONFIGURATION_HPP */


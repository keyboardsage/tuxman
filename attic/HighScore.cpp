#include <iostream>
#include <string>
#include <list>
using namespace std;

#include "HighScore.hpp"
#include "Management.hpp"

namespace Chromium2
{
	/**
	 * Sets the high score list to the default scores.
	 */
	void HighScore::defaultHighScore()
	{
		HighScoreRecord theDefaultRecord = { "Nobody", 1000, 3 };
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
		theScores.push_back(theDefaultRecord);
	}

	/**
	 * Loads the high score list using the high score configuration
	 * file on the system.
	 */
	void HighScore::loadHighScore()
	{
		// status message
		cout << "(II) Loading high scores..." << endl;
	}

	/**
	 * Saves the high score list that is in memory to the high score
	 * configuration file.
	 */
        void HighScore::saveHighScore() const
	{
		// status message
		cout << "(II) Saving high scores..." << endl;
	}

	void HighScore::determineScoreFile()
        {
		// get a handle to the Management class so we can determine the config directory
                Management* theManagerPtr = Management::instance();
                string strConfigDir = theManagerPtr->getConfigDir();

                // determine the high score file's filename for the system in question
#ifdef _WIN32
                strScoreFilename = strConfigDir + "/scores.dat";
#elif macintosh
                strScoreFilename = strConfigDir + "/scores.cfg";
#else
                strScoreFilename = strConfigDir + "/scores.conf";
#endif
        }
}

#ifndef _HighScore_HPP
#define	_HighScore_HPP

#include <string>
#include <list>

#include "Management.hpp"

namespace Chromium2
{
	/**
	 * Manages the HighScoreList for the application.
	 */
	class HighScore {
        public:
                /** Structure defining a high score record */
		struct HighScoreRecord {
			std::string Name; /**< user's name */
			unsigned int Score; /**< user's score */
			unsigned int Stage; /**< stage score was acquired on */
		};

                 typedef std::list<HighScoreRecord> HighScoreList;

	public:
		HighScore()
                {
                        // determine the file path to the score file
                        determineScoreFile();

                       // if a high score file exists
		       if (Management::isAccessible(strScoreFilename))
                                loadHighScore();
		       else
                                defaultHighScore();
		}

		~HighScore()
                {
                       // just make sure the user's settings are saved so they can be used next time
                       saveHighScore();
                }

		void defaultHighScore();
		void loadHighScore();
                void saveHighScore() const;

                // accessor methods
		const HighScoreList* getScores() const
			{ return &theScores; }

        private:
		void determineScoreFile();
	
	private:
		HighScoreList theScores; /**< lost of the high scores used by the application */
                std::string strScoreFilename; /**< filename of the high score file */
	};
}

#endif	/* _HighScore_HPP */

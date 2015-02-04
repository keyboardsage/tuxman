#ifndef TUXMAN_HPP
#define TUXMAN_HPP

#include "ScreenObject.hpp"

	class Tuxman: public ScreenObject {
		public:
			Tuxman(ScreenObjectInfo& objInfo);
			Tuxman(int livesParam, int scoreParam, ScreenObjectInfo& objInfo);
			~Tuxman();

			virtual void draw();
			void accumScore(int intAmount);

			// accessor methods
			int getScore() const;
			void setLivesLeft(int intLivesParam);
			int getLivesLeft() const;

		private:
			int lives; /**< Lives Tuxman has left before game over */
			int score; /**< Score that Tuxman has accumulated */
	};

#endif

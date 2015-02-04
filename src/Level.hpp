#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <fstream>

#include "Tile.hpp"

	class Level {
		public:
			explicit Level(int intLevelPram);
			Level(const Level &LevelParam);
			~Level();

			void buildUpLevel();
			void renderLevel() const;
            void tearDownLevel();

            char tileType(int pos) const;
            bool tileHasFood(int pos) const;
            void tileRemoveFood(int pos);

			// accessor methods
			void setLevel(int intLevelParam);
			const int getLevel() const;

        protected:
            void buildUpLevelHelp(std::ifstream* levelStream, float currentX, float currentY);
            void recordTile(Tile* unloggedTile);

		private:
			int intLevel; /**< the current level */
			std::string file; /**< file containing level information **/
			ObjectMap* objMap; /**< map of position and tiles. */
	};

#endif

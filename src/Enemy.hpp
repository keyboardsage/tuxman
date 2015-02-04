#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "ScreenObject.hpp"

	class Enemy: public ScreenObject {
		public:
			Enemy(ScreenObjectInfo& objInfo);
			~Enemy();

			virtual void draw();

		private:
			//EnemyType type; /**< The Type of enemy Lives Tuxman has left before game over */
	};

#endif

#ifndef GAMELOOP_HPP
#define GAMELOOP_HPP

#include "SDL.h"

#include "Level.hpp"
#include "FramesPerSecond.hpp"
#include "Tuxman.hpp"
#include "Enemy.hpp"

class GameLoop {
    public:
        GameLoop();
        ~GameLoop();

        void run();

    protected:
        int eatFood() const;
        bool isfirstXSecondsOfStagePast(unsigned int seconds) const;
        void adjustTuxman(Uint32 currentKeyPressed);
        void handleEventsHelper();
        void handleEvents();
        void setUpLevel();
        void tearDownLevel();
        int positionToRow(int position) const;
        int positionToColumn(int position) const;
        int xToColumn(float x) const;
        int yToRow(float y) const;
        float invertY(float coordY) const;

    private:
        bool isDone;
        Level* level;
        FramesPerSecond* fps;
        SDL_Event event;
        Tuxman* tuxman;
        Enemy* sealEnemy;
        Enemy* sharkEnemy;
        Enemy* whaleEnemy;
        Enemy* hawkEnemy;
        Uint32 navStartTime; /**< Time a navigation key was last pressed down */
        float originalTuxmanX; /**< Tuxman's original x position when key was pressed */
        float originalTuxmanY; /**< Tuxman's original y position when key was pressed */
        Uint32 lastKeyPressed; /**< The last navigation key pressed */
        Uint32 stageBeginTime; /**< The time at which the stage began */
        bool isLevelBeginning; /**< The beginning of level flag */
        Uint32 queuedDesiredDirection;
        Uint32 delayKeyPressOf; /**< The key to press after some interval. */
        Uint32 delayKeyPressStart; /**< The time when interval starts. */
        Uint32 delayKeyPressInterval; /**< The time that should elapse before pressing button. */
};

#endif

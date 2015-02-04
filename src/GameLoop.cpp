#include "GameLoop.hpp"
#include "GraphicStreamOut.hpp"
#include "DisplayInfo.hpp"
#include "Configuration.hpp"

#include <iostream>
#include <cmath>

extern Configuration config;
extern DisplayInfo info;

/**
 * Constructs the game loop object.
 */
GameLoop::GameLoop() {
    isDone = false;
    level = NULL;
    fps = NULL;
    events = NULL;
    tuxman = NULL;
    sealEnemy = NULL;
    sharkEnemy = NULL;
    whaleEnemy = NULL;
    hawkEnemy = NULL;
    navStartTime = 0;
    originalTuxmanX = 0;
    originalTuxmanY = 0;
    lastKeyPressed = SDLK_RETURN; // RETURN key used as flag to let us know stage just started
    stageBeginTime = 0;
    isLevelBeginning = true;
    queuedDesiredDirection = SDLK_LEFT;
    delayKeyPressOf = SDLK_UNKNOWN;
    delayKeyPressStart = 0;
    delayKeyPressInterval = 0;
}

/**
 * Deconstructs this object.
 */
GameLoop::~GameLoop() {

}

/**
 * Runs the game.
 * @todo if to many bad frames have been tallied, throw an exception and print message
 * @todo cerr << "(EE) It is impossible to guarantee the target fps rate." << endl;
 */
void GameLoop::run() {
    setUpLevel();

    do {
        fps->frameStart();

        // handle input and change objects accordingly
        handleEvents(events);
        if (delayKeyPressStart != 0 && (SDL_GetTicks() - delayKeyPressStart) >= delayKeyPressInterval) // note: ensures queued event happens under special cases
        {
            // DEBUGGING: std::cout << "NOW" << std::endl;SDL_Delay(5000);
            SDL_Event delayedKeyPressEvent;
            delayedKeyPressEvent.type = SDL_KEYDOWN;
            delayedKeyPressEvent.key.keysym.sym = static_cast<SDLKey>(delayKeyPressOf);
            SDL_PushEvent(&delayedKeyPressEvent);
            delayKeyPressStart = 0;
        }

        // eat food & calculate new score
        tuxman->accumScore(eatFood());
        // DEBUGGING: std::cout << tuxman->getScore() << std::endl;

        // use algorithm to move enemies

        // make big dots blink

        // render all these changes
        level->renderLevel();
        gout << static_cast<ScreenObject*>(tuxman);
        gout << static_cast<ScreenObject*>(sealEnemy);
        gout << static_cast<ScreenObject*>(sharkEnemy);
        gout << static_cast<ScreenObject*>(whaleEnemy);
        gout << static_cast<ScreenObject*>(hawkEnemy);
        gout.flush();

        // check for collision
            // reset Tuxman if necessary
            // otherwise end level (loser)

        // check to see if all food has been eaten (winner)

        // keep gameplay smooth
        fps->frameEnd();
        fps->updateFrameCount();
        fps->printStatus();
    } while (!isDone);

    tearDownLevel();
}

/**
 * Removes food from the screen after its been eaten by Tuxman.
 * @todo decompose code for calculating position into separate method, adjustTuxman uses same code
 * @todo throw runtime error if foodtype reaches default case
 * @return the score associated with the food eaten.
 */
int GameLoop::eatFood() const {
    // convert OpenGL coord into level file coord
    int myColumn = (lastKeyPressed != SDLK_LEFT) ? xToColumn(tuxman->getX()) : xToColumn(tuxman->getX() + (0.75 * config.real_tile_width));
    int myRow = (lastKeyPressed != SDLK_UP) ? yToRow(invertY(tuxman->getY())) : yToRow(invertY(tuxman->getY() - (0.75 * config.real_tile_width)));
    int myPosition = ((myRow-1) * config.map_width) + myColumn; // note: If your on row 24, then there are 23 full rows before it

    // if Tuxman's position currently overlaps a tile's food...
    char foodType = level->tileType(myPosition);
    int worthScore;
    if (foodType != 'b' && level->tileHasFood(myPosition))
    {
        // ...then determine the food's worth (by using the tile type)
        switch (foodType) {
            case 'a':
                worthScore = 10;
                break;
            default:
                break;
        }
    } else worthScore = 0; // ...otherwise its worth 0

    // remove the food from the tile
    level->tileRemoveFood(myPosition);

    return worthScore;
}

/**
 * Calculates whether the first X seconds of a level have past.
 * @param seconds the number of seconds to wait
 * @return True if x seconds have past, otherwise false.
 */
bool GameLoop::isfirstXSecondsOfStagePast(unsigned int seconds) const {
    Uint32 timeElapsedSinceStageBegin = 0;

    timeElapsedSinceStageBegin = SDL_GetTicks() - stageBeginTime;

    if (timeElapsedSinceStageBegin < (seconds * 1000)) return false;

    return true;
}

/**
 * Adjusts the Tuxman's current coordinates in response to recent input.
 * @todo This method is to long it should be chopped up.
 * @todo This method needs to be cleaned up(possibly reimplemented), it is terribly hackish and complicated.
 * @todo Teleporting vertically needs to be implemented.
 * @param currentKeyPressed the event requested by the user
 */
void GameLoop::adjustTuxman(Uint32 currentKeyPressed) {
    // convert OpenGL coord into level file coord
    int myColumn = xToColumn(tuxman->getX());
    int myRow = yToRow(invertY(tuxman->getY()));
    int myPosition = ((myRow-1) * config.map_width) + myColumn; // note: If your on row 24, then there are 23 full rows before it

    // calculate displacement
    int elapsedTime = SDL_GetTicks() - navStartTime; // in milliseconds
    int averageTileSize = (config.real_tile_width + config.real_tile_height) / 2;
    float rate = (averageTileSize * 4) / 1000.0; // (4 times average tile size) pixels per 1 second
    float displacement = rate * elapsedTime;

    // if the lastKeyPressed is the same key desired it is business as usual...
    if (lastKeyPressed == currentKeyPressed)
    {
        // if Tuxman's direction is contrary to desired(queued) direction...
        if ( (currentKeyPressed == SDLK_UP || currentKeyPressed == SDLK_DOWN) && queuedDesiredDirection != currentKeyPressed ) {
            // change direction if desired direction is...
            if (queuedDesiredDirection == SDLK_LEFT && // left
                level->tileType(myPosition - 1) != 'b' && // unblocked
                myRow != yToRow(invertY(displacement + originalTuxmanY))) // Tuxman ready to change direction
            {
                tuxman->setY(invertY((myRow-1)*config.real_tile_height));
                adjustTuxman(SDLK_LEFT);
                return;
            } else if (queuedDesiredDirection == SDLK_RIGHT &&
                level->tileType(myPosition + 1) != 'b' &&
                myRow != yToRow(invertY(displacement + originalTuxmanY)))
            {
                tuxman->setY(invertY((myRow-1)*config.real_tile_height));
                adjustTuxman(SDLK_RIGHT);
                return;
            }
        } else if ( (currentKeyPressed == SDLK_LEFT || currentKeyPressed == SDLK_RIGHT) && queuedDesiredDirection != currentKeyPressed ) { // contrary to desired
            // change direction if desired direction is...
            if (currentKeyPressed == SDLK_RIGHT && queuedDesiredDirection == SDLK_UP && // up
                level->tileType(myPosition - config.map_width) != 'b' && // unblocked
                (tuxman->getX()+config.real_tile_width) >= ((myColumn+1)*config.real_tile_width)) // Tuxman ready to change direction
            {
                tuxman->setX(myColumn*config.real_tile_width);
                adjustTuxman(SDLK_UP);
                return;
            } else if (currentKeyPressed == SDLK_RIGHT && queuedDesiredDirection == SDLK_DOWN &&
                level->tileType(myPosition + config.map_width) != 'b' &&
                (tuxman->getX()+config.real_tile_width) >= ((myColumn+1)*config.real_tile_width))
            {
                tuxman->setX(myColumn*config.real_tile_width);
                adjustTuxman(SDLK_DOWN);
                return;
            }

            if (currentKeyPressed == SDLK_LEFT && queuedDesiredDirection == SDLK_UP && // up
                level->tileType(myPosition - config.map_width) != 'b' && // unblocked
                myColumn != xToColumn(originalTuxmanX - displacement)) // Tuxman ready to change direction
            {
                tuxman->setX(myColumn*config.real_tile_width);
                adjustTuxman(SDLK_UP);
                return;
            } else if (currentKeyPressed == SDLK_LEFT && queuedDesiredDirection == SDLK_DOWN &&
                level->tileType(myPosition + config.map_width) != 'b' &&
                myColumn != xToColumn(originalTuxmanX - displacement))
            {
                tuxman->setX(myColumn*config.real_tile_width);
                adjustTuxman(SDLK_DOWN);
                return;
            }
        }

        // move Tuxman around
        int blockingTilePosition;
        char blockingTileType;
        //DEBUGGING: std::cout << myPosition << "    " << blockingTilePosition << std::endl;
        int blockingTileCloseCoord;
        bool nextMoveWouldPastBlockingTile;
        switch (lastKeyPressed) {
            case SDLK_LEFT:
                // find the next tile in this direction Tuxman can't cross (start with adjacent tile, assume Tuxman always on valid tile)
                for (int i = myPosition - 1; blockingTileType != 'b' ; i--) {
                    blockingTilePosition = i;
                    blockingTileType = level->tileType(blockingTilePosition);
                }

                // if the blocking tile is not on this line then we must be teleporting from left to right
                if (blockingTilePosition <= myPosition - ((myPosition % config.map_width)+1)) {
                    // keep moving to edge of screen until close enough to edge for teleport
                    if (tuxman->getX() > (config.real_tile_width / 5.5)) {
                        tuxman->setX(originalTuxmanX - displacement);
                    } else {
                        navStartTime = SDL_GetTicks();
                        originalTuxmanX = (config.map_width-1)*config.real_tile_width;
                        tuxman->setX(originalTuxmanX);
                    }
                    break;
                }

                // adjust Tuxman coordinates (keep going in direction as along as adjacent tile is non-blocking)
                blockingTileCloseCoord = ((blockingTilePosition % config.map_width)*config.real_tile_width) + config.real_tile_width;
                nextMoveWouldPastBlockingTile = (originalTuxmanX - displacement) <= blockingTileCloseCoord;
                if (!nextMoveWouldPastBlockingTile) {
                    tuxman->setX(originalTuxmanX - displacement);
                } else {
                    tuxman->setX(blockingTileCloseCoord);
                }
                break;
            case SDLK_RIGHT:
                // find the next tile in this direction Tuxman can't cross
                for (int i = myPosition+1; blockingTileType != 'b' ; i++) {
                    blockingTilePosition = i;
                    blockingTileType = level->tileType(blockingTilePosition);
                }

                // if the blocking tile is not on this line then we must be teleporting from right to left
                if (blockingTilePosition >= (myPosition + (config.map_width - ((myPosition % config.map_width))))) {
                    // keep moving to edge of screen until close enough to edge for teleport
                    if (tuxman->getX() < (((config.map_width-1)*config.real_tile_width) - config.real_tile_width + (config.real_tile_width - (config.real_tile_width / 5.5)))) {
                        tuxman->setX(originalTuxmanX + displacement);
                    } else {
                        navStartTime = SDL_GetTicks();
                        originalTuxmanX = 0;
                        tuxman->setX(originalTuxmanX);
                    }
                    break;
                }

                // adjust Tuxman coordinates (keep going in direction as along as adjacent tile is non-blocking)
                blockingTileCloseCoord = ((blockingTilePosition % config.map_width)*config.real_tile_width);
                nextMoveWouldPastBlockingTile = (originalTuxmanX + displacement + config.real_tile_width) >= blockingTileCloseCoord;
                if (!nextMoveWouldPastBlockingTile) {
                    tuxman->setX(originalTuxmanX + displacement);
                } else {
                    tuxman->setX(blockingTileCloseCoord - config.real_tile_width);
                }
                break;
            case SDLK_UP:
                // find the next tile in this direction Tuxman can't cross
                for (int i = myPosition; blockingTileType != 'b' ; i -= config.map_width) {
                    blockingTilePosition = i;
                    blockingTileType = level->tileType(blockingTilePosition);
                }

                // adjust Tuxman coordinates (keep going in direction as along as adjacent tile is non-blocking)
                blockingTileCloseCoord = invertY(((blockingTilePosition / config.map_width)*config.real_tile_height) + config.real_tile_height); // opengl coord
                nextMoveWouldPastBlockingTile = (originalTuxmanY + displacement) >= blockingTileCloseCoord;
                if (!nextMoveWouldPastBlockingTile) {
                    tuxman->setY(originalTuxmanY + displacement);
                } else {
                    tuxman->setY(blockingTileCloseCoord);
                }
                break;
            case SDLK_DOWN:
                // find the next tile in this direction Tuxman can't cross
                for (int i = myPosition; blockingTileType != 'b' ; i += config.map_width) {
                    blockingTilePosition = i;
                    blockingTileType = level->tileType(blockingTilePosition);
                }

                // adjust Tuxman coordinates (keep going in direction as along as adjacent tile is non-blocking)
                blockingTileCloseCoord = invertY(((blockingTilePosition / config.map_width)*config.real_tile_height)); // opengl coord
                nextMoveWouldPastBlockingTile = (originalTuxmanY - displacement - config.real_tile_height) <= blockingTileCloseCoord;
                if (!nextMoveWouldPastBlockingTile) {
                    tuxman->setY(originalTuxmanY - displacement);
                } else {
                    tuxman->setY(blockingTileCloseCoord + config.real_tile_height);
                }
                break;
            default:
                break;
        }
    } else { // otherwise...the user wants to change direction...
        // If direction is just the reverse of current direction then allow it
        if ((lastKeyPressed == SDLK_UP && currentKeyPressed == SDLK_DOWN) ||
            (lastKeyPressed == SDLK_DOWN && currentKeyPressed == SDLK_UP) ||
            (lastKeyPressed == SDLK_LEFT && currentKeyPressed == SDLK_RIGHT) ||
            (lastKeyPressed == SDLK_RIGHT && currentKeyPressed == SDLK_LEFT))
        {
            lastKeyPressed = currentKeyPressed;
            navStartTime = SDL_GetTicks();
            originalTuxmanX = tuxman->getX();
            originalTuxmanY = tuxman->getY();
        }

        // Special cases where Tuxman presses button but should not change direction because Tuxman pressed the button to late
        // ex: going right and pressing up/down while in the middle of an intersection
        // ex: going down and pressing left/right while in the middle of an intersection
        else if (level->tileType(myPosition - config.map_width) != 'b' && (static_cast<int>(tuxman->getX()) % static_cast<int>(config.real_tile_width)) > 0 && lastKeyPressed == SDLK_RIGHT && level->tileType(myPosition+1) != 'b' && currentKeyPressed == SDLK_UP)
        {
            adjustTuxman(lastKeyPressed);
            delayKeyPressOf = SDLK_UP;
            delayKeyPressStart = SDL_GetTicks();
            delayKeyPressInterval = ((config.real_tile_width - (static_cast<int>(tuxman->getX()) % static_cast<int>(config.real_tile_width)))/rate) + 30;
            // note: SDL has 10ms accuracy, adding a few more ms above to interval ensures key won't be pressed until Tuxman is in next tile
            return;
        }
        else if (level->tileType(myPosition + config.map_width) != 'b' && (static_cast<int>(tuxman->getX()) % static_cast<int>(config.real_tile_width)) > 0 && lastKeyPressed == SDLK_RIGHT && level->tileType(myPosition+1) != 'b' && currentKeyPressed == SDLK_DOWN)
        {
            adjustTuxman(lastKeyPressed);
            delayKeyPressOf = SDLK_DOWN;
            delayKeyPressStart = SDL_GetTicks();
            delayKeyPressInterval = ((config.real_tile_width - (static_cast<int>(tuxman->getX()) % static_cast<int>(config.real_tile_width)))/rate) + 30;
            // note: SDL has 10ms accuracy, adding a few more ms above to interval ensures key won't be pressed until Tuxman is in next tile
            return;
        }
        else if (level->tileType(myPosition - 1) != 'b' && (static_cast<int>(tuxman->getY()) % static_cast<int>(config.real_tile_height)) > 0 && lastKeyPressed == SDLK_DOWN && level->tileType(myPosition - config.map_width) != 'b' && currentKeyPressed == SDLK_LEFT)
        {
            adjustTuxman(lastKeyPressed);
            delayKeyPressOf = SDLK_LEFT;
            delayKeyPressStart = SDL_GetTicks();
            delayKeyPressInterval = ((static_cast<int>(tuxman->getY()) % static_cast<int>(config.real_tile_height))/rate) + 40;
            // note: SDL has 10ms accuracy, adding a few more ms above to interval ensures key won't be pressed until Tuxman is in next tile
            return;
        }
        else if (level->tileType(myPosition + 1) != 'b' && (static_cast<int>(tuxman->getY()) % static_cast<int>(config.real_tile_height)) > 0 && lastKeyPressed == SDLK_DOWN && level->tileType(myPosition - config.map_width) != 'b' && currentKeyPressed == SDLK_RIGHT)
        {
            adjustTuxman(lastKeyPressed);
            delayKeyPressOf = SDLK_RIGHT;
            delayKeyPressStart = SDL_GetTicks();
            delayKeyPressInterval = ((static_cast<int>(tuxman->getY()) % static_cast<int>(config.real_tile_height))/rate) + 40;
            // note: SDL has 10ms accuracy, adding a few more ms above to interval ensures key won't be pressed until Tuxman is in next tile
            return;
        }

        // If direction is horizontal and currently Tuxman is moving vertical
        else if (lastKeyPressed == SDLK_UP || lastKeyPressed == SDLK_DOWN) {
            // check that Tuxman has clearance on highest/lowest y values in that direction
            if (yToRow(tuxman->getY()) == yToRow(tuxman->getY()+config.real_tile_height-0.1)) { // note: cut off a bit of height so in same tile
                if (level->tileType(myPosition - 1) != 'b' && currentKeyPressed == SDLK_LEFT) {
                    // if so then change direction to left
                    lastKeyPressed = currentKeyPressed;
                    navStartTime = SDL_GetTicks();
                    originalTuxmanX = tuxman->getX();
                    originalTuxmanY = tuxman->getY();
                } else if (level->tileType(myPosition + 1) != 'b' && currentKeyPressed == SDLK_RIGHT) {
                    // right
                    lastKeyPressed = currentKeyPressed;
                    navStartTime = SDL_GetTicks();
                    originalTuxmanX = tuxman->getX();
                    originalTuxmanY = tuxman->getY();
                }
            }
        }

        // if direction is vertical and currently Tuxman is moving horizontal
        else if (lastKeyPressed == SDLK_LEFT || lastKeyPressed == SDLK_RIGHT) {
            // check that Tuxman has clearance on highest/lowest x values in that direction
            if (xToColumn(tuxman->getX()) == xToColumn(tuxman->getX()+config.real_tile_width-0.1)) { // note: cut off a bit of width so in same tile
                if (level->tileType(myPosition - config.map_width) != 'b' && currentKeyPressed == SDLK_UP) {
                    // if so then change direction to up
                    lastKeyPressed = currentKeyPressed;
                    navStartTime = SDL_GetTicks();
                    originalTuxmanX = tuxman->getX();
                    originalTuxmanY = tuxman->getY();
                } else if (level->tileType(myPosition + config.map_width) != 'b' && currentKeyPressed == SDLK_DOWN) {
                    // down
                    lastKeyPressed = currentKeyPressed;
                    navStartTime = SDL_GetTicks();
                    originalTuxmanX = tuxman->getX();
                    originalTuxmanY = tuxman->getY();
                }
            }
        }

        // the requested direction was invalid due to a blocking tile, but remember it
        // ( we will change direction when that direction becomes unblocked )
        if (queuedDesiredDirection != currentKeyPressed) queuedDesiredDirection = currentKeyPressed;
    }
}

/**
 * Handles events created by the user at the beginning of the level.
 * @note this method should be called within the first couple seconds of stage
 * starting, it triggers movement.
 */
void GameLoop::handleEventsHelper() {
    // if stage just begun then wait before allowing tuxman to move
    if (lastKeyPressed == SDLK_RETURN && isfirstXSecondsOfStagePast(config.begin_stage_wait)) {
        lastKeyPressed = SDLK_LEFT;
        navStartTime = SDL_GetTicks();
        originalTuxmanX = tuxman->getX();
        originalTuxmanY = tuxman->getY();
        isLevelBeginning = false; // don't visit this method anymore
    }
}

/**
 * Handles events created by the user throughout the game.
 * @param events The SDL_Event structure that hold's the game's latest events
 */
void GameLoop::handleEvents(SDL_Event* events) {
    bool anEventHasHappened = false;

    // if the first 5 second have not passed then call the handle events helper
    if (isLevelBeginning)
    {
        handleEventsHelper();
        return;
    }

    // otherwise while there are events to process...
    while(SDL_PollEvent(events))
    {
        // initiate the particular type of event that goes with that function...
        switch(events->type)
        {
            case SDL_JOYAXISMOTION:
                break;
            case SDL_JOYBUTTONDOWN:
                break;
            case SDL_JOYBUTTONUP:
                break;
            case SDL_KEYDOWN:
                if (!anEventHasHappened) anEventHasHappened = true;
                switch(events->key.keysym.sym) {
                    case SDLK_LEFT:
                        adjustTuxman(SDLK_LEFT);
                        break;
                    case SDLK_RIGHT:
                        adjustTuxman(SDLK_RIGHT);
                        break;
                    case SDLK_UP:
                        adjustTuxman(SDLK_UP);
                        break;
                    case SDLK_DOWN:
                        adjustTuxman(SDLK_DOWN);
                        break;
                    default:
                        break;
                }
                break;
            case SDL_QUIT:
                isDone = true;
                break;
            default:
                break;
        }
    }

    // if no event happened at all then adjust tuxman using last known event
    if (anEventHasHappened == false) adjustTuxman(lastKeyPressed);
}

/**
 * Sets up a level so that it may be played.
 */
void GameLoop::setUpLevel() {
    gout.clearScreen();

    level = new Level(1);
    events = new SDL_Event;

    fps = new FramesPerSecond();
    fps->target(35);

    level->buildUpLevel();

    ScreenObjectInfo objInfo = {0, 0, config.real_tile_width, config.real_tile_height, 'T'};
    tuxman = new Tuxman(config.lives, 0, objInfo);
    int x = positionToColumn(config.tuxman_position) * config.real_tile_width;
    int y = positionToRow(config.tuxman_position) * config.real_tile_height;
    tuxman->setPosition(x, invertY(y));

    objInfo.type = '1';
    objInfo.x = (14 * config.real_tile_width) - (0.5*config.real_tile_width);
    objInfo.y = invertY(11 * config.real_tile_height);
    sealEnemy = new Enemy(objInfo);

    objInfo.type = '2';
    objInfo.x = (12 * config.real_tile_width);
    objInfo.y = invertY(14 * config.real_tile_height);
    sharkEnemy = new Enemy(objInfo);

    objInfo.type = '3';
    objInfo.x = (14 * config.real_tile_width) - (0.5*config.real_tile_width);
    objInfo.y = invertY(14 * config.real_tile_height);
    whaleEnemy = new Enemy(objInfo);

    objInfo.type = '4';
    objInfo.x = (15 * config.real_tile_width);
    objInfo.y = invertY(14 * config.real_tile_height);
    hawkEnemy = new Enemy(objInfo);

    gout.flush();

    stageBeginTime = SDL_GetTicks();
    isLevelBeginning = true;
}

/**
 * Tears down a level so that resources can be given back to the system.
 */
void GameLoop::tearDownLevel() {
    delete level;
    delete events;
    delete fps;
    delete tuxman;
    delete sealEnemy;
    delete sharkEnemy;
    delete whaleEnemy;
    delete hawkEnemy;
}

/**
 * Takes a tile position and converts it into the corresponding row for that tile number.
 * @param position the position number of the tile
 * @return the row that corresponds to the given tile number
 */
int GameLoop::positionToRow(int position) const {
    return (static_cast<int>(position / config.map_width) - 1); // note: get lvl file's row, subtract 1 because box drawn under coordinates given
}

/**
 * Takes a tile position and converts it into the corresponding column for that tile number.
 * @param position the position number of the tile
 * @return the column that corresponds to the given tile number
 */
int GameLoop::positionToColumn(int position) const {
    return ((position % config.map_width) - 1); // note: get lvl file's column, subtract 1 because box drawn takes up the next column
}

/**
 * Takes in an OpenGL x coord and converts it to the corresponding row in level file.
 * @return the column that relates to that x coordinate
 */
int GameLoop::xToColumn(float x) const {
    return static_cast<int>(floor(x / config.real_tile_width));
}

/**
 * Takes in an OpenGL y coord and converts it to the corresponding row in level file.
 * @return the row that relates to that y coordinate
 */
int GameLoop::yToRow(float y) const {
    return static_cast<int>(floor(y / config.real_tile_height) + 1);
}

/**
 * Inverts a y coordinate's value.
 * @note useful for turning sdl y coordinate into opengl y coordinate and vice versa etc.
 * @param coordY the y cooridnate that needs to be inverted
 * @return the inverted Y coordinate
 */
float GameLoop::invertY(float coordY) const {
    return (info.height - coordY);
}

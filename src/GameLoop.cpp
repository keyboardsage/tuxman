#include "GameLoop.hpp"
#include "GraphicStreamOut.hpp"
#include "DisplayInfo.hpp"
#include "Configuration.hpp"
#include "ScreenObject.hpp"

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
    tuxman = NULL;
    sealEnemy = NULL;
    sharkEnemy = NULL;
    whaleEnemy = NULL;
    hawkEnemy = NULL;
    navStartTime = 0;
    lastDirection = NONE;
    stageBeginTime = 0;
    queuedDesiredDirection = NONE;
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

    SDL_Event event;
    do {
        fps->frameStart();

        // handle input and change objects accordingly
        handleEvents();
        /*if (delayKeyPressStart != 0 && (SDL_GetTicks() - delayKeyPressStart) >= delayKeyPressInterval) // note: ensures queued event happens under special cases
        {
            // DEBUGGING: std::cout << "NOW" << std::endl;SDL_Delay(5000);
            SDL_Event delayedKeyPressEvent;
            delayedKeyPressEvent.type = SDL_KEYDOWN;
            delayedKeyPressEvent.key.keysym.sym = static_cast<SDL_Keycode>(delayKeyPressOf);
            SDL_PushEvent(&delayedKeyPressEvent);
            delayKeyPressStart = 0;
        }*/

        // eat food & calculate new score
        //tuxman->accumScore(eatFood());
        // DEBUGGING: std::cout << tuxman->getScore() << std::endl;

        // use algorithm to move enemies

        // make big dots blink

        // render all these changes
        level->renderLevel();
        gout << static_cast<ScreenObject*>(tuxman);
        /*gout << static_cast<ScreenObject*>(sealEnemy);
        gout << static_cast<ScreenObject*>(sharkEnemy);
        gout << static_cast<ScreenObject*>(whaleEnemy);
        gout << static_cast<ScreenObject*>(hawkEnemy);*/
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
    int myColumn = (lastDirection != LEFT) ? xToColumn(tuxman->getX()) : xToColumn(tuxman->getX() + (0.75 * config.real_tile_width));
    int myRow = (lastDirection != UP) ? yToRow(invertY(tuxman->getY())) : yToRow(invertY(tuxman->getY() - (0.75 * config.real_tile_width)));
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
 * Adjusts the Tuxman's current coordinates in response to recent input.
 * @todo This method is to long it should be chopped up.
 * @todo This method needs to be cleaned up(possibly reimplemented), it is terribly hackish and complicated.
 * @todo Teleporting vertically needs to be implemented.
 * @param currentDirection the direction user wants to move tuxman
 */
void GameLoop::adjustTuxman(Direction currentDirection) {
    static float originalObjectX;
    static float originalObjectY;

    originalObjectX = tuxman->getX();
    originalObjectY = tuxman->getY();

    // convert OpenGL coord into level file coord
    int myColumn = xToColumn(originalObjectX);
    int myRow = yToRow(invertY(originalObjectY));
    int myPosition = ((myRow - 1) * config.map_width) + myColumn; // note: If your on row 24, then there are 23 full rows before it

    // calculate displacement
    int elapsedTime = SDL_GetTicks() - navStartTime; // in milliseconds
    int averageTileSize = (config.real_tile_width + config.real_tile_height) / 2;
    float rate = (averageTileSize * 5) / 1000.0; // (5 times average tile size) pixels per 1 second
    float displacement = rate * elapsedTime;

    // if Tuxman's direction is contrary to desired direction...
    if ( currentDirection != lastDirection ) {
        lastDirection = currentDirection;
    }

    /*if ( currentDirection != lastDirection ) {
        if ( lastDirection == UP || lastDirection == DOWN ) {
            // queue up the pending direction change...
            if (queuedDesiredDirection == SDLK_LEFT && // left
                level->tileType(myPosition - 1) != 'b' && // unblocked
                myRow != yToRow(invertY(displacement + originalObjectY))) // Tuxman ready to change direction
            {
                tuxman->setY(invertY((myRow-1)*config.real_tile_height));
                adjustTuxman(SDLK_LEFT);
                return;
            } else if (queuedDesiredDirection == SDLK_RIGHT &&
                level->tileType(myPosition + 1) != 'b' &&
                myRow != yToRow(invertY(displacement + originalObjectY)))
            {
                tuxman->setY(invertY((myRow-1)*config.real_tile_height));
                adjustTuxman(SDLK_RIGHT);
                return;
            }
        } else if ( currentKeyPressed == SDLK_LEFT || currentKeyPressed == SDLK_RIGHT ) { // contrary to desired
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
                myColumn != xToColumn(originalObjectX - displacement)) // Tuxman ready to change direction
            {
                tuxman->setX(myColumn*config.real_tile_width);
                adjustTuxman(SDLK_UP);
                return;
            } else if (currentKeyPressed == SDLK_LEFT && queuedDesiredDirection == SDLK_DOWN &&
                level->tileType(myPosition + config.map_width) != 'b' &&
                myColumn != xToColumn(originalObjectX - displacement))
            {
                tuxman->setX(myColumn*config.real_tile_width);
                adjustTuxman(SDLK_DOWN);
                return;
            }
        }
    }
    */

    // if the same direction is still desired then simply adjust...
    if (currentDirection == lastDirection)
    {
        //DEBUGGING: std::cout << myPosition << "    " << blockingTilePosition << std::endl;
        // move Tuxman around
        int blockingTilePosition;
        char blockingTileType;
        int blockingTileCloseCoord;
        bool isInvalidMove;
        switch (lastDirection) {
            case LEFT:
                // find the next tile blocking in this direction
                firstBlockingTile(lastDirection, myPosition, blockingTilePosition, blockingTileType, blockingTileCloseCoord);

                // if the blocking tile is not on this line then we must be teleporting from left to right
                /*if (blockingTilePosition <= myPosition - ((myPosition % config.map_width) + 1)) {
                    // keep moving to edge of screen until close enough to edge for teleport
                    if (tuxman->getX() > (config.real_tile_width / 5.5)) {
                        tuxman->setX(originalObjectX - displacement);
                    } else {
                        navStartTime = SDL_GetTicks();
                        originalObjectX = (config.map_width - 1) * config.real_tile_width;
                        tuxman->setX(originalObjectX);
                    }
                    break;
                }*/

                // keep going in direction as long as adjacent tile is non-blocking
                std::cout << originalObjectX << std::endl;
                isInvalidMove = (originalObjectX - displacement) <= blockingTileCloseCoord;
                if (!isInvalidMove) {
                    tuxman->setX(originalObjectX - displacement);
                } else {
                    tuxman->setX(blockingTileCloseCoord);
                }
                break;
            case RIGHT:
                // find the next tile blocking in this direction
                firstBlockingTile(lastDirection, myPosition, blockingTilePosition, blockingTileType, blockingTileCloseCoord);

                // if the blocking tile is not on this line then we must be teleporting from right to left
                /*if (blockingTilePosition >= (myPosition + (config.map_width - ((myPosition % config.map_width))))) {
                    // keep moving to edge of screen until close enough to edge for teleport
                    if (tuxman->getX() < (((config.map_width - 1) * config.real_tile_width) - config.real_tile_width + (config.real_tile_width - (config.real_tile_width / 5.5)))) {
                        tuxman->setX(originalObjectX + displacement);
                    } else {
                        navStartTime = SDL_GetTicks();
                        originalObjectX = 0;
                        tuxman->setX(originalObjectX);
                    }
                    break;
                }*/

                // keep going in direction as along as adjacent tile is non-blocking
                isInvalidMove = (originalObjectX + displacement + config.real_tile_width) >= blockingTileCloseCoord;
                if (!isInvalidMove) {
                    tuxman->setX(originalObjectX + displacement);
                } else {
                    tuxman->setX(blockingTileCloseCoord - config.real_tile_width);
                }
                break;
            case UP:
                // find the next tile blocking in this direction
                firstBlockingTile(lastDirection, myPosition, blockingTilePosition, blockingTileType, blockingTileCloseCoord);

                // adjust Tuxman coordinates (keep going in direction as along as adjacent tile is non-blocking)
                std::cout << originalObjectY << " " << blockingTileCloseCoord << " " << displacement << " " << blockingTilePosition << std::endl;
                isInvalidMove = (originalObjectY + displacement) >= blockingTileCloseCoord;
                if (!isInvalidMove) {
                    tuxman->setY(originalObjectY + displacement);
                } else {
                    tuxman->setY(blockingTileCloseCoord);
                }
                break;
            case DOWN:
                // find the next tile blocking in this direction
                firstBlockingTile(lastDirection, myPosition, blockingTilePosition, blockingTileType, blockingTileCloseCoord);

                // adjust Tuxman coordinates (keep going in direction as along as adjacent tile is non-blocking)
                isInvalidMove = (originalObjectY - displacement - config.real_tile_height) <= blockingTileCloseCoord;
                if (!isInvalidMove) {
                    tuxman->setY(originalObjectY - displacement);
                } else {
                    tuxman->setY(blockingTileCloseCoord + config.real_tile_height);
                }
                break;
            default:
                break;
        }
    } /*else { // otherwise...the user wants to change direction...
        // If direction is just the reverse of current direction then allow it
        if ((lastKeyPressed == SDLK_UP && currentKeyPressed == SDLK_DOWN) ||
            (lastKeyPressed == SDLK_DOWN && currentKeyPressed == SDLK_UP) ||
            (lastKeyPressed == SDLK_LEFT && currentKeyPressed == SDLK_RIGHT) ||
            (lastKeyPressed == SDLK_RIGHT && currentKeyPressed == SDLK_LEFT))
        {
            lastKeyPressed = currentKeyPressed;
            navStartTime = SDL_GetTicks();
            originalObjectX = tuxman->getX();
            originalObjectY = tuxman->getY();
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
                    originalObjectX = tuxman->getX();
                    originalObjectY = tuxman->getY();
                } else if (level->tileType(myPosition + 1) != 'b' && currentKeyPressed == SDLK_RIGHT) {
                    // right
                    lastKeyPressed = currentKeyPressed;
                    navStartTime = SDL_GetTicks();
                    originalObjectX = tuxman->getX();
                    originalObjectY = tuxman->getY();
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
                    originalObjectX = tuxman->getX();
                    originalObjectY = tuxman->getY();
                } else if (level->tileType(myPosition + config.map_width) != 'b' && currentKeyPressed == SDLK_DOWN) {
                    // down
                    lastKeyPressed = currentKeyPressed;
                    navStartTime = SDL_GetTicks();
                    originalObjectX = tuxman->getX();
                    originalObjectY = tuxman->getY();
                }
            }
        }

        // the requested direction was invalid due to a blocking tile, but remember it
        // ( we will change direction when that direction becomes unblocked )
        if (queuedDesiredDirection != currentKeyPressed) queuedDesiredDirection = currentKeyPressed;
    }*/
}

/**
 * Finds first blocking tile in the given direction. Populates arguments with information about the blocking tile.
 * @note function starts with adjacent tile and makes assumption that it begins on coordinate of valid tile
 * @param theDirection The direction to look in
 * @param myPosition Tile number used to describe object's current location
 * @param blockingTilePosition Overwrites argument with tile number used to describe blocking tile's location
 * @param blockingTileType Overwrites argument with tile type that would block
 * @param blockingTileCloseCoord Overwrites argument with closest pixel in that direction that would block movement
 */
void GameLoop::firstBlockingTile(Direction theDirection, int myPosition, int& blockingTilePosition, char& blockingTileType, int& blockingTileCloseCoord) const {
    switch (theDirection) {
        case LEFT:
            for (int i = myPosition - 1; blockingTileType != 'b' ; i--) {
                blockingTilePosition = i;
                blockingTileType = level->tileType(blockingTilePosition);
            }
            blockingTileCloseCoord = ((blockingTilePosition % config.map_width) * config.real_tile_width) + config.real_tile_width;
            break;
        case RIGHT:
            for (int i = myPosition + 1; blockingTileType != 'b' ; i++) {
                blockingTilePosition = i;
                blockingTileType = level->tileType(blockingTilePosition);
            }
            blockingTileCloseCoord = ((blockingTilePosition % config.map_width) * config.real_tile_width);
            break;
        case UP:
            for (int i = myPosition; blockingTileType != 'b' ; i -= config.map_width) {
                blockingTilePosition = i;
                blockingTileType = level->tileType(blockingTilePosition);
            }
            blockingTileCloseCoord = invertY(((blockingTilePosition / config.map_width) * config.real_tile_height) + config.real_tile_height);
            break;
        case DOWN:
            for (int i = myPosition; blockingTileType != 'b' ; i += config.map_width) {
                blockingTilePosition = i;
                blockingTileType = level->tileType(blockingTilePosition);
            }
            blockingTileCloseCoord = invertY(((blockingTilePosition / config.map_width) * config.real_tile_height));
            break;
    }
}

/**
 * Handles events created by the user throughout the game.
 * @param events The SDL_Event structure that hold's the game's latest events
 */
void GameLoop::handleEvents() {
    // delete soon bool anEventHasHappened = false;

    // if the first 5 second have not passed then call the handle events helper
    // TODO: remove this after movement works
    /*if (isLevelBeginning)
    {
        handleEventsHelper();
        return;
    }*/

    // handle event based on event type
    Direction currentDirection = lastDirection;
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_JOYAXISMOTION:
                break;
            case SDL_JOYBUTTONDOWN:
                break;
            case SDL_JOYBUTTONUP:
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_LEFT:
                        currentDirection = LEFT;
                        break;
                    case SDLK_RIGHT:
                        currentDirection = RIGHT;
                        break;
                    case SDLK_UP:
                        currentDirection = UP;
                        break;
                    case SDLK_DOWN:
                        currentDirection = DOWN;
                        break;
                    default:
                        break;
                }
                break;
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        isDone = true;
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
    adjustTuxman(currentDirection);
    navStartTime = SDL_GetTicks();
}

/**
 * Sets up a level so that it may be played.
 */
void GameLoop::setUpLevel() {
    level = new Level(1);

    fps = new FramesPerSecond();
    fps->target(60);

    level->buildUpLevel();

    ScreenObjectInfo objInfo = {0, 0, config.real_tile_width, config.real_tile_height, 'T'};
    tuxman = new Tuxman(config.lives, 0, objInfo);
    int x = positionToColumn(config.tuxman_position) * config.real_tile_width;
    int y = positionToRow(config.tuxman_position) * config.real_tile_height;
    tuxman->setPosition(x, invertY(y));

    objInfo.type = '1';
    objInfo.x = (14 * config.real_tile_width) - (0.5 * config.real_tile_width);
    objInfo.y = invertY(11 * config.real_tile_height);
    sealEnemy = new Enemy(objInfo);

    objInfo.type = '2';
    objInfo.x = (12 * config.real_tile_width);
    objInfo.y = invertY(14 * config.real_tile_height);
    sharkEnemy = new Enemy(objInfo);

    objInfo.type = '3';
    objInfo.x = (14 * config.real_tile_width) - (0.5 * config.real_tile_width);
    objInfo.y = invertY(14 * config.real_tile_height);
    whaleEnemy = new Enemy(objInfo);

    objInfo.type = '4';
    objInfo.x = (15 * config.real_tile_width);
    objInfo.y = invertY(14 * config.real_tile_height);
    hawkEnemy = new Enemy(objInfo);

    gout.flush();



    // seems like a good place to do intro related things
    // TODO: block and play music, after music finishes continue onward
    stageBeginTime = SDL_GetTicks();
    navStartTime = SDL_GetTicks();
}

/**
 * Tears down a level so that resources can be given back to the system.
 */
void GameLoop::tearDownLevel() {
    delete level;
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

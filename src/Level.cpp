#include <iostream>
#include <cstdlib>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

#include "Level.hpp"
#include "Tile.hpp"
#include "DisplayInfo.hpp"
#include "Configuration.hpp"
#include "GraphicStreamOut.hpp"
#include "ScreenObject.hpp"

extern Configuration config;
extern DisplayInfo info;

Level::Level(int intLevelParam) {
    setLevel(intLevelParam);
    objMap = new ObjectMap();
}

Level::~Level() {
    delete objMap;
}

/**
 * Constructs a new level.
 * @note You need to flush after calling this method.
 */
void Level::buildUpLevel() {
    ifstream* levelStream = new ifstream(file.c_str());
    if (levelStream->fail()) {
        cerr << "(EE) Could not open level file." << endl;
        levelStream->close();
        exit(EXIT_FAILURE);
    } else buildUpLevelHelp(levelStream, 0.0, info.height);
    levelStream->close();
}

/**
 * Renders a level based on what is in the objMap structure.
 * @note You need to flush after calling this method.
 */
void Level::renderLevel() const {
    ObjectMap::const_iterator end = objMap->end();
    ScreenObject* tmpObj;
    char tmp;
    for(ObjectMap::const_iterator iter = objMap->begin(); iter != end; ++iter)
    {
        tmpObj = iter->second;
        tmp = tmpObj->getType();
        gout << tmpObj;
    }
}

/**
 * Tears down the current level.
 */
void Level::tearDownLevel() {

}

/**
 * Returns the tile type at a particular position number.
 * @note The top left corner tile is 0, the bottom right corner tile is n
 * @note where n is the tile count on the screen as a whole.
 * @todo Should throw runtime error rather than returning a question mark in the else clause.
 * @return Returns a character representing the tile type, or question mark on error.
 */
char Level::tileType(int pos) const {
    ObjectMap::iterator iter = objMap->find(pos);
    ScreenObject* temp;
    if( iter != objMap->end() )
        temp = iter->second;
    else
        return '?';

    return temp->getType();

    /*ObjectMap::const_iterator end = objMap->end();
    for(ObjectMap::const_iterator iter = objMap->begin(); iter != end; ++iter)
    {
        if (iter->first == pos) return (iter->second)->getType();
    }

    return '?'; // TODO: throw runtime error instead?
    */
}

/**
 * Tests whether a tile does or does not have food on it.
 * @todo Should throw runtime error rather than returning a question mark in the else clause.
 * @param pos The tile number to test for food.
 * @return True if the tile in question has food on it, otherwise false.
 */
bool Level::tileHasFood(int pos) const {
    ObjectMap::iterator iter = objMap->find(pos);
    ScreenObject* temp;
    if( iter != objMap->end() )
        temp = iter->second;
    else
        return '?';

    return static_cast<Tile*>(temp)->getHasFood();
}

/**
 * Removes the food from a tile.
 * @todo Should throw runtime error when position is not found.
 * @param pos The tile number of where the food should be removed.
 */
void Level::tileRemoveFood(int pos) {
    ObjectMap::iterator iter = objMap->find(pos);
    ScreenObject* temp;
    if( iter != objMap->end() )
        temp = iter->second;
    //else
    //    return '?';

    return static_cast<Tile*>(temp)->setHasFood(false);
}

/**
 * Sets the current level.
 * @param intLevelParam the level to set.
 */
void Level::setLevel(int intLevelParam) {
    intLevel = intLevelParam;

    stringstream ss;
    ss << "/Users/malachi/Projects/tuxman/resources/levels/level" << intLevel << ".lvl";
    file = ss.str();
}

/**
 * Returns the current level.
 */
const int Level::getLevel() const {
    return intLevel;
}

/**
 * Helper function for constructing a new level.
 */
void Level::buildUpLevelHelp(ifstream* levelStream, float currentX, float currentY) {
    // for the current column's tile, read character...
    char tmp;
    if (levelStream->get(tmp) && tmp != '\n') // ...build this column's tile based on the character
    {
        // DEBUGGING PURPOSES: cout << currentX << "-" << tmp;
        cout << tmp;
        ScreenObjectInfo objInfo = {currentX, currentY, info.width/config.map_width, info.height/config.map_height, tmp};
        Tile* theTile = new Tile(objInfo);
        gout << dynamic_cast<ScreenObject*>(theTile);
        recordTile(theTile);
    }
    else if (tmp == '\n') // ...move to next map row when necessary
    {
        currentY -= info.height/config.map_height;
        cout << endl;
        buildUpLevelHelp(levelStream, 0.0, currentY);
    }
    else return; // ...then stop drawing upon EOF character

    //currentX = (currentX == info.width) ? currentX = 0 : static_cast<int>(currentX)+(info.width/config.MAP_WIDTH);

    buildUpLevelHelp(levelStream, currentX+(info.width/config.map_width), currentY);
}

/**
 * Records the tile in a map so it can be referenced later.
 */
void Level::recordTile(Tile* unloggedTile) {
    //DEBUGGING: if (objMap->size() == 686) std::cout << '-' << (dynamic_cast<ScreenObject*>(unloggedTile))->getType() << '-';
    objMap->insert(make_pair(objMap->size(), dynamic_cast<ScreenObject*>(unloggedTile)));
}

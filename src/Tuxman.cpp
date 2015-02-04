#include "Tuxman.hpp"
#include "GraphicStreamOut.hpp"
//#include "Configuration.hpp"
//extern Configuration config;

/**
 * Constructs the Tuxman object using default values.
 */
Tuxman::Tuxman(ScreenObjectInfo& objInfo):ScreenObject(objInfo.x, objInfo.y, objInfo.width, objInfo.height, objInfo.type) {
    // some pretty sensible defaults
    lives = 3;
    score = 0;
}

/**
 * Constructs the Tuxman object using user-defined values.
 * @param livesParam the number of lives Tuxman will have once created
 * @param scoreParam the score Tuxman has accumulated
 * @param objInfo a structure that holds basic generic information about screen objects
 */
Tuxman::Tuxman(int livesParam, int scoreParam, ScreenObjectInfo& objInfo):ScreenObject(objInfo.x, objInfo.y, objInfo.width, objInfo.height, objInfo.type) {
    lives = livesParam;
    score = scoreParam;
}

/**
 * Deconstructs Tuxman.
 */
Tuxman::~Tuxman() {

}

/**
 * Draws Tuxman onto the screen.
 */
void Tuxman::draw() {
    gout.drawColor(GraphicStreamOut::ORANGE);

    glBegin(GL_QUADS);
        glVertex2f(0.0, 0.0);
        glVertex2f(info->width, 0.0);
        glVertex2f(info->width, -info->height);
        glVertex2f(0.0, -info->height);
    glEnd();
}

/**
 * Accumulates the score for Tuxman.
 * @param intAmount the value that you want to add to Tuxman's score.
 */
void Tuxman::accumScore(int intAmount) {
    score += intAmount;
}

/**
 * Gets the current score that Tuxman has accumulated.
 * @return Tuxman's current score.
 */
int Tuxman::getScore() const {
    return score;
}

/**
 * Sets the number of lives Tuxman has left.
 * @param intLivesParam the number of lives Tuxman will have left.
 */
void Tuxman::setLivesLeft(int intLivesParam) {
    lives = intLivesParam;
}

/**
 * Gets the number of lives tuxman has.
 * @return the number of lives Tuxman has.
 */
int Tuxman::getLivesLeft() const {
    return lives;
}

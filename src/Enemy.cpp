#include "Enemy.hpp"
#include "GraphicStreamOut.hpp"

/**
 * Constructs the Enemy object using user-defined values.
 * @param objInfo a structure that holds basic generic information about screen objects
 */
Enemy::Enemy(ScreenObjectInfo& objInfo):ScreenObject(objInfo.x, objInfo.y, objInfo.width, objInfo.height, objInfo.type) {
}

/**
 * Deconstructs Tuxman.
 */
Enemy::~Enemy() {

}

/**
 * Draws Enemy onto the screen.
 * @todo the animal image used depends on the enemy type, use enemy type as filename
 */
void Enemy::draw() {
    gout.drawColor(GraphicStreamOut::ORANGE);

    glBegin(GL_QUADS);
        glVertex2f(0.0, 0.0);
        glVertex2f(info->width, 0.0);
        glVertex2f(info->width, -info->height);
        glVertex2f(0.0, -info->height);
    glEnd();
}

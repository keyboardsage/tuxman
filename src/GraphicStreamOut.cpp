#include <iostream>
using namespace std;

#include "OpenGL.hpp"

#include "DisplayInfo.hpp"
#include "GraphicStreamOut.hpp"

/**
 * Constructs a graphic stream object.
 */
GraphicStreamOut::GraphicStreamOut()
{
}

/**
 * Destructs a graphic stream object.
 */
GraphicStreamOut::~GraphicStreamOut()
{
}

/**
 * Draws a hero ship in the proper coordinates.
 */
void GraphicStreamOut::operator<< (ScreenObject* obj)
{
    ScreenObjectInfo* objInfo = obj->getInfo();
    setDrawPosition(objInfo->x, objInfo->y);
    render(obj);
}

/**
 * Changes the color used for drawing in OpenGL.
 * @todo Should throw a runtime error if programs falls under the default case
 * @param color the desired color from the Color enumeration
 */
void GraphicStreamOut::drawColor(Color color) const {
    switch(color) {
        case RED:
            glColor3f(1.0, 0.0, 0.0);
            break;
        case GREEN:
            glColor3f(0.0, 1.0, 0.0);
            break;
        case BLUE:
            glColor3f(0.0, 0.0, 1.0);
            break;
        case PURPLE:
            glColor3f(1.0, 0.0, 1.0);
            break;
        case LIGHTBLUE:
            glColor3f(0.0, 1.0, 1.0);
            break;
        case YELLOW:
            glColor3f(1.0, 1.0, 0.0);
            break;
        case ORANGE:
            glColor3f(1.0, 0.647, 0.0);
            break;
        case WHITE:
            glColor3f(1.0, 1.0, 1.0);
            break;
        default:
            break;
    }
}

void initGraphicsOut(DisplayInfo& info) {
    // enable and disable needed attributes in OpenGL
    glShadeModel(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // lets determine the area to manipulate
    glViewport (0, 0, static_cast<GLsizei>(info.width), static_cast<GLsizei>(info.height));

    // viewing transformation
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble)info.width, 0.0, (GLdouble)info.height, -1.0, 1.0);

    // always leave the beginning state in modelview
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();

    // clear screen
    gout.clearScreen();
}

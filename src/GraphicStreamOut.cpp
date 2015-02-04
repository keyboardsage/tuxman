#include <iostream>
using namespace std;

#include <GL/gl.h>

#include "GraphicStreamOut.hpp"

	/**
	 * Constructs a graphic stream object.
	 */
	GraphicStreamOut::GraphicStreamOut()
	{
		// enable and disable needed attributes in OpenGL
		glMatrixMode (GL_MODELVIEW);
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
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



#ifndef GRAPHICSTREAMOUT_HPP
#define GRAPHICSTREAMOUT_HPP

#include <string>
#include <vector>
#include <map>

#include <SDL/SDL.h>
#include <GL/gl.h>

#include "NotCopyable.hpp"
#include "ScreenObject.hpp"

	/**
	 * This class handles all the drawing done by the application.
	 * It works similar to the ostream object in the C++ standard.
	 */
	class GraphicStreamOut: public NotCopyable
	{
	    public:
            enum Color { RED, GREEN, BLUE, PURPLE, LIGHTBLUE, YELLOW, ORANGE, WHITE };

		public:
			GraphicStreamOut();
			~GraphicStreamOut();

			void operator<< (ScreenObject* surf);

			void flush() const
			{
				glFlush();
                SDL_GL_SwapBuffers();
			}

			void clearScreen() const
			{
				glClearColor(0.0, 0.0, 0.0, 0.0); // black
				glClear(GL_COLOR_BUFFER_BIT);
			}

			void drawColor(Color color) const;

			void setDrawPosition(float fltXMove, float fltYMove) const
			{
			    glPushMatrix();
				glTranslatef(fltXMove, fltYMove, 0.0);
			}

			void render(ScreenObject* obj) {
			    obj->draw();
			    glPopMatrix();
			}

            void setColor(float red, float green, float blue) const
            {
                glColor3f(red, green, blue);
            }

		private:
			void setOpacity(int intAmountParam);
			void setFlag();
			void unsetFlag();
			void checkDrawError();

			void mapResources();
			void unmapResources();
	};

	static GraphicStreamOut gout; /**< global drawing object */

	// NOTE: ***** Helper functions below *****

	/**
	 * Sets the opacity of the object that will drawn.
	 */
	template <typename T>
	GraphicStreamOut& setOpacity(GraphicStreamOut &gstream, int intPercentAmountParam)
	{
		// use opengl and
		gstream.setOpacity(intPercentAmountParam);
		return gstream;
	}

#endif /*GRAPHICSTREAMOUT_HPP*/

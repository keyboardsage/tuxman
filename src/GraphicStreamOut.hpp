#ifndef GRAPHICSTREAMOUT_HPP
#define GRAPHICSTREAMOUT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stdexcept>

#include "SDL.h"
#include "OpenGL.hpp"

#include "DisplayInfo.hpp"
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

				SDL_Window* screen = SDL_GL_GetCurrentWindow();
                if (screen == NULL)
                {
                    std::cerr << "(EE) Unable to execute SDL_GL_GetCurrentWindow()." << std::endl;
                    throw std::runtime_error(SDL_GetError());
                }
                SDL_GL_SwapWindow(screen);
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

            void setOpacity(int intAmountParam);

		private:
			void setFlag();
			void unsetFlag();
			void checkDrawError();

			void mapResources();
			void unmapResources();
	};

	static GraphicStreamOut gout; /**< global drawing object */

	// NOTE: ***** Helper functions below *****

	/**
	 * Sets the opacity of the object that will be drawn.
	 */
	template <typename T>
	GraphicStreamOut& setOpacity(GraphicStreamOut &gstream, int intPercentAmountParam)
	{
		gstream.setOpacity(intPercentAmountParam);
		return gstream;
	}

	/**
	 * Sets OpenGL attributes and clears the screen for drawing.
	 */
	void initGraphicsOut(DisplayInfo& info);

#endif /*GRAPHICSTREAMOUT_HPP*/

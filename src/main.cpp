#include <iostream>
#include <stdexcept>
#include <memory>
using namespace std;

#include <SDL/SDL.h>
#include "OpenGL.hpp"

#include "DisplayInfo.hpp"
#include "Configuration.hpp"

#include "GameLoop.hpp"

void genDispSurf(DisplayInfo& info);
void setAttr(int bpp, int bitDepth);
void setupGLArea(DisplayInfo& info);

Configuration config;
DisplayInfo info;

int main(int argc, char* argv[]) {

// initializing video
try {
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) == -1)
        throw runtime_error(SDL_GetError());
} catch(runtime_error& e) {
    cerr << "(E) An error occurred while loading manager." << endl;
    cerr << "(E) Cause Message: " << e.what() << endl;
    return EXIT_FAILURE;
}

// load configuration
config.tile_size = 16;
config.pixel_depth = 16;
config.map_width = 28;
config.map_height = 31;
config.bonus_type = "cherry";
auto_ptr<BonusTimes> autoptrBonusTimes(new BonusTimes);
autoptrBonusTimes->push_back(11);
autoptrBonusTimes->push_back(48);
config.bonus_times = autoptrBonusTimes.get();
config.tuxman_position = 687;
config.lives = 3;
config.real_tile_width = config.tile_size;
config.real_tile_height = (config.tile_size+(config.tile_size/4));
config.begin_stage_wait = 1;

// create window & surface
info.width = config.real_tile_width*config.map_width;
info.height = config.real_tile_height*config.map_height;
info.depth = config.pixel_depth;
info.flags = SDL_OPENGL;
genDispSurf(info);
SDL_WM_SetCaption("Tuxman", NULL);
setupGLArea(info);

// game loop
GameLoop* gameLoop = new GameLoop();
gameLoop->run();
delete gameLoop;

// uninitializing video
SDL_QuitSubSystem(SDL_INIT_VIDEO);

return EXIT_SUCCESS;
}

/**
 * Generates a new SDL display surface (window gets created).
 */
void genDispSurf(DisplayInfo& info)
{
    // first ensure the specifications are plausible for this system
    int retval = SDL_VideoModeOK(info.width, info.height, info.depth, info.flags);
    if (retval == 0)
    {
        cerr << "(WW) Cannot support video mode at any pixel depth." << endl;
        cerr << "(WW) Emulating intended color depth using a shadow video surface." << endl;
    }
    else if (retval < info.depth)
    {
        cerr << "(WW) System prefers a bit depth of " << retval << " pixels." << endl;
        cerr << "(WW) Emulating intended color depth using a shadow video surface." << endl;
    }
    else
    {
        cout << "(II) Your system can handle " << info.depth << "-bit color with ease." << endl;
        cout << "(II) Your going to have a ball playing this game!" << endl;
    }

	// set the attributes for the window we are creating
    int colorBufferSizes = info.depth / 3;
    setAttr(colorBufferSizes, info.depth);

    // create a display surface, that is our window
    SDL_SetVideoMode(info.width, info.height, info.depth, info.flags);

    // exit application upon failure
    if (SDL_GetVideoSurface() == NULL)
        throw runtime_error(SDL_GetError());
}

void setAttr(int bpp, int bitDepth)
{
    // color component buffers
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, bpp);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, bpp);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, bpp);

    // depth buffer
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, bitDepth);

    // always double buffer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

/**
 * Sets up/Reshapes the area inside the window that the OpenGL API manipulates.
 */
void setupGLArea(DisplayInfo& info)
{
    // lets determine the area to manipulate
    glViewport (0, 0, static_cast<GLsizei>(info.width), static_cast<GLsizei>(info.height));

    // viewing transformation
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble)info.width, 0.0, (GLdouble)info.height, -1.0, 1.0);

    // always leave the beginning state in modelview
    glMatrixMode (GL_MODELVIEW);
    glLoadIdentity();
}

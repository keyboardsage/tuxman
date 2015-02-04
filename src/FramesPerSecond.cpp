#include "FramesPerSecond.hpp"

#include <SDL/SDL.h>

#include <iostream>

FramesPerSecond::FramesPerSecond()
{
    // setting up default values
    targetFps = 0;
    maxFrameTime = 0;
    isPossible = true; // makes no diff if this true/false, reset later anyway
    frameStartTime = 0; // likewise
    frameEndTime = 0; // likewise
    frameCounter = 0;
    frameCount = 0.0;
    frameCountElapsedTime = 0;
}

FramesPerSecond::~FramesPerSecond()
{
}

/**
 * Give this object a target frames-per-second to reach.
 * @param targetFpsParam the target FPS that frames are capped at
 */
void FramesPerSecond::target(int targetFpsParam)
{
    targetFps = targetFpsParam;
    maxFrameTime = 1000/targetFps;
}

/**
 * Marks the beginning of a game frame.
 */
void FramesPerSecond::frameStart()
{
    if (frameCountElapsedTime >= 1000) frameCountElapsedTime = 0;
    frameCounter = 0;
    frameStartTime = SDL_GetTicks();
}

/**
 * Marks the end of a game frame and realigns framerate if skewed.
 * @note technically, anything happening after the first line of this
 * @note method is not being tallied into the elapsed time between
 * @note frames, but its only off by a few milliseconds
 * @return false if framerate is to high, otherwise true
 */
bool FramesPerSecond::frameEnd()
{
    frameEndTime = SDL_GetTicks();

    // delay if there is extra time left in this frame
    Uint32 elapsedFrameTime = frameEndTime - frameStartTime;
    Uint32 timeToSpare = maxFrameTime - elapsedFrameTime;
    if (timeToSpare > 0)
    {
        SDL_Delay(timeToSpare);
        isPossible = true;
    }
    else if (timeToSpare == 0)
    {
        // no waiting necessary, continue
        isPossible = true;
    }
    else isPossible = false;

    // increment frame count
    frameCounter++;

    // flag used to update frame count once per second
    frameCountElapsedTime += (elapsedFrameTime + timeToSpare);

    return isPossible;
}

/**
 * Updates the frames-per-second count based on the number frames that have past.
 * @note no matter how many times this function is called the frame count is
 * @note only updated once per second
 */
void FramesPerSecond::updateFrameCount()
{
    if (frameCountElapsedTime >= 1000) frameCount = static_cast<float>(frameCountElapsedTime) / static_cast<float>(maxFrameTime);
}

/**
 * Prints the frames-per-second status message.
 */
void FramesPerSecond::printStatus()
{
    if (frameCountElapsedTime >= 1000) std::cout << "(II) " << frameCount << " fps" << std::endl;
}

/**
 * Gets the frame count.
 * @return the number of frames counted in the last second.
 */
float FramesPerSecond::getFrameCount() const
{
    return frameCount;
}

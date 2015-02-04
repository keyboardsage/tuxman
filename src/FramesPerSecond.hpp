#ifndef FRAMES_PER_SECOND_HPP
#define FRAMES_PER_SECOND_HPP

#include <iostream>

#include <SDL/SDL.h>

/**
 * A class that keeps track of FPS(Frames Per Second).
 */
class FramesPerSecond
{
    public:
        FramesPerSecond();
        ~FramesPerSecond();

        // methods
        void target(int targetFpsParam);
        void frameStart();
        bool frameEnd();
        void updateFrameCount();
        void printStatus();

        float getFrameCount() const;

    private:
        int targetFps; /**< the target frames per second */
        Uint32 maxFrameTime; /**< max time game can spend on a single frame */
        bool isPossible; /**< true if was possible to manip frame under maxFrameTime */
        Uint32 frameStartTime; /**< saves time when frame activity was started */
        Uint32 frameEndTime; /**< saves time when frame activity was ended */
        Uint32 frameCountElapsedTime; /**< time elapsed since frame count last updated */
        int frameCounter; /**< buffer used to do fps counting for this second */
        float frameCount; /**< fps count of last second */
};

#endif

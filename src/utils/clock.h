#pragma once
#include <SDL.h>

class Clock {
private:
    uint32_t mLastTime = 0;
    uint32_t mDelta = 0;

public:
    Clock() {
        tick();
    }

    void tick() {
        uint32_t currentTime = SDL_GetTicks();
        mDelta = currentTime - mLastTime;
        mLastTime = currentTime;
    }
    double getDelta() {
        return (double)mDelta;
    }
};

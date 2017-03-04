#pragma once

#include "../logger.h"
#include "util.h"

namespace MouseSim
{
    extern int thresh; // halving threshhold for the accumulative pixel / degree
    extern float degree; // total number of degree moved, accumulative
    extern int pixel; // total number of pixels moved, accumulative

    extern int maxPixelMS; // max number of pixels you can move in 1 ms

    extern int fd; // file descriptor to the mouse device

    void mouseInit();
    void mouseDestroy();
    float sensitivity();
    void noise(float error, long dt, float& nx, float& ny);
    float norm(float dx, float dy);
    float limit(float normVec, long dt);
    long getDT();

    // Takes the delta angle to be simulated
    void sim(QAngle deltaAngle);
    void update(CUserCmd* cmd);
}

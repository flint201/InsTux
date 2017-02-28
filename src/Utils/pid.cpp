// pid.cpp
#include "pid.h"
#include <ctime>


PID::PID(double p, double i, double d) : kp(p), ki(i), kd(d) {}

// Takes an error value and returns the output
float PID::step(float err)
{
    float now = time(NULL);
    int dt = now - last_step;
    if (dt > 0.1) dt = 0.1; // cap dt at 0.1 second to avoid extremely big dt, which could cause the output to drift uncontrollably.

    last_step = now;

    float derivative = err - prev_err;
    if (dt > 0.00001) { // prevent devision by zero
        integral += err * dt;
        derivative /= dt;
    }

    float output = kp * err + ki * integral + kd * derivative;
    
    prev_err = err;
    return output;
}

void PID::setParam(double p, double i, double d) {
    kp = p;
    ki = i;
    kd = d;
    clear();
}

// reset prev_err and integral
void PID::clear()
{
    prev_err = 0;
    integral = 0;
}

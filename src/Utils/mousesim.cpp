#include "mousesim.h"
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <linux/input.h>
#include <linux/uinput.h>
#include <time.h>
#include <math.h>

#include "pid.h"

namespace MouseSim
{
    float k = 30; // correction constant for aim sensitivity, bigger number = smoother, smaller = faster.

    int thresh = 10000; // halving threshhold for the accumulative pixel/degree
    float degree = 1; // total number of degree moved, accumulative
    int pixel = 30; // total number of pixels moved, accumulative

    float omega = 2; // angular velocity of noise vector in rad/second
    float theta = 1.5; // angle of noise vector in rad
    float noiseCoeff = 0.618;//1/0.618; // magnitude of noise vector measured in times of norm of error

    int fd; // file descriptor to the mouse device
}

void MouseSim::mouseInit()
{
    fd = open("/dev/input/uinput", O_WRONLY | O_NONBLOCK);
    if(fd < 0)
        fd = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    
    if(fd < 0)
        Log << "error: open \"/dev/input/uinput\" and \"/dev/uinput\"." << endl;

    if(ioctl(fd, UI_SET_EVBIT, EV_KEY) < 0 ||
            ioctl(fd, UI_SET_KEYBIT, BTN_LEFT) < 0 ||
            ioctl(fd, UI_SET_EVBIT, EV_REL) < 0 ||
            ioctl(fd, UI_SET_RELBIT, REL_X) < 0 ||
            ioctl(fd, UI_SET_RELBIT, REL_Y) < 0)
        Log << "error: ioctl." << endl;

    struct uinput_user_dev uidev;
    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "uinput-mouse");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1;
    uidev.id.product = 0x1;
    uidev.id.version = 1;

    if(write(fd, &uidev, sizeof(uidev)) < 0)
        Log << "error: write to uinput fd." << endl;

    if(ioctl(fd, UI_DEV_CREATE) < 0)
        Log << "error: ioctl failed to create device." << endl;
}

void MouseSim::mouseDestroy()
{
    ioctl(fd, UI_DEV_DESTROY);
    close(fd);
}

float MouseSim::sensitivity()
{
    return degree / pixel * k;
}

void MouseSim::noise(float error, long dt, float& nx, float& ny)
{
    theta += (dt / 1000 * omega);
    theta -= floor(theta / 3.14159);

    float magnitude = error * noiseCoeff;
    nx = magnitude * sin(theta);
    ny = magnitude * cos(theta);
}

float MouseSim::norm(float dx, float dy)
{
    float sumsqr = dx*dx + dy*dy;
    return sqrt(sumsqr);
}

float MouseSim::limit(float error, long dt)
{
    return dt * sqrt(error);
}

long MouseSim::getDT()
{
    static long lasttime = 0;
    static struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long msNow = spec.tv_sec*1000 + spec.tv_nsec/1.0e6;
    long dt = msNow - lasttime;

    lasttime = msNow;
    return dt;
}

// Takes the delta angle to be simulated
void MouseSim::sim(QAngle deltaAngle)
{
    float dy = deltaAngle.x / sensitivity();
    float dx = -deltaAngle.y / sensitivity();

    float normVec = norm(dx, dy);

    if (normVec < sensitivity() / 2.0)
        return;
    
    long dt = getDT();

    // pid for both x and y
    static const float ku = 7; // 8
    static const float tu = 0.10; // 0.22

    static const float kp = 0.6 * ku;
    static const float ki = 1.2 * ku / tu;
    static const float kd = 3 / 40 * ku * tu;

    static PID pidX(kp, ki, kd);
    static PID pidY(kp, ki, kd);

    if (dt > 15)
    {
        pidX.clear();
        pidY.clear();
        dt = 5;
    }

    dx = pidX.step(dx);
    dy = pidY.step(dy);

    float lim = limit(normVec, dt) + 6;
    dx = dx<lim ? dx : lim;
    dy = dy<lim ? dy : lim;

    if (dx > 10 || dy > 10)
    {
        float nx, ny;
        noise(lim, dt, nx, ny);
        dx += nx;
        dy += ny;
    }

    struct input_event ev;
    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_REL;
    ev.code = REL_X;
    ev.value = (int) round(dx);

    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        Log << "error: write input_event" << endl;

    ev.code = REL_Y;
    ev.value = (int) round(dy);
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        Log << "error: write input_event" << endl;

    memset(&ev, 0, sizeof(struct input_event));
    ev.type = EV_SYN;
    if(write(fd, &ev, sizeof(struct input_event)) < 0)
        Log << "error: write input_event" << endl;
}

void MouseSim::update(CUserCmd* cmd)
{
    static float lastX = cmd->viewangles.x;
    if (cmd->mousedy == 0)
        return;

    degree += abs(cmd->viewangles.x - lastX);
    lastX = cmd->viewangles.x;
    pixel += abs(cmd->mousedy);

    if (pixel > thresh)
    {
        pixel /= 2;
        degree /= 2;
    }
}

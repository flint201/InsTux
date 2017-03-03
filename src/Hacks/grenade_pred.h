#pragma once

#include "../SDK/SDK.h"
#include "../interfaces.h"
#include "../Utils/entity.h"
#include "../Utils/math.h"

class GrenadePred
{
public:
    C_BaseCombatWeapon* weapon;

    GrenadePred(C_BaseCombatWeapon* weapon, C_BasePlayer* localplayer, Vector eyePos, QAngle viewAngle, Vector playerVel);
    void Predict();

private:
    C_BasePlayer* localplayer;
    Vector muzzlePos;
    QAngle muzzleAngle;
    Vector playerVel;

    float throwPower;
    double fuse_time;

    float gravity;
    float elasticity;

    bool isLauncher;
    float safety_time;

    std::vector<Vector> path;

    void Paint();
    void DrawCross(Vector anchor, Vector normal);

    bool Step(Vector& vecSrc, Vector& vForward, float dt);
    void TraceHull(Vector& vecSrc, Vector& vecDst, trace_t& tr);
};

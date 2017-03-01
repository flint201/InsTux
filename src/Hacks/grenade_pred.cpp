#include "grenade_pred.h"
#include "../Utils/draw.h"
#include "../logger.h"
#include <math.h>

GrenadePred::GrenadePred(C_BaseCombatWeapon* weapon, C_BasePlayer* localplayer, Vector eyePos, QAngle viewAngle, Vector playerVel)
    : weapon(weapon), localplayer(localplayer), eyePos(eyePos), viewAngle(viewAngle), playerVel(playerVel)
{
    gravity = 800.0;
    elasticity = 0.15;

    if (!weapon)
        return;

    std::string name(weapon->GetName());

    if (name.find("weapon_f1") != std::string::npos ||
        name.find("weapon_m67") != std::string::npos)
    {
        throwPower = 1100.0;
        fuse_time = 4.0;
    }
    else if (name.find("weapon_gp25") != std::string::npos ||
            name.find("weapon_m203") != std::string::npos)
    {
        throwPower = 3150.0;
        fuse_time = 5.0;
        isLauncher = true;
        safety_time = 0.2;
    }

}


void Dampen(Vector& vec)
{
    float thresh = 10;
    if (vec.x < thresh && -vec.x < thresh)
        vec.x = 0;

    if (vec.y < thresh && -vec.y < thresh)
        vec.y = 0;
    
    if (vec.z < thresh && -vec.z < thresh)
        vec.z = 0;
}


void GrenadePred::Paint()
{
    if (path.size() < 2)
    {
        return;
    }

    Vector prevPoint;
    debugOverlay->ScreenPosition(path[0], prevPoint);

    for (unsigned i = 1; i < path.size(); i++)
    {
        Vector currPoint;
        if(debugOverlay->ScreenPosition(path[i], currPoint))
            continue;

        Draw::Line(Vector2D(prevPoint.x, prevPoint.y),
                Vector2D(currPoint.x, currPoint.y),
                Color(50, 255, 50, 255));
        
        // draw collisoin cross;

        prevPoint = currPoint;
    }
}


void GrenadePred::DrawCross(Vector anchor, Vector normal)
{
    Vector eye2Anchor = anchor - eyePos;
    Vector base1 = eye2Anchor - normal * eye2Anchor.Dot(normal);

    base1.NormalizeInPlace();
    normal.NormalizeInPlace();

    //base2 = base1 X normal
    Vector base2(base1.y*normal.z - base1.z*normal.y,
                base1.z*normal.x - base1.x*normal.z,
                base1.x*normal.y - base1.y*normal.x);

    base2.NormalizeInPlace();

    float distance = Math::GetDistance(eyePos, anchor);
    base1 *= sqrt(distance);
    base2 *= sqrt(distance);

    /*
    Vector topLeft = anchor - base1 + base2;
    Vector topRight = anchor + base1 + base2;
    Vector botRight = anchor + base1 - base2;
    Vector botLeft = anchor - base1 - base2;
    */
    Vector left = anchor - base2;
    Vector right = anchor + base2;
    Vector top = anchor + base1;
    Vector bottom = anchor - base1;

    Vector start2D, end2D;

    Color colorCross = Color(250, 20, 20, 255);
    debugOverlay->ScreenPosition(left, start2D);
    debugOverlay->ScreenPosition(right, end2D);
    Draw::Line(Vector2D(start2D.x, start2D.y),
            Vector2D(end2D.x, end2D.y),
            colorCross);

    debugOverlay->ScreenPosition(top, start2D);
    debugOverlay->ScreenPosition(bottom, end2D);
    Draw::Line(Vector2D(start2D.x, start2D.y),
            Vector2D(end2D.x, end2D.y),
            colorCross);
}


void GrenadePred::Predict()
{
    float dt = 0.0211;// interval per tick
    int logstep = (int)(0.05 / dt);

    Vector vForward(0, 0, 0);
    Math::AngleVectors(viewAngle, vForward);
    Vector vecSrc = eyePos + vForward * 10.0;
    vecSrc.z += 12;

    Vector vecDst = vecSrc + vForward * 12.0;
    
    trace_t tr;
    TraceHull(vecSrc, vecDst, tr);

    vecSrc = tr.endpos - vForward * 6.0;
    vForward = playerVel * 1.25 + vForward * throwPower;

    for (int i = 0; i < (int)(fuse_time / dt); i++)
    {
        if (i % logstep == 0 &&
                (!isLauncher || Math::GetDistance(eyePos, vecSrc) > 435))
            path.push_back(vecSrc);

        if(Step(vecSrc, vForward, dt) && isLauncher)
            break;
    }

    Paint();
}

void GrenadePred::TraceHull(Vector& vecSrc, Vector& vecDst, trace_t& tr)
{
    Ray_t ray;
    ray.Init(vecSrc, vecDst);

    CTraceFilter traceFilter;
    traceFilter.pSkip = localplayer;
    
    trace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);
}

bool GrenadePred::Step(Vector& vecSrc, Vector& vForward, float dt)
{
    Vector vecDst = vecSrc + vForward * dt;
    vForward.z -= gravity * dt;

    trace_t tr;
    TraceHull(vecSrc, vecDst, tr);

    bool bCollision = false;
    if (tr.fraction < 0.99)
    {
        bCollision = true;

        Vector forceFromSurface = tr.plane.normal * vForward.Dot(tr.plane.normal);
        vForward -= forceFromSurface; // lose all velocity against normal of surface
        vForward *= 0.6; // lose some velocity due to friction
        vForward -= forceFromSurface * elasticity; // regain a portion of it from the surface

        Dampen(vForward);

        if (!isLauncher || Math::GetDistance(eyePos, vecSrc) > 435)
            DrawCross(vecSrc, tr.plane.normal);
    }
    else
    {
        vecSrc = tr.endpos;
    }

    return bCollision;
}

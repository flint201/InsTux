#include <iomanip>

#include "radar.h"
#include "hacks.h"

#include "grenade_pred.h"
#include "../settings.h"

#define PI 3.1415926


void Radar::Paint()
{
    if (!Settings::Radar::enable)
        return;

    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer || !Settings::Radar::enable)
        return;

    int sWidth, sHeight;
    engine->GetScreenSize(sWidth, sHeight);

    TeamID myteam = localplayer->GetTeam();
    Vector vecPos = localplayer->GetVecOrigin();

    QAngle angViewAngle;
    engine->GetViewAngles(angViewAngle);
    float forwardAngle = angViewAngle.y;

    Vector vecBaseY;
    Math::SinCos(forwardAngle / 180.0 * PI, &vecBaseY.y, &vecBaseY.x);

    Vector vecBaseX;
    forwardAngle -= 90;
    Math::SinCos(forwardAngle / 180.0 * PI, &vecBaseX.y, &vecBaseX.x);

    int size = Settings::Radar::size;
    float range = Settings::Radar::range;
    float radius = 4;//Settings::Radar::dot_radius;

    int anchorX = sWidth / 2 - size;
    int anchorY = 0;

    Draw::Line(anchorX, anchorY + size, anchorX + size * 2, anchorY + size, Color(150, 150, 150, 150));
    Draw::Line(anchorX + size, anchorY, anchorX + size, anchorY + size * 2, Color(150, 150, 150, 150));

    for (int i = 0; i <= engine->GetMaxClients(); i++)
    {
        C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);
        if (!player ||
            player->IsDormant() ||
            !player->GetAlive() ||
            player==localplayer)
        {
            continue;
        }

        Vector vecPlayerPosRel = player->GetEyePosition() - vecPos;

        float coordY = vecPlayerPosRel.Dot(vecBaseY);
        float coordX = vecPlayerPosRel.Dot(vecBaseX);

        if (coordY < -range || coordY > range ||
            coordX < -range || coordX > range)
            continue;
        
        coordY = coordY / range * size;
        coordX = coordX / range * size;

        float pixelY = size - coordY + anchorY;
        float pixelX = size + coordX + anchorX;

        Color color = Settings::Radar::color_friendly;
        if (player->GetTeam() != myteam)
            color = Settings::Radar::color_hostile;

        Vector2D circleCenter(pixelX, pixelY);
        Draw::Circle(circleCenter, 16, radius, color);

        float playerFacing = player->GetEyeAngles()->y;
        Vector vecPlayerFacing;
        Math::SinCos(playerFacing / 180.0 * PI, &vecPlayerFacing.y, &vecPlayerFacing.x);

        Vector2D v2dTick;
        v2dTick.y = -vecPlayerFacing.Dot(vecBaseY);
        v2dTick.x = vecPlayerFacing.Dot(vecBaseX);
        v2dTick *= radius + 1;

        Draw::Line(circleCenter, circleCenter + v2dTick, color);
    }
}

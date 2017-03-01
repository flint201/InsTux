#include <iomanip>

#include "esp.h"
#include "hacks.h"

#include "grenade_pred.h"

std::string dbgMsg;

// Helper function for drawing info such as local player health and ammo count.
void DrawInfo(C_BasePlayer* localplayer, int sWidth, int sHeight)
{
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    int ammoInMag = 0;
    if (activeWeapon)
        ammoInMag = activeWeapon->GetAmmo();

    std::stringstream ss;
    ss << "HP: " << localplayer->GetHealth() << "    Ammo: " << ammoInMag;

    Draw::Text(sWidth - 250, sHeight - 30, ss.str().c_str(), 0, Color(66, 180, 255, 255));
    Draw::Text(10, 50, ss.str().c_str(), 0, Color(66, 180, 255, 255));

    // draw crosshairs
    Draw::Line(sWidth/2-4, sHeight/2, sWidth/2+3, sHeight/2, Color(255, 20, 20, 255));
    Draw::Line(sWidth/2, sHeight/2-4, sWidth/2, sHeight/2+3, Color(255, 20, 20, 255));
}


void PredictGrenade(C_BasePlayer* localplayer)
{
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    Vector eyePos = localplayer->GetEyePosition();

    QAngle viewAngles;
    engine->GetViewAngles(viewAngles);

    GrenadePred gp(activeWeapon, localplayer, eyePos, viewAngles, localplayer->GetVelocity());
    gp.Predict();
}


void ESP::Paint()
{
    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer)
    {
        return;
    }

    PredictGrenade(localplayer);

    int sWidth, sHeight;
    engine->GetScreenSize(sWidth, sHeight);

    TeamID myteam = localplayer->GetTeam();

    DrawInfo(localplayer, sWidth, sHeight);

    if(!inputSystem->IsButtonDown(ButtonCode_t::KEY_CAPSLOCK))
    {
        Draw::Text(10, 5, "InsTux", 0, Color(255, 255, 255, 255));
        return;
    }

    for (int i = 0; i <= engine->GetMaxClients(); i++)
    {
        C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);
        if (!player
            || player == localplayer
            || player->GetTeam() == myteam
            || player->GetDormant()
            || !player->GetAlive())
        {
            continue;
        }

        IEngineClient::player_info_t playerInfo;
        if (!engine->GetPlayerInfo(i, &playerInfo))
            continue;

        // draw skeleton
        DrawSkeleton(player);

        // draw name
        Vector vPlayerOrigin;
        debugOverlay->ScreenPosition(player->GetVecOrigin(), vPlayerOrigin);
        Draw::Text(vPlayerOrigin.x, (int)(vPlayerOrigin.y + 10), playerInfo.name, 0, Color(255, 255, 255, 255));
    }

}

void ESP::DrawSkeleton(C_BasePlayer* player)
{
    studiohdr_t* pStudioModel = modelInfo->GetStudioModel(player->GetModel());
    if (!pStudioModel)
        return;

    static matrix3x4_t pBoneToWorldOut[128];
    if (player->SetupBones(pBoneToWorldOut, 128, 256, 0))
    {
        for (int i = 0; i < pStudioModel->numbones; i++)
        {
            mstudiobone_t* pBone = pStudioModel->pBone(i);
            if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
                continue;

            Vector vBonePos1;
            if (debugOverlay->ScreenPosition(Vector(pBoneToWorldOut[i][0][3], pBoneToWorldOut[i][1][3], pBoneToWorldOut[i][2][3]), vBonePos1))
                continue;

            Vector vBonePos2;
            if (debugOverlay->ScreenPosition(Vector(pBoneToWorldOut[pBone->parent][0][3], pBoneToWorldOut[pBone->parent][1][3], pBoneToWorldOut[pBone->parent][2][3]), vBonePos2))
                continue;

            Draw::Line(Vector2D(vBonePos1.x, vBonePos1.y), Vector2D(vBonePos2.x, vBonePos2.y), Color(66, 173, 244, 255));
        }
    }
}

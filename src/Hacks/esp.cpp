#include <iomanip>


#include "esp.h"
#include "hacks.h"

#include "grenade_pred.h"
#include "../settings.h"

std::string dbgMsg;

HFont esp_name_font = 0xe5;
HFont font_foundation18 = 0x110;
HFont font_foundation20 = 0x111;

// Helper function for drawing info such as local player health and ammo count.
void DrawInfo(C_BasePlayer* localplayer, int sWidth, int sHeight)
{
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    int ammoInMag = 0;
    if (activeWeapon)
        ammoInMag = activeWeapon->GetAmmo();

    std::stringstream ss;
    ss << "HP: " << localplayer->GetHealth() << "    Ammo: " << ammoInMag;

    Draw::Text(10, 50, ss.str().c_str(), font_foundation20, Color(0, 0, 0, 255));
    Draw::Text(10, 50, ss.str().c_str(), font_foundation20, Color(66, 180, 255, 255));

    Draw::Text(sWidth - 250, sHeight - 30, ss.str().c_str(), font_foundation20, Color(0, 0, 0, 255));
    Draw::Text(sWidth - 250, sHeight - 30, ss.str().c_str(), font_foundation20, Color(66, 180, 255, 255));

    if (FakeLag::enable)
        Draw::Text(10, 70, "FakeLag", font_foundation18, Color(255, 30, 30, 255));

    // draw reload warning if needed
    if (activeWeapon && ammoInMag < 10 && ammoInMag >= 0 && localplayer->GetAlive())
    {
        std::stringstream ssLowAmmo;
        ssLowAmmo << "LOW AMMO  " << ammoInMag;
        Draw::Text(sWidth/2 - 30, sHeight * 0.618, ssLowAmmo.str().c_str(), font_foundation20, Color(255, 30, 30, 255));
    }

    // draw crosshairs
    Draw::Line(sWidth/2-4, sHeight/2, sWidth/2+3, sHeight/2, Color(255, 20, 20, 255));
    Draw::Line(sWidth/2, sHeight/2-4, sWidth/2, sHeight/2+3, Color(255, 20, 20, 255));

    // draw bullet impact point
    Math::NormalizeAngles(CreateMove::muzzleangle);

    Vector vMuzzleDir;
    Math::AngleVectors(CreateMove::muzzleangle, vMuzzleDir);
    
    Vector vecSrc = CreateMove::muzzlepos;
    Vector vecDst = CreateMove::muzzlepos + vMuzzleDir * 500;

    Ray_t ray;
    ray.Init(vecSrc, vecDst);
    CTraceFilter traceFilter;
    traceFilter.pSkip = localplayer;

    trace_t tr;
    trace->TraceRay(ray, MASK_SHOT, &traceFilter, &tr);

    Vector vImpact = tr.endpos;
    Vector vImpactScr;
    debugOverlay->ScreenPosition(vImpact, vImpactScr);

    Draw::Line(vImpactScr.x-3, vImpactScr.y, vImpactScr.x+2, vImpactScr.y, Color(255, 156, 35, 255));
    Draw::Line(vImpactScr.x, vImpactScr.y-3, vImpactScr.x, vImpactScr.y+2, Color(255, 156, 35, 255));
}


void PredictGrenade(C_BasePlayer* localplayer)
{
    C_BaseCombatWeapon* activeWeapon = (C_BaseCombatWeapon*) entityList->GetClientEntityFromHandle(localplayer->GetActiveWeapon());
    GrenadePred gp(activeWeapon, localplayer, CreateMove::muzzlepos, CreateMove::muzzleangle, localplayer->GetVelocity());
    gp.Predict();
}

void DrawBoxESP(C_BasePlayer* localplayer, int sWidth, int sHeight, Vector enemyPos, Color boxColor, bool snapLine)
{
    Vector enemyScreenPos;
    if (debugOverlay->ScreenPosition(enemyPos, enemyScreenPos) == 0)
    {
        int HBox = 500 * 12 * 4 / Math::GetDistance(enemyPos, localplayer->GetVecOrigin());
        int WBox = 200 * 12 * 4 / Math::GetDistance(enemyPos, localplayer->GetVecOrigin());
        Draw::Rectangle(enemyScreenPos.x - WBox/2.0, enemyScreenPos.y - HBox, enemyScreenPos.x + WBox/2.0, enemyScreenPos.y, boxColor);
        if (snapLine)
            Draw::Line(sWidth/2.0, sHeight, enemyScreenPos.x, enemyScreenPos.y, boxColor);
    }
}


void ESP::Paint()
{
    C_BasePlayer* localplayer = (C_BasePlayer*) entityList->GetClientEntity(engine->GetLocalPlayer());
    if (!localplayer || !Settings::ESP::enable)
        return;

    PredictGrenade(localplayer);

    int sWidth, sHeight;
    engine->GetScreenSize(sWidth, sHeight);

    TeamID myteam = localplayer->GetTeam();

    DrawInfo(localplayer, sWidth, sHeight);

    static bool drawESP = false;
    static bool lastKeyDown = false;
    bool keyDown = inputSystem->IsButtonDown(Settings::ESP::key);
    if (Settings::ESP::is_toggle)
    {
        if (keyDown && !lastKeyDown)
            drawESP = !drawESP;
    } else
    {
        drawESP = keyDown;
    }
    lastKeyDown = keyDown;

    Draw::Text(10, 5, "InsTux", font_foundation18, Color(255, 255, 255, 255));

    for (int i = 0; i <= engine->GetMaxClients(); i++)
    {
        C_BasePlayer* player = (C_BasePlayer*) entityList->GetClientEntity(i);
        if (!player
            || player == localplayer
            || player->GetTeam() == myteam
            || player->IsDormant()
            || !player->GetAlive())
        {
            continue;
        }

        IEngineClient::player_info_t playerInfo;
        if (!engine->GetPlayerInfo(i, &playerInfo))
            continue;

        // draw skeleton
        if (Settings::ESP::show_bone && drawESP)
            DrawSkeleton(player);

        // draw name
        if (Settings::ESP::show_name && drawESP)
        {
            Vector vPlayerOrigin;
            if (debugOverlay->ScreenPosition(player->GetVecOrigin(), vPlayerOrigin) == 0)
                Draw::Text(vPlayerOrigin.x, (int)(vPlayerOrigin.y + 3), playerInfo.name, esp_name_font, Color(255, 255, 255, 255));
        }

        QAngle angViewAngle;
        engine->GetViewAngles(angViewAngle);

        Vector vecView;
        Math::AngleVectors(angViewAngle, vecView);

        Vector vecEnemy = player->GetVecOrigin() - localplayer->GetVecOrigin();
        float dotProd = vecEnemy.Dot(vecView);
        if (dotProd < -1 && Settings::ESP::draw_behind)
        {
            // draw behind
            Vector mirrorEnemyPos = localplayer->GetVecOrigin() * 2.0 - player->GetVecOrigin();
            if (Util::Ray(localplayer, player, i, localplayer->GetEyePosition(), player->GetEyePosition()))
                DrawBoxESP(localplayer, sWidth, sHeight, mirrorEnemyPos, Settings::ESP::color_box_behind_vis, true);
            else if (Math::GetDistance(localplayer->GetEyePosition(), player->GetEyePosition())
                    < Settings::ESP::draw_behind_range * 12 * 3)
                DrawBoxESP(localplayer, sWidth, sHeight, mirrorEnemyPos, Settings::ESP::color_box_behind_hid, true);
        }
        else if (Settings::ESP::show_box && drawESP)
        {
            // draw box front
            if (Util::Ray(localplayer, player, i, localplayer->GetEyePosition(), player->GetEyePosition()))
                DrawBoxESP(localplayer, sWidth, sHeight, player->GetVecOrigin(), Settings::ESP::color_box_vis, true);
            else
                DrawBoxESP(localplayer, sWidth, sHeight, player->GetVecOrigin(), Settings::ESP::color_box_hid, false);
        }
    }

}

void ESP::DrawSkeleton(C_BasePlayer* player)
{
    studiohdr_t* pStudioModel = modelInfo->GetStudioModel(player->GetModel());
    if (!pStudioModel)
        return;

    static matrix3x4_t BoneMatrix[128];
    if (player->SetupBones(BoneMatrix, 128, BONE_USED_BY_HITBOX, 0))
    {
        for (int i = 0; i < pStudioModel->numbones; i++)
        {
            mstudiobone_t* pBone = pStudioModel->pBone(i);
            if (!pBone || !(pBone->flags & 256) || pBone->parent == -1)
                continue;

            Vector vBonePos1;
            if (debugOverlay->ScreenPosition(Vector(BoneMatrix[i][0][3], BoneMatrix[i][1][3], BoneMatrix[i][2][3]), vBonePos1))
                continue;

            Vector vBonePos2;
            if (debugOverlay->ScreenPosition(Vector(BoneMatrix[pBone->parent][0][3], BoneMatrix[pBone->parent][1][3], BoneMatrix[pBone->parent][2][3]), vBonePos2))
                continue;

            Draw::Line(Vector2D(vBonePos1.x, vBonePos1.y), Vector2D(vBonePos2.x, vBonePos2.y), Settings::ESP::color_bone);
        }
    }

    /*
    matrix3x4_t boneHead = BoneMatrix[(int)Bone::NECK];
    Vector vecBoneNeck = Vector(boneHead[0][3], boneHead[1][3], boneHead[2][3]);
    Vector vecBoneHead = vecBoneNeck + Vector(boneHead[0][0], boneHead[1][0], boneHead[2][0]) * 9;
    Vector v1, v2;
    debugOverlay->ScreenPosition(vecBoneNeck, v1);
    debugOverlay->ScreenPosition(vecBoneHead, v2);
    Draw::Line(Vector2D(v1.x, v1.y), Vector2D(v2.x, v2.y), Color(255, 0, 0, 255));
    */
}

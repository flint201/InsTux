#include "winmain.h"
#include "widgets.h"

bool WinMain::showWindow = true;

ImColor mkImColor(Color c)
{
    return ImColor(c.r, c.g, c.b, c.a);
}

void WinMain::RenderWindow()
{
    if (!showWindow)
        return;

    ImColor imcolmain = mkImColor(Settings::GUI::color_main);
    ImColor imcolmain2 = mkImColor(Settings::GUI::color_main2);
    ImColor imcolhl = mkImColor(Settings::GUI::color_hl);

    ImGui::GetStyle().Colors[ImGuiCol_Border] = imcolmain;

    ImGui::GetStyle().Colors[ImGuiCol_TitleBg] = imcolmain;
    ImGui::GetStyle().Colors[ImGuiCol_TitleBgCollapsed] = imcolhl;
    ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = imcolhl;

    ImGui::GetStyle().Colors[ImGuiCol_CheckMark] = imcolmain2;

    ImGui::GetStyle().Colors[ImGuiCol_SliderGrab] = imcolmain2;
    ImGui::GetStyle().Colors[ImGuiCol_SliderGrabActive] = imcolhl;

    ImGui::GetStyle().Colors[ImGuiCol_ButtonHovered] = imcolhl;
    ImGui::GetStyle().Colors[ImGuiCol_ButtonActive] = imcolmain2;

    static int page = 0;
    ImGui::SetNextWindowSize(ImVec2(1130, 700), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("InsTux", &WinMain::showWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
    {
        const char* tabs[] = {
                "Aimbot",
                "Visuals",
                "Radar",
                "Misc",
                "Players"
        };

        for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
        {
            if (page == i)
                ImGui::GetStyle().Colors[ImGuiCol_Button] = imcolmain;
            else
                ImGui::GetStyle().Colors[ImGuiCol_Button] = ImColor(50, 50, 50, 50);

            if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 9, 0)))
                page = i;

            if (i < IM_ARRAYSIZE(tabs) - 1)
                ImGui::SameLine();
        }

        ImGui::GetStyle().Colors[ImGuiCol_Button] = imcolmain;
        ImGui::Separator();

        switch (page)
        {
            case 0:
                TabAimbotRender();
                break;
            case 1:
                TabVisualsRender();
                break;
            case 2:
                TabRadarRender();
                break;
            case 3:
                TabMiscRender();
                break;
            case 4:
                TabPlayersRender();
                break;
        }
    }

    ImGui::Text(" ");
    ImGui::Text(" ");
    if (ImGui::Button("    SAVE    "))
    {
        pstring path = getenv("HOME");
        path << "/.instux.cfg";
        Settings::SaveToFile(path);
    }
    ImGui::End();
}

void WinMain::TabAimbotRender()
{
    ImGui::Checkbox("Enable", &Settings::Aimbot::enable);
    if (!Settings::Aimbot::enable)
    {
        ImGui::Separator();
        return;
    }

    ImGui::SameLine();
    ImGui::Text("        Aimbot Activation Key");
    ImGui::SameLine();
    Widgets::KeyBindButton(&Settings::Aimbot::key);
    ImGui::Separator();

    ImGui::Text(" ");
    ImGui::Text("Aimbot General Settings");
    ImGui::Separator();

    ImGui::Checkbox(" Limit Aim Speed", &Settings::Aimbot::limit_aim_speed);
    ImGui::SliderFloat("##SENSITIVITY", &Settings::Aimbot::sensitivity, 0.f, 5.f, "Sensitivity: %0.2f");
    ImGui::SliderFloat("##FOV", &Settings::Aimbot::fov, 1.f, 360.f, "Target Selection FOV: %0.2f");
    ImGui::SliderFloat("##CONTTHRESH", &Settings::Aimbot::cont_thresh, 0.f, 200, "Continuous Targeting Mode Threshold Distance (Yards) %0.1f");

    ImGui::Text(" ");
    ImGui::SliderFloat("##RECOILX", &Settings::Aimbot::recoilx, 0.f, 20.f, "Recoil cancellation Vertical: %0.2f");
    ImGui::SliderFloat("##RECOILY", &Settings::Aimbot::recoily, 0.f, 20.f, "Recoil cancellation Horizontal: %0.2f");

    ImGui::Separator();

    ImGui::Text(" ");
    ImGui::Text("Aimbot PID Tuning");
    ImGui::Separator();
    ImGui::SliderFloat("##KU", &Settings::Aimbot::ku, 0.f, 50.f, "PID Ku: %0.2f");
    ImGui::SliderFloat("##TU", &Settings::Aimbot::tu, 0.f, 10.f, "PID Tu: %0.2f");
    ImGui::Separator();

    ImGui::Text(" ");
    ImGui::Text("Silent Aim Settings");
    ImGui::Separator();
    ImGui::SliderFloat("##SILENTFOVHIP", &Settings::Aimbot::silent_fov_hip, 0.f, 360.f, "Hip-fire silent aim FOV(Rage bot): %0.2f");
    if (Settings::Aimbot::silent_fov > Settings::Aimbot::fov)
        Settings::Aimbot::silent_fov = Settings::Aimbot::fov;
    ImGui::SliderFloat("##SILENTFOV", &Settings::Aimbot::silent_fov, 0.f, Settings::Aimbot::fov, "Aim-down-sight silent aim FOV: %0.2f");
    ImGui::Text("Caution: Aim-down-sight silent aim is very obvious to spectators, a small FOV is recommended.");
    ImGui::Separator();
}

void WinMain::TabVisualsRender()
{
    ImGui::Text("Visuals");
    ImGui::Separator();

    ImGui::Text("Cham");
    ImGui::BeginChild("Cham", ImVec2(0, 110), true);
    {
        ImGui::Checkbox("Only When Key Pressed  ", &Settings::Cham::only_on_key_down);
        if (Settings::Cham::only_on_key_down)
        {
            ImGui::SameLine();
            Widgets::KeyBindButton(&Settings::Cham::key);
        }
        ImGui::Separator();

        ImGui::Text("Cham Colors");
        ImGui::Separator();
        static float col_vis[4];
        Widgets::ColorEdit4Color("Color Visible", &Settings::Cham::color_visible, col_vis);
        static float col_hid[4];
        Widgets::ColorEdit4Color("Color Hidden", &Settings::Cham::color_hidden, col_hid);
    }
    ImGui::EndChild();

    ImGui::Text(" ");
    ImGui::Text("ESP");
    ImGui::BeginChild("ESP", ImVec2(0, 200), true);
    {
        if (Settings::ESP::enable)
            ImGui::Columns(3);
        ImGui::Checkbox("Enable", &Settings::ESP::enable);
        if (Settings::ESP::enable)
        {
            ImGui::NextColumn();
            ImGui::Text("Activation Key");
            ImGui::SameLine();
            Widgets::KeyBindButton(&Settings::ESP::key);
            ImGui::NextColumn();
            ImGui::Checkbox("Toggle", &Settings::ESP::is_toggle);
            ImGui::NextColumn();
            ImGui::Columns(1);

            ImGui::Separator();
            ImGui::Columns(4);
            ImGui::Checkbox("Show Bone", &Settings::ESP::show_bone);
            ImGui::NextColumn();
            ImGui::Checkbox("Show Name", &Settings::ESP::show_name);
            ImGui::NextColumn();
            ImGui::Checkbox("Show Box", &Settings::ESP::show_box);
            ImGui::NextColumn();
            ImGui::Checkbox("Show Box Behind", &Settings::ESP::draw_behind);
            ImGui::NextColumn();
            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::SliderFloat("##DRAWBEHINDRANGE", &Settings::ESP::draw_behind_range, 0, 200, "Show Box Behind Range Limit (Yards): %.0f");
            ImGui::Separator();

            static float col_bone[4];
            Widgets::ColorEdit4Color("Bone Color", &Settings::ESP::color_bone, col_bone);
            static float col_box_vis[4];
            Widgets::ColorEdit4Color("Box Color Visible", &Settings::ESP::color_box_vis, col_box_vis);
            static float col_box_hid[4];
            Widgets::ColorEdit4Color("Box Color Hidden", &Settings::ESP::color_box_hid, col_box_hid);
            static float col_box_behind_vis[4];
            Widgets::ColorEdit4Color("Box Color Visible Behind", &Settings::ESP::color_box_behind_vis, col_box_behind_vis);
            static float col_box_behind_hid[4];
            Widgets::ColorEdit4Color("Box Color Hidden Behind", &Settings::ESP::color_box_behind_hid, col_box_behind_hid);
        }
    }
    ImGui::EndChild();
}

void WinMain::TabRadarRender()
{
    ImGui::Text("Radar");
    ImGui::Separator();
    ImGui::Checkbox("Enable", &Settings::Radar::enable);
    ImGui::Text(" ");
    ImGui::Separator();
    if (Settings::Radar::enable)
    {
        ImGui::SliderFloat("##RADARRANGE", &Settings::Radar::range, 0.f, 300.f, "Radar Range (Yards): %.0f");
        ImGui::Text(" ");
        ImGui::SliderInt("##RADARSIZE", &Settings::Radar::size, 50, 500, "Radar Size (Pixels): %.0f");
        ImGui::SliderFloat("##RADARDOTSIZE", &Settings::Radar::dot_radius, 3.0, 20.0, "Radar Dot Size (Pixels): %.0f");
        ImGui::Text(" ");
        ImGui::Separator();
        static float col_hos[4];
        Widgets::ColorEdit4Color("Color Hostile", &Settings::Radar::color_hostile, col_hos);

        static float col_fri[4];
        Widgets::ColorEdit4Color("Color Friendly", &Settings::Radar::color_friendly, col_fri);
        ImGui::Separator();

        ImGui::Text(" ");
        ImGui::Checkbox("Show Level Indicator", &Settings::Radar::draw_level);
    }
}

void WinMain::TabMiscRender()
{
    ImGui::Text("Removals");
    ImGui::Separator();
    ImGui::Text("No Flash    ");
    ImGui::SameLine();
    ImGui::SliderFloat("##NOFLASH", &Settings::NoFlash::alpha, 0.f, 255.f, "No Flash Alpha: %.0f");

    ImGui::Columns(2);
    ImGui::Checkbox("No Smoke", &Settings::NoSmoke::enable);
    ImGui::NextColumn();
    ImGui::Checkbox("Diable Post Processing", &Settings::PPDisable::enable);
    ImGui::SameLine();
    ImGui::Columns(1);
    ImGui::Separator();


    ImGui::Text(" ");
    ImGui::Text("Fake Lag");
    ImGui::Separator();

    ImGui::Columns(2);
    ImGui::Text("Toggle Key");
    ImGui::SameLine();
    Widgets::KeyBindButton(&Settings::FakeLag::key);
    ImGui::NextColumn();

    ImGui::Checkbox("Automatic - Only Turns On When Visible By Enemy", &Settings::FakeLag::automatic);
    ImGui::NextColumn();

    ImGui::Columns(1);
    ImGui::SliderInt("##LAGVAL", &Settings::FakeLag::value, 0, 15, "Lag Value (Ticks): %.0f");
    ImGui::Separator();


    ImGui::Text(" ");
    ImGui::Text("GUI");
    ImGui::Separator();
    static float col_main[4];
    Widgets::ColorEdit4Color("Color Primary", &Settings::GUI::color_main, col_main);
    static float col_main2[4];
    Widgets::ColorEdit4Color("Color Secondary", &Settings::GUI::color_main2, col_main2);
    static float col_hl[4];
    Widgets::ColorEdit4Color("Color Highlight", &Settings::GUI::color_hl, col_hl);
    ImGui::Separator();
}

void WinMain::TabPlayersRender()
{
    static int currentPlayer = -1;
    if (engine->IsInGame())
        currentPlayer = engine->GetLocalPlayer();

    IEngineClient::player_info_t playerInfo;

    ImGui::ListBoxHeader("##PLAYERS", ImVec2(-1, (ImGui::GetWindowSize().y - 20)));

    ImGui::Columns(3);

    ImGui::Text("Index");
    ImGui::NextColumn();

    ImGui::Text("Nickname");
    ImGui::NextColumn();

    ImGui::Text("Steam ID");
    ImGui::NextColumn();

    for (int i = 1; i < engine->GetMaxClients(); i++)
    {
        if (!engine->GetPlayerInfo(i, &playerInfo))
            continue;

        ImGui::Separator();

        std::string id = std::to_string(i);
        if (ImGui::Selectable(id.c_str(), i == currentPlayer, ImGuiSelectableFlags_SpanAllColumns))
        {
            currentPlayer = i;
        }
        ImGui::NextColumn();

        ImGui::Text("%s", playerInfo.name);
        ImGui::NextColumn();

        ImGui::Text("%s", playerInfo.guid);
        ImGui::NextColumn();
    }
    ImGui::ListBoxFooter();
}

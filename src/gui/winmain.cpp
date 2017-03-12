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
    ImGui::SetNextWindowSize(ImVec2(960, 645), ImGuiSetCond_FirstUseEver);
    if (ImGui::Begin("InsTux", &WinMain::showWindow, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders))
    {
        const char* tabs[] = {
                "Aimbot",
                "Visuals",
                "Radar",
                "Misc"
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
    ImGui::Separator();
    if (!Settings::Aimbot::enable)
        return;

    ImGui::Text(" ");
    ImGui::Text("Aimbot Activation Key");
    ImGui::SameLine();
    Widgets::KeyBindButton(&Settings::Aimbot::key);
    ImGui::Separator();

    ImGui::Text(" ");
    ImGui::Text("Aimbot General Settings");
    ImGui::Separator();
    ImGui::Checkbox("Limit Aim Speed    ", &Settings::Aimbot::limit_aim_speed);
    ImGui::SliderFloat("##SENSITIVITY", &Settings::Aimbot::sensitivity, 0.f, 5.f, "Sensitivity: %0.2f");
    ImGui::SliderFloat("##FOV", &Settings::Aimbot::fov, 1.f, 360.f, "Target Selection FOV: %0.2f");
    ImGui::SliderFloat("##CONTTHRESH", &Settings::Aimbot::cont_thresh, 0.f, 200, "Continuous Targeting Mode Threshold Distance (Yards) %0.1f");

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
    ImGui::Text(" ");
    ImGui::Text("Caution: Aim-down-sight silent aim is very obvious to spectators, a small FOV is recommended.");
    ImGui::SliderFloat("##SILENTFOV", &Settings::Aimbot::silent_fov, 0.f, Settings::Aimbot::fov, "Aim-down-sight silent aim FOV: %0.2f");
    ImGui::Separator();
}

void WinMain::TabVisualsRender()
{
    ImGui::Text("Visuals");
    ImGui::Separator();

    ImGui::Text("Cham");
    ImGui::BeginChild("Cham", ImVec2(0, 150), true);
    {
        ImGui::Checkbox("Only When Key Pressed", &Settings::Cham::only_on_key_down);
        if (Settings::Cham::only_on_key_down)
        {
            ImGui::Text("    Activation Key");
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
        ImGui::Separator();
    }
    ImGui::EndChild();

    ImGui::Separator();
    ImGui::Text("ESP");
    ImGui::BeginChild("ESP", ImVec2(0, 150), true);
    {
        ImGui::Checkbox("Enable", &Settings::ESP::enable);
        if (Settings::ESP::enable)
        {
            ImGui::Separator();
            ImGui::Text("Activation Key");
            ImGui::SameLine();
            Widgets::KeyBindButton(&Settings::ESP::key);

            ImGui::Separator();
            ImGui::Checkbox("Show Bone", &Settings::ESP::show_bone);
            ImGui::Checkbox("Show Name", &Settings::ESP::show_name);
            ImGui::Checkbox("Draw Behind", &Settings::ESP::draw_behind);
        }

        ImGui::Separator();
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
    ImGui::SliderFloat("##NOFLASH", &Settings::NoFlash::alpha, 0.f, 255.f, "No Flash Alpha: %.0f");
    ImGui::Checkbox("No Smoke", &Settings::NoSmoke::enable);
    ImGui::Separator();


    ImGui::Text(" ");
    ImGui::Text("Fake Lag");
    ImGui::Separator();

    ImGui::Text("Toggle Key");
    ImGui::SameLine();
    Widgets::KeyBindButton(&Settings::FakeLag::key);

    ImGui::Checkbox("Automatic - Only Turns On When Visible By Enemy", &Settings::FakeLag::automatic);
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

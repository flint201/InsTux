#include "widgets.h"


void Widgets::KeyBindButton(ButtonCode_t* key)
{
    const char* text = inputSystem->ButtonCodeToString(*key);
    static bool shouldListen = false;
    if (shouldListen)
    {
        text = "-- press a key --";
        ButtonCode_t keyPressed = Util::GetButtonPressed();
        if (keyPressed != ButtonCode_t::BUTTON_CODE_INVALID)
        {
            *key = keyPressed;
            shouldListen = false;
        }
    }
    else
        text = Util::ToUpper(std::string(text)).c_str();

    if (ImGui::Button(text, ImVec2(-1, 0)))
        shouldListen = true;
}

void Widgets::ColorEdit4Color(const char* label, Color* color, float* colf)
{
    colf[0] = color->r / 255.f;
    colf[1] = color->g / 255.f;
    colf[2] = color->b / 255.f;
    colf[3] = color->a / 255.f;
    if (ImGui::ColorEdit4(label, colf))
    {
        color->r = colf[0] * 255.f;
        color->g = colf[1] * 255.f;
        color->b = colf[2] * 255.f;
        color->a = colf[3] * 255.f;
    }
}

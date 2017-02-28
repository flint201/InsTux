#include "fonts.h"

HFont esp_font = 0;

void Fonts::SetupFonts()
{
    esp_font = Draw::CreateFont("Tahoma", 11, (int) FontFlags::FONTFLAG_DROPSHADOW);
}

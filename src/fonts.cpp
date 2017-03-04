#include "fonts.h"

HFont esp_font;

void Fonts::SetupFonts()
{
    esp_font = Draw::CreateFont("FoundationSans-Bold", 20, (int) FontFlags::FONTFLAG_DROPSHADOW);
}

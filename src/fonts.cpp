#include "fonts.h"

HFont esp_font = 0;

void Fonts::SetupFonts()
{
	esp_font = Draw::CreateFont("Times New Roman", 10, (int) FontFlags::FONTFLAG_DROPSHADOW);
}

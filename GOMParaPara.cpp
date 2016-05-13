//////////////////////////////////////////
// GOMParaPara.cpp
// Inherits the main GOM for use in the game
//////////////////////////////////////////

#include "Globals.h"

GOMParaPara::GOMParaPara() : GOM() { }

GameObject* GOMParaPara::OnCreateObject(int type)
{
    switch(type)
    {
        case OBJ_DEFAULT_ARROWS:
            return new GODefaultArrow();

        case OBJ_DEFAULT_ARROWS_BW:
            return new GOStationaryArrow();

		case OBJ_SHOCKWAVE:
			return new GOShockwave();

        case OBJ_PINK_FLASH:
            return new GOPinkFlash();

		case OBJ_TITLE:
			return new GOTitle();

		case OBJ_SCORE:
			return new GOScore();

		case OBJ_HEADER:
			return new GOHeader();

		case OBJ_FONTSHEET:
			return new GOFontSheet();

        default:
            EHLog("GOMParaPara::OnCreateObject(): No such object defined.");
            break;
    }

    return GOM::OnCreateObject(type);
}

//////////////////////////////////////////
// GOMParaPara.cpp
// Inherits the main GOM for use in the game
//////////////////////////////////////////

#include "Globals.h"

GOMParaPara::GOMParaPara() : BL_GOM() { }

BL_GameObject* GOMParaPara::OnCreateObject(int type)
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

       // case OBJ_SHOCKWAVE:
       //     return new GOShockwave();

        default:
            BL_EHLog("GOMParaPara::OnCreateObject(): No such object defined.");
            break;
    }

    return BL_GOM::OnCreateObject(type);
}

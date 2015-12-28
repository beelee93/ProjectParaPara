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
    }

    return BL_GOM::OnCreateObject(type);
}

#pragma once

#include "brogueGlobal.h"
#include "simpleString.h"

namespace brogueHd::backend::model::game
{
    enum featTypes 
    {
        FEAT_PURE_MAGE = 0,
        FEAT_PURE_WARRIOR,
        FEAT_PACIFIST,
        FEAT_ARCHIVIST,
        FEAT_COMPANION,
        FEAT_SPECIALIST,
        FEAT_JELLYMANCER,
        FEAT_INDOMITABLE,
        FEAT_MYSTIC,
        FEAT_DRAGONSLAYER,
        FEAT_PALADIN
    };

    struct feat 
    {
        featTypes type;
        simpleString name;
        simpleString description;
        bool initialValue;
    };

    Begin_Enum_String(featTypes)
    {
        Enum_String(FEAT_PURE_MAGE);
        Enum_String(FEAT_PURE_WARRIOR);
        Enum_String(FEAT_PACIFIST);
        Enum_String(FEAT_ARCHIVIST);
        Enum_String(FEAT_COMPANION);
        Enum_String(FEAT_SPECIALIST);
        Enum_String(FEAT_JELLYMANCER);
        Enum_String(FEAT_INDOMITABLE);
        Enum_String(FEAT_MYSTIC);
        Enum_String(FEAT_DRAGONSLAYER);
        Enum_String(FEAT_PALADIN);
    }
    End_Enum_String;
}

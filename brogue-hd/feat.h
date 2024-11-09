#pragma once

#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::backend::model
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
}

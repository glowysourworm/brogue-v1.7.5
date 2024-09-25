#pragma once

#include "broguedef.h"

namespace brogueHd::backend::model::game
{
    enum featTypes {
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
        FEAT_PALADIN,

        FEAT_COUNT,
    };

    typedef struct feat {
        char name[100];
        char description[200];
        bool initialValue;
    } feat;

    const feat featTable[FEAT_COUNT] = {
        {"Pure Mage",       "Ascend without using fists or a weapon.", true},
        {"Pure Warrior",    "Ascend without using a staff, wand or charm.", true},
        {"Pacifist",        "Ascend without attacking a creature.", true},
        {"Archivist",       "Ascend without drinking a potion or reading a scroll.", true},
        {"Companion",       "Journey with an ally through 20 depths.", false},
        {"Specialist",      "Enchant an item up to or above +16.", false},
        {"Jellymancer",     "Obtain at least 90 jelly allies simultaneously.", false},
        {"Indomitable",     "Ascend without taking damage.", true},
        {"Mystic",          "Ascend without eating.", true},
        {"Dragonslayer",    "Kill a dragon with a melee attack.", false},
        {"Paladin",         "Ascend without attacking an unaware or fleeing creature.", true},
    };
}

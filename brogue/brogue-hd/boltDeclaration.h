#pragma once

#include "color.h"
#include "broguedef.h"

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::effect
{
    struct bolt {
        char name[DCOLS];
        char description[COLS];
        char abilityDescription[COLS * 2];
        char theChar;
        const color* foreColor;
        const color* backColor;
        short boltEffect;
        short magnitude;
        short pathDF;
        short targetDF;
        unsigned long forbiddenMonsterFlags;
        unsigned long flags;
    };
}
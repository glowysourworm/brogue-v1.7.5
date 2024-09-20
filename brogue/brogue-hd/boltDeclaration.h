#pragma once

#include "color.h"
#include "broguedef.h"

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::effect
{
    typedef struct bolt {
        char name[DCOLS];
        char description[COLS];
        char abilityDescription[COLS * 2];
        uchar theChar;
        const color* foreColor;
        const color* backColor;
        short boltEffect;
        short magnitude;
        short pathDF;
        short targetDF;
        unsigned long forbiddenMonsterFlags;
        unsigned long flags;
    } bolt;
}
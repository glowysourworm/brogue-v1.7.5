#pragma once

#include "broguedef.h"

namespace brogueHd
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
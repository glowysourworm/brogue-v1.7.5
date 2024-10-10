#pragma once

#include "color.h"
#include "brogueGlobal.h"
#include "simpleString.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::effect
{
    struct bolt 
    {
        simpleString name;
        simpleString description;
        simpleString abilityDescription;
        char theChar;
        color foreColor;
        color backColor;
        short boltEffect;
        short magnitude;
        short pathDF;
        short targetDF;
        unsigned long forbiddenMonsterFlags;
        unsigned long flags;
    };
}
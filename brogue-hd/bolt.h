#pragma once

#include "color.h"
#include "brogueGlobal.h"
#include <string>

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::effect
{
    struct bolt 
    {
        std::string name;
        std::string description;
        std::string abilityDescription;
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
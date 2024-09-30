#pragma once

#include "color.h"
#include "colorConstants.h"
#include "simpleHash.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::modelConstant;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model
{
	class brogueColorMap
	{
	public:

		brogueColorMap();
		~brogueColorMap();
		
		void setColor(gameColors colorSelector, color value);
		//void setColor(boltColors colorSelector, color value);
		//void setColor(tileColors colorSelector, color value);
		//void setColor(creatureColors colorSelector, color value);
		//void setColor(lightColors colorSelector, color value);
		//void setColor(flareColors colorSelector, color value);
		//void setColor(colorMultipliers colorSelector, color value);
		//void setColor(bloodColors colorSelector, color value);
		//void setColor(gasColors colorSelector, color value);
		//void setColor(interfaceColors colorSelector, color value);

		color getColor(gameColors colorSelector) const;
		//color getColor(boltColors colorSelector) const;
		//color getColor(tileColors colorSelector) const;
		//color getColor(creatureColors colorSelector) const;
		//color getColor(lightColors colorSelector) const;
		//color getColor(flareColors colorSelector) const;
		//color getColor(colorMultipliers colorSelector) const;
		//color getColor(bloodColors colorSelector) const;
		//color getColor(gasColors colorSelector) const;
		//color getColor(interfaceColors colorSelector) const;

	private:
		//simpleHash<int, bool>* _gameColors;
		//simpleHash<boltColors, color>* _boltColors;
		//simpleHash<tileColors, color>* _tileColors;
		//simpleHash<creatureColors, color>* _creatureColors;
		//simpleHash<lightColors, color>* _lightColors;
		//simpleHash<flareColors, color>* _flareColors;
		//simpleHash<colorMultipliers, color>* _colorMultipliers;
		//simpleHash<bloodColors, color>* _bloodColors;
		//simpleHash<gasColors, color>* _gasColors;
		//simpleHash<interfaceColors, color>* _interfaceColors;
	};
}
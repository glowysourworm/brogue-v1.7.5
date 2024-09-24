#pragma once
#include "colorConstants.h"
#include <map>

using namespace std;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model
{
	class brogueColorMap
	{
	public:

		brogueColorMap();
		~brogueColorMap();
		
		void setColor(gameColors colorSelector, color value);
		void setColor(boltColors colorSelector, color value);
		void setColor(tileColors colorSelector, color value);
		void setColor(creatureColors colorSelector, color value);
		void setColor(lightColors colorSelector, color value);
		void setColor(flareColors colorSelector, color value);
		void setColor(colorMultipliers colorSelector, color value);
		void setColor(bloodColors colorSelector, color value);
		void setColor(gasColors colorSelector, color value);
		void setColor(interfaceColors colorSelector, color value);

		color getColor(gameColors colorSelector) const;
		color getColor(boltColors colorSelector) const;
		color getColor(tileColors colorSelector) const;
		color getColor(creatureColors colorSelector) const;
		color getColor(lightColors colorSelector) const;
		color getColor(flareColors colorSelector) const;
		color getColor(colorMultipliers colorSelector) const;
		color getColor(bloodColors colorSelector) const;
		color getColor(gasColors colorSelector) const;
		color getColor(interfaceColors colorSelector) const;

	private:
		std::map<gameColors, color>* _gameColors;
		std::map<boltColors, color>* _boltColors;
		std::map<tileColors, color>* _tileColors;
		std::map<creatureColors, color>* _creatureColors;
		std::map<lightColors, color>* _lightColors;
		std::map<flareColors, color>* _flareColors;
		std::map<colorMultipliers, color>* _colorMultipliers;
		std::map<bloodColors, color>* _bloodColors;
		std::map<gasColors, color>* _gasColors;
		std::map<interfaceColors, color>* _interfaceColors;
	};
}
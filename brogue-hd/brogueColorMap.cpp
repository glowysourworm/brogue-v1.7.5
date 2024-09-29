#pragma once

#include "color.h"
#include "colorConstants.h"
#include "brogueColorMap.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::modelConstant;

namespace brogueHd::backend::model
{
	brogueColorMap::brogueColorMap()
	{
		//_gameColors = new simpleHash<int, bool>();
		//_boltColors = new simpleHash<boltColors, color>();
		//_tileColors = new simpleHash<tileColors, color>();
		//_creatureColors = new simpleHash<creatureColors, color>();
		//_lightColors = new simpleHash<lightColors, color>();
		//_flareColors = new simpleHash<flareColors, color>();
		//_colorMultipliers = new simpleHash<colorMultipliers, color>();
		//_bloodColors = new simpleHash<bloodColors, color>();
		//_gasColors = new simpleHash<gasColors, color>();
		//_interfaceColors = new simpleHash<interfaceColors, color>();
	}
	brogueColorMap::~brogueColorMap()
	{
		//delete _gameColors;
		//delete _boltColors;
		//delete _tileColors;
		//delete _creatureColors;
		//delete _lightColors;
		//delete _flareColors;
		//delete _colorMultipliers;
		//delete _bloodColors;
		//delete _gasColors;
		//delete _interfaceColors;
	}

	color brogueColorMap::getColor(gameColors colorSelector) const
	{
		//_gameColors->get(2);

		return color();
	}
	//color brogueColorMap::getColor(boltColors colorSelector) const
	//{
	//	return _boltColors->get(colorSelector);
	//}
	//color brogueColorMap::getColor(tileColors colorSelector) const
	//{
	//	return _tileColors->get(colorSelector);
	//}
	//color brogueColorMap::getColor(creatureColors colorSelector) const
	//{
	//	return _creatureColors->get(colorSelector);
	//}
	//color brogueColorMap::getColor(lightColors colorSelector) const
	//{
	//	return _lightColors->get(colorSelector);
	//}
	//color brogueColorMap::getColor(flareColors colorSelector) const
	//{
	//	return _flareColors->get(colorSelector);
	//}
	//color brogueColorMap::getColor(colorMultipliers colorSelector) const
	//{
	//	return _colorMultipliers->get(colorSelector);
	//}
	//color brogueColorMap::getColor(bloodColors colorSelector) const
	//{
	//	return _bloodColors->get(colorSelector);
	//}
	//color brogueColorMap::getColor(gasColors colorSelector) const
	//{
	//	return _gasColors->get(colorSelector);
	//}
	//color brogueColorMap::getColor(interfaceColors colorSelector) const
	//{
	//	return _interfaceColors->get(colorSelector);
	//}

	void brogueColorMap::setColor(gameColors colorSelector, color value)
	{
		//_gameColors->add(2, true);
	}
	//void brogueColorMap::setColor(boltColors colorSelector, color value)
	//{
	//	_boltColors->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(tileColors colorSelector, color value)
	//{
	//	_tileColors->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(creatureColors colorSelector, color value)
	//{
	//	_creatureColors->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(lightColors colorSelector, color value)
	//{
	//	_lightColors->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(flareColors colorSelector, color value)
	//{
	//	_flareColors->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(colorMultipliers colorSelector, color value)
	//{
	//	_colorMultipliers->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(bloodColors colorSelector, color value)
	//{
	//	_bloodColors->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(gasColors colorSelector, color value)
	//{
	//	_gasColors->add(colorSelector, value);
	//}
	//void brogueColorMap::setColor(interfaceColors colorSelector, color value)
	//{
	//	_interfaceColors->add(colorSelector, value);
	//}
}
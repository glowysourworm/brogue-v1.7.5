#pragma once

#include "brogueColorMap.h"
#include <map>

using namespace std;

namespace brogueHd::backend::model
{
	brogueColorMap::brogueColorMap()
	{
		_gameColors = new std::map<gameColors, color>();
		_boltColors = new std::map<boltColors, color>();
		_tileColors = new std::map<tileColors, color>();
		_creatureColors = new std::map<creatureColors, color>();
		_lightColors = new std::map<lightColors, color>();
		_flareColors = new std::map<flareColors, color>();
		_colorMultipliers = new std::map<colorMultipliers, color>();
		_bloodColors = new std::map<bloodColors, color>();
		_gasColors = new std::map<gasColors, color>();
		_interfaceColors = new std::map<interfaceColors, color>();
	}
	brogueColorMap::~brogueColorMap()
	{
		delete _gameColors;
		delete _boltColors;
		delete _tileColors;
		delete _creatureColors;
		delete _lightColors;
		delete _flareColors;
		delete _colorMultipliers;
		delete _bloodColors;
		delete _gasColors;
		delete _interfaceColors;
	}

	color brogueColorMap::getColor(gameColors colorSelector) const
	{
		return _gameColors->at(colorSelector);
	}
	color brogueColorMap::getColor(boltColors colorSelector) const
	{
		return _boltColors->at(colorSelector);
	}
	color brogueColorMap::getColor(tileColors colorSelector) const
	{
		return _tileColors->at(colorSelector);
	}
	color brogueColorMap::getColor(creatureColors colorSelector) const
	{
		return _creatureColors->at(colorSelector);
	}
	color brogueColorMap::getColor(lightColors colorSelector) const
	{
		return _lightColors->at(colorSelector);
	}
	color brogueColorMap::getColor(flareColors colorSelector) const
	{
		return _flareColors->at(colorSelector);
	}
	color brogueColorMap::getColor(colorMultipliers colorSelector) const
	{
		return _colorMultipliers->at(colorSelector);
	}
	color brogueColorMap::getColor(bloodColors colorSelector) const
	{
		return _bloodColors->at(colorSelector);
	}
	color brogueColorMap::getColor(gasColors colorSelector) const
	{
		return _gasColors->at(colorSelector);
	}
	color brogueColorMap::getColor(interfaceColors colorSelector) const
	{
		return _interfaceColors->at(colorSelector);
	}

	void brogueColorMap::setColor(gameColors colorSelector, color value)
	{
		_gameColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(boltColors colorSelector, color value)
	{
		_boltColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(tileColors colorSelector, color value)
	{
		_tileColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(creatureColors colorSelector, color value)
	{
		_creatureColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(lightColors colorSelector, color value)
	{
		_lightColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(flareColors colorSelector, color value)
	{
		_flareColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(colorMultipliers colorSelector, color value)
	{
		_colorMultipliers->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(bloodColors colorSelector, color value)
	{
		_bloodColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(gasColors colorSelector, color value)
	{
		_gasColors->insert(colorSelector, value);
	}
	void brogueColorMap::setColor(interfaceColors colorSelector, color value)
	{
		_interfaceColors->insert(colorSelector, value);
	}
}
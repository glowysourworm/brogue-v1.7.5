#pragma once

#include "gridLocator.h"

using namespace brogueHd::component;

namespace brogueHd::frontend::opengl
{
	class brogueMouseState
	{
	public:
		brogueMouseState()
		{
			_location = default_value::value<gridLocator>();
		}
		brogueMouseState(short column, short row)
		{
			_location = gridLocator(column, row);
		}
		brogueMouseState(gridLocator location)
		{
			_location = location;
		}
		brogueMouseState(const brogueMouseState& copy)
		{
			_location = copy.getLocation();
		}

		gridLocator getLocation() const
		{
			return _location;
		}

	private:

		gridLocator _location;

	};
}
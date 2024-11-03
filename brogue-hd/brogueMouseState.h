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
			_scrollPending = false;
			_scrollUp = false;
		}
		brogueMouseState(short column, short row, bool scrollPending, bool scrollUp)
		{
			_location = gridLocator(column, row);
			_scrollPending = scrollPending;
			_scrollUp = scrollUp;
		}
		brogueMouseState(gridLocator location, bool scrollPending, bool scrollUp)
		{
			_location = location;
			_scrollPending = scrollPending;
			_scrollUp = scrollUp;
		}
		brogueMouseState(const brogueMouseState& copy)
		{
			_location = copy.getLocation();
			_scrollPending = copy.getScrollPending();
			_scrollUp = copy.getIsScrollUp();
		}

		gridLocator getLocation() const
		{
			return _location;
		}
		bool getScrollPending() const
		{
			return _scrollPending;
		}
		bool getIsScrollUp() const
		{
			return _scrollUp;
		}

	private:

		gridLocator _location;
		bool _scrollPending;
		bool _scrollUp;

	};
}
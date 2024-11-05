#pragma once

#include "gridLocator.h"
#include "simple.h"

using namespace brogueHd::component;

namespace brogueHd::frontend::opengl
{
	struct brogueMouseState
	{
		brogueMouseState()
		{
			_location = default_value::value<gridLocator>();
			_scrollPending = false;
			_scrollUp = false;
			_mouseLeft = false;
		}
		brogueMouseState(short column, short row, bool scrollPending, bool scrollUp, bool mouseLeft)
		{
			_location = gridLocator(column, row);
			_scrollPending = scrollPending;
			_scrollUp = scrollUp;
			_mouseLeft = mouseLeft;
		}
		brogueMouseState(gridLocator location, bool scrollPending, bool scrollUp, bool mouseLeft)
		{
			_location = location;
			_scrollPending = scrollPending;
			_scrollUp = scrollUp;
			_mouseLeft = mouseLeft;
		}
		brogueMouseState(const brogueMouseState& copy)
		{
			_location = copy.getLocation();
			_scrollPending = copy.getScrollPending();
			_scrollUp = copy.getIsScrollUp();
			_mouseLeft = copy.getMouseLeft();
		}
		void operator=(const brogueMouseState& copy)
		{
			_location = copy.getLocation();
			_scrollPending = copy.getScrollPending();
			_scrollUp = copy.getIsScrollUp();
			_mouseLeft = copy.getMouseLeft();
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
		bool getMouseLeft() const
		{
			return _mouseLeft;
		}

	private:

		gridLocator _location;
		bool _scrollPending;
		bool _scrollUp;
		bool _mouseLeft;

	};
}
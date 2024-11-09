#pragma once

#include "gridLocator.h"
#include "simple.h"

using namespace brogueHd::component;

namespace brogueHd::frontend
{
	struct brogueMouseState
	{
		brogueMouseState()
		{
			_location = default_value::value<gridLocator>();
			_scrollPendingX = false;
			_scrollPendingY = false;
			_scrollNegativeX = false;
			_scrollNegativeY = false;
			_mouseLeft = false;
		}
		brogueMouseState(short column, short row, bool scrollPendingX, bool scrollPendingY, bool negativeX, bool negativeY, bool mouseLeft)
		{
			_location = gridLocator(column, row);
			_scrollPendingX = scrollPendingX;
			_scrollPendingY = scrollPendingY;
			_scrollNegativeX = negativeX;
			_scrollNegativeY = negativeY;
			_mouseLeft = mouseLeft;
		}
		brogueMouseState(gridLocator location, bool scrollPendingX, bool scrollPendingY, bool negativeX, bool negativeY, bool mouseLeft)
		{
			_location = location;
			_scrollPendingX = scrollPendingX;
			_scrollPendingY = scrollPendingY;
			_scrollNegativeX = negativeX;
			_scrollNegativeY = negativeY;
			_mouseLeft = mouseLeft;
		}
		brogueMouseState(const brogueMouseState& copy)
		{
			_location = copy.getLocation();
			_scrollPendingX = getScrollPendingX();
			_scrollPendingY = getScrollPendingY();
			_scrollNegativeX = getScrollNegativeX();
			_scrollNegativeY = getScrollNegativeY();
			_mouseLeft = copy.getMouseLeft();
		}
		void operator=(const brogueMouseState& copy)
		{
			_location = copy.getLocation();
			_scrollPendingX = getScrollPendingX();
			_scrollPendingY = getScrollPendingY();
			_scrollNegativeX = getScrollNegativeX();
			_scrollNegativeY = getScrollNegativeY();
			_mouseLeft = copy.getMouseLeft();
		}

		gridLocator getLocation() const
		{
			return _location;
		}
		bool getScrollPendingX() const
		{
			return _scrollPendingX;
		}
		bool getScrollPendingY() const
		{
			return _scrollPendingY;
		}
		bool getScrollNegativeX() const
		{
			return _scrollNegativeX;
		}
		bool getScrollNegativeY() const
		{
			return _scrollNegativeY;
		}
		bool getMouseLeft() const
		{
			return _mouseLeft;
		}

	private:

		gridLocator _location;
		bool _scrollPendingX;
		bool _scrollPendingY;
		bool _scrollNegativeX;
		bool _scrollNegativeY;
		bool _mouseLeft;

	};
}
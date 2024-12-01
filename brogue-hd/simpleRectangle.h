#pragma once

#include "simple.h"
#include "simplePoint.h"
#include "simpleLine.h"

namespace brogueHd::simple
{
	template<isNumber T>
	struct simpleRectangle : simpleStruct
	{
		simpleRectangle()
		{
			_topLeft = default_value::value<simplePoint<T>>();
			_bottomRight = default_value::value<simplePoint<T>>();
		}
		simpleRectangle(const simplePoint<T>& topLeft, const simplePoint<T>& bottomRight)
		{
			_topLeft = topLeft;
			_bottomRight = bottomRight;
		}
		simpleRectangle(const simpleRectangle<T>& copy)
		{
			_topLeft = copy.getTopLeft();
			_bottomRight = copy.getBottomRight();
		}
		void operator=(const simpleRectangle<T>& copy)
		{
			_topLeft = copy.getTopLeft();
			_bottomRight = copy.getBottomRight();
		}
		bool operator==(const simpleRectangle<T>& other)
		{
			return _topLeft == other.getTopLeft() &&
				   _bottomRight == other.getBottomRight();
		}
		bool operator!=(const simpleRectangle<T>& other)
		{
			return _topLeft != other.getTopLeft() ||
				   _bottomRight != other.getBottomRight();
		}
		bool intersects(const simpleLine<T>& line)
		{
			simpleLine<T> lineLeft(simplePoint<T>(_topLeft.x, _topLeft.y), simplePoint<T>(_topLeft.x, _bottomRight.y));
			simpleLine<T> lineBottom(simplePoint<T>(_topLeft.x, _bottomRight.y), simplePoint<T>(_bottomRight.x, _bottomRight.y));
			simpleLine<T> lineRight(simplePoint<T>(_bottomRight.x, _bottomRight.y), simplePoint<T>(_bottomRight.x, _topLeft.y));
			simpleLine<T> lineTop(simplePoint<T>(_bottomRight.x, _topLeft.y), simplePoint<T>(_topLeft.x, _topLeft.y));

			simplePoint<T> intersection;

			return lineLeft.intersects(line, intersection) ||
				   lineBottom.intersects(line, intersection) ||
				   lineRight.intersects(line, intersection) ||
				   lineTop.intersects(line, intersection);
		}
		simplePoint<T> getTopLeft() const
		{
			return _topLeft;
		}
		simplePoint<T> getBottomRight() const
		{
			return _bottomRight;
		}

	private:

		simplePoint<T> _topLeft;
		simplePoint<T> _bottomRight;
	};
}
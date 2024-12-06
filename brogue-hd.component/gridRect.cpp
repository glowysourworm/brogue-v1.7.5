#pragma once

#include "gridRect.h"
#include "gridDefinitions.h"
#include "gridLocator.h"

#include <simple.h>
#include <simpleException.h>
#include <simpleMath.h>
#include <simpleRect.h>
#include <functional>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	gridRect::gridRect()
	{
	}

	gridRect::gridRect(const gridRect& copy) : simpleRect(copy)
	{
	}

	gridRect::gridRect(const simpleRect& rect) : simpleRect(rect)
	{
	}

	gridRect::gridRect(int acolumn, int arow, int awidth, int aheight)
		: simpleRect(acolumn, arow, awidth, aheight)
	{
	}

	gridRect operator+(const gridRect& instance, const gridLocator& translation)
	{
		return gridRect(instance.column + translation.column, instance.row + translation.row, instance.width,
						instance.height);
	}

	bool gridRect::validateUI() const
	{
		if (column < 0)
			return false;

		if (row < 0)
			return false;

		if (width <= 0)
			return false;

		if (height <= 0)
			return false;

		return true;
	}

	gridRect gridRect::fromCircle(int centerColumn, int centerRow, int radiusX, int radiusY)
	{
		gridRect result;

		result.column = centerColumn - radiusX;
		result.row = centerRow - radiusY;
		result.width = radiusX;
		result.height = radiusY;

		return result;
	}

	gridLocator gridRect::topLeft() const
	{
		return gridLocator(left(), top());
	}

	gridLocator gridRect::topRight() const
	{
		return gridLocator(right(), top());
	}

	gridLocator gridRect::bottomLeft() const
	{
		return gridLocator(left(), bottom());
	}

	gridLocator gridRect::bottomRight() const
	{
		return gridLocator(right(), bottom());
	}

	gridLocator gridRect::center() const
	{
		return gridLocator(centerX(), centerY());
	}

	bool gridRect::contains(int acolumn, int arow) const
	{
		return simpleRect::contains(acolumn, arow);
	}

	bool gridRect::contains(const gridLocator& location) const
	{
		return simpleRect::contains(location.column, location.row);
	}

	bool gridRect::contains(const gridRect& rect) const
	{
		return simpleRect::contains(rect);
	}

	gridRect gridRect::createExpanded(int amount) const
	{
		gridRect expandedRect = gridRect(column - amount, row - amount, width + (2 * amount),
										 height + (2 * amount));

		if (expandedRect.width <= 0 ||
			expandedRect.height <= 0)
			throw simpleException("Trying to create expanded gridRect created invalid rectangle");

		return expandedRect;
	}

	gridRect gridRect::createPadded(int padding) const
	{
		gridRect paddedRect = gridRect(column + padding, row + padding, width - (2 * padding),
									   height - (2 * padding));

		if (paddedRect.width <= 0 ||
			paddedRect.height <= 0)
			throw simpleException("Trying to create padded gridRect created invalid rectangle");

		return paddedRect;
	}

	void gridRect::clamp(const gridRect& constraint)
	{
		if (!constraint.validateUI())
			throw simpleException("Invalid constraint rect:  gridRect::clamp");

		column = simpleMath::clamp(column, constraint.left(), constraint.right());
		row = simpleMath::clamp(row, constraint.top(), constraint.bottom());

		if (right() > constraint.right())
			width -= right() - constraint.right();

		if (bottom() > constraint.bottom())
			height -= bottom() - constraint.bottom();
	}

	/// <summary>
	/// If this result is true, then the direction carries a SINGLE cardinal result.
	/// </summary>
	bool gridRect::isAdjacent(const gridRect& rect, brogueCompass& direction) const
	{
		int adjacentEdges = 0;
		direction = brogueCompass::None;

		if (left() == rect.right() + 1)
		{
			adjacentEdges++;
			direction = direction | brogueCompass::W;
		}

		if (right() == rect.left() - 1)
		{
			adjacentEdges++;
			direction = direction | brogueCompass::E;
		}

		if (top() == rect.bottom() + 1)
		{
			adjacentEdges++;
			direction = direction | brogueCompass::N;
		}

		if (bottom() == rect.top() - 1)
		{
			adjacentEdges++;
			direction = direction | brogueCompass::S;
		}

		bool indicesOverlapX = (rect.left() >= left() && rect.left() <= right()) ||
							   (rect.right() <= right() && rect.right() >= left());

		bool indicesOverlapY = (rect.top() >= top() && rect.top() <= bottom()) ||
							   (rect.bottom() <= bottom() && rect.bottom() >= top());

		return !overlaps(rect) && adjacentEdges > 0 && (indicesOverlapX || indicesOverlapY);
	}

	void gridRect::translate(int columnOffset, int rowOffset)
	{
		column += columnOffset;
		row += rowOffset;
	}

	void gridRect::translate(const gridLocator& translation)
	{
		translate(translation.column, translation.row);
	}

	gridLocator gridRect::getTranslation(const gridRect& rect)
	{
		return gridLocator(rect.column - column, rect.row - row);
	}

	void gridRect::iterate(gridRectIterator callback) const
	{
		bool userBreak = false;

		for (int i = left(); i <= right() && !userBreak; i++)
		{
			for (int j = top(); j <= bottom() && !userBreak; j++)
			{
				if (callback(i, j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	void gridRect::iterateBoundary(gridRectIterator callback) const
	{
		bool userBreak = false;

		for (int i = left(); i <= right() && !userBreak; i++)
		{
			if (callback(i, top()) == iterationCallback::breakAndReturn)
				userBreak = true;
		}

		for (int i = left(); i <= right() && !userBreak; i++)
		{
			if (callback(i, bottom()) == iterationCallback::breakAndReturn)
				userBreak = true;
		}

		for (int j = top() + 1; j <= bottom() - 1 && !userBreak; j++)
		{
			if (callback(left(), j) == iterationCallback::breakAndReturn)
				userBreak = true;

			if (userBreak)
				break;

			if (callback(right(), j) == iterationCallback::breakAndReturn)
				userBreak = true;
		}
	}

	void gridRect::iterateAdjacent(int column, int row, gridRectIterator callback) const
	{
		for (int columnIndex = column - 1; columnIndex != column + 1; columnIndex++)
		{
			for (int rowIndex = row - 1; rowIndex != row + 1; rowIndex++)
			{
				if (simpleRect::contains(columnIndex, rowIndex))
				{
					if (callback(columnIndex, rowIndex) == iterationCallback::breakAndReturn)
						return;
				}
			}
		}
	}

	void gridRect::iterateRowsFirst_BottomToTop(gridRectIterator callback) const
	{
		bool userBreak = false;

		for (int j = bottom(); j >= top() && !userBreak; j--)
		{
			for (int i = left(); i <= right() && !userBreak; i++)
			{
				if (callback(i, j) == iterationCallback::breakAndReturn)
					userBreak = true;
			}
		}
	}

	void gridRect::iterateInCircle(gridRectIterator callback) const
	{
		bool userBreak = false;

		float heightRadius = height / 2.0f;
		float widthRadius = width / 2.0f;

		for (int i = left(); i <= right() && !userBreak; i++)
		{
			for (int j = top(); j <= bottom() && !userBreak; j++)
			{
				float dx = (column + (width / 2.0f)) - i;
				float dy = (row + (height / 2.0f)) - j;

				// From equation for an ellipse
				float yLimit = simpleMath::squareRoot(
					heightRadius * heightRadius - ((heightRadius / (float)widthRadius) * (heightRadius / (float)
					widthRadius) * dx * dx));
				float xLimit = simpleMath::squareRoot(
					widthRadius * widthRadius - ((widthRadius / (float)heightRadius) * (widthRadius / (float)
					heightRadius) * dy * dy));

				if (simpleMath::abs(dy) <= yLimit &&
					simpleMath::abs(dx) <= xLimit)
				{
					userBreak = callback(i, j);
				}
			}
		}
	}

	void gridRect::iterateOutsideAdjacent(int distance, const gridRect& constraint,gridRectDirectionalIterator callback) const
	{
		// Start adjacent to the outer boundary
		//
		int count = 1;

		while (count <= distance)
		{
			// Top
			for (int column = left() - count; column <= right() + count && constraint.contains(column, top() - count); column++)
			{
				// Repeat (from bottom iterator)
				if (column == left() - count)
					continue;

				if (callback(column, top() - count, brogueCompass::N) == iterationCallback::breakAndReturn)
					return;
			}

			// Right
			for (int row = top() - count; row <= bottom() + count && constraint.contains(right() + count, row); row++)
			{
				// Repeat
				if (row == top() - count)
					continue;

				if (callback(right() + count, row, brogueCompass::E) == iterationCallback::breakAndReturn)
					return;
			}

			// Bottom
			for (int column = right() + count; column >= left() - count && constraint.contains(column, bottom() + count); column--)
			{
				// Repeat
				if (column == right() + count)
					continue;

				if (callback(column, bottom() + count, brogueCompass::S) == iterationCallback::breakAndReturn)
					return;
			}

			// Left
			for (int row = bottom() + count; row >= top() - count && constraint.contains(left() - count, row); row--)
			{
				// Repeat
				if (row == bottom() + count)
					continue;

				if (callback(left() - count, row, brogueCompass::W) == iterationCallback::breakAndReturn)
					return;
			}

			count++;
		}
	}
}

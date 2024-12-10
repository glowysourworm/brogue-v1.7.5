#pragma once

#include "gridDefinitions.h"
#include "gridLocator.h"

#include <simple.h>
#include <simpleException.h>
#include <simpleMath.h>
#include <simpleRect.h>
#include <simpleSize.h>
#include <functional>

#include "simpleSize.h"

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;

	/// <summary>
	/// Iterator delegate for providing feedback to/from gridRect iterator functions.
	/// </summary>
	using gridRectIterator = std::function<iterationCallback(int column, int row)>;

	/// <summary>
	/// Iterator delegate for providing feedback to/from gridRect iterator functions. The direction 
	/// is specific to the "adjacent side" along which iteration was happening.
	/// </summary>
	using gridRectDirectionalIterator = std::function<iterationCallback(int column, int row, brogueCompass direction)>;

	struct gridRect : public simpleRect
	{
	public:
		gridRect();
		gridRect(const gridRect& copy);
		gridRect(const simpleRect& rect);
		gridRect(int acolumn, int arow, const simpleSize& size);
		gridRect(const gridLocator& location, const simpleSize& size);
		gridRect(int acolumn, int arow, int awidth, int aheight);

		friend gridRect operator+(const gridRect& instance, const gridLocator& translation);

		/// <summary>
		/// Returns true if the grid rect represents a valid rectangle in UI coordinates
		/// </summary>
		/// <returns></returns>
		bool validateUI() const;

		static gridRect fromCircle(int centerColumn, int centerRow, int radiusX, int radiusY);

		gridLocator topLeft() const;
		gridLocator topRight() const;
		gridLocator bottomLeft() const;
		gridLocator bottomRight() const;
		gridLocator center() const;
		bool contains(int acolumn, int arow) const;
		bool contains(const gridLocator& location) const;
		bool contains(const gridRect& rect) const;

		simpleSize getSize() const;

		/// <summary>
		/// Creates a new gridRect, expanded in all directions, by the specified amount.
		/// </summary>
		gridRect createExpanded(int amount) const;

		/// <summary>
		/// Creates a new gridRect, shrunk in all directions, by the specified amount.
		/// </summary>
		gridRect createPadded(int padding) const;

		void clamp(const gridRect& constraint);

		/// <summary>
		/// Checks adjacency with the other gridRect. The condition is that it must share
		/// an edge; and that corners don't count.
		/// </summary>
		bool isAdjacent(const gridRect& rect, brogueCompass& direction) const;

		void translate(int columnOffset, int rowOffset);
		void translate(const gridLocator& translation);
		gridLocator getTranslation(const gridRect& rect);

		void iterate(gridRectIterator callback) const;
		void iterateBoundary(gridRectIterator callback) const;
		void iterateAdjacent(int column, int row, gridRectIterator callback) const;
		void iterateRowsFirst_BottomToTop(gridRectIterator callback) const;
		void iterateInCircle(gridRectIterator callback) const;

		/// <summary>
		/// Iterates outside of the rectangle; but within the bounds of the second rectangle. Iteration
		/// will continue for the specified distance.
		/// </summary>
		void iterateOutsideAdjacent(int distance, const gridRect& constraint, gridRectDirectionalIterator callback) const;

		size_t getHash() const override;
	};
}

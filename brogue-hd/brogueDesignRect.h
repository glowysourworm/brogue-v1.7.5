#pragma once

#include "brogueRoomTemplate.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleMath.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;

namespace brogueHd::backend::model
{
	/// <summary>
	/// Accretion tile is used to help design the layout. The connection points are desired
	/// locations for doors - which may or may not be on the edge of the region
	/// </summary>
	struct brogueDesignRect : hashable
	{
	public:

		brogueDesignRect() 
		{
			_configuration = default_value::value<brogueRoomTemplate>();
			_boundary = default_value::value <gridRect>();
			_minSize = default_value::value<gridRect>();
			_padding = 0;
			_actualBoundary = default_value::value <gridRect>();
			_complete = false;
		};

		/// <summary>
		/// Creates a design rectangle with EXTRA padding: which will be honored until the constraint
		/// boundary isn't large enough. 
		/// </summary>
		brogueDesignRect(const brogueRoomTemplate& configuration, const gridRect& constraint, int padding)
		{
			_configuration = configuration;
			_padding = padding;
			_actualBoundary = default_value::value<gridRect>();
			_complete = false;

			applyConstraintImpl(constraint, true);
		}
		brogueDesignRect(const brogueDesignRect& copy)
		{
			copyImpl(copy);
		}
		~brogueDesignRect()
		{
		}

		void operator=(const brogueDesignRect& copy)
		{
			copyImpl(copy);
		}
		bool operator==(const brogueDesignRect& other)
		{
			return compare(other);
		}
		bool operator!=(const brogueDesignRect& other)
		{
			return !compare(other);
		}

		void complete(const gridRect& actualBoundary)
		{
			if (_complete)
				throw simpleException("Already called complete() on brogueDesignRect");

			if (!_boundary.contains(actualBoundary))
				throw simpleException("Actual boundary outside the padded boundary of the brogueDesignRect.");

			_actualBoundary = actualBoundary;
			_complete = true;
		}

		void applyConstraint(const gridRect& constraint)
		{
			applyConstraintImpl(constraint, false);
		}

		void setOffset(const gridLocator& location)
		{
			translate(location.column - _boundary.column, location.row - _boundary.row);
		}

		void translate(const gridLocator& locator)
		{
			translate(locator.column, locator.row);
		}

		void translate(short columnOffset, short rowOffset)
		{
			_boundary.translate(columnOffset, rowOffset);

			if (_complete)
				_actualBoundary.translate(columnOffset, rowOffset);
		}

		size_t getHash() const override
		{
			// Just hashing declarative data (configuration, and pre-design values)
			//
			return hashGenerator::combineHash(_configuration.getHash(), _boundary, _padding);
		}

	public:

		brogueRoomTemplate getConfiguration() const
		{
			return _configuration;
		}
		gridRect getBoundary() const
		{
			return _boundary;
		}
		gridRect getMinSize() const
		{
			return _minSize;
		}
		gridRect getActualBoundary() const
		{
			return _actualBoundary;
		}
		bool getIsComplete() const
		{
			return _complete;
		}
		int getPadding() const
		{
			return _padding;
		}

	private:

		void applyConstraintImpl(const gridRect& constraint, bool forceUpdate)
		{
			if (_complete)
				throw simpleException("Trying to apply constraint to a completed brogueDesignRect");

			if (!constraint.contains(_boundary) || forceUpdate)
			{
				_minSize = _configuration.getMinSize(constraint);

				// Max Size + Padding (if allowed)
				_boundary = _configuration.getMaxSize(constraint).createExpanded(_padding);

				_boundary.column = constraint.column;
				_boundary.row = constraint.row;

				_boundary.width = simpleMath::clamp(_boundary.width, _minSize.width, constraint.width);
				_boundary.height = simpleMath::clamp(_boundary.height, _minSize.height, constraint.height);
			}
		}

		bool compare(const brogueDesignRect& other)
		{
			return _configuration == other.getConfiguration() &&
				_boundary == other.getBoundary() &&
				_minSize == other.getMinSize() &&
				_padding == other.getPadding() &&
				_actualBoundary == other.getActualBoundary() &&
				_complete == other.getIsComplete();
		}
		void copyImpl(const brogueDesignRect& copy)
		{
			_configuration = copy.getConfiguration();
			_boundary = copy.getBoundary();
			_minSize = copy.getMinSize();
			_padding = copy.getPadding();
			_actualBoundary = copy.getActualBoundary();
			_complete = copy.getIsComplete();
		}

	private:

		// Room / Region configuration
		brogueRoomTemplate _configuration;
		gridRect _boundary;
		gridRect _minSize;
		int _padding;

		// Design actuals
		gridRect _actualBoundary;
		bool _complete;
	};
}
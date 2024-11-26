#pragma once

#include "brogueRoomTemplate.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "gridRegion.h"
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
			_complete = false;
			_region = nullptr;
		};

		/// <summary>
		/// Creates a design rectangle with EXTRA padding: which will be honored until the constraint
		/// boundary isn't large enough. 
		/// </summary>
		brogueDesignRect(const brogueRoomTemplate& configuration, const gridRect& constraint, int padding)
		{
			_configuration = configuration;
			_complete = false;
			_region = nullptr;

			applyConstraintImpl(constraint, true, padding);
		}
		brogueDesignRect(const brogueDesignRect& copy)
		{
			copyImpl(copy);
		}
		~brogueDesignRect()
		{
			// Nothing to do.
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

		void complete(gridRegion<gridLocator>* finalRegion)
		{
			if (_complete)
				throw simpleException("Already called complete() on brogueDesignRect");

			if (!_boundary.contains(finalRegion->getBoundary()))
				throw simpleException("Actual boundary outside the padded boundary of the brogueDesignRect.");

			_complete = true;
			_region = finalRegion;
		}

		void setOffset(const gridLocator& location, bool actualOnly = false)
		{
			translate(location.column - _boundary.column, location.row - _boundary.row, actualOnly);
		}

		void translate(const gridLocator& locator, bool actualOnly = false)
		{
			translate(locator.column, locator.row, actualOnly);
		}

		void translate(int columnOffset, int rowOffset, bool actualOnly = false)
		{
			if (actualOnly && _complete)
			{
				_region->translate_StackLike(columnOffset, rowOffset);
			}

			else
			{
				_boundary.translate(columnOffset, rowOffset);

				if (_complete)
				{
					_region->translate_StackLike(columnOffset, rowOffset);
				}
			}

			// Validate
			if (_complete && !_boundary.contains(_region->getBoundary()))
				throw simpleException("Actual boundary outside the padded boundary of the brogueDesignRect.");
		}

		size_t getHash() const override
		{
			// Just hashing declarative data (configuration, and pre-design values)
			//
			return hashGenerator::combineHash(_configuration.getHash(), _boundary);
		}

	public:

		gridRegion<gridLocator>* getRegion() const
		{
			return _region;
		}
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
			if (!_complete)
				throw simpleException("Cannot retrieve actual boundary from the design rect until it is complete:  brogueDesignRect.h");

			return _region->getBoundary();
		}
		bool getIsComplete() const
		{
			return _complete;
		}

	private:

		void applyConstraintImpl(const gridRect& constraint, bool forceUpdate, int padding)
		{
			if (_complete)
				throw simpleException("Trying to apply constraint to a completed brogueDesignRect");

			if (!constraint.contains(_boundary) || forceUpdate)
			{
				_minSize = _configuration.getMinSize();

				_boundary = _configuration.getMaxSize().createExpanded(padding);

				_boundary.column = constraint.column;
				_boundary.row = constraint.row;

				_boundary.width = simpleMath::clamp(_boundary.width, _minSize.width, constraint.width);
				_boundary.height = simpleMath::clamp(_boundary.height, _minSize.height, constraint.height);

				if (_boundary.width < _minSize.width ||
					_boundary.height < _minSize.height)
					throw simpleException("Invalid design rect:  brogueDesignRect::applyConstraintImpl");
			}
		}

		bool compare(const brogueDesignRect& other)
		{
			return _configuration == other.getConfiguration() &&
				_boundary == other.getBoundary() &&
				_minSize == other.getMinSize() &&
				_complete == other.getIsComplete() &&
				_region == other.getRegion();
		}
		void copyImpl(const brogueDesignRect& copy)
		{
			_configuration = copy.getConfiguration();
			_boundary = copy.getBoundary();
			_minSize = copy.getMinSize();
			_complete = copy.getIsComplete();
			_region = copy.getRegion();						// Pointer Only!
		}

	private:

		// Final (Actual) Region
		gridRegion<gridLocator>* _region;

		// Room / Region configuration
		brogueRoomTemplate _configuration;
		gridRect _boundary;
		gridRect _minSize;

		bool _complete;
	};
}
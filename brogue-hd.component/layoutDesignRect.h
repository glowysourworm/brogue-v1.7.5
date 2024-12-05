#pragma once

#include "gridLocator.h"
#include "gridRegion.h"
#include "gridRect.h"
#include "gridRegionOutline.h"
#include "layoutCoordinateConverter.h"

#include <simple.h>
#include <simpleRect.h>
#include <simpleException.h>
#include <simpleMath.h>
#include <brogueRoomTemplate.h>

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;
	using namespace brogueHd::model;

	/// <summary>
	/// Design-time grid rect / tile data structure used for storing region, region outline, and
	/// room (or) other region configuration data during design.
	/// </summary>
	class layoutDesignRect : simpleObject
	{
	public:
		layoutDesignRect()
		{
			_coordinateConverter = nullptr;
			_configuration = default_value::value<brogueRoomTemplate>();
			_boundary = default_value::value<gridRect>();
			_minSize = default_value::value<gridRect>();
			_complete = false;
			_region = nullptr;
			_regionOutline = nullptr;
		};

		/// <summary>
		/// Creates a design rectangle with EXTRA padding: which will be honored until the constraint
		/// boundary isn't large enough. 
		/// </summary>
		layoutDesignRect(layoutCoordinateConverter* coordinateConverter, const brogueRoomTemplate& configuration, const gridRect& constraint, int padding)
		{
			_coordinateConverter = coordinateConverter;
			_configuration = configuration;
			_complete = false;
			_region = nullptr;

			applyConstraintImpl(constraint, true, padding);
		}

		layoutDesignRect(const layoutDesignRect& copy)
		{
			copyImpl(copy);
		}

		~layoutDesignRect()
		{
			// Nothing to do.
		}

		void operator=(const layoutDesignRect& copy)
		{
			copyImpl(copy);
		}

		bool operator==(const layoutDesignRect& other)
		{
			return compare(other);
		}

		bool operator!=(const layoutDesignRect& other)
		{
			return !compare(other);
		}

		void complete(gridRegion<gridLocator>* finalRegion, gridRegionOutline* finalRegionOutline)
		{
			if (_complete)
				throw simpleException("Already called complete() on layoutDesignRect");

			if (!_boundary.contains(finalRegion->getBoundary()))
				throw simpleException("Actual boundary outside the padded boundary of the layoutDesignRect.");

			if (finalRegion == nullptr ||
				finalRegionOutline == nullptr)
				throw simpleException("Null reference exception:  Must set region and region outline in layoutDesignRect.h");

			_complete = true;
			_region = finalRegion;
			_regionOutline = finalRegionOutline;
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
			gridLocator translationGrid = gridLocator(columnOffset, rowOffset);
			simplePoint<int> translationUI = _coordinateConverter->convertToUI(translationGrid);

			if (actualOnly && _complete)
			{
				_region->translate_StackLike(columnOffset, rowOffset);
				_regionOutline->translate(translationGrid, translationUI);
			}

			else
			{
				_boundary.translate(columnOffset, rowOffset);

				if (_complete)
				{
					_region->translate_StackLike(columnOffset, rowOffset);
					_regionOutline->translate(translationGrid, translationUI);
				}
			}

			// Validate
			if (_complete && !_boundary.contains(_region->getBoundary()))
				throw simpleException("Actual boundary outside the padded boundary of the layoutDesignRect.");
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

		gridRegionOutline* getRegionOutline() const
		{
			return _regionOutline;
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
				throw simpleException(
					"Cannot retrieve actual boundary from the design rect until it is complete:  layoutDesignRect.h");

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
				throw simpleException("Trying to apply constraint to a completed layoutDesignRect");

			if (!constraint.contains(_boundary) || forceUpdate)
			{
				_minSize = _configuration.getMinSize();

				gridRect maxSize(_configuration.getMaxSize());

				_boundary = maxSize.createExpanded(padding);

				_boundary.column = constraint.column;
				_boundary.row = constraint.row;

				_boundary.width = simpleMath::clamp(_boundary.width, _minSize.width, constraint.width);
				_boundary.height = simpleMath::clamp(_boundary.height, _minSize.height, constraint.height);

				if (_boundary.width < _minSize.width ||
					_boundary.height < _minSize.height)
					throw simpleException("Invalid design rect:  layoutDesignRect::applyConstraintImpl");
			}
		}

		bool compare(const layoutDesignRect& other)
		{
			return _configuration == other.getConfiguration() &&
				_boundary == other.getBoundary() &&
				_minSize == other.getMinSize() &&
				_complete == other.getIsComplete() &&
				_region == other.getRegion();
		}

		void copyImpl(const layoutDesignRect& copy)
		{
			_configuration = copy.getConfiguration();
			_boundary = copy.getBoundary();
			_minSize = copy.getMinSize();
			_complete = copy.getIsComplete();
			_region = copy.getRegion(); // Pointer Only!
		}

	private:

		layoutCoordinateConverter* _coordinateConverter;

		// Final (Actual) Region
		gridRegion<gridLocator>* _region;
		gridRegionOutline* _regionOutline;

		// Room / Region configuration
		brogueRoomTemplate _configuration;
		gridRect _boundary;
		gridRect _minSize;

		bool _complete;
	};
}

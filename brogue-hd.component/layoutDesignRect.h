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

#include "gridRectAdjacency.h"
#include "gridRegionConstructor.h"
#include "simpleSize.h"

namespace brogueHd::component
{
	using namespace simple;
	using namespace simple::math;
	using namespace brogueHd::model;

	/// <summary>
	/// Design-time grid rect / tile data structure used for storing region, region outline, and
	/// room (or) other region configuration data during design.
	/// </summary>
	class layoutDesignRect : public simpleObject
	{
	public:

		layoutDesignRect()
		{
			_coordinateConverter = nullptr;
			_configuration = default_value::value<brogueRoomTemplate>();
			_boundary = default_value::value<gridRect>();
			_complete = false;
			_region = nullptr;
			_regionOutline = nullptr;
		}

		/// <summary>
		/// Creates a design rectangle with EXTRA padding: which will be honored until the constraint
		/// boundary isn't large enough. 
		/// </summary>
		layoutDesignRect(layoutCoordinateConverter* coordinateConverter, const brogueRoomTemplate& configuration, const gridLocator& roomLocation)
		{
			_coordinateConverter = coordinateConverter;
			_configuration = configuration;
			_complete = false;
			_region = nullptr;
			_regionOutline = nullptr;
			_boundary = gridRect(roomLocation, configuration.getTileSize());
		}

		layoutDesignRect(const layoutDesignRect& copy)
		{
			copyImpl(copy);
		}

		~layoutDesignRect() override
		{}

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

		void complete(gridRegion* finalRegion, gridRegionOutline* finalRegionOutline)
		{
			if (_complete)
				throw simpleException("Already called complete() on layoutDesignRect");

			if (!_boundary.contains(finalRegion->getBoundary()))
				throw simpleException("Actual boundary outside the padded boundary of the layoutDesignRect.");

			//if (finalRegion == nullptr ||
			//	finalRegionOutline == nullptr)
			//	throw simpleException("Null reference exception:  Must set region and region outline in layoutDesignRect.h");

			_complete = true;
			_region = finalRegion;
			//_regionOutline = finalRegionOutline;
		}

		void translateBoundary(int columnOffset, int rowOffset)
		{
			if (_complete)
				throw simpleException("Cannot further translate the boundary after calling complete:  layoutDesignRect.h");

			_boundary.translate(columnOffset, rowOffset);
		}

		void finalize(const gridRect& finalBoundary, const gridRect& finalActualBoundary)
		{
			if (!_complete)
				throw simpleException("Must call complete before finalize:  layoutDesignRect.h");

			// The final "actual" boundary is produced by the rectangle packing - translating
			// the region (boundary) into place. It will not retain the padding for the region;
			// and so must be the same size as the region boundary. The padding will be part of
			// the "final" boundary (this boundary) - which is essentially the tile boundary.
			//

			// Validate
			if (_region->getBoundary().getSize() != finalActualBoundary.getSize())
				throw simpleException("Invalid finalization boundary:  layoutDesignRect::finalize");

			// Translate the region / region outline
			if (_region->getBoundary().topLeft() != finalActualBoundary.topLeft())
			{
				gridLocator translation = _region->getBoundary().getTranslation(finalActualBoundary);

				// Use Region Constructor:
				gridRegionConstructor constructor(_region->getParentBoundary(), true);

				// Apply translation to new cells
				constructor.setTranslationFilter(translation.column, translation.row);

				// Add cells from the region
				_region->iterateLocations([&constructor] (int column, int row, const gridLocator& location)
				{
					constructor.add(column, row, location);
					return iterationCallback::iterate;
				});

				// (MEMORY!) Created new region
				gridRegion* translatedRegion = constructor.complete();

				delete _region;

				_region = translatedRegion;

				//_regionOutline->translate(translationUI);

				// Set the tile boundary to be this boundary
				_boundary = finalBoundary;
			}

			// Validate
			if (!_boundary.contains(_region->getBoundary()))
				throw simpleException("Actual boundary outside the tile boundary of the layoutDesignRect.");
		}

		size_t getHash() const override
		{
			// Just hashing declarative data (configuration, and pre-design values)
			//
			return hashGenerator::combineHash(_configuration.getHash(), _boundary);
		}

	public:
		gridRegion* getRegion() const
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

		simpleSize getTileSize() const
		{
			return _configuration.getTileSize();
		}

		gridRect getBoundary() const
		{
			return _boundary;
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

		bool compare(const layoutDesignRect& other)
		{
			return _configuration == other.getConfiguration() &&
				_boundary == other.getBoundary() &&
				_complete == other.getIsComplete() &&
				_region == other.getRegion() &&
				_regionOutline == other.getRegionOutline();
		}

		void copyImpl(const layoutDesignRect& copy)
		{
			_configuration = copy.getConfiguration();
			_boundary = copy.getBoundary();
			_complete = copy.getIsComplete();
			_region = copy.getRegion();					// Pointer Only!
			_regionOutline = copy.getRegionOutline();	// Pointer Only!
		}

	private:

		layoutCoordinateConverter* _coordinateConverter;

		// Final (Actual) Region
		gridRegion* _region;
		gridRegionOutline* _regionOutline;

		// Room / Region configuration
		brogueRoomTemplate _configuration;
		gridRect _boundary;

		bool _complete;
	};
}

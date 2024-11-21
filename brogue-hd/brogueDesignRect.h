#pragma once

#include "dungeon.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"

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
			_configuration = default_value::value<brogueRoomInfo>();
			_boundary = default_value::value <gridRect>();
			_paddedBoundary = default_value::value <gridRect>();
			_actualBoundary = default_value::value <gridRect>();
			_complete = false;
		};
		brogueDesignRect(const brogueRoomInfo& configuration, const gridRect& boundary, int padding)
		{
			_configuration = configuration;
			_boundary = boundary;
			_paddedBoundary = gridRect(boundary.column + padding,
									   boundary.row + padding,
									   boundary.width - (2 * padding),
									   boundary.height - (2 * padding));

			_actualBoundary = default_value::value<gridRect>();
			_complete = false;
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
			if (!_paddedBoundary.contains(actualBoundary))
				throw simpleException("Actual boundary outside the padded boundary of the brogueDesignRect.");

			_actualBoundary = actualBoundary;
			_complete = true;
		}

		void translate(const gridLocator& locator)
		{
			translate(locator.column, locator.row);
		}

		void translate(short columnOffset, short rowOffset)
		{
			_boundary.translate(columnOffset, rowOffset);
			_paddedBoundary.translate(columnOffset, rowOffset);

			if (_complete)
				_actualBoundary.translate(columnOffset, rowOffset);
		}

		size_t getHash() const override
		{
			// Just hashing declarative data (configuration, and pre-design values)
			//
			return hashGenerator::combineHash(_configuration.getHash(), _boundary, _paddedBoundary);
		}

	public:

		brogueRoomInfo getConfiguration() const
		{
			return _configuration;
		}
		gridRect getBoundary() const
		{
			return _boundary;
		}
		gridRect getPaddedBoundary() const
		{
			return _paddedBoundary;
		}
		gridRect getActualBoundary() const
		{
			return _actualBoundary;
		}
		bool getIsComplete() const
		{
			return _complete;
		}

	private:

		bool compare(const brogueDesignRect& other)
		{
			return _configuration == other.getConfiguration() &&
				_boundary == other.getBoundary() &&
				_paddedBoundary == other.getPaddedBoundary() &&
				_actualBoundary == other.getActualBoundary() &&
				_complete == other.getIsComplete();
		}
		void copyImpl(const brogueDesignRect& copy)
		{
			_configuration = copy.getConfiguration();
			_boundary = copy.getBoundary();
			_paddedBoundary = copy.getPaddedBoundary();
			_actualBoundary = copy.getActualBoundary();
			_complete = copy.getIsComplete();
		}

	private:

		// Room / Region configuration
		brogueRoomInfo _configuration;
		gridRect _boundary;
		gridRect _paddedBoundary;

		// Design actuals
		gridRect _actualBoundary;
		bool _complete;
	};
}
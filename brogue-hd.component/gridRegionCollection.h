#pragma once

#include <simpleList.h>

#include "gridDefinitions.h"
#include "gridRegion.h"
#include "gridLocator.h"

namespace brogueHd::component
{
	using namespace simple;

	class gridRegionCollection
	{
	public:

		gridRegionCollection(const gridRect& levelParentBoundary, const simpleList<gridRegion*>& regions);
		~gridRegionCollection();

		int getCount();

		gridLocator get(int column, int row) const;

		gridRegion* getRegion(int column, int row) const;
		gridRegion* getRegion(const gridLocator& location) const;

		bool isDefined(int column, int row) const;
		bool isDefined(const gridLocator& location) const;

	public:

		void iterate(gridCallbackConst<gridLocator> callback) const;
		void iterateAdjacent(int column, int row, gridCallbackConst<gridLocator> callback) const;

	private:

		simpleList<gridRegion*>* _regions;
		gridRect* _levelParentBoundary;
	};

	gridRegionCollection::gridRegionCollection(const gridRect& levelParentBoundary, const simpleList<gridRegion*>& regions)
	{
		_regions = new simpleList<gridRegion*>(regions);
		_levelParentBoundary = new gridRect(levelParentBoundary);
	}

	gridRegionCollection::~gridRegionCollection()
	{
		delete _regions;
		delete _levelParentBoundary;
	}

	int gridRegionCollection::getCount()
	{
		return _regions->count();
	}

	gridLocator gridRegionCollection::get(int column, int row) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(column, row))
				return _regions->get(index)->get(column, row);
		}

		return default_value::value<gridLocator>();
	}

	gridRegion* gridRegionCollection::getRegion(int column, int row) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(column, row))
				return _regions->get(index);
		}

		return nullptr;
	}

	gridRegion* gridRegionCollection::getRegion(const gridLocator& location) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(location))
				return _regions->get(index);
		}

		return nullptr;
	}

	bool gridRegionCollection::isDefined(int column, int row) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(column, row))
				return true;
		}

		return false;
	}

	bool gridRegionCollection::isDefined(const gridLocator& location) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(location))
				return true;
		}

		return false;
	}

	void gridRegionCollection::iterate(gridCallbackConst<gridLocator> callback) const
	{
		simpleList<gridRegion*>* regions = _regions;

		_levelParentBoundary->iterate([&callback, &regions] (int column, int row)
		{
			for (int index = 0; index < regions->count(); index++)
			{
				if (regions->get(index)->isDefined(column, row))
				{
					return callback(column, row, regions->get(index)->get(column, row));
				}
			}

			return iterationCallback::iterate;
		});
	}

	void gridRegionCollection::iterateAdjacent(int column, int row, gridCallbackConst<gridLocator> callback) const
	{
		simpleList<gridRegion*>* regions = _regions;

		_levelParentBoundary->iterateAdjacent(column, row, [&callback, &regions] (int column, int row)
		{
			for (int index = 0; index < regions->count(); index++)
			{
				if (regions->get(index)->isDefined(column, row))
				{
					return callback(column, row, regions->get(index)->get(column, row));
				}
			}

			return iterationCallback::iterate;
		});
	}
}
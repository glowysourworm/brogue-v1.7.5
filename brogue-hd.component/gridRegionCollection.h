#pragma once

#include <simpleList.h>

#include "gridDefinitions.h"
#include "gridRegion.h"
#include "gridLocator.h"

namespace brogueHd::component
{
	using namespace simple;

	template<typename T>
	class gridRegionCollection
	{
	public:

		gridRegionCollection(const gridRect& levelParentBoundary, const simpleList<gridRegion<T>*>& regions);
		~gridRegionCollection();

		int getCount();

		T get(int column, int row) const;
		T get(const gridLocator& location) const;

		gridRegion<T>* getRegion(int column, int row) const;
		gridRegion<T>* getRegion(const gridLocator& location) const;

		bool isDefined(int column, int row) const;
		bool isDefined(const gridLocator& location) const;

	public:

		void iterate(gridCallback<T> callback) const;
		void iterateAdjacent(int column, int row, gridCallback<T> callback) const;

	private:

		simpleList<gridRegion<T>*>* _regions;
		gridRect* _levelParentBoundary;
	};

	template<typename T>
	gridRegionCollection<T>::gridRegionCollection(const gridRect& levelParentBoundary, const simpleList<gridRegion<T>*>& regions)
	{
		_regions = new simpleList<gridRegion<T>*>(regions);
		_levelParentBoundary = new gridRect(levelParentBoundary);
	}

	template<typename T>
	gridRegionCollection<T>::~gridRegionCollection()
	{
		delete _regions;
		delete _levelParentBoundary;
	}

	template<typename T>
	int gridRegionCollection<T>::getCount()
	{
		return _regions->count();
	}

	template<typename T>
	T gridRegionCollection<T>::get(int column, int row) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(column, row))
				return _regions->get(index)->get(column, row);
		}

		return default_value::value<T>();
	}

	template<typename T>
	T gridRegionCollection<T>::get(const gridLocator& location) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(location))
				return _regions->get(index)->get(location);
		}

		return default_value::value<T>();
	}
	template<typename T>
	gridRegion<T>* gridRegionCollection<T>::getRegion(int column, int row) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(column, row))
				return _regions->get(index);
		}

		return nullptr;
	}

	template<typename T>
	gridRegion<T>* gridRegionCollection<T>::getRegion(const gridLocator& location) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(location))
				return _regions->get(index);
		}

		return nullptr;
	}

	template<typename T>
	bool gridRegionCollection<T>::isDefined(int column, int row) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(column, row))
				return true;
		}

		return false;
	}

	template<typename T>
	bool gridRegionCollection<T>::isDefined(const gridLocator& location) const
	{
		for (int index = 0; index < _regions->count(); index++)
		{
			if (_regions->get(index)->isDefined(location))
				return true;
		}

		return false;
	}

	template<typename T>
	void gridRegionCollection<T>::iterate(gridCallback<T> callback) const
	{
		_levelParentBoundary->iterate([&callback] (int column, int row)
		{
			for (int index = 0; index < _regions->count(); index++)
			{
				if (_regions->get(index)->isDefined(column, row))
				{
					callback(column, row, _regions->get(index)->get(column, row));
					return iterationCallback::breakAndReturn;
				}
			}
		});
	}

	template<typename T>
	void gridRegionCollection<T>::iterateAdjacent(int column, int row, gridCallback<T> callback) const
	{
		_levelParentBoundary->iterateAdjacent([&callback] (int column, int row)
		{
			for (int index = 0; index < _regions->count(); index++)
			{
				if (_regions->get(index)->isDefined(column, row))
				{
					callback(column, row, _regions->get(index)->get(column, row));
					return iterationCallback::breakAndReturn;
				}
			}
		});
	}
}
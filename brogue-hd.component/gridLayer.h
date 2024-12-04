#pragma once

#include "gridDefinitions.h"
#include "gridRegionCollection.h"

namespace brogueHd::component
{
	template<typename T>
	class gridLayer
	{
	public:
		gridLayer(gridRegionCollection<T>* collection);
		~gridLayer();

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

		gridRegionCollection<T>* _regionCollection;
	};

	template<typename T>
	gridLayer<T>::gridLayer(gridRegionCollection<T>* collection)
	{
		_regionCollection = collection;
	}

	template<typename T>
	gridLayer<T>::~gridLayer()
	{
		
	}

	template<typename T>
	T gridLayer<T>::get(int column, int row) const
	{
		return _regionCollection->get(column, row);
	}

	template<typename T>
	T gridLayer<T>::get(const gridLocator& location) const
	{
		return _regionCollection->get(location);
	}

	template<typename T>
	gridRegion<T>* gridLayer<T>::getRegion(int column, int row) const
	{
		if (!_regionCollection->isDefined(column, row))
			throw simpleException("Region not defined for cell location:  gridLayer.h");

		return _regionCollection->getRegion(column, row);
	}

	template<typename T>
	gridRegion<T>* gridLayer<T>::getRegion(const gridLocator& location) const
	{
		if (!_regionCollection->isDefined(location))
			throw simpleException("Region not defined for cell location:  gridLayer.h");

		return _regionCollection->getRegion(location);
	}

	template<typename T>
	bool gridLayer<T>::isDefined(int column, int row) const
	{
		return _regionCollection->isDefined(column, row);
	}

	template<typename T>
	bool gridLayer<T>::isDefined(const gridLocator& location) const
	{
		return _regionCollection->isDefined(location);
	}

	template<typename T>
	void gridLayer<T>::iterate(gridCallback<T> callback) const
	{
		_regionCollection->iterate(callback);
	}

	template<typename T>
	void gridLayer<T>::iterateAdjacent(int column, int row, gridCallback<T> callback) const
	{
		_regionCollection->iterateAdjacent(column, row, callback);
	}
}

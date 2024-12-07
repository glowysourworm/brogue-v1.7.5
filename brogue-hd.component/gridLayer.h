#pragma once

#include "gridDefinitions.h"
#include "gridRegionCollection.h"

namespace brogueHd::component
{
	class gridLayer
	{
	public:
		gridLayer(gridRegionCollection* collection);
		~gridLayer();

		gridLocator get(int column, int row) const;

		gridRegion* getRegion(int column, int row) const;
		gridRegion* getRegion(const gridLocator& location) const;

		bool isDefined(int column, int row) const;
		bool isDefined(const gridLocator& location) const;

	public:

		void iterate(gridCallbackConst<gridLocator> callback) const;
		void iterateAdjacent(int column, int row, gridCallbackConst<gridLocator> callback) const;

	private:

		gridRegionCollection* _regionCollection;
	};

	gridLayer::gridLayer(gridRegionCollection* collection)
	{
		_regionCollection = collection;
	}

	gridLayer::~gridLayer()
	{
		
	}

	gridLocator gridLayer::get(int column, int row) const
	{
		return _regionCollection->get(column, row);
	}

	gridRegion* gridLayer::getRegion(int column, int row) const
	{
		if (!_regionCollection->isDefined(column, row))
			throw simpleException("Region not defined for cell location:  gridLayer.h");

		return _regionCollection->getRegion(column, row);
	}

	gridRegion* gridLayer::getRegion(const gridLocator& location) const
	{
		if (!_regionCollection->isDefined(location))
			throw simpleException("Region not defined for cell location:  gridLayer.h");

		return _regionCollection->getRegion(location);
	}

	bool gridLayer::isDefined(int column, int row) const
	{
		return _regionCollection->isDefined(column, row);
	}

	bool gridLayer::isDefined(const gridLocator& location) const
	{
		return _regionCollection->isDefined(location);
	}

	void gridLayer::iterate(gridCallbackConst<gridLocator> callback) const
	{
		_regionCollection->iterate(callback);
	}

	void gridLayer::iterateAdjacent(int column, int row, gridCallbackConst<gridLocator> callback) const
	{
		_regionCollection->iterateAdjacent(column, row, callback);
	}
}

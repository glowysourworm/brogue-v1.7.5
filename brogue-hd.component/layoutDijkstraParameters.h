#pragma once

#include "dijkstra.h"
#include "gridDefinitions.h"

namespace brogueHd::component
{
	template<isGridLocator T>
	class layoutDijkstraParameters
	{
	public:

		/// <summary>
		/// Dijkstra's algorithm parameter (predicates) and initialization parameters
		/// </summary>
		/// <param name="mapPredicate">Is the location included in the map?</param>
		/// <param name="mapCostPredicate">What is the movement cost for this location?</param>
		/// <param name="locatorCallback">Gets a locator data structure from the grid / array component</param>
		layoutDijkstraParameters(const gridRect& layoutParentBoundary,
								 const gridRect& layoutRelativeBoundary,
		                         bool obeyCardinalMovement,
								 const dijkstraPredicate& mapPredicate,
								 const dijkstraCostCallback& mapCostPredicate,
								 const dijkstraLocatorCallback<T>& locatorCallback);
		~layoutDijkstraParameters() {};

		gridRect getParentBoundary() const;
		gridRect getRelativeBoundary() const;
		bool getObeyCardinalMovement() const;
		dijkstraPredicate getMapPredicate() const;
		dijkstraCostCallback getMapCostPredicate() const;
		dijkstraLocatorCallback<T> getLocatorCallback() const;

	private:

		gridRect _layoutParentBoundary;
		gridRect _layoutRelativeBoundary;
		bool _obeyCardinalMovement;
		dijkstraPredicate _mapPredicate;
		dijkstraCostCallback _mapCostPredicate;
		dijkstraLocatorCallback<T> _locatorCallback;
	};

	template<isGridLocator T>
	layoutDijkstraParameters<T>::layoutDijkstraParameters(const gridRect& layoutParentBoundary,
														 const gridRect& layoutRelativeBoundary,
														 bool obeyCardinalMovement,
														 const dijkstraPredicate& mapPredicate,
														 const dijkstraCostCallback& mapCostPredicate,
														 const dijkstraLocatorCallback<T>& locatorCallback)
	{
		_layoutParentBoundary = layoutParentBoundary;
		_layoutRelativeBoundary = layoutRelativeBoundary;
		_obeyCardinalMovement = obeyCardinalMovement;
		_mapPredicate = mapPredicate;
		_mapCostPredicate = mapCostPredicate;
		_locatorCallback = locatorCallback;
	}

	template<isGridLocator T>
	gridRect layoutDijkstraParameters<T>::getParentBoundary() const
	{
		return _layoutParentBoundary;
	}

	template<isGridLocator T>
	gridRect layoutDijkstraParameters<T>::getRelativeBoundary() const
	{
		return _layoutRelativeBoundary;
	}

	template<isGridLocator T>
	bool layoutDijkstraParameters<T>::getObeyCardinalMovement() const
	{
		return _obeyCardinalMovement;
	}

	template<isGridLocator T>
	dijkstraPredicate layoutDijkstraParameters<T>::getMapPredicate() const
	{
		return _mapPredicate;
	}

	template<isGridLocator T>
	dijkstraCostCallback layoutDijkstraParameters<T>::getMapCostPredicate() const
	{
		return _mapCostPredicate;
	}

	template<isGridLocator T>
	dijkstraLocatorCallback<T> layoutDijkstraParameters<T>::getLocatorCallback() const
	{
		return _locatorCallback;
	}
}
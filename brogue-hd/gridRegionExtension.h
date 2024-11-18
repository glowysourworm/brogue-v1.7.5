#pragma once

#include "simple.h"
#include "gridDefinitions.h"
#include "gridRegionConstructor.h"
#include "gridRegion.h"

using namespace brogueHd::simple;

namespace brogueHd::component
{
	class gridRegionExtension
	{
	public:

		/// <summary>
		/// (MEMORY!) Creates a new grid region instance utilizing a mapping function and gridRegionConstructor in unsafe mode.
		/// </summary>
		template<isGridLocator T, isGridLocator TResult>
		static gridRegion<TResult>* map(gridRegion<T>* region, const gridLocatorMapper<TResult, T>& mapper)
		{
			gridRegionConstructor<TResult> constructor(this->getParentBoundary(), this->getBoundary(), true);

			region->iterateLocations([&constructor, &mapper] (short column, short row, const T& item)
			{
				TResult mappedItem = mapper(item);

				// Add Unsafe Mode:  Does not check adjacency
				constructor.add(mappedItem);

				return iterationCallback::iterate;
			});

			// (MEMORY!)
			return constructor.complete();
		}
	};
}
#pragma once

#include "extensionDefinitions.h"
#include <vector>
#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	/// <summary>
	/// Set of methods for iterating, querying, and working with vector iteration.
	/// </summary>
	template<typename T, typename V>
	struct vectorExtension 
	{
		/// <summary>
		/// Iterates the specified collection and calls the user's callback
		/// </summary>
		static void forEach(std::vector<T> collection, extensionDelegates::simpleCallback callback)
		{
			for (int index = 0; index < collection.size(); index++)
			{
				if ((callback(collection[index]) & iterationCallback::breakAndReturn) != 0)
					return;
			}
		}

		/// <summary>
		/// Returns the first element in the collection that matches the given predicate
		/// </summary>
		static T first(std::vector<T> collection, extensionDelegates::simplePredicate predicate)
		{
			for (int index = 0; index < collection.size(); index++)
			{
				if (predicate(collection[index])
					return collection[index];
			}

			return NULL;
		}

		/// <summary>
		/// Returns true if the collection contains any items that pass the supplied predicate
		/// </summary>
		static bool any(std::vector<T> collection, extensionDelegates::simplePredicate predicate)
		{
			for (int index = 0; index < collection.size(); index++)
			{
				if (predicate(collection[index])
					return true;
			}

			return false;
		}

		/// <summary>
		/// Returns a new collection with elements that match the given predicate
		/// </summary>
		static std::vector<T> where(std::vector<T> collection, extensionDelegates::simplePredicate predicate)
		{
			std::vector<T> result;

			for (int index = 0; index < collection.size(); index++)
			{
				if (predicate(collection[index])
					return result.push_back(collection[index]);
			}

			return result;
		}
	};

	template<typename T, typename V>
	struct vectorExtensionSelectors
	{
		/// <summary>
		/// Returns the element of the collection that contains the maximum value for the selector
		/// </summary>
		static T maxOf(std::vector<T> collection, extensionDelegates::simpleSelector selector)
		{
			V max = NULL;
			int maxIndex = -1;

			for (int index = 0; index < collection.size(); index++)
			{
				V current = selector(collection[index]);

				if (max == NULL)
				{
					max = collection[index];
					maxIndex = index;
				}

				else if (max < current)
				{
					max = current;
					maxIndex = index;
				}
			}

			return max == NULL ? NULL : collection[maxIndex];
		}

		/// <summary>
		/// Returns the element of the collection that contains the minimum value for the selector
		/// </summary>
		static T minOf(std::vector<T> collection, extensionDelegates::simpleSelector selector)
		{
			V min = NULL;
			int minIndex = -1;

			for (int index = 0; index < collection.size(); index++)
			{
				V current = selector(collection[index]);

				if (min == NULL)
				{
					min = collection[index];
					minIndex = index;
				}

				else if (min > current)
				{
					min = current;
					minIndex = index;
				}
			}

			return min == NULL ? NULL : collection[minIndex];
		}
	};
}
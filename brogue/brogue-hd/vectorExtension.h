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
	template<typename T>
	struct vectorExtension 
	{
		/// <summary>
		/// Iterates the specified collection looking for the specified element
		/// </summary>
		static bool contains(std::vector<T> collection, T item)
		{
			for (int index = 0; index < collection.size(); index++)
			{
				if (collection[index] == item)
					return true;
			}

			return false;
		}

		template<typename TResult>
		static std::vector<TResult> select(std::vector<T> collection, extensionDelegates<T>::simpleSelector selector)
		{
			std::vector<TResult> result;

			for (int index = 0; index < collection.size(); index++)
			{
				result.push_back(selector(collection[index]));
			}

			return result;
		}

		/// <summary>
		/// (ALTERS COLLECTION!) Removes and returns all elements that match the given predicate
		/// </summary>
		static std::vector<T> remove(std::vector<T>& collection, extensionDelegates<T>::simplePredicate predicate)
		{
			std::vector<T> result;

			for (std::iterator it = collection.end(); it != collection.begin(); it--)
			{
				if (predicate(collection[index]))
				{
					result.push_back(collection.at(it));
					collection.erase(it);
				}
			}

			return result;
		}

		/// <summary>
		/// Creates a collection EXCEPT for those that pass the predicate.
		/// </summary>
		static std::vector<T> except(const std::vector<T>&, extensionDelegates<T>::simplePredicate predicate)
		{
			std::vector<T> result;

			for (std::iterator it = collection.begin(); it != collection.end(); it++)
			{
				if (!predicate(collection[index]))
				{
					result.push_back(collection.at(it));
				}
			}

			return result;
		}

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


		/// <summary>
		/// Makes a callback to the user function each time a distinc pair of elements if found
		/// </summary>
		static void distinctPairs(const std::vector<T>& collection1,
								  const std::vector<T>& collection2,
								  extensionDelegates::pairs callback)
		{
			std::map<T, std::map<T, T>> lookup;

			for (int index1 = 0; index1 < collection1.size() ;index1++)
			{
				for (int index2 = 0; index2 < collection2.size(); index2++)
				{
					// Ignore equal items
					if (collection1[index1] == collection2[index2])
						continue;

					// Ignore duplicate pairs (item1, item2)
					if (lookup.contains(collection1[index1]) &&
						lookup[collection1[index1]].contains(collection2[index2]))
						continue;

					// Ignore duplicate pairs (item2, item1)
					if (lookup.contains(collection2[index2]) &&
						lookup[collection2[index2]].contains(collection1[index1]))
						continue;

					else
					{
						// RESULT
						callback(collection1[index1], collection2[index2]);

						// Store lookup 1 -> 2
						if (lookup.contains(key1))
							lookup[collection1[index1]].insert(collection2[index2], collection2[index2]);

						else
							lookup.insert(collection1[index1], std::map<T, T>(){ { collection2[index2], collection2[index2] } };

						// Store lookup 2 -> 1
						if (lookup.contains(key2))
							lookup[collection2[index2]].insert(collection1[index1], collection1[index1]);

						else
							lookup.insert(collection2[index2], std::map<T, T>() { { collection1[index1], collection1[index1] } };
					}
				}
			}
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
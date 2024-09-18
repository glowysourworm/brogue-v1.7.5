#pragma once

#include <vector>
#include <functional>

using namespace std;

namespace brogueHd
{
	namespace collection
	{
		/// <summary>
		/// Iterates the specified collection and calls the user's callback
		/// </summary>
		template<typename T>
		static void forEach(std::vector<T> collection, function<void(T)> callback)
		{
			for (int index = 0; index < collection.size(); index++)
			{
				predicate(collection[index]);
			}
		}

		/// <summary>
		/// Returns the first element in the collection that matches the given predicate
		/// </summary>
		template<typename T>
		static T first(std::vector<T> collection, function<bool(T)> predicate)
		{
			for (int index = 0; index < collection.size(); index++)
			{
				if (predicate(collection[index])
					return collection[index];
			}

			return NULL;
		}

		/// <summary>
		/// Returns a new collection with elements that match the given predicate
		/// </summary>
		template<typename T>
		static std::vector<T> where(std::vector<T> collection, function<bool(T)> predicate)
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
		/// Returns the element of the collection that contains the maximum value for the selector
		/// </summary>
		template<typename T, typename V>
		static T max(std::vector<T> collection, function<V(T)> selector)
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
		template<typename T, typename V>
		static T min(std::vector<T> collection, function<V(T)> selector)
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
	}
}
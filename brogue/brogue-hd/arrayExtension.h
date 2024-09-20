#pragma once

#include "extensionDefinitions.h"
#include "broguedef.h"
#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	/// <summary>
	/// Iterates the specified collection and calls the user's callback
	/// </summary>
	template<typename T>
	struct arrayExtension
	{
		/// <summary>
		/// Iterates the collection using the simpleCallback delegate
		/// </summary>
		/// <param name="collection">simple array</param>
		/// <param name="callback">(see extensionDelegates)</param>
		static void forEach(const T collection[], extensionDelegates::simpleCallback callback)
		{
			for (int index = 0; index < SIZEOF(collection); index++)
			{
				if ((callback(collection[index]) & iterationCallback::breakAndReturn) != 0)
					return;
			}
		}

		/// <summary>
		/// Returns true if collection contains item
		/// </summary>
		static bool contains(const T collection[], T item)
		{
			for (int index = 0; index < SIZEOF(collection); index++)
			{
				// Comparable T
				if (item == *collection[index])
					return true;
			}

			return false;
		}
	};
}
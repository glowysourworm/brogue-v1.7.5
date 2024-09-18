#pragma once

#include "iteratordef.h"
#include "comparabledef.h"
#include "broguedef.h"
#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	/// <summary>
	/// Iterates the specified collection and calls the user's callback
	/// </summary>
	template<comparable T>
	static void forEach(T* collection, function<iterationCallback(T)> callback)
	{
		for (int index = 0; index < SIZEOF(collection); index++)
		{
			if ((predicate(collection[index]) & iterationCallback::breakAndReturn) != 0)
				return;
		}
	}

	/// <summary>
	/// Returns true if collection contains item
	/// </summary>
	template<comparable T>
	static bool contains(T* collection, T item)
	{
		for (int index = 0; index < SIZEOF(collection); index++)
		{
			// Comparable T
			if (item == *collection[index])
				return true;
		}

		return false;
	}
}
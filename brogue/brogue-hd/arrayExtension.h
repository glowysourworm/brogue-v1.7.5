#pragma once

#include "extensionDefinitions.h"
#include "broguedef.h"
#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	/// <summary>
	/// Definitions for most of the basic delegates in the extensions namespace
	/// </summary>
	template<typename T>
	struct arrayDelegates
	{
	public:

		/// <summary>
		/// Definition of function to provide callback: 1) user can return iterationCallback 
		/// value to either break, or continue the loop.
		/// </summary>
		/// <param name="value">callback (current) value</param>
		typedef std::function<iterationCallback(T item)> callback;
	};

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
		static void forEach(const T collection[], arrayDelegates<T>::callback callback)
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
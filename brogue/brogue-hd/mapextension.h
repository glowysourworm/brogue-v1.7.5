#pragma once

#include "extensionDefinitions.h"
#include <map>
#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	template<typename K, typename V>
	struct mapDelegates
	{
		/// <summary>
		/// Definition of simple predicate for any key-value map
		/// </summary>
		typedef std::function<bool(K key, V value)> predicate;

		/// <summary>
		/// Definition of function to provide callback: 1) user can return iterationCallback 
		/// value to either break, or continue the loop.
		/// </summary>
		typedef std::function<iterationCallback(K key, V value)> callback;

		/// <summary>
		/// Definition of selector for the value type
		/// </summary>
		template<typename VResult>
		typedef std::function<VResult(V value)> selector;
	};

	/// <summary>
	/// Static methods to operate on the std::map data structure
	/// </summary>
	template<typename K, typename V, typename VResult>
	struct mapExtension
	{
		static bool any(const std::map<K, V>& map, mapDelegates<K, V>::predicate predicate)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (predicate(it->first, it->second))
					return true;
			}

			return false;
		}

		static K firstKey(const std::map<K, V>& map, mapDelegates<K, V>::callback callback)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (predicate(it->first, it->second))
					return it->first;
			}

			return NULL;
		}

		static void forEach(const std::map<K, V>& map, mapDelegates<K, V>::callback callback)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if ((callback(it->first, it->second) & iterationCallback::breakAndReturn) != 0)
					break;
			}
		}

		static K firstOrDefaultKey(const std::map<K, V>& map, mapDelegates<K, V>::predicate predicate)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (predicate(it->first, it->second))
					return it->first;
			}

			return NULL;
		}

		static std::vector<VResult> selectFromValues(const std::map<K, V>& map, mapDelegates<K, V>::selector selector)
		{
			std::vector<VResult> result;

			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				result.push_back(selector(it->second));
			}

			return result;
		}

		static K getKeyAt(const std::map<K, V>& map, int index)
		{
			int counter = 0;

			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (counter++ == index)
					return it->first;
			}

			return NULL;
		}

		static V getValueAt(const std::map<K, V>& map, int index)
		{
			int counter = 0;

			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (counter++ == index)
					return it->second;
			}

			return NULL;
		}

		static std::vector<K> getKeys(const std::map<K, V>& map)
		{
			std::vector<K> result;

			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				result.push_back(it->first);
			}

			return result;
		}
	};
}
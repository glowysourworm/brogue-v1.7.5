#pragma once

#include "extensionDefinitions.h"
#include <map>
#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	/// <summary>
	/// Static methods to operate on the std::map data structure
	/// </summary>
	template<typename K, typename V>
	struct mapExtension
	{
		static bool any(const std::map<K, V>& map, extensionMapDelegates::simplePredicate predicate)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (predicate(it->first, it->second))
					return true;
			}

			return false;
		}

		static K firstKey(const std::map<K, V>& map, extensionMapDelegates::simpleCallback callback)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (predicate(it->first, it->second))
					return it->first;
			}

			return NULL;
		}

		static void forEach(const std::map<K, V>& map, extensionMapDelegates::simpleCallback callback)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if ((callback(it->first, it->second) & iterationCallback::breakAndReturn) != 0)
					break;
			}
		}

		static K firstOrDefaultKey(const std::map<K, V>& map, extensionMapDelegates::simplePredicate predicate)
		{
			for (std::iterator it = map->begin(); it <= map->end(); it++)
			{
				if (predicate(it->first, it->second))
					return it->first;
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
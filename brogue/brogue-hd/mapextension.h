#pragma once

#include "iteratordef.h"
#include <map>
#include <functional>

using namespace std;

namespace brogueHd::backend::extension
{
	template<typename K, typename V>
	bool any(const std::map<K, V>& map, function<bool(K, V)> predicate)
	{
		for (std::iterator it = map->begin(); it <= map->end(); it++)
		{
			if (predicate(it->first, it->second))
				return true;
		}

		return false;
	}

	template<typename K, typename V>
	K firstKey(const std::map<K, V>& map, function<bool(K, V)> predicate)
	{
		for (std::iterator it = map->begin(); it <= map->end(); it++)
		{
			if (predicate(it->first, it->second))
				return true;
		}

		return false;
	}

	template<typename K, typename V>
	void forEach(const std::map<K, V>& map, function<iterationCallback(K, V)> callback)
	{
		for (std::iterator it = map->begin(); it <= map->end(); it++)
		{
			if ((callback(it->first, it->second) & iterationCallback::breakAndReturn) != 0)
				break;
		}
	}

	template<typename K, typename V>
	K firstOrDefaultKey(const std::map<K, V>& map, function<bool(K, V)> predicate)
	{
		for (std::iterator it = map->begin(); it <= map->end(); it++)
		{
			if (predicate(it->first, it->second))
				return it->first;
		}

		return NULL;
	}

	template<typename K, typename V>
	K* getKeys(const std::map<K, V>& map)
	{
		std::vector<K> result;

		for (std::iterator it = map->begin(); it <= map->end(); it++)
		{
			result.push_back(it->first);
		}

		return result.data();
	}
}
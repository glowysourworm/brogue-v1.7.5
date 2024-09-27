#pragma once

#include "simpleHash.h"
#include "exceptionHandler.h"

namespace brogueHd::component
{
	template<typename K, typename V>
	simpleHash<K, V>::simpleHash()
	{
		_table = new simpleArray<simpleList<simplePair<K, V>>*>(0);
		_list = new simpleList<simplePair<K, V>>();
		_generator = generator;
		_maxBucketSize = 0;
	}

	template<typename K, typename V>
	simpleHash<K, V>::~simpleHash()
	{
		delete _table;
		delete _list;
	}

	template<typename K, typename V>
	V simpleHash<K, V>::get(K key)
	{
		long hashCode = _generator(key);
		int bucketIndex = this->calculateBucketIndex(hashCode);

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index).key == key)
				return _table->get(bucketIndex)->get(index).value;
		}

		brogueException::show("Key not found in hash table:  simpleHash.cpp");
	}

	template<typename K, typename V>
	simplePair<K, V> simpleHash<K, V>::getAt(int index)
	{
		return _list->get(index);
	}

	template<typename K, typename V>
	V simpleHash<K, V>::operator[](K key) const
	{
		return this->get(key);
	}

	template<typename K, typename V>
	void simpleHash<K, V>::add(K key, V value)
	{
		// First rehash will give 100 buckets
		if (_table->count() == 0)
			rehash(100);

		// Decision to rehash:
		//
		//	1) First bucket set is 100 items (what's "big"?) (# of buckets / 10)?
		//	2) Log_10 -> An overflow of 3 = 1000 buckets. overflow of 6 = 1000000 buckets. (probably too much)
		//  3) Log_e  -> Ln -> (~6.9, 1000); (~13.8, 1000000). That's much less aggressive.
		//

		if (_maxBucketSize > log(_table->count()))
		{
			// Multiply the bucket size by e ~ 2.718281828
			//
			rehash(_table-count() * exp(1));
		}

		// If there's still overflow, the max size will be set for the next call to set(..)
		//
		long hashCode = _generator(key);
		int bucketIndex = this->calculateBucketIndex(hashCode);
		
		// Add to the i-th bucket
		//
		simplePair<K,V> pair = simplePair<K, V>(key, value);

		_table->get(bucketIndex)->add(pair);
		_list->add(pair);
	}

	template<typename K, typename V>
	bool simpleHash<K, V>::remove(K key)
	{
		long hashCode = _generator(key);
		int bucketIndex = this->calculateBucketIndex(hashCode);

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index).key == key)
			{
				// Check max bucket size / track
				if (_table->get(bucketIndex)->count() == _maxBucketSize)
					_maxBucketSize--;

				_table->get(bucketIndex)->removeAt(index);

				return true;
			}
		}		

		// TODO: Use Ordered List
		for (int index = 0; index < _list->count(); index++)
		{
			if (_list->get(index).key == key)
			{
				_list->removeAt(index);
				break;
			}
		}

		return false;
	}

	template<typename K, typename V>
	long simpleHash<K, V>::calculateHashCode(K key)
	{
		// Use std::hash until we find issues with it
		//
		return std::hash<K>();
	}

	template<typename K, typename V>
	int simpleHash<K, V>::calculateBucketIndex(long hashCode) const
	{
		return hashCode % _table->count();
	}

	template<typename K, typename V>
	bool simpleHash<K, V>::contains(K key) const
	{
		long hashCode = _generator(key);
		int bucketIndex = this->calculateBucketIndex(hashCode);

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index).key == key)
				return true;
		}

		return false;
	}

	template<typename K, typename V>
	int simpleHash<K, V>::count() const
	{
		return _list->count();
	}

	template<typename K, typename V>
	void simpleHash<K, V>::iterate(simpleHashDelegates<K, V>::callback callback)
	{
		for (int index = 0; index < _table->count; index++)
		{
			for (int bucketIndex = 0; bucketIndex < _table->get(index)->count(); bucketIndex++)
			{
				simplePair<K, V> pair = _table->get(index)->get(index);

				if (callback(pair.key, pair.value) == iterationCallback::breakAndReturn)
					return;
			}
		}
	}

	template<typename K, typename V>
	void simpleHash<K, V>::rehash(int newSize)
	{
		// Setup new hash table with the specified size limit
		simpleArray<simpleList<simplePair<K, V>>*>* newTable = new simpleArray<simpleList<simplePair<K,V>>*>(newSize);

		// Setup new follower list for indexOf retrieval
		simpleList<simplePair<K, V>>* newList = new simpleList<simplePair<K, V>>();

		// Reset the max bucket size tracker
		_maxBucketSize = 0;

		for (int index = 0; index < _table->size(); index++)
		{
			for (int bucketIndex = 0; bucketIndex < _table->get(index)->count(); bucketIndex++)
			{
				// Get details from each bucket copied over
				K key = _table->get(index)->get(bucketIndex);
				V value = _table->get(index)->get(bucketIndex);

				long hashCode = _generator(key);
				long newBucketIndex = hashCode % newSize;		// Find a bucket for the data

				if (newTable->get(newBucketIndex) == NULL)
					newTable->set(newBucketIndex, new simpleList<simplePair<K,V>>());

				simplePair<K, V> pair(key, value);

				newTable->get(newBucketIndex)->add(pair);
				newList->add(pair);

				// Track the new bucket size while we're here
				if (newTable->get(newBucketIndex)->count() > _maxBucketSize)
					_maxBucketSize = newTable->get(newBucketIndex)->count();
			}
		}

		// Delete the old data container
		delete _table;
		delete _list;

		// Set the new pointer
		_table = newTable;
		_list = newList;
	}

	template<typename K, typename V>
	bool simpleHash<K, V>::any(simpleHashDelegates<K, V>::predicate predicate)
	{
		bool result = false;

		this->iterate([&result](K key, V value)
		{
			if (predicate(key, value))
			{
				result = true;
				return iterationCallback::breakAndReturn;
			}
		});

		return result;
	}

	template<typename K, typename V>
	simpleList<simplePair<K, V>> simpleHash<K, V>::removeWhere(simpleHashDelegates<K, V>::predicate predicate)
	{
		simpleList<simplePair<K, V>> result;

		for (int index = _list->count() - 1; index >= 0; index--)
		{
			if (predicate(_list[index]))
			{
				result.add(_list[index]);
				
				// TODO: Either do this all in one loop (here), or make a removeRange function
				this->remove(_list[index].key);
			}
		}

		return result;
	}

	template<typename K, typename V>
	K simpleHash<K, V>::firstKey(simpleHashDelegates<K, V>::predicate predicate)
	{
		K result = NULL;

		this->iterate([&result](K key, V value)
		{
			if (predicate(key, value))
			{
				result = key;
				return iterationCallback::breakAndReturn;
			}
		});

		return result;
	}

	template<typename K, typename V>
	void simpleHash<K, V>::forEach(simpleHashDelegates<K, V>::callback callback)
	{
		this->iterate([](K key, V value)
		{
			return callback(key, value);
		});
	}

	template<typename K, typename V>
	K simpleHash<K, V>::firstOrDefaultKey(simpleHashDelegates<K, V>::predicate predicate)
	{
		K result = NULL;

		this->iterate([&result](K key, V value)
		{
			if (predicate(key, value))
			{
				result = key;
				return iterationCallback::breakAndReturn;
			}
		});

		return result;
	}

	template<typename K, typename V>
	template<typename VResult>
	simpleList<VResult> simpleHash<K, V>::selectFromValues(simpleHashDelegates<K, V>::selector selector)
	{
		simpleList<VResult> result;

		this->iterate([&result](K key, V value)
		{
			result.add(selector(value));
		});

		return result;
	}

	template<typename K, typename V>
	simpleList<K> simpleHash<K, V>::getKeys() const
	{
		simpleList<K> result;

		this->iterate([&result](K key, V value)
		{
			result.add(key);
		});

		return result;
	}
}
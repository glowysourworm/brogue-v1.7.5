#pragma once

#include "simple.h"
#include "simpleArray.h"
#include "simpleList.h"
#include <functional>

using namespace std;

namespace brogueHd::component
{
	/// <summary>
	/// Definition of simple predicate for any key-value map
	/// </summary>
	template<typename K, typename V>
	using simpleHashPredicate = std::function<bool(K key, V value)>;

	/// <summary>
	/// Definition of simple predicate for any key-value map
	/// </summary>
	template<typename K, typename V>
	using simpleHashCallback = std::function<iterationCallback(K key, V value)>;

	/// <summary>
	/// Definition of selector for the value type
	/// </summary>
	template<typename K, typename V, typename VResult>
	using simpleHashSelector = std::function<VResult(V value)>;

	template<typename K, typename V>
	struct simplePair
	{
		K key;
		V value;

		simplePair(){}
		simplePair(K akey, V avalue)
		{
			key = akey;
			value = avalue;
		}

		bool operator==(const simplePair& pair)
		{
			return pair.key == key && pair.value == value;
		}
		bool operator!=(const simplePair& pair)
		{
			return pair.key != key || pair.value != value;
		}
	};

	template<typename K, typename V>
	class simpleHash
	{
	public:
		simpleHash();
		~simpleHash();

		V operator[](K key) const;

		V get(K key) const;
		void add(K key, V value);
		void set(K key, V value);

		simplePair<K,V> getAt(int index);

		bool contains(K key) const;
		int count() const;

		bool remove(K key);
		void clear();

		void iterate(simpleHashCallback<K, V> callback) const;

	private:

		int calculateHashCode(K key) const;
		int calculateBucketIndex(int hashCode) const;
		void rehash(int newSize);

	public:	// Extension Methods:  mostly queries

		bool any(simpleHashPredicate<K, V> predicate);
		void forEach(simpleHashCallback<K, V> callback);
		K firstKey(simpleHashPredicate<K, V> predicate);
		K firstOrDefaultKey(simpleHashPredicate<K, V> predicate);
		simpleList<simplePair<K, V>> removeWhere(simpleHashPredicate<K, V> predicate);
		
		template<typename VResult>
		simpleList<VResult> selectFromValues(simpleHashSelector<K, V, VResult> selector);

		//K getKeyAt(int index);
		//V getValueAt(const std::map<K, V>& map, int index);

		simpleList<K> getKeys() const;
		simpleList<V> getValues() const;

	private:

		// Static Hash Table (with dynamic buckets)
		simpleArray<simpleList<simplePair<K, V>>*>* _table;

		// List follower for the primary table - for index lookup
		simpleList<simplePair<K, V>>* _list;

		// Bucket Sizes (prevents iteration of bucket lists during set(..))
		int _maxBucketSize;
	};

	template<typename K, typename V>
	simpleHash<K, V>::simpleHash()
	{
		_table = new simpleArray<simpleList<simplePair<K, V>>*>(0);
		_list = new simpleList<simplePair<K, V>>();
		_maxBucketSize = 0;
	}

	template<typename K, typename V>
	simpleHash<K, V>::~simpleHash()
	{
		delete _table;
		delete _list;
	}

	template<typename K, typename V>
	void simpleHash<K, V>::clear()
	{
		simpleList<K> keys = this->getKeys();

		for (int index = 0; index < keys.count(); index++)
			this->remove(keys[index]);
	}

	template<typename K, typename V>
	V simpleHash<K, V>::get(K key) const
	{
		int hashCode = this->calculateHashCode(key);
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
		if (this->contains(key))
			brogueException::show("Trying to add duplicate value to simpleHash table. Use set(...)");

		// First rehash will give 100 buckets
		if (_table->size() == 0)
			rehash(100);

		// Decision to rehash:
		//
		//	1) First bucket set is 100 items (what's "big"?) (# of buckets / 10)?
		//	2) Log_10 -> An overflow of 3 = 1000 buckets. overflow of 6 = 1000000 buckets. (probably too much)
		//  3) Log_e  -> Ln -> (~6.9, 1000); (~13.8, 1000000). That's much less aggressive.
		//

		if (_maxBucketSize > log(_table->size()))
		{
			// Multiply the bucket size by e ~ 2.718281828
			//
			rehash(_table->size() * (int)exp(1));
		}

		// If there's still overflow, the max size will be set for the next call to set(..)
		//
		int hashCode = this->calculateHashCode(key);
		int bucketIndex = this->calculateBucketIndex(hashCode);

		// Add to the i-th bucket
		//
		simplePair<K, V> pair = simplePair<K, V>(key, value);

		_table->get(bucketIndex)->add(pair);
		_list->add(pair);
	}

	template<typename K, typename V>
	void simpleHash<K, V>::set(K key, V value)
	{
		if (!this->contains(key))
			brogueException::show("Trying to set value for a key-value pair that doesn't exist. Use add(...)");

		V oldValue = this->get(key);

		// Iterate to find the location in the ith bucket
		int hashCode = this->calculateHashCode(key);
		int bucketIndex = this->calculateBucketIndex(hashCode);

		// Iterate to set the item value
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index).value == oldValue)
			{
				simplePair<K,V> pair = _table->get(bucketIndex)->get(index);

				pair.value = value;
				break;
			}
		}
	}

	template<typename K, typename V>
	bool simpleHash<K, V>::remove(K key)
	{
		int hashCode = this->calculateHashCode(key);
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
	int simpleHash<K, V>::calculateHashCode(K key) const
	{
		return (int)std::hash<K>{}(key);
	}

	template<typename K, typename V>
	int simpleHash<K, V>::calculateBucketIndex(int hashCode) const
	{
		return hashCode % _table->size();
	}

	template<typename K, typename V>
	bool simpleHash<K, V>::contains(K key) const
	{
		int hashCode = this->calculateHashCode(key);
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
	void simpleHash<K, V>::iterate(simpleHashCallback<K, V> callback) const
	{
		for (int index = 0; index < _table->size(); index++)
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
		simpleArray<simpleList<simplePair<K, V>>*>* newTable = new simpleArray<simpleList<simplePair<K, V>>*>(newSize);

		// Setup new follower list for indexOf retrieval
		simpleList<simplePair<K, V>>* newList = new simpleList<simplePair<K, V>>();

		// Reset the max bucket size tracker
		_maxBucketSize = 0;

		for (int index = 0; index < _table->size(); index++)
		{
			for (int bucketIndex = 0; bucketIndex < _table->get(index)->count(); bucketIndex++)
			{
				// Get details from each bucket copied over
				K key = _table->get(index)->get(bucketIndex).key;
				V value = _table->get(index)->get(bucketIndex).value;

				int hashCode = this->calculateHashCode(key);
				int newBucketIndex = hashCode % newSize;		// Find a bucket for the data

				if (newTable->get(newBucketIndex) == NULL)
					newTable->set(newBucketIndex, new simpleList<simplePair<K, V>>());

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
	bool simpleHash<K, V>::any(simpleHashPredicate<K, V> predicate)
	{
		bool result = false;

		this->iterate([&result, &predicate](K key, V value)
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
	simpleList<simplePair<K, V>> simpleHash<K, V>::removeWhere(simpleHashPredicate<K, V> predicate)
	{
		simpleList<simplePair<K, V>> result;

		for (int index = _list->count() - 1; index >= 0; index--)
		{
			if (predicate(_list[index].key, _list[index].value))
			{
				result.add(_list[index]);

				// TODO: Either do this all in one loop (here), or make a removeRange function
				this->remove(_list[index].key);
			}
		}

		return result;
	}

	template<typename K, typename V>
	K simpleHash<K, V>::firstKey(simpleHashPredicate<K, V> predicate)
	{
		K result = NULL;

		this->iterate([&result, &predicate](K key, V value)
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
	void simpleHash<K, V>::forEach(simpleHashCallback<K, V> callback)
	{
		this->iterate([&callback](K key, V value)
		{
			return callback(key, value);
		});
	}

	template<typename K, typename V>
	K simpleHash<K, V>::firstOrDefaultKey(simpleHashPredicate<K, V> predicate)
	{
		K result = default_value<K>::value;

		this->iterate([&result, &predicate](K key, V value)
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
	simpleList<VResult> simpleHash<K, V>::selectFromValues(simpleHashSelector<K, V, VResult> selector)
	{
		simpleList<VResult> result;

		this->iterate([&result, &selector](K key, V value)
		{
			result.add(selector(value));
			return iterationCallback::iterate;
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

			return iterationCallback::iterate;
		});

		return result;
	}

	template<typename K, typename V>
	simpleList<V> simpleHash<K, V>::getValues() const
	{
		simpleList<V> result;

		this->iterate([&result](K key, V value)
		{
			result.add(value);

			return iterationCallback::iterate;
		});

		return result;
	}
}
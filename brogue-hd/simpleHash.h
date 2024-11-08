#pragma once

#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleList.h"
#include "simpleMath.h"
#include <chrono>
#include <cmath>
#include <functional>

namespace brogueHd::simple
{
	/// <summary>
	/// Definition of simple predicate for any key-value map
	/// </summary>
	template<isHashable K, isHashable V>
	using simpleHashPredicate = std::function<bool(const K& key, const V& value)>;

	/// <summary>
	/// Definition of simple predicate for any key-value map
	/// </summary>
	template<isHashable K, isHashable V>
	using simpleHashCallback = std::function<iterationCallback(const K &key, const V& value)>;

	/// <summary>
	/// Definition of selector for the value type
	/// </summary>
	template<isHashable K, isHashable V, typename VResult>
	using simpleHashSelector = std::function<VResult(const V& value)>;

	template<isHashable K, isHashable V>
	struct simplePair : hashable
	{
		K key;
		V value;

		simplePair() {}
		simplePair(const K& akey, const V& avalue)
		{
			key = akey;
			value = avalue;
		}

		bool operator==(const simplePair& pair) const
		{
			return pair.key == key && pair.value == value;
		}
		bool operator!=(const simplePair& pair) const
		{
			return pair.key != key || pair.value != value;
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(key);
		}
	};

	template<isHashable K, isHashable V>
	class simpleHash : public hashableObject
	{
	public:
		simpleHash();
		~simpleHash();

		V operator[](const K& key) const;

		V get(const K& key) const;
		void add(const K& key, const V& value);
		void set(const K& key, const V& value);

		simplePair<K, V>* getAt(int index);

		bool contains(const K& key) const;
		int count() const;

		bool remove(const K& key);
		void clear();

		void iterate(const simpleHashCallback<K, V>& callback) const;

	private:

		size_t calculateHashCode(const K& key) const;
		size_t calculateBucketIndex(int hashCode) const;
		void rehash(size_t newSize);

	public:	// Extension Methods:  mostly queries

		bool any(const simpleHashPredicate<K, V>& predicate);
		void forEach(const simpleHashCallback<K, V>& callback);
		V firstValue();
		V firstValue(const simpleHashPredicate<K, V>& predicate);
		K firstKey();
		K firstKey(const simpleHashPredicate<K, V>& predicate);
		K firstOrDefaultKey(const simpleHashPredicate<K, V>& predicate);
		simpleList<simplePair<K, V>> removeWhere(const simpleHashPredicate<K, V>& predicate);

		template<typename VResult>
		simpleList<VResult> selectFromValues(const simpleHashSelector<K, V, VResult>& selector);

		//K getKeyAt(int index);
		//V getValueAt(const std::map<K, V>& map, int index);

		simpleList<K> getKeys() const;
		simpleList<V> getValues() const;

	public:
		
		size_t getHash() const override;

	private:

		// Static Hash Table (with dynamic buckets)
		simpleArray<simpleList<simplePair<K, V>*>*>* _table;

		// List follower for the primary table - for index lookup
		simpleList<simplePair<K, V>*>* _list;

		// Bucket Sizes (prevents iteration of bucket lists during set(..))
		int _maxBucketSize;
	};

	template<isHashable K, isHashable V>
	simpleHash<K, V>::simpleHash()
	{
		_table = new simpleArray<simpleList<simplePair<K, V>*>*>(100);
		_list = new simpleList<simplePair<K, V>*>();
		_maxBucketSize = 0;

		// (MEMORY!)
		for (int index = 0; index < _table->count(); index++)
		{
			_table->set(index, new simpleList<simplePair<K, V>*>());
		}
	}

	template<isHashable K, isHashable V>
	simpleHash<K, V>::~simpleHash()
	{
		// (MEMORY!)
		for (int index = 0; index < _table->count(); index++)
		{
			for (int arrayIndex = 0; arrayIndex < _table->get(index)->count(); arrayIndex++)
			{
				delete _table->get(index)->get(arrayIndex);
			}

			delete _table->get(index);
		}

		delete _table;
		delete _list;
	}

	template<isHashable K, isHashable V>
	void simpleHash<K, V>::clear()
	{
		simpleList<K> keys = this->getKeys();

		for (int index = 0; index < keys.count(); index++)
			this->remove(keys.get(index));
	}

	template<isHashable K, isHashable V>
	V simpleHash<K, V>::get(const K& key) const
	{
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index)->key == key)
				return _table->get(bucketIndex)->get(index)->value;
		}

		simpleException::showCstr("Key not found in hash table:  simpleHash.cpp");
	}

	template<isHashable K, isHashable V>
	simplePair<K, V>* simpleHash<K, V>::getAt(int index)
	{
		return _list->get(index);
	}

	template<isHashable K, isHashable V>
	V simpleHash<K, V>::operator[](const K& key) const
	{
		return this->get(key);
	}

	template<isHashable K, isHashable V>
	void simpleHash<K, V>::add(const K& key, const V& value)
	{
		if (this->contains(key))
			simpleException::showCstr("Trying to add duplicate value to simpleHash table. Use set(...)");

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
			rehash(_table->count() * simpleMath::exponential(1));
		}

		// If there's still overflow, the max size will be set for the next call to set(..)
		//
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		// (MEMORY!) Add to the i-th bucket
		//
		simplePair<K, V>* pair = new simplePair<K, V>(key, value);

		_table->get(bucketIndex)->add(pair);
		_list->add(pair);
	}

	template<isHashable K, isHashable V>
	void simpleHash<K, V>::set(const K& key, const V& value)
	{
		if (!this->contains(key))
			simpleException::showCstr("Trying to set value for a key-value pair that doesn't exist. Use add(...)");

		V oldValue = this->get(key);

		// Iterate to find the location in the ith bucket
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		// Iterate to set the item value
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index).value == oldValue)
			{
				simplePair<K, V>* pair = _table->get(bucketIndex)->get(index);

				pair->value = value;
				break;
			}
		}
	}

	template<isHashable K, isHashable V>
	bool simpleHash<K, V>::remove(const K& key)
	{
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index)->key == key)
			{
				// Check max bucket size / track
				if (_table->get(bucketIndex)->count() == _maxBucketSize)
					_maxBucketSize--;

				simplePair<K, V>* pair = _table->get(bucketIndex)->get(index);

				// (MEMORY!)
				delete pair;

				_table->get(bucketIndex)->removeAt(index);

				return true;
			}
		}

		// TODO: Use Ordered List
		for (int index = 0; index < _list->count(); index++)
		{
			if (_list->get(index)->key == key)
			{
				simplePair<K, V>* pair = _list->get(index);

				// (MEMORY!)
				delete pair;

				_list->removeAt(index);
				break;
			}
		}

		return false;
	}

	template<isHashable K, isHashable V>
	size_t simpleHash<K, V>::calculateHashCode(const K& key) const
	{
		return hashGenerator::generateHash(key);
	}

	template<isHashable K, isHashable V>
	size_t simpleHash<K, V>::calculateBucketIndex(int hashCode) const
	{
		return hashCode % (size_t)_table->count();
	}

	template<isHashable K, isHashable V>
	bool simpleHash<K, V>::contains(const K& key) const
	{
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index)->key == key)
				return true;
		}

		return false;
	}

	template<isHashable K, isHashable V>
	int simpleHash<K, V>::count() const
	{
		return _list->count();
	}

	template<isHashable K, isHashable V>
	void simpleHash<K, V>::iterate(const simpleHashCallback<K, V>& callback) const
	{
		for (int index = 0; index < _table->count(); index++)
		{
			for (int bucketIndex = 0; bucketIndex < _table->get(index)->count(); bucketIndex++)
			{
				simplePair<K, V>* pair = _table->get(index)->get(bucketIndex);

				if (callback(pair->key, pair->value) == iterationCallback::breakAndReturn)
					return;
			}
		}
	}

	template<isHashable K, isHashable V>
	void simpleHash<K, V>::rehash(size_t newSize)
	{
		// Setup new hash table with the specified size limit
		simpleArray<simpleList<simplePair<K, V>*>*>* newTable = new simpleArray<simpleList<simplePair<K, V>*>*>(newSize);

		// Setup new follower list for indexOf retrieval
		simpleList<simplePair<K, V>*>* newList = new simpleList<simplePair<K, V>*>();

		// Reset the max bucket size tracker
		_maxBucketSize = 0;

		for (int index = 0; index < _table->count(); index++)
		{
			for (int bucketIndex = 0; bucketIndex < _table->get(index)->count(); bucketIndex++)
			{
				// Get details from each bucket copied over
				simplePair<K, V>* pair = _table->get(index)->get(bucketIndex);

				size_t hashCode = this->calculateHashCode(pair->key);
				size_t newBucketIndex = hashCode % newSize;		// Find a bucket for the data

				if (newTable->get(newBucketIndex) == NULL)
					newTable->set(newBucketIndex, new simpleList<simplePair<K, V>*>());

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

	template<isHashable K, isHashable V>
	bool simpleHash<K, V>::any(const simpleHashPredicate<K, V>& predicate)
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

	template<isHashable K, isHashable V>
	simpleList<simplePair<K, V>> simpleHash<K, V>::removeWhere(const simpleHashPredicate<K, V>& predicate)
	{
		simpleList<simplePair<K, V>> result;

		for (int index = _list->count() - 1; index >= 0; index--)
		{
			if (predicate(_list[index]->key, _list[index]->value))
			{
				result.add(_list[index]);

				// TODO: Either do this all in one loop (here), or make a removeRange function
				this->remove(_list[index]->key);
			}
		}

		return result;
	}

	template<isHashable K, isHashable V>
	K simpleHash<K, V>::firstKey()
	{
		if (this->count() == 0)
			throw simpleException("Trying to access empty simpleHash:  simpleHash::firstKey");

		return this->getAt(0)->key;
	}

	template<isHashable K, isHashable V>
	K simpleHash<K, V>::firstKey(const simpleHashPredicate<K, V>& predicate)
	{
		K result = default_value::value<K>();

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

	template<isHashable K, isHashable V>
	V simpleHash<K, V>::firstValue()
	{
		return this->getAt(0)->value;
	}

	template<isHashable K, isHashable V>
	V simpleHash<K, V>::firstValue(const simpleHashPredicate<K, V>& predicate)
	{
		V result = default_value::value<V>();

		this->iterate([&result, &predicate] (K key, V value)
		{
			if (predicate(key, value))
			{
				result = value;
				return iterationCallback::breakAndReturn;
			}
		});

		return result;
	}

	template<isHashable K, isHashable V>
	void simpleHash<K, V>::forEach(const simpleHashCallback<K, V>& callback)
	{
		this->iterate([&callback](K key, V value)
		{
			return callback(key, value);
		});
	}

	template<isHashable K, isHashable V>
	K simpleHash<K, V>::firstOrDefaultKey(const simpleHashPredicate<K, V>& predicate)
	{
		K result = default_value::value<K>();

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

	template<isHashable K, isHashable V>
	template<typename VResult>
	simpleList<VResult> simpleHash<K, V>::selectFromValues(const simpleHashSelector<K, V, VResult>& selector)
	{
		simpleList<VResult> result;

		this->iterate([&result, &selector](K key, V value)
		{
			result.add(selector(value));
			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable K, isHashable V>
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

	template<isHashable K, isHashable V>
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

	template<isHashable K, isHashable V>
	size_t simpleHash<K, V>::getHash() const
	{
		size_t hash = 0;
		
		// Generate hash from just the key hash values
		//
		this->iterate([&hash] (K key, V value)
		{
			if (hash == 0)
				hash = hashGenerator::generateHash(key);

			else
				hash = hashGenerator::combineHash(hash, key);
				
			return iterationCallback::iterate;
		});

		return hash;
	}
}
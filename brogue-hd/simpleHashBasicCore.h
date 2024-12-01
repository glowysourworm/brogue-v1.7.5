#pragma once

#include "simple.h"
#include "simpleArray.h"
#include "simpleList.h"
#include "simplePair.h"
#include "simpleException.h"
#include "simpleHashCore.h"
#include "simpleMath.h"

namespace brogueHd::simple
{
	template<isHashable K, typename V>
	class simpleHashBasicCore : public simpleHashCore<K, V>
	{
	public:

		simpleHashBasicCore();
		~simpleHashBasicCore();

		V get(const K& key) const override;
		void add(const K& key, const V& value) override;
		void set(const K& key, const V& value) override;

		simplePair<K, V>* getAt(int index) override;

		bool contains(const K& key) const override;
		int count() const override;

		bool remove(const K& key) override;
		void clear() override;

	public:

		simpleList<K> getKeys() const;
		void iterate(const simpleHashCallback<K, V>& callback) const override;

	private:

		size_t calculateHashCode(const K& key) const;
		size_t calculateBucketIndex(size_t hashCode) const;
		void rehash(size_t newSize);

	private:

		// Static Hash Table (with dynamic buckets)
		simpleArray<simpleList<simplePair<K, V>*>*>* _table;

		// List follower for the primary table - for index lookup
		simpleList<simplePair<K, V>*>* _list;

		// Bucket Sizes (prevents iteration of bucket lists during set(..))
		int _maxBucketSize;
	};

	template<isHashable K, typename V>
	simpleHashBasicCore<K, V>::simpleHashBasicCore()
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

	template<isHashable K, typename V>
	simpleHashBasicCore<K, V>::~simpleHashBasicCore()
	{
		// (MEMORY!)
		for (int arrayIndex = 0; arrayIndex < _table->count(); arrayIndex++)
		{
			for (int index = 0; index < _table->get(arrayIndex)->count(); index++)
			{
				delete _table->get(arrayIndex)->get(index);
			}

			delete _table->get(arrayIndex);
		}

		delete _table;
		delete _list;
	}

	template<isHashable K, typename V>
	size_t simpleHashBasicCore<K, V>::calculateHashCode(const K& key) const
	{
		return hashGenerator::generateHash(key);
	}

	template<isHashable K, typename V>
	size_t simpleHashBasicCore<K, V>::calculateBucketIndex(size_t hashCode) const
	{
		return hashCode % _table->count();
	}

	template<isHashable K, typename V>
	void simpleHashBasicCore<K, V>::rehash(size_t newSize)
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

				size_t hashCode = this->calculateHashCode(pair->getKey());
				size_t newBucketIndex = hashCode % newSize;		// Find a bucket for the data

				if (newTable->get(newBucketIndex) == nullptr)
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

	template<isHashable K, typename V>
	void simpleHashBasicCore<K, V>::clear()
	{
		simpleList<K> keys = this->getKeys();

		for (int index = 0; index < keys.count(); index++)
			this->remove(keys.get(index));
	}

	template<isHashable K, typename V>
	V simpleHashBasicCore<K, V>::get(const K& key) const
	{
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index)->getKey() == key)
				return _table->get(bucketIndex)->get(index)->getValue();
		}

		throw simpleException("Key not found in hash table:  simpleHash.cpp");
	}

	template<isHashable K, typename V>
	simplePair<K, V>* simpleHashBasicCore<K, V>::getAt(int index)
	{
		return _list->get(index);
	}

	template<isHashable K, typename V>
	void simpleHashBasicCore<K, V>::add(const K& key, const V& value)
	{
		if (this->contains(key))
			throw simpleException("Trying to add duplicate value to simpleHash table. Use set(...)");

		// Check hash integrity
		//checkHash();

		// First rehash will give 100 buckets
		if (_table->count() == 0)
			rehash(100);

		// Decision to rehash:
		//
		//	1) First bucket set is 100 items (what's "big"?) (# of buckets / 10)?
		//	2) Log_10 -> An overflow of 3 = 1000 buckets. overflow of 6 = 1000000 buckets. (probably too much)
		//  3) Log_e  -> Ln -> (~6.9, 1000); (~13.8, 1000000). That's much less aggressive.
		//

		if (_maxBucketSize > simpleMath::naturalLog(_table->count()))
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

	template<isHashable K, typename V>
	void simpleHashBasicCore<K, V>::set(const K& key, const V& value)
	{
		if (!this->contains(key))
			throw simpleException("Trying to set value for a key-value pair that doesn't exist. Use add(...)");

		V oldValue = this->get(key);

		// Iterate to find the location in the ith bucket
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		// Iterate to set the item value
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index)->getValue() == oldValue)
			{
				simplePair<K, V>* pair = _table->get(bucketIndex)->get(index);

				pair->setValue(value);
				break;
			}
		}
	}

	template<isHashable K, typename V>
	bool simpleHashBasicCore<K, V>::remove(const K& key)
	{
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		simplePair<K, V>* item = nullptr;

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index)->getKey() == key)
			{
				// Check max bucket size / track
				if (_table->get(bucketIndex)->count() == _maxBucketSize)
					_maxBucketSize--;

				// Save item pointer
				item = _table->get(bucketIndex)->get(index);

				// Remove from the array
				_table->get(bucketIndex)->removeAt(index);
			}
		}

		// TODO: Use Ordered List
		for (int index = 0; index < _list->count(); index++)
		{
			if (_list->get(index)->getKey() == key)
			{
				// Remove from the parallel list
				_list->removeAt(index);
				break;
			}
		}

		if (item != nullptr)
		{
			delete item;
			return true;
		}

		return false;
	}

	template<isHashable K, typename V>
	bool simpleHashBasicCore<K, V>::contains(const K& key) const
	{
		size_t hashCode = this->calculateHashCode(key);
		size_t bucketIndex = this->calculateBucketIndex(hashCode);

		if (bucketIndex < 0 ||
			bucketIndex >= _table->count())
			return false;

		// TODO: Use Ordered List
		for (int index = 0; index < _table->get(bucketIndex)->count(); index++)
		{
			if (_table->get(bucketIndex)->get(index)->getKey() == key)
				return true;
		}

		return false;
	}

	template<isHashable K, typename V>
	int simpleHashBasicCore<K, V>::count() const
	{
		return _list->count();
	}

	template<isHashable K, typename V>
	simpleList<K> simpleHashBasicCore<K, V>::getKeys() const
	{
		return _list->select<K>([] (simplePair<K, V>* pair)
		{
			return pair->getKey();
		});
	}

	template<isHashable K, typename V>
	void simpleHashBasicCore<K, V>::iterate(const simpleHashCallback<K, V>& callback) const
	{
		_list->forEach([&callback] (simplePair<K, V>* pair)
		{
			return callback(pair->getKey(), pair->getValue());
		});
	}
}
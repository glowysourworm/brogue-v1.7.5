#pragma once

#include "simple.h"
#include "simpleException.h"
#include "simpleHashCore.h"
#include "simpleHashStdCore.h"
#include "simpleHashBasicCore.h"
#include "simpleList.h"
#include "simplePair.h"

namespace brogueHd::simple
{
	template<isHashable K, typename V>
	class simpleHash : public simpleObject
	{
	public:
		simpleHash();
		~simpleHash();

		V get(const K& key) const;
		void add(const K& key, const V& value);
		void set(const K& key, const V& value);

		simplePair<K, V>* getAt(int index);

		bool contains(const K& key) const;
		int count() const;

		bool remove(const K& key);
		void clear();

		void iterate(const simpleHashCallback<K, V>& callback) const;

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

		/// <summary>
		/// Returns a stack-copyable simpleHash table for key-value pairs except for those
		/// indicated by a truthy predicate.
		/// </summary>
		simpleHash<K, V> except(const simpleHashPredicate<K, V>& predicate) const;

		simpleList<K> getKeys() const;
		simpleList<V> getValues() const;

	public:

		size_t getHash() const override;

	private:

		simpleHashCore<K, V>* _core;

	};

	template<isHashable K, typename V>
	simpleHash<K, V>::simpleHash()
	{
		//_core = new simpleHashStdCore<K, V>();
		_core = new simpleHashBasicCore<K, V>();
	}

	template<isHashable K, typename V>
	simpleHash<K, V>::~simpleHash()
	{
		delete _core;
	}

	template<isHashable K, typename V>
	void simpleHash<K, V>::clear()
	{
		_core->clear();
	}

	template<isHashable K, typename V>
	V simpleHash<K, V>::get(const K& key) const
	{
		return _core->get(key);
	}

	template<isHashable K, typename V>
	simplePair<K, V>* simpleHash<K, V>::getAt(int index)
	{
		return _core->getAt(index);
	}

	template<isHashable K, typename V>
	void simpleHash<K, V>::add(const K& key, const V& value)
	{
		_core->add(key, value);
	}

	template<isHashable K, typename V>
	void simpleHash<K, V>::set(const K& key, const V& value)
	{
		_core->set(key, value);
	}

	template<isHashable K, typename V>
	bool simpleHash<K, V>::remove(const K& key)
	{
		return _core->remove(key);
	}

	template<isHashable K, typename V>
	bool simpleHash<K, V>::contains(const K& key) const
	{
		return _core->contains(key);
	}

	template<isHashable K, typename V>
	int simpleHash<K, V>::count() const
	{
		return _core->count();
	}

	template<isHashable K, typename V>
	void simpleHash<K, V>::iterate(const simpleHashCallback<K, V>& callback) const
	{
		_core->iterate(callback);
	}

	template<isHashable K, typename V>
	bool simpleHash<K, V>::any(const simpleHashPredicate<K, V>& predicate)
	{
		bool result = false;

		this->iterate([&result, &predicate] (K key, V value)
		{
			if (predicate(key, value))
			{
				result = true;
				return iterationCallback::breakAndReturn;
			}

			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable K, typename V>
	simpleList<simplePair<K, V>> simpleHash<K, V>::removeWhere(const simpleHashPredicate<K, V>& predicate)
	{
		simpleList<simplePair<K, V>> result;

		// Mark the remove elements
		_core->iterate([&result, &predicate] (const K& key, const V& value)
		{
			if (predicate(key, value))
			{
				result.add(simplePair<K, V>(key, value));
			}

			return iterationCallback::iterate;
		});

		// Remove the indicated elements (Check Memory related issues!)
		for (int index = 0; index < result.count(); index++)
		{
			_core->remove(result.get(index).getKey());
		}

		return result;
	}

	template<isHashable K, typename V>
	K simpleHash<K, V>::firstKey()
	{
		if (this->count() == 0)
			throw simpleException("Trying to access empty simpleHash:  simpleHash::firstKey");

		return _core->getAt(0)->getKey();
	}

	template<isHashable K, typename V>
	K simpleHash<K, V>::firstKey(const simpleHashPredicate<K, V>& predicate)
	{
		K result = default_value::value<K>();

		this->iterate([&result, &predicate] (K key, V value)
		{
			if (predicate(key, value))
			{
				result = key;
				return iterationCallback::breakAndReturn;
			}
		});

		return result;
	}

	template<isHashable K, typename V>
	V simpleHash<K, V>::firstValue()
	{
		return _core->getAt(0)->getValue();
	}

	template<isHashable K, typename V>
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

	template<isHashable K, typename V>
	void simpleHash<K, V>::forEach(const simpleHashCallback<K, V>& callback)
	{
		this->iterate([&callback] (K key, V value)
		{
			return callback(key, value);
		});
	}

	template<isHashable K, typename V>
	K simpleHash<K, V>::firstOrDefaultKey(const simpleHashPredicate<K, V>& predicate)
	{
		K result = default_value::value<K>();

		this->iterate([&result, &predicate] (K key, V value)
		{
			if (predicate(key, value))
			{
				result = key;
				return iterationCallback::breakAndReturn;
			}

			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable K, typename V>
	template<typename VResult>
	simpleList<VResult> simpleHash<K, V>::selectFromValues(const simpleHashSelector<K, V, VResult>& selector)
	{
		simpleList<VResult> result;

		this->iterate([&result, &selector] (K key, V value)
		{
			result.add(selector(value));
			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable K, typename V>
	simpleHash<K, V> simpleHash<K, V>::except(const simpleHashPredicate<K, V>& predicate) const
	{
		simpleHash<K, V> result;

		this->iterate([&result, &predicate] (const K& key, const V& value)
		{
			if (!predicate(key, value))
				result.add(key, value);

			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable K, typename V>
	simpleList<K> simpleHash<K, V>::getKeys() const
	{
		simpleList<K> result;

		this->iterate([&result] (K key, V value)
		{
			result.add(key);

			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable K, typename V>
	simpleList<V> simpleHash<K, V>::getValues() const
	{
		simpleList<V> result;

		this->iterate([&result] (K key, V value)
		{
			result.add(value);

			return iterationCallback::iterate;
		});

		return result;
	}

	template<isHashable K, typename V>
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
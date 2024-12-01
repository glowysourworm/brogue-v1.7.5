#pragma once

#include "simple.h"
#include "simpleHashCore.h"
#include "simplePair.h"
#include "simpleList.h"

#include "simpleException.h"
#include <unordered_map>

namespace brogueHd::simple
{
	template<isHashable K, typename V>
	class simpleHashStdCore : public simpleHashCore<K, V>
	{
	public:

		simpleHashStdCore();
		~simpleHashStdCore();

		V get(const K& key) const override;
		void add(const K& key, const V& value) override;
		void set(const K& key, const V& value) override;

		simplePair<K, V>* getAt(int index) override;

		bool contains(const K& key) const override;
		int count() const override;

		bool remove(const K& key) override;
		void clear() override;

	public:

		void iterate(const simpleHashCallback<K, V>& callback) const override;

	private:

		size_t calculateHashCode(const K& key) const;

	private:

		// https://en.cppreference.com/w/cpp/utility/hash
		// Key comparer:   operator ==
		// Hash function:  std::hash <-- simpleStruct, simpleObject, hashGenerator
		// 
		std::unordered_map<size_t, simplePair<K, V>*>* _map;

		// Parallel list (for index retrieval)
		simpleList<simplePair<K, V>*>* _list;
	};

	template<isHashable K, typename V>
	simpleHashStdCore<K, V>::simpleHashStdCore()
	{
		_map = new std::unordered_map<size_t, simplePair<K, V>*>();
		_list = new simpleList<simplePair<K, V>*>();
	}

	template<isHashable K, typename V>
	simpleHashStdCore<K, V>::~simpleHashStdCore()
	{
		// Delete the simplePair instances
		for (std::const_iterator iter = _map->begin(); iter != _map->end(); ++iter)
		{
			delete iter->second;
		}

		delete _map;
		delete _list;
	}

	template<isHashable K, typename V>
	size_t simpleHashStdCore<K, V>::calculateHashCode(const K& key) const
	{
		return hashGenerator::generateHash(key);
	}

	template<isHashable K, typename V>
	V simpleHashStdCore<K, V>::get(const K& key) const
	{
		size_t hash = this->calculateHashCode(key);

		if (!_map->contains(hash))
			throw simpleException("Key not found in simpleHash:  simpleHashStdCore::get");

		return _map->at(hash)->getValue();
	}

	template<isHashable K, typename V>
	void simpleHashStdCore<K, V>::add(const K& key, const V& value)
	{
		size_t hash = this->calculateHashCode(key);

		if (_map->contains(hash))
			throw simpleException("Trying to add duplicate key to simpleHash:  simpleHashStdCore::get");

		simplePair<K, V>* pair = new simplePair<K, V>(key, value);

		// (MEMORY!)
		_map->emplace(hash, pair);

		// Keep a parallel list for index lookup
		_list->add(pair);
	}

	template<isHashable K, typename V>
	void simpleHashStdCore<K, V>::set(const K& key, const V& value)
	{
		size_t hash = this->calculateHashCode(key);

		if (!_map->contains(hash))
			throw simpleException("Key not found in simpleHash:  simpleHashStdCore::set");

		_map->at(hash)->set(key, value);

		// Parallel list already has the same pointer to memory
	}

	template<isHashable K, typename V>
	simplePair<K, V>* simpleHashStdCore<K, V>::getAt(int index)
	{
		if (index < 0 || index >= _list->count())
			throw simpleException("Key not found in simpleHash:  simpleHashStdCore::getAt");

		return _list->get(index);
	}

	template<isHashable K, typename V>
	bool simpleHashStdCore<K, V>::contains(const K& key) const
	{
		size_t hash = this->calculateHashCode(key);

		return _map->contains(hash);
	}

	template<isHashable K, typename V>
	int simpleHashStdCore<K, V>::count() const
	{
		return _list->count();
	}

	template<isHashable K, typename V>
	bool simpleHashStdCore<K, V>::remove(const K& key)
	{
		size_t hash = this->calculateHashCode(key);

		if (!_map->contains(hash))
			throw simpleException("Key not found in simpleHash:  simpleHashStdCore::remove");

		simplePair<K, V>* pair = _map->at(hash);

		// Remove follower list
		_list->remove(pair);

		// "Erase" from the std map
		int success = _map->erase(hash);

		if (success != 1)
			throw simpleException("std::unordered_map::erase failed to complete successfully:  simpleHashStdCore::remove");

		return true;
	}

	template<isHashable K, typename V>
	void simpleHashStdCore<K, V>::clear()
	{
		// Go ahead and call std::unordered_map::clear
		_map->clear();

		// Now, clear our memory cache
		for (int index = 0; index < _list->count(); index++)
		{
			// (MEMORY!) simplePair instances
			delete _list->get(index);
		}

		// Clear our list out
		_list->clear();
	}

	template<isHashable K, typename V>
	void simpleHashStdCore<K, V>::iterate(const simpleHashCallback<K, V>& callback) const
	{
		bool userBreak = false;

		for (int index = 0; index < _list->count() && !userBreak; index++)
		{
			simplePair<K, V>* pair = _list->get(index);

			if (callback(pair->getKey(), pair->getValue()) == iterationCallback::breakAndReturn)
				userBreak = true;
		}
	}
}
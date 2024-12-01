#pragma once

#include "simple.h"
#include "simplePair.h"
#include "simpleException.h"
#include <functional>

namespace brogueHd::simple
{
	/// <summary>
	/// Definition of simple predicate for any key-value map
	/// </summary>
	template<isHashable K, typename V>
	using simpleHashPredicate = std::function<bool(const K& key, const V& value)>;

	/// <summary>
	/// Definition of simple predicate for any key-value map
	/// </summary>
	template<isHashable K, typename V>
	using simpleHashCallback = std::function<iterationCallback(const K& key, const V& value)>;

	/// <summary>
	/// Definition of selector for the value type
	/// </summary>
	template<isHashable K, isHashable V, typename VResult>
	using simpleHashSelector = std::function<VResult(const V& value)>;

	template<isHashable K, typename V>
	class simpleHashCore : public simpleObject
	{
	public:

		simpleHashCore();
		virtual ~simpleHashCore();

		virtual V get(const K& key) const;
		virtual void add(const K& key, const V& value);
		virtual void set(const K& key, const V& value);

		virtual simplePair<K, V>* getAt(int index);

		virtual bool contains(const K& key) const;
		virtual int count() const;

		virtual bool remove(const K& key);
		virtual void clear();

	public:  // Iterators and Queries

		virtual void iterate(const simpleHashCallback<K, V>& callback) const;
	};

	template<isHashable K, typename V>
	simpleHashCore<K, V>::simpleHashCore()
	{
	}

	template<isHashable K, typename V>
	simpleHashCore<K, V>::~simpleHashCore()
	{
	}

	template<isHashable K, typename V>
	void simpleHashCore<K, V>::clear()
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::clear");
	}

	template<isHashable K, typename V>
	V simpleHashCore<K, V>::get(const K& key) const
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::get");
	}

	template<isHashable K, typename V>
	simplePair<K, V>* simpleHashCore<K, V>::getAt(int index)
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::getAt");
	}

	template<isHashable K, typename V>
	void simpleHashCore<K, V>::add(const K& key, const V& value)
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::add");
	}

	template<isHashable K, typename V>
	void simpleHashCore<K, V>::set(const K& key, const V& value)
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::set");
	}

	template<isHashable K, typename V>
	bool simpleHashCore<K, V>::remove(const K& key)
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::remove");
	}

	template<isHashable K, typename V>
	bool simpleHashCore<K, V>::contains(const K& key) const
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::contains");
	}

	template<isHashable K, typename V>
	int simpleHashCore<K, V>::count() const
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::count");
	}

	template<isHashable K, typename V>
	void simpleHashCore<K, V>::iterate(const simpleHashCallback<K, V>& callback) const
	{
		throw simpleException("simpleHashCore functions must be overridden in a child class:  simpleHashCore::iterate");
	}
}
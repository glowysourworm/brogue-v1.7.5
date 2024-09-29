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
	};

	template<typename K, typename V>
	class simpleHash
	{
	public:
		simpleHash();
		~simpleHash();

		V operator[](K key) const;

		V get(K key);
		void add(K key, V value);

		simplePair<K,V> getAt(int index);

		bool contains(K key) const;
		int count() const;

		bool remove(K key);

		void iterate(simpleHashCallback<K, V> callback);

	private:

		long calculateHashCode(K key);
		int calculateBucketIndex(long hashCode) const;
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

	private:

		// Static Hash Table (with dynamic buckets)
		simpleArray<simpleList<simplePair<K, V>>*>* _table;

		// List follower for the primary table - for index lookup
		simpleList<simplePair<K, V>>* _list;

		// Bucket Sizes (prevents iteration of bucket lists during set(..))
		int _maxBucketSize;
	};
}
#pragma once

#include "simple.h"
#include "simpleArray.h"
#include "simpleList.h"
#include <functional>

using namespace std;

namespace brogueHd::component
{
	template<typename K>
	struct simpleHashGenerator
	{
		static typedef function<long(K key)> hashDelegate;
	};

	template<typename K, typename V>
	struct simpleHashDelegates
	{
		/// <summary>
		/// Definition of simple predicate for any key-value map
		/// </summary>
		typedef std::function<bool(K key, V value)> predicate;

		/// <summary>
		/// Definition of function to provide callback: 1) user can return iterationCallback 
		/// value to either break, or continue the loop.
		/// </summary>
		typedef std::function<iterationCallback(K key, V value)> callback;

		/// <summary>
		/// Definition of selector for the value type
		/// </summary>
		template<typename VResult>
		typedef std::function<VResult(V value)> selector;
	};

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
		simpleHash(simpleHashGenerator<K>::hashDelegate generator);
		~simpleHash();

		V operator[](K key) const;

		V get(K key);
		void add(K key, V value);

		simplePair<K,V> getAt(int index);

		bool contains(K key) const;
		int count() const;

		bool remove(K key);

		void iterate(simpleHashDelegates<K,V>::callback callback);

	private:

		int calculateBucketIndex(long hashCode) const;

		void rehash(int newSize);

	public:	// Extension Methods:  mostly queries

		bool any(simpleHashDelegates<K, V>::predicate predicate);
		void forEach(simpleHashDelegates<K, V>::callback callback);
		K firstKey(simpleHashDelegates<K, V>::predicate predicate);
		K firstOrDefaultKey(simpleHashDelegates<K, V>::predicate predicate);
		simpleList<simplePair<K, V>> removeWhere(simpleHashDelegates<K, V>::predicate predicate);
		
		template<typename VResult>
		simpleList<VResult> selectFromValues(simpleHashDelegates<K, V>::selector selector);

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

		// Generates hash codes for keys
		simpleHashGenerator<K>::hashDelegate _generator;
	};
}
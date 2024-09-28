#pragma once

#include "simple.h"
#include "simpleArray.h"
#include <functional>

using namespace std;

namespace brogueHd::component
{
	/// <summary>
	/// Definitions for most of the basic delegates in the extensions namespace
	/// </summary>
	template<typename T>
	struct simpleListDelegates
	{
	public:

		/// <summary>
		/// Definition of function to provide callback: 1) user can return iterationCallback 
		/// value to either break, or continue the loop.
		/// </summary>
		/// <param name="value">callback (current) value</param>
		typedef std::function<iterationCallback(T item)> callback;

		/// <summary>
		/// Definition of simple predicate (decision making function) for most collection types
		/// </summary>
		typedef std::function<bool(T item)> predicate;

		/// <summary>
		/// Defines a pair of items for use with a user callback
		/// </summary>
		typedef std::function<void(T item1, T item2)> pairs;
	};

	/// </summary>
	template<typename T, typename TResult>
	struct simpleListSelectorDelegates
	{
	public:

		/// <summary>
		/// Definition of selector for a value V from an item T.
		/// </summary>
		typedef std::function<TResult(T item)> selector;
	};

	template<typename T>
	class simpleList
	{
	public:
		simpleList();
		simpleList(T* anArray);
		simpleList(const simpleList<T>& copy);
		~simpleList();

		T operator[](int index);

		T get(int index) const;
		int count() const;

		void add(T item);
		void addRange(T* list);
		void addRange(const simpleList<T>& list);
		void insert(int index, T item);
		void remove(T item);
		T removeAt(int index);
		void clear();

	protected:

		int MaxIncrementalCapacity = 1000;

	public:

		// Container Selectors
		T* getArray();
		simpleArray<T> toArray();

		bool contains(T item);

		// Queries

		simpleList<T> remove(simpleListDelegates<T>::predicate predicate);
		simpleList<T> except(simpleListDelegates<T>::predicate predicate);

		T first(simpleListDelegates<T>::predicate predicate);
		bool any(simpleListDelegates<T>::predicate predicate);
		void forEach(simpleListDelegates<T>::callback callback);
		simpleList<T> where(simpleListDelegates<T>::predicate predicate);
		
		// Static Methods

		static void distinctPairs(const simpleList<T>& collection1,const simpleList<T>& collection2,simpleListDelegates<T>::pairs callback);

		// Selectors

		template<typename TResult>
		simpleList<TResult> select(simpleListSelectorDelegates<T, TResult>::selector selector);

		template<typename TResult>
		TResult max(simpleListSelectorDelegates<T, TResult>::selector selector);

		template<typename TResult>
		TResult min(simpleListSelectorDelegates<T, TResult>::selector selector);

		template<typename TResult>
		T withMin(simpleListSelectorDelegates<T, TResult>::selector selector);

		template<typename TResult>
		T withMax(simpleListSelectorDelegates<T, TResult>::selector selector);

	private:

		void reAllocate();

	private:

		T* _list;

		int _size;
		int _sizeAlloc;
	};
}
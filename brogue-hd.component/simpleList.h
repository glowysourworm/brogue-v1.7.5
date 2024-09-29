#pragma once

#include "simple.h"
#include <functional>

using namespace std;

namespace brogueHd::component
{
	/// <summary>
	/// Definition of function to provide callback: 1) user can return iterationCallback 
	/// value to either break, or continue the loop.
	/// </summary>
	/// <param name="value">callback (current) value</param>
	template<typename T>
	using simpleListCallback = std::function<iterationCallback(T item)>;

	/// <summary>
	/// Definition of simple predicate (decision making function) for most collection types
	/// </summary>
	template<typename T>
	using simpleListPredicate = std::function<bool(T item)>;

	/// <summary>
	/// Defines a pair of items for use with a user callback
	/// </summary>
	template<typename T>
	using simpleListPairDelegate = std::function<void(T item1, T item2)>;

	/// <summary>
	/// Definition of selector for a value V from an item T.
	/// </summary>
	template<typename T, typename TResult>
	using simpleListSelector = std::function<TResult(T item)>;

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

		bool contains(T item);

		// Queries

		simpleList<T> remove(simpleListPredicate<T> predicate);
		simpleList<T> except(simpleListPredicate<T> predicate);

		T first(simpleListPredicate<T> predicate);
		bool any(simpleListPredicate<T> predicate);
		void forEach(simpleListCallback<T> callback);
		simpleList<T> where(simpleListPredicate<T> predicate);
		
		// Selectors

		template<typename TResult>
		simpleList<TResult> select(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		TResult max(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		TResult min(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		T withMin(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		T withMax(simpleListSelector<T, TResult> selector);

	private:

		void reAllocate();

	private:

		T* _list;

		int _size;
		int _sizeAlloc;
	};
}
#pragma once

#include "brogueException.h"
#include "simple.h"
#include <functional>

namespace brogueHd::component
{	
	/// <summary>
	/// Definition of function to provide callback: 1) user can return iterationCallback 
	/// value to either break, or continue the loop.
	/// </summary>
	/// <param name="value">callback (current) value</param>
	template<typename T>
	using simpleArrayCallback = std::function<iterationCallback(T item)>;

	/// <summary>
	/// Definition of a simple decision predicate
	/// </summary>
	template<typename T>
	using simpleArrayPredicate = std::function<bool(T item)>;

	/// <summary>
	/// Aggregate uses a callback with the current value of iteration. Setting the return 
	/// value will forward the aggregate. (e.g. aggregate = simpleArrayAggregate(aggregate, currentValue))
	/// </summary>
	template<typename T, typename TResult>
	using simpleArrayAggregate = std::function<TResult(TResult, T)>;

	/// <summary>
	/// Selector delegate that transforms T->TResult by use of user function
	/// </summary>
	template<typename T, typename TResult>
	using simpleArraySelector = std::function<TResult(T)>;

	template<typename T>
	class simpleArray
	{
	public:

		simpleArray();
		simpleArray(int count);
		simpleArray(const T* anArray);
		simpleArray(const simpleArray<T>& copy);
		~simpleArray();

		int count() const;

		T get(int index) const;
		T* getArray() const;

		// This may be required for simpleHash->set
		//void operator=(const simpleArray<T>& other);

		void set(int index, T value);		
		bool contains(T item);

	public:

		void forEach(simpleArrayCallback<T> callback);
		bool areAll(T value);
		bool areAll(simpleArrayPredicate<T> predicate);

		template<typename TResult>
		TResult aggregate(TResult& seedValue, simpleArrayAggregate<T, TResult> aggregator);

	private:

		T* _array;
		int _count;
	};

	template<typename T>
	simpleArray<T>::simpleArray()
	{
		std::allocator<T> alloc;

		_array = alloc.allocate(1);
		_count = 1;
	}

	template<typename T>
	simpleArray<T>::simpleArray(int count)
	{
		std::allocator<T> alloc;

		_array = alloc.allocate(count);
		_count = count;
	}

	template<typename T>
	simpleArray<T>::simpleArray(const T* anArray)
	{
		if (anArray == NULL)
			brogueException::show("Trying to allocate an array with a null array:  simpleArray.h");

		// TODO: Better way to calculate sizeof

		_array = new T[sizeof(*anArray)];
		_count = sizeof(*anArray) / sizeof(anArray[0]);

		for (int index = 0; index < _count; index++)
			_array[index] = anArray[index];
	}

	template<typename T>
	simpleArray<T>::simpleArray(const simpleArray<T>& copy)
	{
		std::allocator<T> alloc;

		_array = alloc.allocate(copy.count());
		_count = copy.count();

		for (int index = 0; index < _count; index++)
			_array[index] = copy.get(index);
	}

	template<typename T>
	simpleArray<T>::~simpleArray()
	{
		delete[] _array;

		_count = 0;
	}

	template<typename T>
	int simpleArray<T>::count() const
	{
		return _count;
	}

	template<typename T>
	T simpleArray<T>::get(int index) const
	{
		return _array[index];
	}

	template<typename T>
	T* simpleArray<T>::getArray() const
	{
		return _array;
	}

	template<typename T>
	void simpleArray<T>::set(int index, T value)
	{
		_array[index] = value;
	}

	template<typename T>
	void simpleArray<T>::forEach(simpleArrayCallback<T> callback)
	{
		for (int index = 0; index < _count; index++)
		{
			if (callback(_array[index]) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<typename T>
	bool simpleArray<T>::areAll(T value)
	{
		for (int index = 0; index < _count; index++)
		{
			if (_array->get(index) != value)
				return false;
		}

		return true;
	}
	template<typename T>
	bool simpleArray<T>::areAll(simpleArrayPredicate<T> predicate)
	{
		for (int index = 0; index < _count; index++)
		{
			if (!predicate(_array->get(index)))
				return false;
		}

		return true;
	}

	template<typename T>
	bool simpleArray<T>::contains(T item)
	{
		for (int index = 0; index < _count; index++)
		{
			// Comparable T
			if (item == _array[index])
				return true;
		}

		return false;
	}

	template<typename T>
	template<typename TResult>
	TResult simpleArray<T>::aggregate(TResult& seedValue, simpleArrayAggregate<T, TResult> aggregator)
	{
		for (int index = 0; index < _count; index++)
		{
			seedValue = aggregator(seedValue, _array[index]);
		}

		return seedValue;
	}
}
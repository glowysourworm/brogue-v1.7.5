#pragma once

#include "simple.h"
#include "simpleException.h"
#include <iostream>
#include <functional>

namespace brogueHd::simple
{
	/// <summary>
	/// Definition of function to provide callback: 1) user can return iterationCallback 
	/// value to either break, or continue the loop.
	/// </summary>
	/// <param name="value">callback (current) value</param>
	template<isHashable T>
	using simpleArrayCallback = std::function<iterationCallback(T item)>;

	/// <summary>
	/// Definition of a simple decision predicate
	/// </summary>
	template<isHashable T>
	using simpleArrayPredicate = std::function<bool(T item)>;

	/// <summary>
	/// Aggregate uses a callback with the current value of iteration. Setting the return 
	/// value will forward the aggregate. (e.g. aggregate = simpleArrayAggregate(aggregate, currentValue))
	/// </summary>
	template<isHashable T, typename TResult>
	using simpleArrayAggregate = std::function<TResult(TResult, T)>;

	/// <summary>
	/// Selector delegate that transforms T->TResult by use of user function
	/// </summary>
	template<isHashable T, typename TResult>
	using simpleArraySelector = std::function<TResult(T)>;

	template<isHashable T>
	class simpleArray : public hashableObject
	{
	public:

		simpleArray();
		simpleArray(int count);
		simpleArray(const T* anArray, int anArrayLength);
		simpleArray(const simpleArray<T>& copy);
		~simpleArray();

		int count() const;

		T get(int index) const;
		void set(int index, T value);

		void operator=(const simpleArray<T>& other);

	public:

		const T* getArray();

	public:

		size_t getHash() const override;

	public:

		void forEach(simpleArrayCallback<T> callback);
		bool areAll(T value);
		bool areAllWhere(simpleArrayPredicate<T> predicate);

		bool contains(T item);

		template<typename TResult>
		TResult aggregate(TResult& seedValue, simpleArrayAggregate<T, TResult> aggregator);

	private:

		void allocate(int capacity);
		void reAllocate(const T* anArray, int anArrayLength);

	private:

		T* _array;
		int _count;
		int _countAlloc;
	};

	template<isHashable T>
	simpleArray<T>::simpleArray()
	{
		_array = nullptr;
		_count = 0;
		_countAlloc = 0;
	}

	template<isHashable T>
	simpleArray<T>::simpleArray(int count)
	{
		_count = 0;
		_array = nullptr;

		if (count > 0)
			allocate(count);
	}

	template<isHashable T>
	simpleArray<T>::simpleArray(const T* anArray, int anArrayLength)
	{
		_count = 0;
		_countAlloc = 0;
		_array = nullptr;

		reAllocate(anArray, anArrayLength);
	}

	template<isHashable T>
	simpleArray<T>::simpleArray(const simpleArray<T>& copy)
	{
		_count = 0;
		_countAlloc = 0;
		_array = nullptr;

		if (copy.count() > 0)
		{
			allocate(copy.count());

			for (int index = 0; index < copy.count(); index++)
				this->set(index, copy.get(index));
		}
	}

	template<isHashable T>
	simpleArray<T>::~simpleArray()
	{
		if (_array != nullptr)
		{
			delete[] _array;

			_array = nullptr;
			_count = 0;
			_countAlloc = 0;
		}
	}

	template<isHashable T>
	void simpleArray<T>::allocate(int capacity)
	{
		// Free old memory
		//
		if (_array != nullptr)
		{
			delete[] _array;
		}

		T* newArray = nullptr;

		// Handle isChar<T> null terminator (was adding garbage to the end (?????))
		if (isChar<T>)
		{
			newArray = new T[capacity + 1]{ default_value::value<T>() };

			_countAlloc = capacity + 1;

			// HANDLE NULL TERMINATOR RIGHT HERE! (DON'T PROPAGATE THIS ISSUE!)
			newArray[capacity] = default_value::value<T>();
		}
		else
		{
			newArray = new T[capacity]{ default_value::value<T>() };

			_countAlloc = capacity;
		}

		_array = newArray;
		_count = capacity;
	}

	template<isHashable T>
	void simpleArray<T>::reAllocate(const T* anArray, int anArrayLength)
	{
		if (anArray == nullptr)
			simpleException::showCstr("Trying to allocate from a null array:  simpleArray::reAllocate");

		// Free old memory
		//
		if (_array != nullptr)
		{
			delete[] _array;
		}

		T* newArray = nullptr;

		// Handle isChar<T> null terminator (was adding garbage to the end (?????))
		if (isChar<T>)
		{
			newArray = new T[anArrayLength + 1]{ default_value::value<T>() };

			_countAlloc = anArrayLength + 1;

			// HANDLE NULL TERMINATOR RIGHT HERE! (DON'T PROPAGATE THIS ISSUE!)
			newArray[anArrayLength] = default_value::value<T>();
		}
		else
		{
			newArray = new T[anArrayLength]{ default_value::value<T>() };

			_countAlloc = anArrayLength;
		}

		// Initialize new memory
		int index = 0;

		while (index < anArrayLength)
		{
			newArray[index] = anArray[index];
			index++;
		}

		_array = newArray;
		_count = anArrayLength;
	}

	template<isHashable T>
	int simpleArray<T>::count() const
	{
		return _count;
	}

	template<isHashable T>
	T simpleArray<T>::get(int index) const
	{
		if (index >= _count)
			simpleException::showCstr("Index is outside the bounds of the array: simpleArray.h");

		return _array[index];
	}

	template<isHashable T>
	void simpleArray<T>::set(int index, T value)
	{
		if (index >= _count)
			simpleException::showCstr("Index is outside the bounds of the array: simpleArray.h");

		_array[index] = value;
	}

	template<isHashable T>
	const T* simpleArray<T>::getArray()
	{
		return _array;
	}

	template<isHashable T>
	void simpleArray<T>::operator=(const simpleArray<T>& other)
	{
		if (_count > 0)
		{
			delete[] _array;

			_count = 0;
			_array = nullptr;
		}

		allocate(other.count());

		for (int index = 0; index < other.count(); index++)
		{
			_array[index] = other.get(index);
		}
	}

	template<isHashable T>
	size_t simpleArray<T>::getHash() const
	{
		size_t hash = 0;

		for (int index = 0; index < _count; index++)
		{
			if (hash == 0)
				hash = hashGenerator::generateHash(_array[index]);

			else
				hash = hashGenerator::combineHash(hash, _array[index]);
		}

		return hash;
	}

	template<isHashable T>
	std::ostream& operator<<(std::ostream& stream, const simpleArray<T>& other)
	{
		for (int index = 0; index < other.count(); index++)
		{
			// Rely on template overloads
			stream << other.get(index);
		}

		return stream;
	}

	template<isHashable T>
	std::istream& operator>>(std::istream& stream, simpleArray<T>& other)
	{
		// Compiler will try to build an instance of simpleArray<T> on the stack
		//
		if (other == nullptr)
			stream.setstate(std::ios::failbit);

		// Probably nothing to do. Use breakpoint to make sure data made it 
		// into the simpleArray<T>
		//
		if (true)
		{
			bool isOtherGood;
		}

		return stream;
	}

	template<isHashable T>
	void simpleArray<T>::forEach(simpleArrayCallback<T> callback)
	{
		for (int index = 0; index < _count; index++)
		{
			if (callback(_array[index]) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<isHashable T>
	bool simpleArray<T>::areAll(T value)
	{
		for (int index = 0; index < _count; index++)
		{
			if (_array[index] != value)
				return false;
		}

		return true;
	}
	template<isHashable T>
	bool simpleArray<T>::areAllWhere(simpleArrayPredicate<T> predicate)
	{
		for (int index = 0; index < _count; index++)
		{
			if (!predicate(_array[index]))
				return false;
		}

		return true;
	}

	template<isHashable T>
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

	template<isHashable T>
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
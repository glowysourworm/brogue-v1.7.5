#pragma once

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
	class simpleArray : public hashableObject
	{
	public:

		simpleArray();
		simpleArray(int count);
		simpleArray(const T* anArray);
		simpleArray(const simpleArray<T>& copy);
		~simpleArray();

		int count() const;

		T get(int index) const;

		void operator=(const simpleArray<T>& other);

		void set(int index, T value);
		bool contains(T item);

		T* getArray();

		bool operator==(const hashableObject& other) const override;
		bool operator!=(const hashableObject& other) const override;
		size_t getHash() const override;

	public:

		void forEach(simpleArrayCallback<T> callback);
		bool areAll(T value);
		bool areAllWhere(simpleArrayPredicate<T> predicate);

		template<typename TResult>
		TResult aggregate(TResult& seedValue, simpleArrayAggregate<T, TResult> aggregator);

	private:

		void allocate(size_t capacity);
		void reAllocate(const T* anArray);
		size_t getArrayCount(const T* anArray);

	private:

		T* _array;
		size_t _count;
	};

	template<typename T>
	simpleArray<T>::simpleArray()
	{
		_array = nullptr;
		_count = 0;
	}

	template<typename T>
	simpleArray<T>::simpleArray(int count)
	{
		_count = 0;
		_array = nullptr;

		if (count > 0)
			allocate(count);
	}

	template<typename T>
	simpleArray<T>::simpleArray(const T* anArray)
	{
		_count = 0;
		_array = nullptr;

		reAllocate(anArray);
	}

	template<typename T>
	simpleArray<T>::simpleArray(const simpleArray<T>& copy)
	{
		_count = 0;
		_array = nullptr;

		if (copy.count() > 0)
		{
			allocate(copy.count());

			for (int index = 0; index < copy.count(); index++)
				this->set(index, copy.get(index));
		}
	}

	template<typename T>
	simpleArray<T>::~simpleArray()
	{
		if (_array != nullptr)
		{
			delete[] _array;

			_array = nullptr;
			_count = 0;
		}
	}

	template<typename T>
	size_t simpleArray<T>::getArrayCount(const T* anArray)
	{
		size_t index = 0;
		size_t count = 0;

		while (anArray[index++] != NULL)
		{
			count++;
		}

		return count;
	}

	template<typename T>
	void simpleArray<T>::allocate(size_t capacity)
	{
		T* newArray = new T[capacity];

		// Free old memory
		//
		if (_array != nullptr)
		{
			delete[] _array;
		}

		_array = newArray;
		_count = capacity;
	}

	template<typename T>
	void simpleArray<T>::reAllocate(const T* anArray)
	{
		if (anArray == nullptr)
			simpleException::show("Trying to allocate from a null array:  simpleArray::reAllocate");

		// Get the number of actual elements in the array
		//
		size_t count = this->getArrayCount(anArray);

		T* newArray = new T[count];

		if (newArray == nullptr)
			simpleException::show("malloc returned nullptr:  Unable to allocate memory. simpleArray.h");

		// Initialize new memory
		//
		for (size_t i = 0; i < count; i++)
			newArray[i] = anArray[i];

		// Free old memory
		//
		if (_array != nullptr)
		{
			delete[] _array;
		}

		_array = newArray;
		_count = count;
	}

	template<typename T>
	int simpleArray<T>::count() const
	{
		return _count;
	}

	template<typename T>
	T simpleArray<T>::get(int index) const
	{
		if (index >= _count)
			simpleException::show("Index is outside the bounds of the array: simpleArray.h");

		return _array[index];
	}

	template<typename T>
	void simpleArray<T>::set(int index, T value)
	{
		if (index >= _count)
			simpleException::show("Index is outside the bounds of the array: simpleArray.h");

		_array[index] = value;
	}

	template<typename T>
	T* simpleArray<T>::getArray()
	{
		return _array;
	}

	template<typename T>
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

	template<typename T>
	bool simpleArray<T>::operator==(const hashableObject& other) const
	{
		simpleException::show("Invalid use of comparison operator:  simpleArray.h");

		return false;
	}

	template<typename T>
	bool simpleArray<T>::operator!=(const hashableObject& other) const
	{
		simpleException::show("Invalid use of comparison operator:  simpleArray.h");

		return false;
	}

	template<typename T>
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

	template<typename T>
	std::ostream& operator<<(std::ostream& stream, const simpleArray<T>& other)
	{
		for (int index = 0; index < other.count(); index++)
		{
			// Rely on template overloads
			stream << other.get(index);
		}

		return stream;
	}

	template<typename T>
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
			if (_array[index] != value)
				return false;
		}

		return true;
	}
	template<typename T>
	bool simpleArray<T>::areAllWhere(simpleArrayPredicate<T> predicate)
	{
		for (int index = 0; index < _count; index++)
		{
			if (!predicate(_array[index]))
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
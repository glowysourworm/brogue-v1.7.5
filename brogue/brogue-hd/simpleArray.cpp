#pragma once

#include "broguedef.h"
#include "simple.h"
#include "simpleArray.h"

namespace brogueHd::component
{
	template<typename T>
	simpleArray<T>::simpleArray(int size)
	{
		_array = new T[size];
		_size = size;
	}

	template<typename T>
	simpleArray<T>::~simpleArray()
	{
		delete [] _array;
	}

	template<typename T>
	int simpleArray<T>::size() const
	{
		return _size;
	}

	template<typename T>
	T simpleArray<T>::get(int index)
	{
		return _array[index];
	}

	template<typename T>
	T simpleArray<T>::operator[](int index) const
	{
		return _array[index];
	}

	template<typename T>
	T& simpleArray<T>::operator[](int index)
	{
		return &_array[index];
	}

	template<typename T>
	void simpleArray<T>::set(int index, T value)
	{
		_array[index] = value;
	}

	template<typename T>
	void simpleArray<T>::forEach(simpleArrayDelegates<T>::callback callback)
	{
		for (int index = 0; index < SIZEOF(collection); index++)
		{
			if (callback(_array[index]) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<typename T>
	bool simpleArray<T>::contains(T item)
	{
		for (int index = 0; index < _size; index++)
		{
			// Comparable T
			if (item == _array[index])
				return true;
		}

		return false;
	}
}
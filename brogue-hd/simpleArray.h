#pragma once

#include "simple.h"
#include "simpleList.h"
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
	using simpleArrayCallback = std::function<iterationCallback(T item)>;

	/// <summary>
	/// Definition of a simple decision predicate
	/// </summary>
	template<typename T>
	using simpleArrayPredicate = std::function<bool(T item)>;

	template<typename T>
	class simpleArray
	{
	public:

		simpleArray();
		simpleArray(int size);
		simpleArray(T* anArray);
		~simpleArray();

		int size() const;

		T get(int index);
		T operator[](int index) const;
		T& operator[](int index);

		void set(int index, T value);		
		bool contains(T item);

		void forEach(simpleArrayCallback<T> callback);
		simpleArray<T> where(simpleArrayPredicate<T> predicate);

	private:

		T* _array;
		int _size;
	};

	template<typename T>
	simpleArray<T>::simpleArray()
	{
		_array = new T[0];
		_size = 0;
	}

	template<typename T>
	simpleArray<T>::simpleArray(int size)
	{
		_array = new T[size];
		_size = size;
	}

	template<typename T>
	simpleArray<T>::simpleArray(T* anArray)
	{
		_array = new T[SIZEOF(anArray)];
		_size = SIZEOF(anArray);

		for (int index = 0; index < _size; index++)
			_array[index] = anArray[index];
	}

	template<typename T>
	simpleArray<T>::~simpleArray()
	{
		delete[] _array;
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
		return _array[index];
	}

	template<typename T>
	void simpleArray<T>::set(int index, T value)
	{
		_array[index] = value;
	}

	template<typename T>
	void simpleArray<T>::forEach(simpleArrayCallback<T> callback)
	{
		for (int index = 0; index < _size; index++)
		{
			if (callback(_array[index]) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<typename T>
	simpleArray<T> simpleArray<T>::where(simpleArrayPredicate<T> predicate)
	{
		simpleList<T> result;

		for (int index = 0; index < _size; index++)
		{
			if (predicate(_array[index]))
				result.add(_array[index]);
		}

		return simpleArray<T>(result.getArray());
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
#pragma once

#include "brogueGlobal.h"
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
		simpleArray(int count);
		simpleArray(const T* anArray);
		simpleArray(const simpleArray<T>& copy);
		~simpleArray();

		int count() const;

		T get(int index) const;

		// This may be required for simpleHash->set
		//void operator=(const simpleArray<T>& other);

		void set(int index, T value);		
		bool contains(T item);

		void forEach(simpleArrayCallback<T> callback);

	private:

		T* _array;
		int _count;
	};

	template<typename T>
	simpleArray<T>::simpleArray()
	{
		_array = new T[sizeof(T)];
		_count = 1;
	}

	template<typename T>
	simpleArray<T>::simpleArray(int count)
	{
		_array = new T[count * sizeof(T)];
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
		_array = new T[copy.count() * sizeof(T)];
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
}
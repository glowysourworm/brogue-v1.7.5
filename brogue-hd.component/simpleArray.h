#pragma once

#include "simple.h"
#include <functional>

using namespace std;

namespace brogueHd::component
{	
	/// <summary>
	/// Definitions for most of the basic delegates in the extensions namespace
	/// </summary>
	template<typename T>
	struct simpleArrayDelegates
	{
	public:

		/// <summary>
		/// Definition of function to provide callback: 1) user can return iterationCallback 
		/// value to either break, or continue the loop.
		/// </summary>
		/// <param name="value">callback (current) value</param>
		typedef std::function<iterationCallback(T item)> callback;

		/// <summary>
		/// Definition of a simple decision predicate
		/// </summary>
		typedef std::function<bool(T item)> predicate;
	};

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

		void forEach(simpleArrayDelegates<T>::callback callback);
		simpleArray<T> where(simpleArrayDelegates<T>::predicate predicate);

	private:

		T* _array;
		int _size;
	};
}
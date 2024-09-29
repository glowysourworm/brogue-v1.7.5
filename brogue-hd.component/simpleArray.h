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
}
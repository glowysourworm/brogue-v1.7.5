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
	};

	template<typename T>
	class simpleArray
	{
	public:

		simpleArray(int size);
		~simpleArray();

		int size() const;

		T get(int index);
		T operator[](int index) const override;
		T& operator[](int index) override;

		void set(int index, T value);		

		void forEach(simpleArrayDelegates<T>::callback callback);
		bool contains(T item);

	private:

		T* _array;
		int _size;
	};
}
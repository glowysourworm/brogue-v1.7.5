#pragma once

#include "simple.h"
#include "simpleList.h"

namespace brogueHd::simple
{
	template<isHashable T>
	class simpleStack : simpleObject
	{
	public:

		simpleStack();
		~simpleStack();

		void push(const T& item);
		T pop();
		T peek() const;

		int count() const;

	private:

		simpleList<T>* _list;
	};

	template<isHashable T>
	simpleStack<T>::simpleStack()
	{
		_list = new simpleList<T>();
	}

	template<isHashable T>
	simpleStack<T>::~simpleStack()
	{
		delete _list;
	}

	template<isHashable T>
	void simpleStack<T>::push(const T& item)
	{
		_list->insert(0, item);
	}

	template<isHashable T>
	T simpleStack<T>::pop()
	{
		return _list->removeAt(0);
	}

	template<isHashable T>
	T simpleStack<T>::peek() const
	{
		return _list->get(0);
	}

	template<isHashable T>
	int simpleStack<T>::count() const
	{
		return _list->count();
	}
}
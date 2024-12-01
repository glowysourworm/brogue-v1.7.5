#pragma once

#include "simple.h"
#include "simpleList.h"
#include <functional>

namespace brogueHd::simple
{
	template<isHashable T>
	using simpleQueueIterator = std::function<iterationCallback(const T& item)>;

	template<isHashable T>
	class simpleQueue : public simpleObject
	{
	public:
		
		simpleQueue();
		simpleQueue(const simpleQueue<T>& copy);
		~simpleQueue();

		void enqueue(const T& item);
		T dequeue() const;

		void count() const;

	public:

		size_t getHash() const override;

	public:

		void forEach(simpleQueueIterator<T> callback);

	private:

		simpleList<T>* _list;
	};

	template<isHashable T>
	simpleQueue<T>::simpleQueue()
	{
		_list = new simpleList<T>();
	}

	template<isHashable T>
	simpleQueue<T>::simpleQueue(const simpleQueue<T>& copy)
	{
		_list = new simpleList<T>();

		simpleList<T>* list = _list;

		copy.forEach([&list] (T item)
		{
			list->add(item);
		});
	}
	
	template<isHashable T>
	simpleQueue<T>::~simpleQueue()
	{
		delete _list;
	}

	template<isHashable T>
	void simpleQueue<T>::enqueue(const T& item)
	{
		_list->add(item);
	}

	template<isHashable T>
	T simpleQueue<T>::dequeue() const
	{
		return _list->removeAt(0);
	}

	template<isHashable T>
	void simpleQueue<T>::count() const
	{
		return _list->count();
	}

	template<isHashable T>
	size_t simpleQueue<T>::getHash() const
	{
		return _list->getHash();
	}

	template<isHashable T>
	void simpleQueue<T>::forEach(simpleQueueIterator<T> callback)
	{
		_list->forEach(callback);
	}
}
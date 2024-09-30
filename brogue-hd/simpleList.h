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
	using simpleListCallback = std::function<iterationCallback(T item)>;

	/// <summary>
	/// Definition of simple predicate (decision making function) for most collection types
	/// </summary>
	template<typename T>
	using simpleListPredicate = std::function<bool(T item)>;

	/// <summary>
	/// Defines a pair of items for use with a user callback
	/// </summary>
	template<typename T>
	using simpleListPairDelegate = std::function<void(T item1, T item2)>;

	/// <summary>
	/// Definition of selector for a value V from an item T.
	/// </summary>
	template<typename T, typename TResult>
	using simpleListSelector = std::function<TResult(T item)>;

	template<typename T>
	class simpleList
	{
	public:
		simpleList();
		simpleList(T* anArray);
		simpleList(const simpleList<T>& copy);
		~simpleList();

		T operator[](int index);

		T get(int index) const;
		int count() const;

		void add(T item);
		void addRange(T* list);
		void addRange(const simpleList<T>& list);
		void insert(int index, T item);
		void remove(T item);
		T removeAt(int index);
		void clear();

	protected:

		int MaxIncrementalCapacity = 1000;

	public:

		// Container Selectors
		T* getArray();

		bool contains(T item);

		// Queries

		simpleList<T> remove(simpleListPredicate<T> predicate);
		simpleList<T> except(simpleListPredicate<T> predicate);

		T first(simpleListPredicate<T> predicate);
		bool any(simpleListPredicate<T> predicate);
		void forEach(simpleListCallback<T> callback);
		simpleList<T> where(simpleListPredicate<T> predicate);
		
		// Selectors

		template<typename TResult>
		simpleList<TResult> select(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		TResult max(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		TResult min(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		T withMin(simpleListSelector<T, TResult> selector);

		template<typename TResult>
		T withMax(simpleListSelector<T, TResult> selector);

	private:

		void reAllocate();

	private:

		T* _list;

		int _size;
		int _sizeAlloc;
	};

	template<typename T>
	simpleList<T>::simpleList()
	{
		_list = NULL;
		_size = 0;
		_sizeAlloc = 0;
	}
	template<typename T>
	simpleList<T>::simpleList(T* anArray)
	{
		_list = NULL;
		_size = 0;
		_sizeAlloc = 0;

		for (int index = 0; index < SIZEOF(anArray); index++)
			this->add(anArray[index]);
	}
	template<typename T>
	simpleList<T>::simpleList(const simpleList<T>& copy)
	{
		_list = NULL;
		_size = 0;
		_sizeAlloc = 0;

		for (int index = 0; index < copy.count(); index++)
			this->add(copy.get(index));
	}
	template<typename T>
	simpleList<T>::~simpleList()
	{
		if (_list != NULL)
		{
			delete[] _list;

			_list = NULL;
			_size = 0;
			_sizeAlloc = 0;
		}
	}

	template<typename T>
	T simpleList<T>::operator[](int index)
	{
		return _list[index];
	}

	template<typename T>
	T simpleList<T>::get(int index) const
	{
		return _list[index];
	}

	template<typename T>
	int simpleList<T>::count() const
	{
		return _size;
	}

	template<typename T>
	void simpleList<T>::add(T item)
	{
		// Reached capacity
		//
		if (_size == _sizeAlloc)
			this->reAllocate();

		// Add the next item
		_list[_size++] = item;
	}

	template<typename T>
	void simpleList<T>::addRange(T* list)
	{
		for (int index = 0; index < SIZEOF(list); index++)
			this->add(list[index]);
	}

	template<typename T>
	void simpleList<T>::addRange(const simpleList<T>& list)
	{
		for (int index = 0; index < list.count(); index++)
			this->add(list[index]);
	}

	template<typename T>
	void simpleList<T>::insert(int insertIndex, T item)
	{
		// Check capacity before using extra "swap space"
		if (_size == _sizeAlloc)
			this->reAllocate();

		// Start at back + 1  ->  insertion index
		for (int index = _size; index >= insertIndex; index--)
		{
			_list[index] = _list[index - 1];
		}

		// Have space for the next item
		_list[insertIndex] = item;

		_size++;
	}

	template<typename T>
	void simpleList<T>::reAllocate()
	{
		if (_size != _sizeAlloc)
			brogueException::show("Trying to re-allocate memory for simple list before capacity is reached");

		// Use doubling method: Always multiply size by 2 until {MaxIncrementalCapacity} is reached
		//
		int newSize = (_sizeAlloc == 0) ? 100 :
			(_sizeAlloc >= this->MaxIncrementalCapacity) ? (_sizeAlloc + this->MaxIncrementalCapacity) :
			_sizeAlloc * 2;

		// Copy over the data
		T* newList = new T[newSize];

		for (int index = 0; index < _size; index++)
		{
			newList[index] = _list[index];
		}

		delete[] _list;

		_sizeAlloc = newSize;
	}

	template<typename T>
	T simpleList<T>::removeAt(int index)
	{
		if (index >= _size)
			brogueException::show("Index is outside the bounds of the array");

		T item = this->get(index);

		for (int i = index; i < _size - 1; i++)
		{
			_list[i] = _list[i + 1];
		}

		_size--;

		return item;
	}

	template<typename T>
	void simpleList<T>::remove(T item)
	{
		int itemIndex = -1;

		for (int index = 0; index < _size; index++)
		{
			if (item == _list[index])
			{
				itemIndex = index;
				break;
			}
		}

		if (itemIndex > -1)
			this->removeAt(itemIndex);

		else
			brogueException::show("Item not found in simpleList::remove");
	}

	template<typename T>
	void simpleList<T>::clear()
	{
		delete[] _list;

		_size = 0;
		_sizeAlloc = 0;
	}

	template<typename T>
	bool simpleList<T>::contains(T item)
	{
		for (int index = 0; index < _size; index++)
		{
			if (_list[index] == item)
				return true;
		}

		return false;
	}

	template<typename T>
	T* simpleList<T>::getArray()
	{
		return _list;
	}

	template<typename T>
	template<typename TResult>
	simpleList<TResult> simpleList<T>::select(simpleListSelector<T, TResult> selector)
	{
		simpleList<TResult> result;

		for (int index = 0; index < _size; index++)
		{
			result.add(selector(_list[index]));
		}

		return result;
	}

	template<typename T>
	simpleList<T> simpleList<T>::remove(simpleListPredicate<T> predicate)
	{
		simpleList<T> result;

		for (int index = _size - 1; index >= 0; index--)
		{
			if (predicate(_list[index]))
			{
				result.add(_list[index]);

				this->removeAt(index);
			}
		}

		return result;
	}

	template<typename T>
	simpleList<T> simpleList<T>::except(simpleListPredicate<T> predicate)
	{
		simpleList<T> result;

		for (int index = 0; index < _size; index++)
		{
			if (!predicate(_list[index]))
				result.add(_list[index]);
		}

		return result;
	}

	template<typename T>
	void simpleList<T>::forEach(simpleListCallback<T> callback)
	{
		for (int index = 0; index < _size; index++)
		{
			if (callback(_list[index]) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<typename T>
	T simpleList<T>::first(simpleListPredicate<T> predicate)
	{
		for (int index = 0; index < _size; index++)
		{
			if (predicate(_list[index]))
				return _list[index];
		}

		return NULL;
	}

	template<typename T>
	bool simpleList<T>::any(simpleListPredicate<T> predicate)
	{
		for (int index = 0; index < _size; index++)
		{
			if (predicate(_list[index]))
				return true;
		}

		return false;
	}

	template<typename T>
	simpleList<T> simpleList<T>::where(simpleListPredicate<T> predicate)
	{
		simpleList<T> result;

		for (int index = 0; index < _size; index++)
		{
			if (predicate(_list[index]))
				result.add(_list[index]);
		}

		return result;
	}

	template<typename T>
	template<typename TResult>
	TResult simpleList<T>::max(simpleListSelector<T, TResult> selector)
	{
		TResult max = NULL;
		int maxIndex = -1;

		for (int index = 0; index < _size; index++)
		{
			TResult current = selector(_list[index]);

			if (max == NULL)
			{
				max = current;
				maxIndex = index;
			}

			else if (max < current)
			{
				max = current;
				maxIndex = index;
			}
		}

		return max;
	}

	template<typename T>
	template<typename TResult>
	TResult simpleList<T>::min(simpleListSelector<T, TResult> selector)
	{
		TResult min = NULL;
		int minIndex = -1;

		for (int index = 0; index < _size; index++)
		{
			TResult current = selector(_list[index]);

			if (min == NULL)
			{
				min = current;
				minIndex = index;
			}

			else if (min > current)
			{
				min = current;
				minIndex = index;
			}
		}

		return min;
	}

	template<typename T>
	template<typename TResult>
	T simpleList<T>::withMin(simpleListSelector<T, TResult> selector)
	{
		TResult min = NULL;
		int minIndex = -1;

		for (int index = 0; index < _size; index++)
		{
			TResult value = selector(_list[index]);

			if (min == NULL)
			{
				min = value;
				minIndex = index;
			}

			else if (min > value)
			{
				min = value;
				minIndex = index;
			}
		}

		return min == NULL ? NULL : _list[minIndex];
	}

	template<typename T>
	template<typename TResult>
	T simpleList<T>::withMax(simpleListSelector<T, TResult> selector)
	{
		TResult max = NULL;
		int maxIndex = -1;

		for (int index = 0; index < _size; index++)
		{
			TResult current = selector(_list[index]);

			if (max == NULL)
			{
				max = current;
				maxIndex = index;
			}

			else if (max < current)
			{
				max = current;
				maxIndex = index;
			}
		}

		return max == NULL ? NULL : _list[maxIndex];
	}
}
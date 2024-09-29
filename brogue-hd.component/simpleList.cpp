#pragma once

#include "simpleList.h"
#include "simpleHash.h"
#include "exceptionHandler.h"

namespace brogueHd::component
{
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
			this->add(copy[index]);
	}
	template<typename T>
	simpleList<T>::~simpleList()
	{
		if (_list != NULL)
		{
			delete [] _list;

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
		delete [] _list;

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
	simpleArray<T> simpleList<T>::toArray()
	{
		return simpleArray<T>(_list);
	}

	template<typename T>
	T* simpleList<T>::getArray()
	{
		return _list;
	}
	
	template<typename T>
	template<typename TResult>
	simpleList<TResult> simpleList<T>::select(simpleListSelectorDelegates<T, TResult>::selector selector)
	{
		simpleList<TResult> result;

		for (int index = 0; index < _size; index++)
		{
			result.add(selector(_list[index]));
		}

		return result;
	}

	template<typename T>
	simpleList<T> simpleList<T>::remove(simpleListDelegates<T>::predicate predicate)
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
	simpleList<T> simpleList<T>::except(simpleListDelegates<T>::predicate predicate)
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
	void simpleList<T>::forEach(simpleListDelegates<T>::callback callback)
	{
		for (int index = 0; index < _size; index++)
		{
			if (callback(_list[index]) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<typename T>
	T simpleList<T>::first(simpleListDelegates<T>::predicate predicate)
	{
		for (int index = 0; index < _size; index++)
		{
			if (predicate(_list[index]))
				return _list[index];
		}

		return NULL;
	}

	template<typename T>
	bool simpleList<T>::any(simpleListDelegates<T>::predicate predicate)
	{
		for (int index = 0; index < _size; index++)
		{
			if (predicate(_list[index]))
				return true;
		}

		return false;
	}

	template<typename T>
	simpleList<T> simpleList<T>::where(simpleListDelegates<T>::predicate predicate)
	{
		simpleList<T> result;

		for (int index = 0; index < _size; index++)
		{
			if (predicate(_list[index]))
				return result.add(_list[index]);
		}

		return result;
	}


	template<typename T>
	void simpleList<T>::distinctPairs(const simpleList<T>& collection1, const simpleList<T>& collection2, simpleListDelegates<T>::pairs callback)
	{
		simpleHash<T, simpleHash<T, T>> lookup;

		for (int index1 = 0; index1 < collection1.count(); index1++)
		{
			for (int index2 = 0; index2 < collection2.count(); index2++)
			{
				// Ignore equal items
				if (collection1[index1] == collection2[index2])
					continue;

				// Ignore duplicate pairs (item1, item2)
				if (lookup.contains(collection1[index1]) &&
					lookup[collection1[index1]].contains(collection2[index2]))
					continue;

				// Ignore duplicate pairs (item2, item1)
				if (lookup.contains(collection2[index2]) &&
					lookup[collection2[index2]].contains(collection1[index1]))
					continue;

				else
				{
					// RESULT
					callback(collection1[index1], collection2[index2]);

					// Store lookup 1 -> 2
					if (lookup.contains(collection1[index1]))
						lookup[collection1[index1]].add(collection2[index2], collection2[index2]);

					else
					{
						simpleHash<T, T> hash;
						hash.add(collection2[index2], collection2[index2]);

						lookup.add(collection1[index1], hash );
					}

					// Store lookup 2 -> 1
					if (lookup.contains(collection2[index2]))
						lookup[collection2[index2]].add(collection1[index1], collection1[index1]);

					else
					{
						simpleHash<T, T> hash;
						hash.add(collection1[index1], collection1[index1]);

						lookup.add(collection2[index2], hash);;
					}
				}
			}
		}
	}

	template<typename T>
	template<typename TResult>
	TResult simpleList<T>::max(simpleListSelectorDelegates<T, TResult>::selector selector)
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
	TResult simpleList<T>::min(simpleListSelectorDelegates<T, TResult>::selector selector)
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
	T simpleList<T>::withMin(simpleListSelectorDelegates<T, TResult>::selector selector)
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
	T simpleList<T>::withMax(simpleListSelectorDelegates<T, TResult>::selector selector)
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
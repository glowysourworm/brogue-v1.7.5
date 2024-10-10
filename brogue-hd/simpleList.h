#pragma once

#include "simple.h"
#include "simpleException.h"
#include "simpleArray.h"
#include <functional>

namespace brogueHd::simple
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

	/// <summary>
	/// Delegate used to create a user-defined aggregate of the quantities in the list
	/// </summary>
	template<typename T, typename TResult>
	using simpleListAggregator = std::function<TResult(TResult current, T item)>;

	template<typename T>
	class simpleList : public hashableObject
	{
	public:
		simpleList();
		simpleList(const T* anArray);
		//simpleList(const simpleArray<T>& anArray);
		simpleList(const simpleList<T>& copy);
		~simpleList();

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

		int ArrayIncrement = 100;

	public:

		// Container Selectors
		simpleArray<T> toArray() const;

		bool contains(T item) const;

		// Queries

		simpleList<T> remove(simpleListPredicate<T> predicate);
		simpleList<T> except(simpleListPredicate<T> predicate) const;

		T first(simpleListPredicate<T> predicate) const;
		T first() const;
		bool any(simpleListPredicate<T> predicate) const;
		void forEach(simpleListCallback<T> callback) const;
		simpleList<T> where(simpleListPredicate<T> predicate) const;

		// Selectors

		template<typename TResult>
		simpleList<TResult> select(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		TResult maxOf(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		TResult minOf(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		TResult aggregate(TResult& seed, simpleListAggregator<T, TResult> aggregator);

		template<typename TResult>
		T withMin(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		T withMax(simpleListSelector<T, TResult> selector) const;

	public:

		bool operator==(const simpleList<T>& other);
		bool operator!=(const simpleList<T>& other);
		size_t getHash() const;

	private:

		void reAllocate();
		bool compare(const simpleList<T>& other);

	private:

		simpleArray<T>* _array;

		int _count;
	};

	template<typename T>
	simpleList<T>::simpleList()
	{
		_array = new simpleArray<T>(0);
		_count = 0;
	}
	template<typename T>
	simpleList<T>::simpleList(const T* anArray)
	{
		_array = new simpleArray<T>(anArray);
		_count = 0;
	}

	//template<typename T>
	//simpleList<T>::simpleList(const simpleArray<T>& anArray)
	//{
	//	_array = new simpleArray<T>(anArray);
	//	_count = _array->count();
	//}

	template<typename T>
	simpleList<T>::simpleList(const simpleList<T>& copy)
	{
		_array = new simpleArray<T>(copy.count());
		_count = 0;

		for (int index = 0; index < copy.count(); index++)
			this->add(copy.get(index));
	}
	template<typename T>
	simpleList<T>::~simpleList()
	{
		delete _array;

		_array = nullptr;
		_count = 0;
	}

	template<typename T>
	T simpleList<T>::get(int index) const
	{
		if (index >= _count)
			simpleException::showCstr("Index is outside the bounds of the list:  simpleList.h");

		return _array->get(index);
	}

	template<typename T>
	bool simpleList<T>::operator==(const simpleList<T>& other)
	{
		return compare(other);
	}

	template<typename T>
	bool simpleList<T>::operator!=(const simpleList<T>& other)
	{
		return !compare(other);
	}

	template<typename T>
	size_t simpleList<T>::getHash() const
	{
		size_t hash = 0;

		for (int index = 0; index < _count; index++)
		{
			if (hash == 0)
				hash = hashGenerator::generateHash(this->get(index));

			else
				hash = hashGenerator::combineHash(hash, this->get(index));
		}

		return hash;
	}

	template<typename T>
	int simpleList<T>::count() const
	{
		return _count;
	}

	template<typename T>
	void simpleList<T>::add(T item)
	{
		// Reached capacity
		//
		if (_count == _array->count())
			this->reAllocate();

		// Add the next item
		_array->set(_count++, item);
	}

	template<typename T>
	void simpleList<T>::addRange(T* list)
	{
		int count = sizeof(list) / sizeof(T);

		for (int index = 0; index < count; index++)
			this->add(list[index]);
	}

	template<typename T>
	void simpleList<T>::addRange(const simpleList<T>& list)
	{
		for (int index = 0; index < list.count(); index++)
			this->add(list.get(index));
	}

	template<typename T>
	void simpleList<T>::insert(int insertIndex, T item)
	{
		// Check capacity before using extra "swap space"
		if (_count == _array->count())
			this->reAllocate();

		// Start at back + 1  ->  insertion index
		for (int index = _count; index >= insertIndex; index--)
		{
			_array->set(index, _array->get(index - 1));
		}

		// Have space for the next item
		_array->set(insertIndex, item);

		_count++;
	}

	template<typename T>
	void simpleList<T>::reAllocate()
	{
		if (_count != _array->count())
			simpleException::showCstr("Trying to re-allocate memory for simple list before capacity is reached");

		// Use doubling method: Always multiply size by 2 until {MaxElementIncrement} is reached
		//
		int newSize = (_array->count() == 0) ? 10 : (_array->count() >= this->ArrayIncrement) ? 
													(_array->count() + this->ArrayIncrement) :
													 _array->count() * 2;

		// Copy over the data
		simpleArray<T>* newArray = new simpleArray<T>(newSize);

		for (int index = 0; index < _array->count(); index++)
		{
			newArray->set(index, _array->get(index));
		}

		delete _array;

		_array = newArray;
	}

	template<typename T>
	bool simpleList<T>::compare(const simpleList<T>& other)
	{
		if (other == nullptr)
			return false;

		else if (_count != other.count())
			return false;

		for (int index = 0; index < _count; index++)
		{
			if (this->get(index) != other.get(index))
				return false;
		}

		return true;


	}

	template<typename T>
	T simpleList<T>::removeAt(int index)
	{
		if (index >= _count)
			simpleException::showCstr("Index is outside the bounds of the array");

		T item = this->get(index);

		for (int i = index; i < _count - 1; i++)
		{
			_array->set(i, _array->get(i + 1));
		}

		_count--;

		return item;
	}

	template<typename T>
	void simpleList<T>::remove(T item)
	{
		int itemIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			if (item == _array->get(index))
			{
				itemIndex = index;
				break;
			}
		}

		if (itemIndex > -1)
			this->removeAt(itemIndex);

		else
			simpleException::showCstr("Item not found in simpleList::remove");
	}

	template<typename T>
	void simpleList<T>::clear()
	{
		// Soft Delete
		//
		_count = 0;
	}

	template<typename T>
	bool simpleList<T>::contains(T item) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (_array->get(index) == item)
				return true;
		}

		return false;
	}

	template<typename T>
	simpleArray<T> simpleList<T>::toArray() const
	{
		simpleArray<T> result(_count);

		for (int index = 0; index < _count; index++)
			result.set(index, this->get(index));

		return result;
	}

	template<typename T>
	template<typename TResult>
	simpleList<TResult> simpleList<T>::select(simpleListSelector<T, TResult> selector) const
	{
		simpleList<TResult> result;

		for (int index = 0; index < _count; index++)
		{
			result.add(selector(_array->get(index)));
		}

		return result;
	}

	template<typename T>
	simpleList<T> simpleList<T>::remove(simpleListPredicate<T> predicate)
	{
		simpleList<T> result;

		for (int index = _count - 1; index >= 0; index--)
		{
			if (predicate(_array->get(index)))
			{
				result.add(_array->get(index));

				this->removeAt(index);
			}
		}

		return result;
	}

	template<typename T>
	simpleList<T> simpleList<T>::except(simpleListPredicate<T> predicate) const
	{
		simpleList<T> result;

		for (int index = 0; index < _count; index++)
		{
			if (!predicate(_array->get(index)))
				result.add(_array->get(index));
		}

		return result;
	}

	template<typename T>
	void simpleList<T>::forEach(simpleListCallback<T> callback) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (callback(_array->get(index)) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<typename T>
	T simpleList<T>::first(simpleListPredicate<T> predicate) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (predicate(_array->get(index)))
				return _array->get(index);
		}

		return default_value<T>::value;
	}

	template<typename T>
	T simpleList<T>::first() const
	{
		if (this->count() == 0)
			simpleException::showCstr("Trying to get element from an empty list:  simpleList::first()");

		return this->get(0);
	}

	template<typename T>
	bool simpleList<T>::any(simpleListPredicate<T> predicate) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (predicate(_array->get(index)))
				return true;
		}

		return false;
	}

	template<typename T>
	simpleList<T> simpleList<T>::where(simpleListPredicate<T> predicate) const
	{
		simpleList<T> result;

		for (int index = 0; index < _count; index++)
		{
			if (predicate(_array->get(index)))
				result.add(_array->get(index));
		}

		return result;
	}

	template<typename T>
	template<typename TResult>
	TResult simpleList<T>::maxOf(simpleListSelector<T, TResult> selector) const
	{
		TResult max = default_value<T>::value;
		int maxIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(_array->get(index));

			if (max == default_value<T>::value)
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
	TResult simpleList<T>::minOf(simpleListSelector<T, TResult> selector) const
	{
		TResult min = default_value<T>::value;
		int minIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(_array->get(index));

			if (min == default_value<T>::value)
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
	TResult simpleList<T>::aggregate(TResult& seed, simpleListAggregator<T, TResult> aggregator)
	{
		for (int index = 0; index < _count; index++)
		{
			seed = aggregator(seed, _array->get(index));
		}

		return seed;
	}

	template<typename T>
	template<typename TResult>
	T simpleList<T>::withMin(simpleListSelector<T, TResult> selector) const
	{
		TResult min = default_value<T>::value;
		int minIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult value = selector(_array->get(index));

			if (min == default_value<T>::value)
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

		return min == default_value<T>::value ? default_value<T>::value : _array->get(minIndex);
	}

	template<typename T>
	template<typename TResult>
	T simpleList<T>::withMax(simpleListSelector<T, TResult> selector) const
	{
		TResult max = default_value<TResult>::value;
		int maxIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(_array->get(index));

			if (max == default_value<TResult>::value)
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

		if (max == default_value<TResult>::value)
			return default_value<T>::ctor();

		else
			return _array->get(maxIndex);
	}
}
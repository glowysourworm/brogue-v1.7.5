#pragma once

#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleMath.h"
#include <functional>

namespace brogueHd::simple
{
	/// <summary>
	/// Definition of function to provide callback: 1) user can return iterationCallback 
	/// value to either break, or continue the loop.
	/// </summary>
	/// <param name="value">callback (current) value</param>
	template<isHashable T>
	using simpleListCallback = std::function<iterationCallback(const T& item)>;

	/// <summary>
	/// Definition of simple predicate (decision making function) for most collection types
	/// </summary>
	template<isHashable T>
	using simpleListPredicate = std::function<bool(const T& item)>;

	/// <summary>
	/// Defines a pair of items for use with a user callback
	/// </summary>
	template<isHashable T>
	using simpleListPairDelegate = std::function<void(const T& item1, const T& item2)>;

	/// <summary>
	/// Definition of selector for a value V from an item T.
	/// </summary>
	template<isHashable T, typename TResult>
	using simpleListSelector = std::function<TResult(const T& item)>;

	/// <summary>
	/// Delegate used to create a user-defined aggregate of the quantities in the list
	/// </summary>
	template<isHashable T, typename TResult>
	using simpleListAggregator = std::function<TResult(const TResult& current, const T& item)>;

	/// <summary>
	/// Delegate that is used to compare two items
	/// </summary>
	template<isHashable T>
	using simpleListComparer = std::function<int(const T& item1, const T& item2)>;

	template<isHashable T>
	class simpleList : public simpleObject
	{
	public:
		simpleList();
		simpleList(const T* anArray, int arrayLength);
		simpleList(const simpleArray<T>& anArray);
		simpleList(const simpleList<T>& copy);
		~simpleList();

		void operator=(const simpleList<T>& other);

		T get(int index) const;
		int count() const;
		void set(int index, const T& item);
		void add(const T& item);
		void addRange(const T* list, int listLength);
		void addRange(const simpleList<T>& list);
		void insert(int index, const T& item);
		void remove(const T& item);
		T removeAt(int index);
		void clear();

	protected:

		int ArrayIncrement = 100;

	public:

		// Container Selectors
		simpleArray<T> toArray() const;

		bool contains(const T& item) const;

		// Queries

		simpleList<T> remove(const simpleListPredicate<T>& predicate);
		simpleList<T> except(const simpleListPredicate<T>& predicate) const;

		T first(const simpleListPredicate<T>& predicate) const;
		T first() const;
		bool any(const simpleListPredicate<T>& predicate) const;
		void forEach(const simpleListCallback<T>& callback) const;
		simpleList<T> where(const simpleListPredicate<T>& predicate) const;
		T firstOrDefault(const simpleListPredicate<T>& predicate) const;

		// Selectors

		template<typename TResult>
		simpleList<TResult> select(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		TResult maxOf(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		TResult minOf(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		TResult aggregate(TResult& seed, const simpleListAggregator<T, TResult>& aggregator) const;

		template<typename TResult>
		T withMin(simpleListSelector<T, TResult> selector) const;

		template<typename TResult>
		T withMax(simpleListSelector<T, TResult> selector) const;

		int count(simpleListPredicate<T> predicate) const;

		simpleList<T> sort(simpleListComparer<T> comparer) const;

	public:

		size_t getHash() const override;

	private:

		void reAllocate();
		bool compare(const simpleList<T>& other);

	private:

		simpleArray<T>* _array;

		int _count;
	};

	template<isHashable T>
	simpleList<T>::simpleList()
	{
		_array = new simpleArray<T>(0);
		_count = 0;
	}
	template<isHashable T>
	simpleList<T>::simpleList(const T* anArray, int arrayLength)
	{
		_array = new simpleArray<T>(anArray, arrayLength);
		_count = 0;
	}

	template<isHashable T>
	simpleList<T>::simpleList(const simpleArray<T>& anArray)
	{
		_array = new simpleArray<T>(anArray);
		_count = _array->count();
	}

	template<isHashable T>
	simpleList<T>::simpleList(const simpleList<T>& copy)
	{
		_array = new simpleArray<T>(copy.count());
		_count = 0;

		for (int index = 0; index < copy.count(); index++)
			this->add(copy.get(index));
	}
	template<isHashable T>
	simpleList<T>::~simpleList()
	{
		delete _array;

		_array = nullptr;
		_count = 0;
	}

	template<isHashable T>
	void simpleList<T>::operator=(const simpleList<T>& other)
	{
		delete _array;

		_array = new simpleArray<T>(other.count());
		_count = _array->count();

		for (int index = 0; index < _count; index++)
			_array->set(index, other.get(index));
	}


	template<isHashable T>
	T simpleList<T>::get(int index) const
	{
		if (index >= _count)
			throw simpleException("Index is outside the bounds of the list:  simpleList.h");

		return _array->get(index);
	}

	template<isHashable T>
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

	template<isHashable T>
	int simpleList<T>::count() const
	{
		return _count;
	}

	template<isHashable T>
	void simpleList<T>::set(int index, const T& item)
	{
		_array->set(index, item);
	}

	template<isHashable T>
	void simpleList<T>::add(const T& item)
	{
		// Reached capacity
		//
		if (_count == _array->count())
			this->reAllocate();

		// Add the next item
		_array->set(_count++, item);
	}

	template<isHashable T>
	void simpleList<T>::addRange(const T* list, int listLength)
	{
		for (int index = 0; index < listLength; index++)
			this->add(list[index]);
	}

	template<isHashable T>
	void simpleList<T>::addRange(const simpleList<T>& list)
	{
		for (int index = 0; index < list.count(); index++)
			this->add(list.get(index));
	}

	template<isHashable T>
	void simpleList<T>::insert(int insertIndex, const T& item)
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

	template<isHashable T>
	void simpleList<T>::reAllocate()
	{
		if (_count != _array->count())
			throw simpleException("Trying to re-allocate memory for simple list before capacity is reached");

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

	template<isHashable T>
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

	template<isHashable T>
	T simpleList<T>::removeAt(int index)
	{
		if (index >= _count)
			throw simpleException("Index is outside the bounds of the array");

		T item = this->get(index);

		for (int i = index; i < _count - 1; i++)
		{
			_array->set(i, _array->get(i + 1));
		}

		_count--;

		return item;
	}

	template<isHashable T>
	void simpleList<T>::remove(const T& item)
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
			throw simpleException("Item not found in simpleList::remove");
	}

	template<isHashable T>
	void simpleList<T>::clear()
	{
		// Soft Delete
		//
		_count = 0;
	}

	template<isHashable T>
	bool simpleList<T>::contains(const T& item) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (_array->get(index) == item)
				return true;
		}

		return false;
	}

	template<isHashable T>
	simpleArray<T> simpleList<T>::toArray() const
	{
		simpleArray<T> result(_count);

		for (int index = 0; index < _count; index++)
			result.set(index, this->get(index));

		return result;
	}

	template<isHashable T>
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

	template<isHashable T>
	simpleList<T> simpleList<T>::remove(const simpleListPredicate<T>& predicate)
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

	template<isHashable T>
	simpleList<T> simpleList<T>::except(const simpleListPredicate<T>& predicate) const
	{
		simpleList<T> result;

		for (int index = 0; index < _count; index++)
		{
			if (!predicate(_array->get(index)))
				result.add(_array->get(index));
		}

		return result;
	}

	template<isHashable T>
	void simpleList<T>::forEach(const simpleListCallback<T>& callback) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (callback(_array->get(index)) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<isHashable T>
	T simpleList<T>::firstOrDefault(const simpleListPredicate<T>& predicate) const
	{
		T defaultValue = default_value::value<T>();

		for (int index = 0; index < _count; index++)
		{
			if (_array->get(index) != defaultValue)
			{
				if (predicate(_array->get(index)))
					return _array->get(index);
			}
		}
	}

	template<isHashable T>
	T simpleList<T>::first(const simpleListPredicate<T>& predicate) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (predicate(_array->get(index)))
				return _array->get(index);
		}

		return default_value::value<T>();
	}

	template<isHashable T>
	T simpleList<T>::first() const
	{
		if (this->count() == 0)
			throw simpleException("Trying to get element from an empty list:  simpleList::first()");

		return this->get(0);
	}

	template<isHashable T>
	bool simpleList<T>::any(const simpleListPredicate<T>& predicate) const
	{
		for (int index = 0; index < _count; index++)
		{
			if (predicate(_array->get(index)))
				return true;
		}

		return false;
	}

	template<isHashable T>
	simpleList<T> simpleList<T>::where(const simpleListPredicate<T>& predicate) const
	{
		simpleList<T> result;

		for (int index = 0; index < _count; index++)
		{
			if (predicate(_array->get(index)))
				result.add(_array->get(index));
		}

		return result;
	}

	template<isHashable T>
	simpleList<T> simpleList<T>::sort(simpleListComparer<T> comparer) const
	{
		simpleList<T> result;

		// Binary Insert
		//
		for (int index = 0; index < this->count(); index++)
		{
			/*
				function binary_search(A, n, T) is
					L := 0
					R := n − 1
					while L ≤ R do
						m := floor((L + R) / 2)
						if A[m] < T then
							L := m + 1
						else if A[m] > T then
							R := m − 1
						else:
							return m
					return unsuccessful
			 */

			int leftIndex = 0;
			int rightIndex = result.count() - 1;

			// Initialize insert index to be the left index
			int insertIndex = leftIndex;

			T searchItem = this->get(index);

			while (leftIndex <= rightIndex)
			{
				int middleIndex = (int)simpleMath::floor((leftIndex + rightIndex) / 2.0f);
				T item = result.get(middleIndex);

				// Set insert index
				insertIndex = middleIndex;

				// Item's value is LESS THAN search value
				if (comparer(item, searchItem) < 0)
				{
					leftIndex = middleIndex + 1;

					// Set insert index for catching final iteration
					insertIndex = leftIndex;
				}

				// GREATER THAN
				else if (comparer(item, searchItem) > 0)
					rightIndex = middleIndex - 1;

				else
				{
					insertIndex = middleIndex;
					break;
				}
			}

			if (insertIndex >= 0 && insertIndex < result.count())
				result.insert(insertIndex, searchItem);

			else
				result.add(searchItem);
		}

		return result;
	}

	template<isHashable T>
	template<typename TResult>
	TResult simpleList<T>::maxOf(simpleListSelector<T, TResult> selector) const
	{
		TResult max = default_value::value<T>();
		int maxIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(_array->get(index));

			if (max == default_value::value<T>())
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

	template<isHashable T>
	template<typename TResult>
	TResult simpleList<T>::minOf(simpleListSelector<T, TResult> selector) const
	{
		TResult min = default_value::value<T>();
		int minIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(_array->get(index));

			if (min == default_value::value<T>())
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

	template<isHashable T>
	int simpleList<T>::count(simpleListPredicate<T> predicate) const
	{
		int result = 0;

		for (int index = 0; index < this->count(); index++)
		{
			if (predicate(this->get(index)))
				result++;
		}

		return result;
	}

	template<isHashable T>
	template<typename TResult>
	TResult simpleList<T>::aggregate(TResult& seed, const simpleListAggregator<T, TResult>& aggregator) const
	{
		for (int index = 0; index < this->count(); index++)
		{
			seed = aggregator(seed, this->get(index));
		}

		return seed;
	}

	template<isHashable T>
	template<typename TResult>
	T simpleList<T>::withMin(simpleListSelector<T, TResult> selector) const
	{
		TResult min = default_value::value<T>();
		int minIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult value = selector(_array->get(index));

			if (min == default_value::value<T>())
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

		return min == default_value::value<T>() ? default_value::value<T>() : _array->get(minIndex);
	}

	template<isHashable T>
	template<typename TResult>
	T simpleList<T>::withMax(simpleListSelector<T, TResult> selector) const
	{
		TResult max = default_value::value<TResult>();
		int maxIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(_array->get(index));

			if (max == default_value::value<TResult>())
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

		if (max == default_value::value<TResult>())
			return default_value::value<T>();

		else
			return _array->get(maxIndex);
	}
}
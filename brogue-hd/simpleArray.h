#pragma once

#include "simple.h"
#include "simpleException.h"
#include "simpleRange.h"
#include <functional>
#include <iosfwd>
#include <limits>

namespace brogueHd::simple
{
	/// <summary>
	/// Definition of function to provide callback: 1) user can return iterationCallback 
	/// value to either break, or continue the loop.
	/// </summary>
	/// <param name="value">callback (current) value</param>
	template<isHashable T>
	using simpleArrayCallback = std::function<iterationCallback(const T& item)>;

	/// <summary>
	/// Definition of a simple decision predicate
	/// </summary>
	template<isHashable T>
	using simpleArrayPredicate = std::function<bool(const T& item)>;

	/// <summary>
	/// Aggregate uses a callback with the current value of iteration. Setting the return 
	/// value will forward the aggregate. (e.g. aggregate = simpleArrayAggregate(aggregate, currentValue))
	/// </summary>
	template<isHashable T, typename TResult>
	using simpleArrayAggregate = std::function<TResult(const TResult& aggregator, const T& current)>;

	/// <summary>
	/// Delegate used to compare two array items
	/// </summary>
	template<isHashable T>
	using simpleArrayComparer = std::function<int(const T& item1, const T& item2)>;

	/// <summary>
	/// Selector delegate that transforms T->TResult by use of user function
	/// </summary>
	template<isHashable T, typename TResult>
	using simpleArraySelector = std::function<TResult(const T& item)>;

	/// <summary>
	/// Assigns a number value to any item of the array
	/// </summary>
	template<isHashable T, isNumber TResult>
	using simpleArrayValueIndicator = std::function<TResult(const T& item)>;

	template<isHashable T>
	class simpleArray : public simpleObject
	{
	public:

		simpleArray();
		simpleArray(int count);
		simpleArray(const T* anArray, int anArrayLength);
		simpleArray(const simpleArray<T>& copy);
		~simpleArray();

		int count() const;

		T get(int index) const;
		void set(int index, const T& value);
		void setAll(const T& value);

		void operator=(const simpleArray<T>& other);

		bool operator==(const simpleArray<T>& other);
		bool operator!=(const simpleArray<T>& other);

	public:

		const T* getArray();

	public:

		size_t getHash() const override;

	public:

		void forEach(simpleArrayCallback<T> callback);
		void iterate(simpleArrayCallback<T> callback);
		bool areAll(const T& value);
		bool areAllWhere(simpleArrayPredicate<T> predicate);

		bool contains(const T& item);
		bool any(simpleArrayPredicate<T> predicate);

		template<typename TResult>
		TResult aggregate(TResult& seedValue, simpleArrayAggregate<T, TResult> aggregator);

		template<isHashable TResult>
		simpleArray<TResult> select(simpleArraySelector<T, TResult> selector);

		template<isHashable TAggregate>
		T minWith(TAggregate& seedValue, simpleArrayAggregate<T, TAggregate> aggregator);

		template<typename TResult>
		TResult maxOf(simpleArraySelector<T, TResult> selector) const;

		template<typename TResult>
		int maxIndexOf(simpleArraySelector<T, TResult> selector) const;

		simpleArray<T> whereArray(simpleArrayPredicate<T> predicate);

		template<isNumber TResult>
		TResult kadanesAlgorithm(simpleRange<int>& result, simpleArrayValueIndicator<T, TResult> valueIndicator);

	private:

		void allocate(int capacity);
		void reAllocate(const T* anArray, int anArrayLength);

	private:

		T* _array;
		int _count;
		int _countAlloc;
	};

	template<isHashable T>
	simpleArray<T>::simpleArray()
	{
		_array = nullptr;
		_count = 0;
		_countAlloc = 0;
	}

	template<isHashable T>
	simpleArray<T>::simpleArray(int count)
	{
		_count = 0;
		_array = nullptr;

		if (count > 0)
			allocate(count);
	}

	template<isHashable T>
	simpleArray<T>::simpleArray(const T* anArray, int anArrayLength)
	{
		_count = 0;
		_countAlloc = 0;
		_array = nullptr;

		reAllocate(anArray, anArrayLength);
	}

	template<isHashable T>
	simpleArray<T>::simpleArray(const simpleArray<T>& copy)
	{
		_count = 0;
		_countAlloc = 0;
		_array = nullptr;

		if (copy.count() > 0)
		{
			allocate(copy.count());

			for (int index = 0; index < copy.count(); index++)
				this->set(index, copy.get(index));
		}
	}

	template<isHashable T>
	simpleArray<T>::~simpleArray()
	{
		if (_array != nullptr)
		{
			delete[] _array;

			_array = nullptr;
			_count = 0;
			_countAlloc = 0;
		}
	}

	template<isHashable T>
	void simpleArray<T>::allocate(int capacity)
	{
		// Free old memory
		//
		if (_array != nullptr)
		{
			delete[] _array;
		}

		T* newArray = nullptr;

		// Handle isChar<T> null terminator (was adding garbage to the end (?????))
		if (isChar<T>)
		{
			newArray = new T[capacity + 1]{ default_value::value<T>() };

			_countAlloc = capacity + 1;

			// HANDLE NULL TERMINATOR RIGHT HERE! (DON'T PROPAGATE THIS ISSUE!)
			newArray[capacity] = default_value::value<T>();
		}
		else
		{
			newArray = new T[capacity]{ default_value::value<T>() };

			_countAlloc = capacity;
		}

		_array = newArray;
		_count = capacity;
	}

	template<isHashable T>
	void simpleArray<T>::reAllocate(const T* anArray, int anArrayLength)
	{
		if (anArray == nullptr)
			throw simpleException("Trying to allocate from a null array:  simpleArray::reAllocate");

		// Free old memory
		//
		if (_array != nullptr)
		{
			delete[] _array;
		}

		T* newArray = nullptr;

		// Handle isChar<T> null terminator (was adding garbage to the end (?????))
		if (isChar<T>)
		{
			newArray = new T[anArrayLength + 1]{ default_value::value<T>() };

			_countAlloc = anArrayLength + 1;

			// HANDLE NULL TERMINATOR RIGHT HERE! (DON'T PROPAGATE THIS ISSUE!)
			newArray[anArrayLength] = default_value::value<T>();
		}
		else
		{
			newArray = new T[anArrayLength]{ default_value::value<T>() };

			_countAlloc = anArrayLength;
		}

		// Initialize new memory
		int index = 0;

		while (index < anArrayLength)
		{
			newArray[index] = anArray[index];
			index++;
		}

		_array = newArray;
		_count = anArrayLength;
	}

	template<isHashable T>
	int simpleArray<T>::count() const
	{
		return _count;
	}

	template<isHashable T>
	T simpleArray<T>::get(int index) const
	{
		if (index >= _count)
			throw simpleException("Index is outside the bounds of the array: simpleArray.h");

		return _array[index];
	}

	template<isHashable T>
	void simpleArray<T>::set(int index, const T& value)
	{
		if (index >= _count)
			throw simpleException("Index is outside the bounds of the array: simpleArray.h");

		_array[index] = value;
	}

	template<isHashable T>
	void simpleArray<T>::setAll(const T& value)
	{
		for (int index = 0; index < _count; index++)
			_array[index] = value;
	}

	template<isHashable T>
	const T* simpleArray<T>::getArray()
	{
		return _array;
	}

	template<isHashable T>
	void simpleArray<T>::operator=(const simpleArray<T>& other)
	{
		if (_count > 0)
		{
			delete[] _array;

			_count = 0;
			_array = nullptr;
		}

		allocate(other.count());

		for (int index = 0; index < other.count(); index++)
		{
			_array[index] = other.get(index);
		}
	}

	template<isHashable T>
	bool simpleArray<T>::operator==(const simpleArray<T>& other)
	{
		if (_count != other.count())
			return false;

		for (int index = 0; index < _count; index++)
		{
			if (this->get(index) != other.get(index))
				return false;
		}

		return true;
	}

	template<isHashable T>
	bool simpleArray<T>::operator!=(const simpleArray<T>& other)
	{
		if (_count != other.count())
			return true;

		for (int index = 0; index < _count; index++)
		{
			if (this->get(index) != other.get(index))
				return true;
		}

		return false;
	}

	template<isHashable T>
	size_t simpleArray<T>::getHash() const
	{
		// Take hash of the pointer value (could easily just take the pointer itself!)
		size_t hash = 0;

		for (int index = 0; index < this->count(); index++)
		{
			size_t lastHash = hash;

			if (index == 0)
				hash = hashGenerator::generateHash(this->get(index));

			else
				hash = hashGenerator::combineHash(hash, this->get(index));

			if (hash == lastHash)
				throw simpleException("Hash zero-value detected (this may cause a hash collision):  simpleArray.h");
		}

		return hash;
	}

	template<isHashable T>
	std::ostream& operator<<(std::ostream& stream, const simpleArray<T>& other)
	{
		for (int index = 0; index < other.count(); index++)
		{
			// Rely on template overloads
			stream << other.get(index);
		}

		return stream;
	}

	template<isHashable T>
	std::istream& operator>>(std::istream& stream, simpleArray<T>& other)
	{
		// Compiler will try to build an instance of simpleArray<T> on the stack
		//
		if (other == nullptr)
			stream.setstate(std::ios::failbit);

		// Probably nothing to do. Use breakpoint to make sure data made it 
		// into the simpleArray<T>
		//
		if (true)
		{
			bool isOtherGood;
		}

		return stream;
	}

	template<isHashable T>
	void simpleArray<T>::forEach(simpleArrayCallback<T> callback)
	{
		// Probably will go with iterate (naming issue)
		iterate(callback);
	}

	template<isHashable T>
	void simpleArray<T>::iterate(simpleArrayCallback<T> callback)
	{
		for (int index = 0; index < _count; index++)
		{
			if (callback(_array[index]) == iterationCallback::breakAndReturn)
				return;
		}
	}

	template<isHashable T>
	bool simpleArray<T>::areAll(const T& value)
	{
		for (int index = 0; index < _count; index++)
		{
			if (_array[index] != value)
				return false;
		}

		return true;
	}
	template<isHashable T>
	bool simpleArray<T>::areAllWhere(simpleArrayPredicate<T> predicate)
	{
		for (int index = 0; index < _count; index++)
		{
			if (!predicate(_array[index]))
				return false;
		}

		return true;
	}

	template<isHashable T>
	bool simpleArray<T>::contains(const T& item)
	{
		for (int index = 0; index < _count; index++)
		{
			// Comparable T
			if (item == _array[index])
				return true;
		}

		return false;
	}


	template<isHashable T>
	bool simpleArray<T>::any(simpleArrayPredicate<T> predicate)
	{
		for (int index = 0; index < _count; index++)
		{
			if (predicate(_array[index]))
				return true;
		}

		return false;
	}

	template<isHashable T>
	template<typename TResult>
	TResult simpleArray<T>::aggregate(TResult& seedValue, simpleArrayAggregate<T, TResult> aggregator)
	{
		for (int index = 0; index < _count; index++)
		{
			seedValue = aggregator(seedValue, _array[index]);
		}

		return seedValue;
	}

	template<isHashable T>
	simpleArray<T> simpleArray<T>::whereArray(simpleArrayPredicate<T> predicate)
	{
		int count = 0;

		// Count first
		for (int index = 0; index < this->count(); index++)
		{
			if (predicate(this->get(index)))
				count++;
		}

		// Allocate the new array
		simpleArray<T> result(count);

		count = 0;

		for (int index = 0; index < this->count(); index++)
		{
			if (predicate(this->get(index)))
				result.set(count++, this->get(index));
		}

		return result;
	}

	template<isHashable T>
	template<isHashable TResult>
	simpleArray<TResult> simpleArray<T>::select(simpleArraySelector<T, TResult> selector)
	{
		simpleArray<TResult> result(this->count());

		for (int index = 0; index < this->count(); index++)
		{
			result.set(index, selector(this->get(index)));
		}

		return result;
	}

	template<isHashable T>
	template<isHashable TAggregate>
	T simpleArray<T>::minWith(TAggregate& seedValue, simpleArrayAggregate<T, TAggregate> aggregator)
	{
		T result = default_value::value<T>();

		for (int index = 0; index < this->count(); index++)
		{
			TAggregate next = aggregator(seedValue, this->get(index));

			if (next < seedValue)
			{
				seedValue = next;
				result = this->get(index);
			}
		}

		return result;
	}

	template<isHashable T>
	template<typename TResult>
	TResult simpleArray<T>::maxOf(simpleArraySelector<T, TResult> selector) const
	{
		TResult max = default_value::value<T>();
		int maxIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(this->get(index));

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
	int simpleArray<T>::maxIndexOf(simpleArraySelector<T, TResult> selector) const
	{
		TResult max = default_value::value<T>();
		int maxIndex = -1;

		for (int index = 0; index < _count; index++)
		{
			TResult current = selector(this->get(index));

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

		return maxIndex;
	}

	template<isHashable T>
	template<isNumber TResult>
	TResult simpleArray<T>::kadanesAlgorithm(simpleRange<int>& result, simpleArrayValueIndicator<T, TResult> valueIndicator)
	{
		TResult maxSum = 0;
		TResult currentSum = 0;

		simpleRange<int> current(-1, -1, -1, this->count() - 1);

		for (int index = 0; index < this->count(); index++)
		{
			TResult currentValue = valueIndicator(this->get(index));

			if (currentValue > 0)
			{
				currentSum += currentValue;

				if (current.getLow() == -1)
				{
					current.set(index, index);
				}
				else
				{
					current.setHigh(index);
				}
			}
			else
			{
				// Check maximum value
				if (maxSum < currentSum && current.getLow() >= 0 && current.getHigh() >= 0)
				{
					result.set(current.getLow(), current.getHigh());
					maxSum = currentSum;
				}

				// Reset (current block only)
				current.set(-1, -1);
				currentSum = 0;
			}
		}

		// Result (trailing value was set, so the maxSum and result weren't updated)
		if (currentSum > 0)
		{
			// Check maximum value
			if (maxSum < currentSum && current.getLow() >= 0 && current.getHigh() >= 0)
			{
				result.set(current.getLow(), current.getHigh());
				maxSum = currentSum;
			}
		}

		// Default
		return maxSum;
	}
}
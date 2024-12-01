#pragma once

#include "simple.h"
#include "simpleException.h"

namespace brogueHd::simple
{
	template<isNumber T>
	class simpleRange : simpleObject
	{
	public:

		simpleRange();
		simpleRange(T lowLimit, T highLimit);
		simpleRange(T lowLimit, T low, T high, T highLimit);
		simpleRange(const simpleRange& copy);
		~simpleRange();

		void operator=(const simpleRange& copy);
		void operator==(const simpleRange& other) const;
		void operator!=(const simpleRange& other) const;

		void set(T low, T high);
		void setLow(T value);
		void setHigh(T value);
		T getLow() const;
		T getHigh() const;

		int size() const;
		bool contains(const T& value) const;

	private:

		T getHighLimit() const;
		T getLowLimit() const;

		void copyImpl(const simpleRange& copy);
		bool compare(const simpleRange& other) const;
		bool validate() const;

	private:

		T _low;
		T _high;
		T _lowLimit;
		T _highLimit;
	};

	template<isNumber T>
	simpleRange<T>::simpleRange()
	{
		_low = default_value::value<T>();
		_high = default_value::value<T>();
		_lowLimit = default_value::value<T>();
		_highLimit = default_value::value<T>();

		if (!validate())
			throw simpleException("Invalid simpleRange:  range out of bounds");
	}

	template<isNumber T>
	simpleRange<T>::simpleRange(T lowLimit, T highLimit)
	{
		_low = lowLimit;
		_high = highLimit;
		_lowLimit = lowLimit;
		_highLimit = highLimit;

		if (!validate())
			throw simpleException("Invalid simpleRange:  range out of bounds");
	}

	template<isNumber T>
	simpleRange<T>::simpleRange(T lowLimit, T low, T high, T highLimit)
	{
		_low = low;
		_high = high;
		_lowLimit = lowLimit;
		_highLimit = highLimit;

		if (!validate())
			throw simpleException("Invalid simpleRange:  range out of bounds");
	}

	template<isNumber T>
	simpleRange<T>::simpleRange(const simpleRange& copy)
	{
		copyImpl(copy);
	}

	template<isNumber T>
	simpleRange<T>::~simpleRange() {}

	template<isNumber T>
	void simpleRange<T>::operator=(const simpleRange& copy)
	{
		copyImpl(copy);
	}

	template<isNumber T>
	void simpleRange<T>::operator==(const simpleRange& other) const
	{
		return compare(other);
	}

	template<isNumber T>
	void simpleRange<T>::operator!=(const simpleRange& other) const
	{
		return compare(other);
	}

	template<isNumber T>
	void simpleRange<T>::set(T low, T high)
	{
		_low = low;
		_high = high;

		if (!validate())
			throw simpleException("Invalid simpleRange:  range out of bounds");
	}

	template<isNumber T>
	void simpleRange<T>::setLow(T value)
	{
		_low = value;

		if (!validate())
			throw simpleException("Invalid simpleRange:  range out of bounds");
	}

	template<isNumber T>
	void simpleRange<T>::setHigh(T value)
	{
		_high = value;

		if (!validate())
			throw simpleException("Invalid simpleRange:  range out of bounds");
	}

	template<isNumber T>
	int simpleRange<T>::size() const
	{
		return _high - _low + 1;
	}

	template<isNumber T>
	bool simpleRange<T>::contains(const T& value) const
	{
		return value >= _low && value <= _high;
	}

	template<isNumber T>
	T simpleRange<T>::getLow() const
	{
		return _low;
	}

	template<isNumber T>
	T simpleRange<T>::getHigh() const
	{
		return _high;
	}

	template<isNumber T>
	T simpleRange<T>::getHighLimit() const
	{
		return _highLimit;
	}

	template<isNumber T>
	T simpleRange<T>::getLowLimit() const
	{
		return _lowLimit;
	}

	template<isNumber T>
	void simpleRange<T>::copyImpl(const simpleRange& copy)
	{
		_low = copy.getLow();
		_high = copy.getHigh();
		_lowLimit = copy.getLowLimit();
		_highLimit = copy.getHighLimit();
	}

	template<isNumber T>
	bool simpleRange<T>::compare(const simpleRange& copy) const
	{
		return _low == copy.getLow() &&
			_high == copy.getHigh() &&
			_lowLimit == copy.getLowLimit() &&
			_highLimit == copy.getHighLimit();
	}

	template<isNumber T>
	bool simpleRange<T>::validate() const
	{
		if (_low < _lowLimit)
			return false;

		if (_high > _highLimit)
			return false;

		if (_low > _high)
			return false;

		if (_lowLimit > _highLimit)
			return false;

		return true;
	}
}
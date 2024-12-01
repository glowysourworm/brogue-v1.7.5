#pragma once

#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleList.h"
#include "simpleMath.h"

namespace brogueHd::simple
{
	template<isHashable T>
	class simpleOrderedList : public simpleObject
	{
	public:
		simpleOrderedList(simpleListComparer<T> comparer);
		//simpleOrderedList(const simpleListComparer<T>& comparer, const simpleList<T>& copy);
		simpleOrderedList(const simpleOrderedList<T>& copy);
		~simpleOrderedList();

		T get(int index) const;
		int count() const;

		void add(const T& item);
		void remove(const T& item);
		T removeAt(int index);
		void clear();

		size_t getHash() const override;

	public:

		bool contains(const T& item) const;
		void forEach(const simpleListCallback<T>& callback) const;
		simpleList<T> where(const simpleListPredicate<T>& predicate) const;
		void addRange(const simpleArray<T>& collection);

	protected:

		simpleListComparer<T> getComparer() const;

	private:

		bool compare(const simpleOrderedList<T>& other);

		// O(n log n)
		int getInsertIndex(const T& searchItem) const;

	private:

		simpleListComparer<T>* _comparer;
		simpleList<T>* _list;
	};

	template<isHashable T>
	simpleOrderedList<T>::simpleOrderedList(simpleListComparer<T> comparer)
	{
		_comparer = new simpleListComparer<T>(comparer);
		_list = new simpleList<T>();
	}
	//template<isHashable T>
	//simpleOrderedList<T>::simpleOrderedList(const simpleListComparer<T>& comparer, const simpleList<T>& list)
	//{
	//	_comparer = new simpleListComparer<T>(comparer);
	//	_list = new simpleList<T>(list);
	//}
	template<isHashable T>
	simpleOrderedList<T>::simpleOrderedList(const simpleOrderedList<T>& copy)
	{
		_comparer = new simpleListComparer<T>(copy.getComparer());
		_list = new simpleList<T>(copy.count());

		for (int index = 0; index < copy.count(); index++)
			this->add(copy.get(index));
	}
	template<isHashable T>
	simpleOrderedList<T>::~simpleOrderedList()
	{
		delete _list;
		delete _comparer;

		_list = nullptr;
	}

	template<isHashable T>
	T simpleOrderedList<T>::get(int index) const
	{
		return _list->get(index);
	}

	template<isHashable T>
	simpleListComparer<T> simpleOrderedList<T>::getComparer() const
	{
		return *_comparer;
	}

	template<isHashable T>
	size_t simpleOrderedList<T>::getHash() const
	{
		size_t hash = 0;

		for (int index = 0; index < _list->count(); index++)
		{
			if (hash == 0)
				hash = hashGenerator::generateHash(this->get(index));

			else
				hash = hashGenerator::combineHash(hash, this->get(index));
		}

		return hash;
	}

	template<isHashable T>
	int simpleOrderedList<T>::count() const
	{
		return _list->count();
	}

	template<isHashable T>
	void simpleOrderedList<T>::add(const T& item)
	{
		int insertIndex = this->getInsertIndex(item);

		if (insertIndex >= 0)
			_list->insert(insertIndex, item);

		else
			_list->add(item);
	}

	template<isHashable T>
	void simpleOrderedList<T>::addRange(const simpleArray<T>& collection)
	{
		for (int index = 0; index < collection.count(); index++)
		{
			this->add(collection.get(index));
		}
	}

	template<isHashable T>
	bool simpleOrderedList<T>::compare(const simpleOrderedList<T>& other)
	{
		if (other == nullptr)
			return false;

		else if (_list->count() != other.count())
			return false;

		for (int index = 0; index < _list->count(); index++)
		{
			if (this->get(index) != other.get(index))
				return false;
		}

		return true;
	}

	template<isHashable T>
	T simpleOrderedList<T>::removeAt(int index)
	{
		if (index >= _list->count())
			throw simpleException("Index is outside the bounds of the array");

		return _list->removeAt(index);
	}

	template<isHashable T>
	void simpleOrderedList<T>::remove(const T& item)
	{
		_list->remove(item);
	}

	template<isHashable T>
	void simpleOrderedList<T>::clear()
	{
		_list->clear();
	}

	template<isHashable T>
	bool simpleOrderedList<T>::contains(const T& item) const
	{
		int insertIndex = this->getInsertIndex(item);

		return insertIndex >= 0;
	}

	template<isHashable T>
	void simpleOrderedList<T>::forEach(const simpleListCallback<T>& callback) const
	{
		_list->forEach(callback);
	}

	template<isHashable T>
	simpleList<T> simpleOrderedList<T>::where(const simpleListPredicate<T>& predicate) const
	{
		simpleList<T> result;

		for (int index = 0; index < _list->count(); index++)
		{
			if (predicate(_list->get(index)))
				result.add(_list->get(index));
		}

		return result;
	}

	template<isHashable T>
	int simpleOrderedList<T>::getInsertIndex(const T& searchItem) const
	{
		/*
			Binary Search

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
		int rightIndex = _list->count() - 1;

		// Initialize insert index to be the left index
		int insertIndex = leftIndex;

		while (leftIndex <= rightIndex)
		{
			int middleIndex = (int)simpleMath::floor((leftIndex + rightIndex) / 2.0f);
			T item = _list->get(middleIndex);

			// Set insert index
			insertIndex = middleIndex;

			// Item's value is LESS THAN search value
			if ((*_comparer)(item, searchItem) < 0)
			{
				leftIndex = middleIndex + 1;

				// Set insert index for catching final iteration
				insertIndex = leftIndex;
			}

			// GREATER THAN
			else if ((*_comparer)(item, searchItem) > 0)
				rightIndex = middleIndex - 1;

			else
			{
				insertIndex = middleIndex;
				break;
			}
		}

		if (insertIndex >= 0 && insertIndex < _list->count())
			return insertIndex;

		else
			return -1;
	}
}
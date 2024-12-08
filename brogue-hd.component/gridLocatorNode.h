#pragma once

#include <simple.h>

#include "gridLocator.h"

namespace brogueHd::component
{
	template<isHashable T>
	class gridLocatorNode : public gridLocator
	{
	public:

		gridLocatorNode();
		gridLocatorNode(const T& nodeData, const gridLocator& nodeLocator);
		gridLocatorNode(const gridLocatorNode<T>& copy);
		virtual ~gridLocatorNode();

		void operator=(const gridLocatorNode<T>& copy);

		virtual bool operator==(const gridLocatorNode<T>& other) const;
		virtual bool operator!=(const gridLocatorNode<T>& other) const;

		T getData() const;
		gridLocator getLocator() const;

		virtual size_t getHash() const override;

	private:

		void copyImpl(const gridLocatorNode<T>& copy);
		bool compare(const gridLocatorNode<T>& other) const;

	private:

		T _data;
		gridLocator _locator;
	};

	template<isHashable T>
	gridLocatorNode<T>::gridLocatorNode()
	{}

	template<isHashable T>
	gridLocatorNode<T>::gridLocatorNode(const T& nodeData, const gridLocator& nodeLocator) : gridLocator(nodeLocator)
	{
		_data = nodeData;
	}

	template<isHashable T>
	gridLocatorNode<T>::gridLocatorNode(const gridLocatorNode<T>& copy) : gridLocator(copy.getLocator())
	{
		_data = copy.getData();
	}

	template<isHashable T>
	gridLocatorNode<T>::~gridLocatorNode()
	{
		
	}

	template<isHashable T>
	void gridLocatorNode<T>::operator=(const gridLocatorNode<T>& copy)
	{
		_data = copy.getData();
		this->column = copy.column;
		this->row = copy.row;
	}

	template<isHashable T>
	bool gridLocatorNode<T>::operator==(const gridLocatorNode<T>& other) const
	{
		return _data == other.getData() &&
			this->column == other.column &&
			this->row == other.row;
	}

	template<isHashable T>
	bool gridLocatorNode<T>::operator!=(const gridLocatorNode<T>& other) const
	{
		return _data != other.getData() ||
			this->column != other.column ||
			this->row != other.row;
	}

	template<isHashable T>
	T gridLocatorNode<T>::getData() const
	{
		return _data;
	}

	template<isHashable T>
	gridLocator gridLocatorNode<T>::getLocator() const
	{
		return _locator;
	}

	template<isHashable T>
	size_t gridLocatorNode<T>::getHash() const
	{
		return hashGenerator::generateHash(_data, _locator);
	}
}

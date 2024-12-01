#pragma once

#include "simple.h"

namespace brogueHd::simple
{
	template<isHashable K, typename T>
	class simpleBSTNode : public simpleObject
	{
	public:

		/// <summary>
		/// Creates an instance of the new BST node. The left, right, and height data
		/// must be set by the BST tree implementation.
		/// </summary>
		simpleBSTNode(const K& key, const T& value)
		{
			_key = key;
			_value = value;
			_left = nullptr;
			_right = nullptr;
			_height = 0;
		}
		simpleBSTNode(const simpleBSTNode& copy)
		{
			copyImpl(copy);
		}
		~simpleBSTNode()
		{
			// Handled by the tree
		}
		void operator=(const simpleBSTNode& copy)
		{
			copyImpl(copy);
		}
		bool operator==(const simpleBSTNode& other)
		{
			return compare(other);
		}
		bool operator!=(const simpleBSTNode& other)
		{
			return !compare(other);
		}

	public:

		K getKey() const
		{
			return _key;
		}
		T getValue() const
		{
			return _value;
		}
		int balanceFactor() const
		{
			return ((_left != nullptr) ? _left->getHeight() : -1) - ((_right != nullptr) ? _right->getHeight() : -1);
		}
		int getHeight() const
		{
			return _height;
		}
		simpleBSTNode<K, T>* getLeft() const
		{
			return _left;
		}
		simpleBSTNode<K, T>* getRight() const
		{
			return _right;
		}

		void setLeft(simpleBSTNode<K, T>* node)
		{
			_left = node;
		}
		void setRight(simpleBSTNode<K, T>* node)
		{
			_right = node;
		}
		void setHeight(int height)
		{
			_height = height;
		}

	public:

		size_t getHash() const override
		{
			return hashGenerator::generateHash(_key);
		}

	private:

		void copyImpl(const simpleBSTNode<K, T>& copy)
		{
			_left = copy.getLeft();
			_right = copy.getRight();
			_height = copy.getHeight();
			_key = copy.getKey();
			_value = copy.getValue();
		}

		bool compare(const simpleBSTNode<K, T>& other)
		{
			return _key == other.getKey() &&
				_value == other.getValue() &&
				_left == other.getLeft() &&
				_right == other.getRight() &&
				_height == other.getHeight();
		}

	private:

		simpleBSTNode<K, T>* _left;
		simpleBSTNode<K, T>* _right;
		K _key;
		T _value;

		int _height;

	};
}
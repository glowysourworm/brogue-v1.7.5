#pragma once

#include "simple.h"
#include "simpleBSTNode.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleHashCore.h"
#include "simpleMath.h"

namespace brogueHd::simple
{
	// Stole this implementation:  This should be a balanced BST. The implementation looks
	//                             clean enough; and will be changed if there are any issues.



	// AVL Binary Search Tree Implementation - https://en.wikipedia.org/wiki/AVL_tree
	//
	// - O(log n) Insert, Delete
	// - Self balancing
	// - O(1) Search using dictionary backup (also to retrieve count)
	// 
	// UPDATE:  https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/AVLTreeST.java.html
	//
	//          Must cleaner implementation - uses just a left and right pointer; calculates the
	//          height; and rebalances the tree on each operation without using any additional
	//          recursive lookups (height, or balance factor). Also, has an optional "assert check()"
	//          to check the AVL tree "invariants" (the definition of the tree) after each operation
	//  

	/// <summary>
	/// AVL Binary Search Tree implementation that gets the key from the value node by use of an 
	/// indexer delegate. Also, uses a user supplied comparer to compare two node keys. The keys are
	/// supplied by the indexer from the user.
	/// </summary>
	/// <typeparam name="K">Key type</typeparam>
	/// <typeparam name="T">Node type</typeparam>
	template<isHashable K, typename T>
	class simpleBST
	{
	public:

		simpleBST();
		~simpleBST();

		void insert(K key, T value);
		T remove(K key);
		bool containsKey(K key);
		int count() const;

		void iterate(simpleHashCallback<K, T> callback) const;

		T get(K key);
		T search(K key);

		T minValue();
		K minKey();
		T maxValue();
		K maxKey();

		void clear();

	public:

		simpleString createOutput();

	private:

		std::string createExceptionMessage(const char* message);
		void createOutputRecurse(simpleBSTNode<K, T>* node, int level, simpleString& result);

	private:

		static int keyCompare(K key1, K key2)
		{
			if (key1 > key2)
				return 1;

			else if (key1 < key2)
				return -1;

			else if (key1 == key2)
				return 0;

			else
				throw simpleException("Unhandled binary comparison operator:  simpleBST.h");
		}

		simpleBSTNode<K, T>* successor(K searchKey);
		simpleBSTNode<K, T>* predecessor(K searchKey);

		simpleBSTNode<K, T>* insertImpl(simpleBSTNode<K, T>* node, K key, T value);
		simpleBSTNode<K, T>* removalImpl(simpleBSTNode<K, T>* node, K key);
		simpleBSTNode<K, T>* deleteMin(simpleBSTNode<K, T>* node);
		simpleBSTNode<K, T>* minImpl(simpleBSTNode<K, T>* node);
		simpleBSTNode<K, T>* maxImpl(simpleBSTNode<K, T>* node);
		simpleBSTNode<K, T>* searchImpl(K key, simpleBSTNode<K, T>* node) const;
		simpleBSTNode<K, T>* successorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent);
		simpleBSTNode<K, T>* predecessorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent);
		simpleBSTNode<K, T>* balance(simpleBSTNode<K, T>* node);
		simpleBSTNode<K, T>* rotateLeft(simpleBSTNode<K, T>* subTree);
		simpleBSTNode<K, T>* rotateRight(simpleBSTNode<K, T>* node);

		void clearImpl(simpleBSTNode<K, T>* node);

	private:

		// Root of the BST
		simpleBSTNode<K, T>* _root;

		// Map for boosting performance for direct key lookup
		simpleHash<K, simpleBSTNode<K, T>*>* _nodeMap;
	};

	template<isHashable K, typename T>
	simpleBST<K, T>::simpleBST()
	{
		// Track values to boost performance for direct lookups
		_nodeMap = new simpleHash<K, simpleBSTNode<K, T>*>();

		_root = nullptr;
	}

	template<isHashable K, typename T>
	simpleBST<K, T>::~simpleBST()
	{
		this->clear();

		delete _nodeMap;
	}

	template<isHashable K, typename T>
	void simpleBST<K, T>::clear()
	{
		// Delete nodes recursively
		clearImpl(_root);

		_root = nullptr;

		// Clear out hash table
		_nodeMap->clear();
	}

	template<isHashable K, typename T>
	void simpleBST<K, T>::insert(K key, T value)
	{
		if (_nodeMap->contains(key))
			throw simpleException(createExceptionMessage("Trying to insert a duplicate key in simpleBST tree:  simpleBST::insert"));

		// Insert value into the tree -> Rebalance the tree
		_root = this->insertImpl(_root, key, value);

		// Take the O(n log n) hit to find the node and hash it
		simpleBSTNode<K, T>* node = this->searchImpl(key, _root);

		// Track the values for debugging and a fast retrieval using the key
		_nodeMap->add(key, node);
	}

	template<isHashable K, typename T>
	T simpleBST<K, T>::remove(K key)
	{
		if (!_nodeMap->contains(key))
			throw simpleException(createExceptionMessage("Trying to remove non-existing key from binary search tree"));

		// Remove the specified key -> Rebalance the tree
		_root = this->removalImpl(_root, key);

		// Item to return to user
		T item = _nodeMap->get(key)->getValue();

		// Track the values for debugging and fast retrieval using the key
		_nodeMap->remove(key);

		return item;
	}

	template<isHashable K, typename T>
	int simpleBST<K, T>::count() const
	{
		return _nodeMap->count();
	}

	template<isHashable K, typename T>
	bool simpleBST<K, T>::containsKey(K key)
	{
		return _nodeMap->contains(key);
	}

	template<isHashable K, typename T>
	void simpleBST<K, T>::iterate(simpleHashCallback<K, T> callback) const
	{
		_nodeMap->iterate([&callback] (K key, simpleBSTNode<K, T>* node)
		{
			return callback(key, node->getValue());
		});
	}

	template<isHashable K, typename T>
	T simpleBST<K, T>::get(K key)
	{
		// Utilize dictionary for O(1) lookup
		if (_nodeMap->contains(key))
			return _nodeMap->get(key)->getValue();

		else
			throw simpleException(createExceptionMessage("Trying to retrieve hash-backed node from BST without checking"));
	}

	template<isHashable K, typename T>
	T simpleBST<K, T>::search(K key)
	{
		// Utilize dictionary for O(1) lookup
		if (_nodeMap->contains(key))
			return _nodeMap->get(key)->getValue();

		simpleBSTNode<K, T>* node = this->searchImpl(key, _root);

		if (node != nullptr)
			return node->getValue();

		return nullptr;
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::successor(K searchKey)
	{
		if (!_nodeMap->contains(searchKey))
			return nullptr;

		return this->successorImpl(searchKey, _root, nullptr);
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::predecessor(K searchKey)
	{
		if (!_nodeMap->contains(searchKey))
			return nullptr;

		return this->predecessorImpl(searchKey, _root, nullptr);
	}

	template<isHashable K, typename T>
	T simpleBST<K, T>::minValue()
	{
		simpleBSTNode<K, T>* result = this->minImpl(_root);

		if (result == nullptr)
			return nullptr;

		else
			return result->getValue();
	}

	template<isHashable K, typename T>
	K simpleBST<K, T>::minKey()
	{
		simpleBSTNode<K, T>* minNode = this->minImpl(_root);

		if (minNode == nullptr)
			throw simpleException(createExceptionMessage("Trying to resolve min key from an empty Binary Search Tree"));

		else
			return minNode->getKey();
	}

	template<isHashable K, typename T>
	T simpleBST<K, T>::maxValue()
	{
		simpleBSTNode<K, T>* result = this->maxImpl(_root);

		if (result == nullptr)
			return nullptr;

		else
			return result->getValue();
	}

	template<isHashable K, typename T>
	K simpleBST<K, T>::maxKey()
	{
		simpleBSTNode<K, T>* maxNode = this->maxImpl(_root);

		if (maxNode == nullptr)
			throw simpleException(createExceptionMessage("Trying to resolve max key from an empty Binary Search Tree"));

		else
			return maxNode->getKey();
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::insertImpl(simpleBSTNode<K, T>* node, K key, T value)
	{
		if (node == nullptr)
			return new simpleBSTNode<K, T>(key, value);

		int comparison = keyCompare(key, node->getKey());

		// Insert Left
		if (comparison < 0)
			node->setLeft(this->insertImpl(node->getLeft(), key, value));

		// Insert Right
		else if (comparison > 0)
			node->setRight(this->insertImpl(node->getRight(), key, value));

		else
			throw simpleException(createExceptionMessage("Duplicate key insertion BinarySearchTree"));

		// Set the height
		int height = simpleMath::maxOf((node->getLeft() != nullptr) ? node->getLeft()->getHeight() : -1,
									   (node->getRight() != nullptr) ? node->getRight()->getHeight() : -1) + 1;

		node->setHeight(height);

		return this->balance(node);
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::removalImpl(simpleBSTNode<K, T>* node, K key)
	{
		// Procedure:  https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/AVLTreeST.java.html
		//

		int comparison = keyCompare(key, node->getKey());

		if (comparison < 0)
			node->setLeft(this->removalImpl(node->getLeft(), key));

		else if (comparison > 0)
			node->setRight(this->removalImpl(node->getRight(), key));

		else
		{
			// One child case
			if (node->getLeft() == nullptr)
				return node->getRight();

			else if (node->getRight() == nullptr)
				return node->getLeft();

			// Next successor case
			else
			{
				simpleBSTNode<K, T>* temp = node;

				node = this->minImpl(temp->getRight());
				node->setRight(this->deleteMin(temp->getRight()));
				node->setLeft(temp->getLeft());
			}
		}

		// Set the height
		int height = simpleMath::maxOf((node->getLeft() != nullptr) ? node->getLeft()->getHeight() : -1,
					   				   (node->getRight() != nullptr) ? node->getRight()->getHeight() : -1) + 1;

		node->setHeight(height);

		return this->balance(node);
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::deleteMin(simpleBSTNode<K, T>* node)
	{
		if (node->getLeft() == nullptr)
			return node->getRight();

		node->setLeft(this->deleteMin(node->getLeft()));

		// Set the height
		int height = simpleMath::maxOf((node->getLeft() != nullptr) ? node->getLeft()->getHeight() : -1,
									   (node->getRight() != nullptr) ? node->getRight()->getHeight() : -1) + 1;

		node->setHeight(height);

		return this->balance(node);
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::minImpl(simpleBSTNode<K, T>* node)
	{
		if (node == nullptr)
			return nullptr;

		if (node->getLeft() == nullptr)
			return node;

		return this->minImpl(node->getLeft());
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::maxImpl(simpleBSTNode<K, T>* node)
	{
		if (node == nullptr)
			return nullptr;

		if (node->getRight() == nullptr)
			return node;

		return this->maxImpl(node->getRight());
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::searchImpl(K key, simpleBSTNode<K, T>* node) const
	{
		if (node == nullptr)
			return nullptr;

		int comparison = keyCompare(key, node->getKey());

		if (comparison < 0 && node->getLeft() != nullptr)
			return this->searchImpl(key, node->getLeft());

		else if (comparison > 0 && node->getRight() != nullptr)
			return this->searchImpl(key, node->getRight());

		else
			return node;
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::successorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent)
	{
		// Recursively look for the search key's node - keeping track of the last parent with a LEFT child. This 
		// will be the successor if the final search node has no left child.
		//
		// At the final leaf node - continue looking for the MAX of it's RIGHT sub-tree
		//

		if (node == nullptr)
			return nullptr;

		int comparison = keyCompare(key, node.Key);

		if (comparison < 0 && node->getLeft() != nullptr)
		{
			// Keep track of this last parent
			savedParent = node;

			return this->successorImpl(key, node->getLeft(), savedParent);
		}

		else if (comparison > 0 && node->getRight() != nullptr)
			return this->successorImpl(key, node->getRight(), savedParent);

		// FOUND NODE!
		else
		{
			if (node->getRight() != nullptr)
				return this->minImpl(node->getRight());

			else
				return savedParent;
		}
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::predecessorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent)
	{
		// Recursively look for the search key's node - keeping track of the last parent with a RIGHT child. This 
		// will be the predecessor if the final search node has no right child.
		//
		// At the final leaf node - continue looking for the MIN of it's LEFT sub-tree
		//

		if (node == nullptr)
			return nullptr;

		int comparison = keyCompare(key, node->getKey());

		if (comparison < 0 && node->getLeft() != nullptr)
			return this->predecessorImpl(key, node->getLeft(), savedParent);

		else if (comparison > 0 && node->getRight() != nullptr)
		{
			// Keep track of this last parent
			savedParent = node;

			return PredecessorImpl(key, node->getRight(), savedParent);
		}

		// FOUND NODE!
		else
		{
			if (node->getLeft() != nullptr)
				return this->maxImpl(node->getLeft());

			else
				return savedParent;
		}
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::balance(simpleBSTNode<K, T>* node)
	{
		// Procedure - https://en.wikipedia.org/wiki/AVL_tree
		//
		// Examine situation with heavy node and direct child:
		//
		// Let: subTree = X, and nextNode = Z. (Next node would be left or right depending on BalanceFactor(Z))

		/*
			Right Right  => Z is a right child of its parent X and Z is  not left-heavy 	(i.e. BalanceFactor(Z) ≥ 0)  (Rotate Left)
			Left  Left 	 => Z is a left  child of its parent X and Z is  not right-heavy    (i.e. BalanceFactor(Z) ≤ 0)  (Rotate Right)
			Right Left 	 => Z is a right child of its parent X and Z is  left-heavy 	    (i.e. BalanceFactor(Z) = −1) (Double Rotate RightLeft)
			Left  Right  => Z is a left  child of its parent X and Z is  right-heavy 	    (i.e. BalanceFactor(Z) = +1) (Double Rotate LeftRight)
		*/

		// Update:  https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/AVLTreeST.java.html

		if (node->balanceFactor() < -1)
		{
			// Left Right
			if (node->getRight()->balanceFactor() > 0)
				node->setRight(this->rotateRight(node->getRight()));

			node = this->rotateLeft(node);
		}
		else if (node->balanceFactor() > 1)
		{
			// Right Left
			if (node->getLeft()->balanceFactor() < 0)
				node->setLeft(this->rotateLeft(node->getLeft()));

			node = this->rotateRight(node);
		}

		return node;
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::rotateLeft(simpleBSTNode<K, T>* subTree)
	{
		// Procedure - https://en.wikipedia.org/wiki/AVL_tree
		//
		// - Pre-condition:  Node must have a right-child and a balance factor of -2
		// - Node's parent becomes node's right child
		// - Node's right child become node's right child's left child
		// - Set new balance factors (see Wikipedia)

		//if (subTree.BalanceFactor != -2)
		//    throw new Exception("Invalid RotateLeft pre-condition BinarySearchTree");

		//if (subTree.RightChild == null)
		//    throw new Exception("Invalid RotateLeft pre-condition BinarySearchTree");

		// Refering to variables from Wikipedia entry
		simpleBSTNode<K, T>* X = subTree;
		simpleBSTNode<K, T>* Z = subTree->getRight();
		simpleBSTNode<K, T>* W = subTree->getRight() == nullptr ? nullptr : subTree->getRight()->getLeft();

		// Node's left child becomes node's parent's right child:  X -> W
		X->setRight(W);

		// Node's parent becomes the left child of node:  X <- Z
		Z->setLeft(X);

		// Set up height of nodes
		X->setHeight(simpleMath::maxOf((X->getLeft() != nullptr) ? X->getLeft()->getHeight() : -1, (X->getRight() != nullptr) ? X->getRight()->getHeight() : -1) + 1);
		Z->setHeight(simpleMath::maxOf((Z->getLeft() != nullptr) ? Z->getLeft()->getHeight() : -1, (Z->getRight() != nullptr) ? Z->getRight()->getHeight() : -1) + 1);

		// Return node of the new sub-tree
		return Z;
	}

	template<isHashable K, typename T>
	simpleBSTNode<K, T>* simpleBST<K, T>::rotateRight(simpleBSTNode<K, T>* node)
	{
		// Procedure - https://en.wikipedia.org/wiki/AVL_tree
		//
		// - Pre-condition:  Node must have a left-child and a balance factor of +2
		// - Node's parent becomes node's left child
		// - Node's left child become node's left child's right child
		// - Set new height

		//if (node.BalanceFactor != 2)
		//    throw new Exception("Invalid RotateLeft pre-condition BinarySearchTree");

		//if (node.LeftChild == null)
		//    throw new Exception("Invalid RotateLeft pre-condition BinarySearchTree");

		// Refering to variables from Wikipedia entry
		simpleBSTNode<K, T>* X = node;
		simpleBSTNode<K, T>* Z = node->getLeft();
		simpleBSTNode<K, T>* W = node->getLeft() == nullptr ? nullptr : node->getLeft()->getRight();

		// Node's right child becomes node's parent's left child:  T <- X
		X->setLeft(W);

		// Node's parent becomes the right child of node:  Z -> X
		Z->setRight(X);

		// Set up height of nodes
		X->setHeight(simpleMath::maxOf((X->getLeft() != nullptr) ? X->getLeft()->getHeight() : -1, (X->getRight() != nullptr) ? X->getRight()->getHeight() : -1) + 1);
		Z->setHeight(simpleMath::maxOf((Z->getLeft() != nullptr) ? Z->getLeft()->getHeight() : -1, (Z->getRight() != nullptr) ? Z->getRight()->getHeight() : -1) + 1);

		// Return node of the new sub-tree
		return Z;
	}

	template<isHashable K, typename T>
	void simpleBST<K, T>::clearImpl(simpleBSTNode<K, T>* node)
	{
		if (node == nullptr)
			return;

		if (node->getLeft() != nullptr)
			clearImpl(node->getLeft());

		if (node->getRight() != nullptr)
			clearImpl(node->getRight());

		if (node != nullptr)
			delete node;
	}

	template<isHashable K, typename T>
	std::string simpleBST<K, T>::createExceptionMessage(const char* message)
	{
		simpleString result;

		result.appendLine(message);

		simpleString treeOutput = createOutput();

		result.appendLine(treeOutput);

		return std::string(result.c_str());
	}

	template<isHashable K, typename T>
	simpleString simpleBST<K, T>::createOutput()
	{
		simpleString output;
		createOutputRecurse(_root, 0, output);
		return output;
	}

	template<isHashable K, typename T>
	void simpleBST<K, T>::createOutputRecurse(simpleBSTNode<K,T>* node, int level, simpleString& result)
	{
		const char* messageFormat = "Node level {}: Key={} Value={} Height={} MAPPED={}";
		bool nodeFound = _nodeMap->contains(node->getKey());
		std::string message = simpleExt::format(messageFormat, level, node->getKey(), node->getValue(), node->getHeight(), (int)nodeFound);

		result.appendLine(message.c_str());

		if (node->getLeft() != nullptr)
			createOutputRecurse(node->getLeft(), level + 1, result);

		if (node->getRight() != nullptr)
			createOutputRecurse(node->getRight(), level + 1, result);
	}
}
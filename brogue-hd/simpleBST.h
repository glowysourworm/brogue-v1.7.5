#pragma once

#include "simple.h"
#include "simpleHash.h"

namespace brogueHd::simple
{
    // Stole this implementation:  This should be a balanced BST. The implementation looks
    //                             clean enough; and will be changed if there are any issues.

    template<isHashable K, typename T>
    struct simpleBSTNode
    {
        simpleBSTNode<K, T>* left;
        simpleBSTNode<K, T>* right;
        K key;
        T value;

        int height;

        int balanceFactor()
        {
            return ((left == NULL) ? left->height : -1) - ((right == NULL) ? right->height : -1);
        }

        simpleBSTNode(K akey, T avalue)
        {
            key = akey;
            value = avalue;
        }
    };

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

        T min();
        K minKey();
        T max();
        K maxKey();

        void clear();

    private:

        static int keyCompare(K key1, K key2)
        {
            return (key1 > key2) - (key1 < key2);
        }

        simpleBSTNode<K, T>* successor(K searchKey);
        simpleBSTNode<K, T>* predecessor(K searchKey);

        simpleBSTNode<K, T>* insertImpl(simpleBSTNode<K, T>* node, K key, T value);
        simpleBSTNode<K, T>* removalImpl(simpleBSTNode<K, T>* node, K key);
        simpleBSTNode<K, T>* deleteMin(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* minImpl(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* maxImpl(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* searchImpl(K key, simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* successorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent);
        simpleBSTNode<K, T>* predecessorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent);
        simpleBSTNode<K, T>* balance(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* rotateLeft(simpleBSTNode<K, T>* subTree);
        simpleBSTNode<K, T>* rotateRight(simpleBSTNode<K, T>* node);

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

        _root = NULL;
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
        if (_root != NULL)
            this->remove(_root->key);

        _nodeMap->clear();
    }

    template<isHashable K, typename T>
    void simpleBST<K, T>::insert(K key, T value)
    {
        // Insert value into the tree -> Rebalance the tree
        _root = this->insertImpl(_root, key, value);

        // Take the O(n log n) hit to find the node and hash it
        simpleBSTNode<K, T>* node = this->searchImpl(key, _root);

        // Track the values for debugging and a fast retrieval using the key
        _nodeMap->set(key, node);
    }

    template<isHashable K, typename T>
    T simpleBST<K, T>::remove(K key)
    {
        if (!_nodeMap->contains(key))
            simpleException::showCstr("Trying to remove non-existing key from binary search tree");

        // Remove the specified key -> Rebalance the tree
        _root = this->removalImpl(_root, key);

        // Item to return to user
        T item = _nodeMap->get(key)->value;

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
        _nodeMap->iterate([&callback](K key, simpleBSTNode<K, T>* node)
            {
                return callback(key, node->value);
            });
    }

    template<isHashable K, typename T>
    T simpleBST<K, T>::get(K key)
    {
        // Utilize dictionary for O(1) lookup
        if (_nodeMap->contains(key))
            return _nodeMap->get(key)->value;

        else
            simpleException::showCstr("Trying to retrieve hash-backed node from BST without checking");
    }

    template<isHashable K, typename T>
    T simpleBST<K, T>::search(K key)
    {
        // Utilize dictionary for O(1) lookup
        if (_nodeMap->contains(key))
            return _nodeMap->get(key)->value;

        simpleBSTNode<K, T>* node = this->searchImpl(key, _root);

        if (node != NULL)
            return node->value;

        return NULL;
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::successor(K searchKey)
    {
        if (!_nodeMap->contains(searchKey))
            return NULL;

        return this->successorImpl(searchKey, _root, NULL);
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::predecessor(K searchKey)
    {
        if (!_nodeMap->contains(searchKey))
            return NULL;

        return this->predecessorImpl(searchKey, _root, NULL);
    }

    template<isHashable K, typename T>
    T simpleBST<K, T>::min()
    {
        simpleBSTNode<K, T>* result = this->minImpl(_root);

        if (result == NULL)
            return NULL;

        else
            return result->value;
    }

    template<isHashable K, typename T>
    K simpleBST<K, T>::minKey()
    {
        simpleBSTNode<K, T>* minNode = this->minImpl(_root);

        if (minNode == NULL)
            simpleException::showCstr("Trying to resolve min key from an empty Binary Search Tree");

        else
            return minNode->key;
    }

    template<isHashable K, typename T>
    T simpleBST<K, T>::max()
    {
        simpleBSTNode<K, T>* result = this->maxImpl(_root);

        if (result == NULL)
            return NULL;

        else
            return result->value;
    }

    template<isHashable K, typename T>
    K simpleBST<K, T>::maxKey()
    {
        simpleBSTNode<K, T>* maxNode = this->maxImpl(_root);

        if (maxNode == NULL)
            simpleException::showCstr("Trying to resolve max key from an empty Binary Search Tree");

        else
            return maxNode->key;
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::insertImpl(simpleBSTNode<K, T>* node, K key, T value)
    {
        if (node == NULL)
            return new simpleBSTNode<K, T>(key, value);

        int comparison = keyCompare(key, node->key);

        // Insert Left
        if (comparison < 0)
            node->left = this->insertImpl(node->left, key, value);

        // Insert Right
        else if (comparison > 0)
            node->right = this->insertImpl(node->right, key, value);

        else
            simpleException::showCstr("Duplicate key insertion BinarySearchTree");

        // Set the height
        node->height = simpleMath::maxOf((node->left != NULL) ? node->left->height : -1,
            (node->right != NULL) ? node->right->height : -1) + 1;

        return this->balance(node);
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::removalImpl(simpleBSTNode<K, T>* node, K key)
    {
        // Procedure:  https://algs4.cs.princeton.edu/code/edu/princeton/cs/algs4/AVLTreeST.java.html
        //

        int comparison = keyCompare(key, node->key);

        if (comparison < 0)
            node->left = this->removalImpl(node->left, key);

        else if (comparison > 0)
            node->right = this->removalImpl(node->right, key);

        else
        {
            // One child case
            if (node->left == NULL)
                return node->right;

            else if (node->right == NULL)
                return node->left;

            // Next successor case
            else
            {
                simpleBSTNode<K, T>* temp = node;

                node = this->minImpl(temp->right);
                node->right = this->deleteMin(temp->right);
                node->left = temp->left;
            }
        }

        // Set the height
        node->height = simpleMath::maxOf((node->left != NULL) ? node->left->height : -1,
            (node->right != NULL) ? node->right->height : -1) + 1;

        return this->balance(node);
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::deleteMin(simpleBSTNode<K, T>* node)
    {
        if (node->left == NULL)
            return node->right;

        node->left = this->deleteMin(node->left);

        // Set the height
        node->height = simpleMath::maxOf((node->left != NULL) ? node->left->height : -1,
            (node->right != NULL) ? node->right->height : -1) + 1;

        return this->balance(node);
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::minImpl(simpleBSTNode<K, T>* node)
    {
        if (node == NULL)
            return NULL;

        return this->minImpl(node->left) != NULL ? node : NULL;
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::maxImpl(simpleBSTNode<K, T>* node)
    {
        if (node == NULL)
            return NULL;

        return this->maxImpl(node->right) != NULL ? node : NULL;
    }

    template<isHashable K, typename T>
    simpleBSTNode<K, T>* simpleBST<K, T>::searchImpl(K key, simpleBSTNode<K, T>* node)
    {
        if (node == NULL)
            return NULL;

        int comparison = keyCompare(key, node->key);

        if (comparison < 0 && node->left != NULL)
            return this->searchImpl(key, node->left);

        else if (comparison > 0 && node->right != NULL)
            return this->searchImpl(key, node->right);

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

        if (node == NULL)
            return NULL;

        int comparison = keyCompare(key, node.Key);

        if (comparison < 0 && node->left != NULL)
        {
            // Keep track of this last parent
            savedParent = node;

            return this->successorImpl(key, node->left, savedParent);
        }

        else if (comparison > 0 && node->right != NULL)
            return this->successorImpl(key, node->right, savedParent);

        // FOUND NODE!
        else
        {
            if (node->right != NULL)
                return this->minImpl(node->right);

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

        if (node == NULL)
            return NULL;

        int comparison = keyCompare(key, node.Key);

        if (comparison < 0 && node->left != NULL)
            return this->predecessorImpl(key, node->left, savedParent);

        else if (comparison > 0 && node->right != NULL)
        {
            // Keep track of this last parent
            savedParent = node;

            return PredecessorImpl(key, node->right, savedParent);
        }

        // FOUND NODE!
        else
        {
            if (node->left != NULL)
                return this->maxImpl(node->left);

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
            if (node->right->balanceFactor() > 0)
                node->right = this->rotateRight(node->right);

            node = this->rotateLeft(node);
        }
        else if (node->balanceFactor() > 1)
        {
            // Right Left
            if (node->left->balanceFactor() < 0)
                node->left = this->rotateLeft(node->left);

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
        simpleBSTNode<K, T>* Z = subTree->right;
        simpleBSTNode<K, T>* W = subTree->right == NULL ? NULL : subTree->right->left;

        // Node's left child becomes node's parent's right child:  X -> T
        X->right = W;

        // Node's parent becomes the left child of node:  X <- Z
        Z->left = X;

        // Set up height of nodes
        X->height = simpleMath::maxOf((X->left != NULL) ? X->left->height : -1, (X->right != NULL) ? X->right->height : -1) + 1;
        Z->height = simpleMath::maxOf((Z->left != NULL) ? Z->left->height : -1, (Z->right != NULL) ? Z->right->height : -1) + 1;

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
        simpleBSTNode<K, T>* Z = node->left;
        simpleBSTNode<K, T>* W = node->left == NULL ? NULL : node->left->right;

        // Node's right child becomes node's parent's left child:  T <- X
        X->left = W;

        // Node's parent becomes the right child of node:  Z -> X
        Z->right = X;

        // Set up height of nodes
        X->height = simpleMath::maxOf((X->left != NULL) ? X->left->height : -1, (X->right != NULL) ? X->right->height : -1) + 1;
        Z->height = simpleMath::maxOf((Z->left != NULL) ? Z->left->height : -1, (Z->right != NULL) ? Z->right->height : -1) + 1;

        // Return node of the new sub-tree
        return Z;
    }
}


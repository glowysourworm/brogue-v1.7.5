#pragma once

#include "simple.h"
#include "simpleHash.h"

namespace brogueHd::component
{
    // Stole this implementation:  This should be a balanced BST. The implementation looks
    //                             clean enough; and will be changed if there are any issues.

    template<isComparable K, typename T>
    struct simpleBSTNode    
    {
        simpleBSTNode<K, T>* left;
        simpleBSTNode<K, T>* right;
        K key;
        T value;

        int height;
        
        int balanceFactor()
        {
            return ((left == NULL) ? left->height ? -1) - ((right == NULL) ? right->height ?? -1);
        }

        public simpleBSTNode(K akey, T avalue)
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
    template<isComparable K, typename T>
    class simpleBST
    {
    public:

        simpleBST<K, T>::simpleBST();
        simpleBST<K, T>::~simpleBST();

        void simpleBST<K, T>::insert(K key, T value);
        bool simpleBST<K, T>::remove(K key);
        bool simpleBST<K, T>::containsKey(K key);

        T simpleBST<K, T>::get(K key);
        T simpleBST<K, T>::search(K key);

        T simpleBST<K, T>::min();
        K simpleBST<K, T>::minKey();
        T simpleBST<K, T>::max();
        K simpleBST<K, T>::maxKey();

    private:

        static int keyCompare(K key1, K key2)
        {
            return (key1 > key2) - (key1 < key2);
        }

        simpleBSTNode<K, T>* simpleBST<K, T>::successor(K searchKey);
        simpleBSTNode<K, T>* simpleBST<K, T>::predecessor(K searchKey);

        simpleBSTNode<K, T>* simpleBST<K, T>::insertImpl(simpleBSTNode<K, T>* node, K key, T value);
        simpleBSTNode<K, T>* simpleBST<K, T>::removalImpl(simpleBSTNode<K, T>* node, K key);
        simpleBSTNode<K, T>* simpleBST<K, T>::deleteMin(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* simpleBST<K, T>::minImpl(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* simpleBST<K, T>::maxImpl(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* simpleBST<K, T>::searchImpl(K key, simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* simpleBST<K, T>::successorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent);
        simpleBSTNode<K, T>* simpleBST<K, T>::predecessorImpl(K key, simpleBSTNode<K, T>* node, simpleBSTNode<K, T>* savedParent);
        simpleBSTNode<K, T>* simpleBST<K, T>::balance(simpleBSTNode<K, T>* node);
        simpleBSTNode<K, T>* simpleBST<K, T>::rotateLeft(simpleBSTNode<K, T>* subTree);
        simpleBSTNode<K, T>* simpleBST<K, T>::rotateRight(simpleBSTNode<K, T>* node);

    private:

        // Root of the BST
        simpleBSTNode<K, T>* _root;

        // Map for boosting performance for direct key lookup
        simpleHash<K, T>* _nodeMap;
    };
}


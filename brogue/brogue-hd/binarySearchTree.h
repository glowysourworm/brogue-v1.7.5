#pragma once

#include <map>

using namespace std;

namespace brogueHd
{
    // Stole this implementation:  This should be a balanced BST. The implementation looks
    //                             clean enough; and will be changed if there are any issues.

    template<typename K, typename T>
    struct Node 
    {
        K key;
        T data;
        Node<K, T>* left;
        Node<K, T>* right;
    };

    template<typename K, typename T>
    class binarySearchTree
    {
    public:

        binarySearchTree();
        ~binarySearchTree();

        /// <summary>
        /// Creates data node for the tree; and balances the tree
        /// </summary>
        void insert(K key, T data);

        /// <summary>
        /// Searches for the node closest (greater than) to the specified key
        /// </summary>
        T search(K key);

        /// <summary>
        /// Removes the node with the specified key
        /// </summary>
        /// <param name="key">Key for an existing node. Throws exception for mismatched key.</param>
        void remove(K key);

        /// <summary>
        /// Gets the minimum value from the tree
        /// </summary>
        K minKey();

        /// <summary>
        /// Gets the minimum value from the tree
        /// </summary>
        T min();

    private:

        Node<K, T>* create(K key, T data);
        Node<K, T>* insertImpl(Node<K, T>* node, K key, T data);
        Node<K, T>* searchImpl(Node<K, T>* node, K key);
        Node<K, T>* deleteImpl(Node<K, T>* root, K key);
        Node<K, T>* minNode(Node<K, T>* node);                      // Returns min node from current node

    private:

        // Root of the BST
        Node<K, T>* _root;

        // Map for boosting performance for direct key lookup
        std::map<K, T>* _nodeMap;

    };
}


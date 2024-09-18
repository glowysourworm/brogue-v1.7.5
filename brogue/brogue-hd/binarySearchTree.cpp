#pragma once

#include "binarySearchTree.h"
#include <map>
#include <exception>

using namespace std;

namespace brogueHd
{
    template<typename K, typename T>
    binarySearchTree<K, T>::binarySearchTree()
    {
        _root = NULL;
        _nodeMap = new std::map<K, T>();
    }

    template<typename K, typename T>
    binarySearchTree<K, T>::~binarySearchTree()
    {
        if (_root != NULL)
        {
            // Deletes recursively
            deleteNode(_root);

            _root = NULL;
        }

        _nodeMap->clear();
        _nodeMap = NULL;
    }

    template<typename K, typename T>
    Node<K, T>* binarySearchTree<K, T>::create(K key, T data)
    {
        Node<K, T>* newNode = new Node<K, T>();
        newNode->data = data;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    template<typename K, typename T>
    void binarySearchTree<K, T>::insert(K key, T data)
    {
        // First
        if (_root == NULL)
        {
            _root = createNode(key, data);
            _nodeMap->insert(key, data);

            return;
        }

        // Otherwise, recur down the tree
        else if (key < _root->key)
        {
            _root->left = insertImpl(_root->left, key, data);
        }

        else if (key > _root->key)
        {
            _root->right = insertImpl(_root->right, key, data);
        }

        else
            throw std::runtime_error("Error in BST tree:  Trying to insert duplicate node");
    }

    template<typename K, typename T>
    Node<K, T>* binarySearchTree<K, T>::insertImpl(Node<K, T>* node, K key, T data)
    {
        // Found next entry
        if (node == NULL)
        {
            // Create next node
            node = createNode(key, data);

            // Insert node map entry
            _nodeMap->insert(key, data);

            return node;
        }

        // Recurse
        if (key < _root->key) 
        {
            node->left = insertNode(_root->left, key, data);
        }
        else if (key > _root->key) 
        {
            node->right = insertNode(_root->right, key, data);
        }
        
        return node;
    }

    template<typename K, typename T>
    T binarySearchTree<K, T>::search(K key)
    {        
        if (_root == NULL)
            return NULL;

        else if (_nodeMap->find(key) != NULL)
            return _nodeMap[key];

        // Recurse
        else if (root->data < key) 
            return searchNode(root->right, key);

        else
            return searchNode(root->left, key);
    }

    template<typename K, typename T>
    Node<K, T>* binarySearchTree<K, T>::searchImpl(Node<K, T>* node, K key)
    {
        // Base Cases: root is null or key is present at root
        if (node == NULL)
            return NULL;

        // Map already searched

        // Recurse
        else if (root->data < key) 
        {
            return searchNode(node->right, key);
        }

        return searchNode(node->left, key);
    }

    template<typename K, typename T>
    T binarySearchTree<K, T>::min()
    {
        Node<K, T>* current = _root;

        // loop down to find the leftmost leaf
        while (current != NULL && current->left != NULL)
        {
            current = current->left;
        }

        return current->data;
    }

    template<typename K, typename T>
    K binarySearchTree<K, T>::minKey()
    {
        Node<K, T>* current = _root;

        // loop down to find the leftmost leaf
        while (current != NULL && current->left != NULL)
        {
            current = current->left;
        }

        return current->key;
    }

    template<typename K, typename T>
    Node<K, T>* binarySearchTree<K, T>::minNode(Node<K, T>* node)
    {
        // loop down to find the leftmost leaf
        while (node != NULL && node->left != NULL)
        {
            node = node->left;
        }

        return node;
    }

    template<typename K, typename T>
    Node<K, T>* binarySearchTree<K, T>::deleteImpl(Node<K, T>* node, K key)
    {
        if (node == NULL)
            return NULL;

        // If the data to be deleted is smaller than the root's
        // data, then it lies in the left subtree
        if (data < node->data)
        {
            node->left = deleteImpl(node->left, data);
        }

        // If the data to be deleted is greater than the root's
        // data, then it lies in the right subtree
        else if (data > root->data) 
        {
            node->right = deleteImpl(node->right, data);
        }

        // if data is same as root's data, then This is the node
        // to be deleted
        else 
        {
            // node with only one child or no child
            if (node->left == NULL)
            {
                Node<K, T>* temp = node->right;
                delete node;
                return temp;
            }
            else if (node->right == NULL) 
            {
                Node<K, T>* temp = node->left;
                delete node;
                return temp;
            }

            // node with two children: Get the inorder successor
            // (smallest in the right subtree)
            Node<K, T>* temp = minNode(node->right);

            // Copy the inorder successor's content to this node
            node->data = temp->data;

            // Delete the inorder successor
            node->right = deleteNode(node->right, temp->data);
        }

        return node;
    }
}


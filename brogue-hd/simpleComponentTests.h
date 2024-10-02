#pragma once

#include "brogueTestPackage.h"
#include "simpleArray.h"
#include "simpleArrayExtension.h"
#include "simpleList.h"
#include "simpleHash.h"
#include "simpleBST.h"

using namespace brogueHd::component;

namespace brogueHd::test
{
	template<typename T, typename K, typename V>
	class simpleComponentTests : public brogueTestPackage
	{
	public:
		simpleComponentTests() : brogueTestPackage("Simple Component Tests")
		{

		}
		~simpleComponentTests()
		{

		}

		simpleArray<T>* Array;
		simpleList<T>* List;
		simpleHash<K, V>* Hash;
		simpleBST<K, V>* BST;
	};

	template<typename T, typename K, typename V>
	simpleComponentTests<T, K, V>::simpleComponentTests()
	{
		this->Array = new simpleArray<T>();
		this->List = new simpleList<T>();
		this->Hash = new simpleHash<T>();
		this->BST = new simpleBST<T>();
	}

	template<typename T, typename K, typename V>
	simpleComponentTests<T, K, V>::~simpleComponentTests()
	{
		delete this->Array;
		delete this->List;
		delete this->Hash;
		delete this->BST;
	}
}
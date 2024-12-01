#pragma once

#include "brogueTestFunction.h"
#include "brogueTestPackage.h"
#include "simple.h"
#include "simpleBST.h"
#include "simpleArray.h"
#include "simpleString.h"
#include <functional>

using namespace brogueHd::simple;

namespace brogueHd::test
{
	class simpleBSTTests : public brogueTestPackage
	{
	public:

		simpleBSTTests() : brogueTestPackage("Simple BST Tree Tests")
		{
			// simpelBST<int, int>
			this->addTest(brogueTestFunction("simpleBSTTests_createAndBalance", std::bind(&simpleBSTTests::createAndBalance, this)));
		}
		~simpleBSTTests()
		{
		}

		void visualizeTree(const simpleBST<int, int>& tree)
		{
			simpleBSTNode<int, int>* root = tree.getRoot();

			output("Visualizing Tree:  ->");

			visualizeTreeRecurse(root, 0);
		}
		void visualizeTreeRecurse(const simpleBSTNode<int, int>* node, int level)
		{
			const char* messageFormat = "Node level {}: Key={} Value={} Height={}";

			output(simpleExt::format(messageFormat, level, node->getKey(), node->getValue(), node->getHeight()).c_str());

			if (node->getLeft() != nullptr)
				visualizeTreeRecurse(node->getLeft(), level + 1);

			if (node->getRight() != nullptr)
				visualizeTreeRecurse(node->getRight(), level + 1);
		}


		bool createAndBalance()
		{
			simpleBST<int, int> tree;

			tree.insert(0, 1);
			visualizeTree(tree);

			tree.insert(1, 1);
			visualizeTree(tree);

			tree.insert(8, 2);
			visualizeTree(tree);

			tree.insert(-2, 9);
			visualizeTree(tree);

			tree.insert(-3, 9);
			visualizeTree(tree);

			tree.insert(33, 2);
			visualizeTree(tree);

			tree.insert(35, 2);
			visualizeTree(tree);

			tree.insert(36, 2);
			visualizeTree(tree);

			this->testAssert("simpleBSTTests_createAndBalance:  Node Count", [&tree] ()
			{
				return tree.count() == 8;
			});

			return true;
		}
	};
}
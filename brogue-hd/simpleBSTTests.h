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

		bool createAndBalance()
		{
			simpleBST<int, int> tree;

			tree.insert(0, 1);
			this->output(tree.createOutput().c_str());

			tree.insert(1, 1);
			this->output(tree.createOutput().c_str());

			tree.insert(8, 2);
			this->output(tree.createOutput().c_str());

			tree.insert(-2, 9);
			this->output(tree.createOutput().c_str());

			tree.insert(-3, 9);
			this->output(tree.createOutput().c_str());

			tree.insert(33, 2);
			this->output(tree.createOutput().c_str());

			tree.insert(35, 2);
			this->output(tree.createOutput().c_str());

			tree.insert(36, 2);
			this->output(tree.createOutput().c_str());

			this->testAssert("simpleBSTTests_createAndBalance:  Node Count", [&tree] ()
			{
				return tree.count() == 8;
			});

			return true;
		}
	};
}
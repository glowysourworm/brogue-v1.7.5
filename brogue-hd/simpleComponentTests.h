#pragma once

#include "brogueObject.h"
#include "brogueTestFunction.h"
#include "brogueTestPackage.h"
#include "simpleHash.h"
#include <functional>

using namespace brogueHd::backend::model;
using namespace brogueHd::simple;

namespace brogueHd::test
{
	class simpleComponentTests : public brogueTestPackage
	{
	public:
		simpleComponentTests() : brogueTestPackage("Simple Component Tests")
		{
			this->addTest(brogueTestFunction("create_simpleHash_brogueObject", std::bind(&simpleComponentTests::create_simpleHash_brogueObject, this)));
		}
		~simpleComponentTests()
		{

		}
		bool create_simpleHash_brogueObject()
		{
			simpleHash<brogueObject, int> hash;

			hash.add(brogueObject(), 9);

			return true;
		}
	};
}
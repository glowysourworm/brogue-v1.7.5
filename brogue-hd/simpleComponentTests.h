#pragma once

#include "brogueObject.h"
#include <simpleTestFunction.h>
#include <simpleTestPackage.h>
#include <simpleHash.h>
#include <functional>

namespace brogueHd::test
{
	using namespace brogueHd::model;
	using namespace simple::test;
	using namespace simple;

	class brogueComponentTests : public simpleTestPackage
	{
	public:
		brogueComponentTests() : simpleTestPackage("Simple Component Tests")
		{
			this->addTest(simpleTestFunction("create_simpleHash_brogueObject",
			                                 std::bind(&brogueComponentTests::create_simpleHash_brogueObject, this)));
		}

		~brogueComponentTests()
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

#pragma once

#include "simple.h"
#include "simpleString.h"
#include <functional>

using namespace brogueHd::simple;

namespace brogueHd::test
{
	struct brogueTestFunction : simpleStruct
	{
		simpleString name;
		std::function<bool(void)> testRun;

		brogueTestFunction()
		{
		}
		brogueTestFunction(const char* aname, std::function<bool(void)> runFunc)
		{
			name = aname;
			testRun = runFunc;
		}
		bool operator==(const brogueTestFunction& other)
		{
			return name == other.name;
		}
		bool operator!=(const brogueTestFunction& other)
		{
			return name != other.name;
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(name);
		}
	};
}
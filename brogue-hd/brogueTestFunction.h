#pragma once

#include "simple.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::test
{
	struct brogueTestFunction : hashable
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

		size_t getHash() const override
		{
			return hashGenerator::generateHash(name);
		}
	};
}
#pragma once

#include "brogueTestFunction.h"
#include "simple.h"
#include "simpleHash.h"
#include "simpleLogger.h"
#include "simpleString.h"
#include <exception>
#include <functional>

using namespace brogueHd::simple;

namespace brogueHd::test
{
	using brogueTestAssertion = std::function<bool(void)>;

	class brogueTestPackage
	{
	public:
		brogueTestPackage(const char* name)
		{
			_tests = new simpleHash<simpleString, brogueTestFunction>();
			_batteryName = name;
		}
		brogueTestPackage(simpleString name)
		{
			_tests = new simpleHash<simpleString, brogueTestFunction>();
			_batteryName = name;
		}
		~brogueTestPackage()
		{
			delete _tests;
		}

		bool run()
		{
			brogueTestPackage* that = this;
			bool anyErrors = false;

			_tests->iterate([&that, &anyErrors] (simpleString key, brogueTestFunction value)
			{
				that->setCurrentTest(key);

				simpleLogger::log("Running Test:  {}", key.c_str());

				bool result = false;

				try
				{
					result = value.testRun();
				}
				catch (const std::exception& ex)
				{
					simpleLogger::logColor(brogueConsoleColor::Red, "Exception: {}", ex.what());

					result = false;
				}

				if (result)
					simpleLogger::logColor(brogueConsoleColor::Green, "Test {} Success!", key.c_str());
				else
					simpleLogger::logColor(brogueConsoleColor::Red, "Test {} Fail!", key.c_str());

				anyErrors &= result;

				return iterationCallback::iterate;
			});

			return anyErrors;
		}

		void testAssert(simpleString assertName, brogueTestAssertion assertion)
		{
			bool result = false;

			try
			{
				result = assertion();
			}
			catch (const std::exception& ex)
			{
				simpleLogger::logColor(brogueConsoleColor::Red, "Unit Test {} Assertion Exception {}:  {}", _currentTestName.c_str(), assertName.c_str(), ex.what());
				result = false;
			}

			if (!result)
				simpleLogger::logColor(brogueConsoleColor::Red, "Unit Test {} Assertion {} Failed!", _currentTestName.c_str(), assertName.c_str());
		}

		simpleString getName()
		{
			return _batteryName;
		}

	protected:

		void addTest(brogueTestFunction testDelegate)
		{
			_tests->add(testDelegate.name, testDelegate);
		}

		void setCurrentTest(simpleString testName)
		{
			_currentTestName = testName;
		}

	private:

		simpleHash<simpleString, brogueTestFunction>* _tests;

		simpleString _currentTestName;
		simpleString _batteryName;
	};
}
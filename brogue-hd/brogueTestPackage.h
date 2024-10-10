#pragma once

#include <functional>
#include "simpleLogger.h"
#include "simpleHash.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::test
{
	typedef std::function<bool(void)> brogueTestDelegate;
	typedef std::function<bool(void)> brogueTestAssertDelegate;

	class brogueTestPackage
	{
	public:
		brogueTestPackage(const char* name)
		{
			_tests = new simpleHash<simpleString, brogueTestDelegate>();
			_batteryName = name;
		}
		brogueTestPackage(simpleString name)
		{
			_tests = new simpleHash<simpleString, brogueTestDelegate>();
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
			
			_tests->iterate([&that, &anyErrors](simpleString key, brogueTestDelegate value)
			{
				that->setCurrentTest(key);

				simpleLogger::log("Running Test:  {}", key.c_str());

				bool result = false;

				try
				{
					 result = value();
				}
				catch (const std::exception& ex)
				{
					simpleLogger::logColor(brogueConsoleColor::Red, "Exception: {}", ex.what());

					result = false;
				}

				if (result)
					simpleLogger::logColor(brogueConsoleColor::Green, "Test {} Success!", key);
				else
					simpleLogger::logColor(brogueConsoleColor::Red, "Test {} Fail!", key);

				anyErrors &= result;

				return iterationCallback::iterate;
			});

			return anyErrors;
		}

		void testAssert(simpleString assertName, brogueTestAssertDelegate assertion)
		{
			bool result = false;

			try
			{
				result = assertion();
			}
			catch (const std::exception& ex)
			{
				simpleLogger::logColor(brogueConsoleColor::Red, "Unit Test {} Assertion Exception {}:  {}", _currentTestName, assertName, ex.what());
				result = false;
			}
				
			if (!result)
				simpleLogger::logColor(brogueConsoleColor::Red, "Unit Test {} Assertion {} Failed!", _currentTestName, assertName);
		}

		simpleString getName()
		{
			return _batteryName;
		}

	protected:

		void addTest(simpleString testName, brogueTestDelegate testDelegate)
		{
			_tests->add(testName, testDelegate);
		}

		void setCurrentTest(simpleString testName)
		{
			_currentTestName = testName;
		}

	private:

		simpleHash<simpleString, brogueTestDelegate>* _tests;

		simpleString _currentTestName;
		simpleString _batteryName;
	};
}
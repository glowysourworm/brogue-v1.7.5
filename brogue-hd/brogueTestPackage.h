#pragma once

#include <functional>
#include "brogueLogger.h"
#include "simpleHash.h"
#include "simpleString.h"

using namespace brogueHd::component;

namespace brogueHd::test
{
	typedef std::function<bool(void)> brogueTestDelegate;
	typedef std::function<bool(void)> brogueTestAssertDelegate;

	class brogueTestPackage
	{
	public:

		

	public:
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

				brogueLogger::log("Running Test:  " + key);

				bool result = false;

				try
				{
					 result = value();
				}
				catch (const std::exception& ex)
				{
					brogueLogger::logRed("Exception: " + simpleString(ex.what()));

					result = false;
				}

				if (result)
					brogueLogger::logGreen("Test " + key + " Success!");
				else
					brogueLogger::logRed("Test " + key + "Fail!");

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
				brogueLogger::logRed("Unit Test " + _currentTestName + " Assertion Exception" + assertName + ":  " + simpleString(ex.what()));
				result = false;
			}
				
			if (!result)
				brogueLogger::logRed("Unit Test " + _currentTestName + " Assertion " + assertName + " Failed!");
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
#pragma once

#include <functional>
#include "brogueLogger.h"
#include "simpleHash.h"

using namespace brogueHd::component;

namespace brogueHd::test
{
	typedef std::function<bool(void)> brogueTestDelegate;
	typedef std::function<bool(void)> brogueTestAssertDelegate;

	class brogueTestPackage
	{
	public:

		

	public:
		brogueTestPackage(std::string name)
		{
			_tests = new simpleHash<std::string, brogueTestDelegate>();
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
			
			_tests->iterate([&that, &anyErrors](std::string key, brogueTestDelegate value)
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
					brogueLogger::logRed("Exception: " + std::string(ex.what()));

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

		void testAssert(std::string assertName, brogueTestAssertDelegate assertion)
		{
			bool result = false;

			try
			{
				result = assertion();
			}
			catch (const std::exception& ex)
			{
				brogueLogger::logRed("Unit Test " + _currentTestName + " Assertion Exception" + assertName + ":  " + std::string(ex.what()));
				result = false;
			}
				
			if (!result)
				brogueLogger::logRed("Unit Test " + _currentTestName + " Assertion " + assertName + " Failed!");
		}

		std::string getName()
		{
			return _batteryName;
		}

	protected:

		void addTest(std::string testName, brogueTestDelegate testDelegate)
		{
			_tests->add(testName, testDelegate);
		}

		void setCurrentTest(std::string testName)
		{
			_currentTestName = testName;
		}

	private:

		simpleHash<std::string, brogueTestDelegate>* _tests;

		std::string _currentTestName;
		std::string _batteryName;
	};
}
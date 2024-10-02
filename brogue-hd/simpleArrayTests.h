#pragma once

#include "brogueTestPackage.h"
#include "simpleArray.h"

using namespace brogueHd::component;

namespace brogueHd::test
{
	class simpleArrayTests : public brogueTestPackage
	{
	public:

		simpleArrayTests() : brogueTestPackage("Simple Array Tests")
		{
			// simpleArray<int>
			this->addTest("instantiate_IntArray_OnStack", std::bind(&simpleArrayTests::instantiate_IntArray_OnStack, this));
			this->addTest("instantiate_IntArray_OnHeap_And_Delete", std::bind(&simpleArrayTests::instantiate_IntArray_OnHeap_And_Delete, this));
			this->addTest("intArray_OnStack_Set_Get", std::bind(&simpleArrayTests::intArray_OnStack_Set_Get, this));
			this->addTest("intArray_OnHeap_Set_Get_Delete", std::bind(&simpleArrayTests::intArray_OnHeap_Set_Get_Delete, this));
			this->addTest("intArray_OnStack_forEach", std::bind(&simpleArrayTests::intArray_OnStack_forEach, this));
			this->addTest("intArray_OnHeap_forEach", std::bind(&simpleArrayTests::intArray_OnHeap_forEach, this));

			// simpleArray<std::string>
			this->addTest("instantiate_StringArray_OnStack", std::bind(&simpleArrayTests::instantiate_StringArray_OnStack, this));
			this->addTest("instantiate_StringArray_OnHeap_And_Delete", std::bind(&simpleArrayTests::instantiate_StringArray_OnHeap_And_Delete, this));
			this->addTest("stringArray_OnStack_Set_Get", std::bind(&simpleArrayTests::stringArray_OnStack_Set_Get, this));
			this->addTest("stringArray_OnHeap_Set_Get_Delete", std::bind(&simpleArrayTests::stringArray_OnHeap_Set_Get_Delete, this));
			this->addTest("stringArray_OnStack_forEach", std::bind(&simpleArrayTests::stringArray_OnStack_forEach, this));
			this->addTest("stringArray_OnHeap_forEach", std::bind(&simpleArrayTests::stringArray_OnHeap_forEach, this));
		}
		~simpleArrayTests()
		{
		}


		bool instantiate_IntArray_OnStack()
		{
			simpleArray<int> theArray(10);

			return true;
		}

		bool instantiate_IntArray_OnHeap_And_Delete()
		{
			simpleArray<int>* theArray = new simpleArray<int>();

			delete theArray;

			return true;
		}

		bool intArray_OnStack_Set_Get()
		{
			simpleArray<int> theArray(10);

			theArray.set(2, 3);

			this->testAssert("intArray_OnStack_Set_Get", [&theArray]()
			{
				return theArray.get(2) == 3;
			});

			return true;
		}

		bool intArray_OnHeap_Set_Get_Delete()
		{
			simpleArray<int>* theArray = new simpleArray<int>(10);

			theArray->set(2, 3);

			this->testAssert("intArray_OnHeap_Set_Get_Delete", [&theArray]()
			{
				return theArray->get(2) == 3;
			});

			delete theArray;

			return true;
		}

		bool intArray_OnStack_forEach()
		{
			simpleArray<int> theArray(10);

			theArray.forEach([](int number)
			{
				return iterationCallback::iterate;
			});

			return true;
		}

		bool intArray_OnHeap_forEach()
		{
			simpleArray<int>* theArray = new simpleArray<int>(10);

			theArray->forEach([](int number)
			{
				return iterationCallback::iterate;
			});

			delete theArray;

			return true;
		}

		bool instantiate_StringArray_OnStack()
		{
			simpleArray<std::string> theArray(10);

			return true;
		}

		bool instantiate_StringArray_OnHeap_And_Delete()
		{
			simpleArray<std::string>* theArray = new simpleArray<std::string>();

			delete theArray;

			return true;
		}

		bool stringArray_OnStack_Set_Get()
		{
			simpleArray<std::string> theArray(10);

			theArray.set(2, "some string");

			this->testAssert("stringArray_OnStack_Set_Get", [&theArray]()
			{
				return theArray.get(2) == "some string";
			});

			return true;
		}

		bool stringArray_OnHeap_Set_Get_Delete()
		{
			simpleArray<std::string>* theArray = new simpleArray<std::string>(3);

			theArray->set(2, "some string");

			this->testAssert("stringArray_OnHeap_Set_Get_Delete", [&theArray]()
			{
				return theArray->get(2) == "some string";
			});

			delete theArray;

			return true;
		}

		bool stringArray_OnStack_forEach()
		{
			simpleArray<std::string> theArray(10);

			theArray.forEach([](std::string str)
			{
				return iterationCallback::iterate;
			});

			return true;
		}

		bool stringArray_OnHeap_forEach()
		{
			simpleArray<std::string>* theArray = new simpleArray<std::string>(7);

			theArray->forEach([](std::string str)
			{
				return iterationCallback::iterate;
			});

			delete theArray;

			return true;
		}
	};
}
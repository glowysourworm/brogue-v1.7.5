#pragma once

#include "brogueTestPackage.h"
#include "brogueTestFunction.h"
#include "simpleHash.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::test
{
	class simpleHashTests : public brogueTestPackage
	{
	public:

		simpleHashTests() : brogueTestPackage("Simple Hash Tests")
		{
			// simpleArray<int>
			this->addTest(brogueTestFunction("instantiate_IntInt_OnStack", std::bind(&simpleHashTests::instantiate_IntInt_OnStack, this)));
			this->addTest(brogueTestFunction("instantiate_IntInt_OnHeap_And_Delete", std::bind(&simpleHashTests::instantiate_IntInt_OnHeap_And_Delete, this)));
			this->addTest(brogueTestFunction("intInt_OnStack_Set_Get", std::bind(&simpleHashTests::intInt_OnStack_Set_Get, this)));
			this->addTest(brogueTestFunction("intInt_OnHeap_Set_Get_Delete", std::bind(&simpleHashTests::intInt_OnHeap_Set_Get_Delete, this)));
			this->addTest(brogueTestFunction("intInt_OnStack_iterate", std::bind(&simpleHashTests::intInt_OnStack_iterate, this)));
			this->addTest(brogueTestFunction("intInt_OnHeap_iterate", std::bind(&simpleHashTests::intInt_OnHeap_iterate, this)));

			// simpleHash<shaderResource>
			this->addTest(brogueTestFunction("cache_Retrieve_Shader_Resources", std::bind(&simpleHashTests::cache_Retrieve_Shader_Resources, this)));

			// simpleArray<simpleString>
			this->addTest(brogueTestFunction("instantiate_StringString_OnStack", std::bind(&simpleHashTests::instantiate_StringString_OnStack, this)));
			this->addTest(brogueTestFunction("instantiate_StringString_OnHeap_And_Delete", std::bind(&simpleHashTests::instantiate_StringString_OnHeap_And_Delete, this)));
			this->addTest(brogueTestFunction("stringString_OnStack_Set_Get", std::bind(&simpleHashTests::stringString_OnStack_Set_Get, this)));
			this->addTest(brogueTestFunction("stringString_OnHeap_Set_Get_Delete", std::bind(&simpleHashTests::stringString_OnHeap_Set_Get_Delete, this)));
			this->addTest(brogueTestFunction("stringString_OnStack_iterate", std::bind(&simpleHashTests::stringString_OnStack_iterate, this)));
			this->addTest(brogueTestFunction("stringString_OnHeap_iterate", std::bind(&simpleHashTests::stringString_OnHeap_iterate, this)));
		}
		~simpleHashTests()
		{
		}


		bool instantiate_IntInt_OnStack()
		{
			simpleHash<int, int> theHash();

			return true;
		}

		bool instantiate_IntInt_OnHeap_And_Delete()
		{
			simpleHash<int, int>* theHash = new simpleHash<int, int>();

			return true;
		}

		bool intInt_OnStack_Set_Get()
		{
			simpleHash<int, int> theHash;

			theHash.add(2, 3);

			this->testAssert("intInt_OnStack_Set_Get", [&theHash]()
			{
				return theHash.get(2) == 3;
			});

			return true;
		}

		bool intInt_OnHeap_Set_Get_Delete()
		{
			simpleHash<int, int>* theHash = new simpleHash<int, int>();

			theHash->add(2, 3);

			this->testAssert("intInt_OnHeap_Set_Get_Delete", [&theHash]()
			{
				return theHash->get(2) == 3;
			});

			return true;
		}

		bool intInt_OnStack_iterate()
		{
			simpleHash<int, int> theHash;

			theHash.add(2, 1);
			theHash.add(4, 4);

			theHash.iterate([](int key, int value)
			{
				return iterationCallback::iterate;
			});

			return true;
		}

		bool intInt_OnHeap_iterate()
		{
			simpleHash<int, int>* theHash = new simpleHash<int, int>();

			theHash->add(2, 3);

			theHash->iterate([](int key, int value)
			{
				return iterationCallback::iterate;
			});

			return true;
		}

		bool cache_Retrieve_Shader_Resources()
		{
			simpleHash<shaderResource, simpleString> shaders;

			shaders.add(shaderResource::brogueBaseFrag, "brogueBaseFrag");
			shaders.add(shaderResource::brogueBaseVert, "brogueBaseVert");
			shaders.add(shaderResource::brogueFrameFrag, "brogueFrameFrag");
			shaders.add(shaderResource::brogueFrameVert, "brogueFrameVert");
			shaders.add(shaderResource::brogueFlameMenuFrag, "brogueFlameMenuFrag");
			shaders.add(shaderResource::brogueFlameMenuVert, "brogueFlameMenuVert");

			simpleHashTests* that = this;

			shaders.iterate([&that] (shaderResource key, simpleString value)
			{
				switch (key)
				{
				case shaderResource::brogueBaseFrag:
					that->testAssert("Invalid Hash Key", [&value] ()
					{
						return value == "brogueBaseFrag";
					});
					break;
				case shaderResource::brogueBaseVert:
					that->testAssert("Invalid Hash Key", [&value] ()
					{
						return value == "brogueBaseVert";
					});
					break;
				case shaderResource::brogueFrameFrag:
					that->testAssert("Invalid Hash Key", [&value] ()
					{
						return value == "brogueFrameFrag";
					});
					break;
				case shaderResource::brogueFrameVert:
					that->testAssert("Invalid Hash Key", [&value] ()
					{
						return value == "brogueFrameVert";
					});
					break;
				case shaderResource::brogueFlameMenuFrag:
					that->testAssert("Invalid Hash Key", [&value] ()
					{
						return value == "brogueFlameMenuFrag";
					});
					break;
				case shaderResource::brogueFlameMenuVert:
					that->testAssert("Invalid Hash Key", [&value] ()
					{
						return value == "brogueFlameMenuVert";
					});
					break;
				}
				return iterationCallback::iterate;
			});

			return true;
		}

		bool instantiate_StringString_OnStack()
		{
			simpleHash<simpleString, simpleString> theHash();

			return true;
		}

		bool instantiate_StringString_OnHeap_And_Delete()
		{
			simpleHash<simpleString, simpleString>* theArray = new simpleHash<simpleString, simpleString>();

			delete theArray;

			return true;
		}

		bool stringString_OnStack_Set_Get()
		{
			simpleHash<simpleString, simpleString> theHash;

			theHash.add("2", "some string");

			this->testAssert("stringString_OnStack_Set_Get", [&theHash]()
			{
				return theHash.get("2") == "some string";
			});

			return true;
		}

		bool stringString_OnHeap_Set_Get_Delete()
		{
			simpleHash<simpleString, simpleString>* theHash = new simpleHash<simpleString, simpleString>();

			theHash->add("2", "some string");

			this->testAssert("stringString_OnHeap_Set_Get_Delete", [&theHash]()
			{
				return theHash->get("2") == "some string";
			});

			delete theHash;

			return true;
		}

		bool stringString_OnStack_iterate()
		{
			simpleHash<simpleString, simpleString> theHash;

			theHash.add("sdf", "wefefe");

			theHash.iterate([](simpleString key, simpleString value)
			{
				return iterationCallback::iterate;
			});

			return true;
		}

		bool stringString_OnHeap_iterate()
		{
			simpleHash<simpleString, simpleString>* theHash = new simpleHash<simpleString, simpleString>();

			theHash->add("some string key", "value");

			theHash->forEach([](simpleString key, simpleString value)
			{
				return iterationCallback::iterate;
			});

			delete theHash;

			return true;
		}
	};
}
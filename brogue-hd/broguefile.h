#pragma once

#include "gameConstants.h"
#include "simple.h"
#include "simpleList.h"
#include <iosfwd>

using namespace brogueHd::simple;
using namespace brogueHd::backend::modelConstant;

namespace brogueHd::backend::model::io
{
	typedef struct fileEntry {
		char* path;
		char date[100];
	} fileEntry;

	/// <summary>
	/// Simple keymap used to customize key processor functionality
	/// </summary>
	typedef struct keyMap {
		char inputCharacter;
		char outputCharacter;
	} keyMap;

	struct brogueEvent : hashable
	{
	public:

		enum BrogueEventType eventType;
		signed long param1;
		signed long param2;
		bool controlKey;
		bool shiftKey;


		/// <summary>
		/// Serializes public data to the output stream
		/// </summary>
		void serialize(std::ostream& stream)
		{
			// Fields (IN ORDER)
			//stream.write(reinterpret_cast<char*>(this->eventType), SIZEOF(BrogueEventType));
			//stream.write(reinterpret_cast<char*>(this->param1), SIZEOF(long));
			//stream.write(reinterpret_cast<char*>(this->param2), SIZEOF(long));
			//stream.write(reinterpret_cast<char*>(this->controlKey), SIZEOF(bool));
			//stream.write(reinterpret_cast<char*>(this->shiftKey), SIZEOF(bool));
		}

		static brogueEvent deserialize(std::istream& stream)
		{
			brogueEvent result;

			//stream.read(reinterpret_cast<char*>(&result.eventType), SIZEOF(BrogueEventType));
			//stream.read(reinterpret_cast<char*>(&result.param1), SIZEOF(long));
			//stream.read(reinterpret_cast<char*>(&result.param2), SIZEOF(long));
			//stream.read(reinterpret_cast<char*>(&result.controlKey), SIZEOF(bool));
			//stream.read(reinterpret_cast<char*>(&result.shiftKey), SIZEOF(bool));

			return result;
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(eventType, param1, param2, controlKey, shiftKey);
		}
	};

	struct brogueEventFile
	{
		unsigned long eventCount;
		simpleList<brogueEvent> eventList;

		void serialize(std::ostream& stream)
		{
			//// Fields (IN ORDER)
			//stream.write(reinterpret_cast<char*>(this->eventCount), SIZEOF(unsigned long));

			//// Event List
			//for (int index = 0; index < eventCount; index++)
			//{
			//	eventList[index].serialize(stream);
			//}
		}

		static brogueEventFile deserialize(std::istream& stream)
		{
			brogueEventFile result;

			//// Fields (IN ORDER)
			//stream.read(reinterpret_cast<char*>(&result.eventCount), SIZEOF(unsigned long));

			//// Event List
			//for (int index = 0; index < result.eventCount; index++)
			//{
			//	result.eventList.add(brogueEvent::deserialize(stream));
			//}

			return result;
		}
	};
}
#pragma once

#include "broguedef.h"
#include "gameConstants.h"
#include "simpleList.h"
#include <iostream>
#include <time.h>

using namespace std;

using namespace brogueHd::component;

namespace brogueHd::backend::model::io
{
	typedef struct fileEntry {
		char* path;
		char date[100];
	} fileEntry;

	struct brogueScoreEntry
	{

		long int score;
		time_t dateTime;
		char description[COLS];

		brogueScoreEntry() {}
		brogueScoreEntry(long int score, time_t dateTime, char description[COLS])
		{
			this->score = score;
			this->dateTime = dateTime;

			STRCPY(this->description, description, COLS);
		}
		~brogueScoreEntry()
		{

		}

		void serialize(ostream& stream)
		{
			// Fields (IN ORDER)
			//stream.write(reinterpret_cast<char*>(this->score), SIZEOF(long int));
			//stream.write(reinterpret_cast<char*>(this->dateTime), SIZEOF(time_t));
			//stream.write(reinterpret_cast<char*>(this->description), SIZEOF(char) * COLS);
		}

		static brogueScoreEntry deserialize(istream& stream)
		{
			brogueScoreEntry result;

			//stream.read(reinterpret_cast<char*>(&result.score), SIZEOF(long int));
			//stream.read(reinterpret_cast<char*>(&result.dateTime), SIZEOF(time_t));
			//stream.read(reinterpret_cast<char*>(&result.description), SIZEOF(char) * COLS);

			return result;
		}

	};

	/// <summary>
	/// Simple keymap used to customize key processor functionality
	/// </summary>
	typedef struct keyMap {
		char inputCharacter;
		char outputCharacter;
	} keyMap;

	struct brogueEvent
	{

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
		}
	};

	struct brogueScoresFile 
	{

		unsigned long scoreCount;
		simpleList<brogueScoreEntry> scoreList;

		brogueScoresFile()
		{

		}
		~brogueScoresFile()
		{

		}

		void add(brogueScoreEntry entry)
		{
			scoreList.add(entry);
		}

		void serialize(std::ostream& stream)
		{
			//// Fields (IN ORDER)
			//stream.write(reinterpret_cast<char*>(this->scoreCount), SIZEOF(unsigned long));

			//// Event List
			//for (int index = 0; index < scoreCount; index++)
			//{
			//	scoreList[index].serialize(stream);
			//}
		}

		static brogueScoresFile* deserialize(std::istream& stream)
		{
			brogueScoresFile* result = new brogueScoresFile();

			//// Fields (IN ORDER)
			//stream.read(reinterpret_cast<char*>(&result->scoreCount), SIZEOF(unsigned long));

			//// Event List
			//for (int index = 0; index < result->scoreCount; index++)
			//{
			//	result->scoreList.add(brogueScoreEntry::deserialize(stream));
			//}

			return result;
		}

	};
}
#pragma once

#include "broguedef.h"
#include "gameConstants.h"
#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

namespace brogueHd
{
	typedef struct brogueEventFile {

		unsigned long eventCount;
		std::vector<brogueEvent> eventList;

	public:

		void serialize(std::ostream& stream)
		{
			// Fields (IN ORDER)
			stream.write(reinterpret_cast<char*>(this->eventCount), SIZEOF(unsigned long));

			// Event List
			for (int index = 0; index < eventCount; index++)
			{
				eventList[index].serialize(stream);
			}
		}

		static brogueEventFile deserialize(std::istream& stream)
		{
			brogueEventFile result;

			// Fields (IN ORDER)
			stream.read(reinterpret_cast<char*>(&result.eventCount), SIZEOF(unsigned long));

			// Event List
			for (int index = 0; index < result.eventCount; index++)
			{
				result.eventList.push_back(brogueEvent::deserialize(stream));
			}
		}

	} brogueEventFile;

	typedef struct brogueScoresFile {

		unsigned long scoreCount;
		std::vector<brogueScoreEntry> scoreList;

	public:

		brogueScoresFile()
		{

		}
		~brogueScoresFile()
		{

		}

		void add(brogueScoreEntry entry)
		{
			scoreList.push_back(entry);
		}

		void serialize(std::ostream& stream)
		{
			// Fields (IN ORDER)
			stream.write(reinterpret_cast<char*>(this->scoreCount), SIZEOF(unsigned long));

			// Event List
			for (int index = 0; index < scoreCount; index++)
			{
				scoreList[index].serialize(stream);
			}
		}

		static brogueScoresFile* deserialize(std::istream& stream)
		{
			brogueScoresFile* result = new brogueScoresFile();

			// Fields (IN ORDER)
			stream.read(reinterpret_cast<char*>(&result->scoreCount), SIZEOF(unsigned long));

			// Event List
			for (int index = 0; index < result->scoreCount; index++)
			{
				result->scoreList.push_back(brogueScoreEntry::deserialize(stream));
			}

			return result;
		}

	} brogueEventFile;

	typedef struct brogueEvent {

		enum BrogueEventType eventType;
		signed long param1;
		signed long param2;
		boolean controlKey;
		boolean shiftKey;

	public:

		/// <summary>
		/// Serializes public data to the output stream
		/// </summary>
		void serialize(std::ostream& stream)
		{
			// Fields (IN ORDER)
			stream.write(reinterpret_cast<char*>(this->eventType), SIZEOF(BrogueEventType));
			stream.write(reinterpret_cast<char*>(this->param1), SIZEOF(long));
			stream.write(reinterpret_cast<char*>(this->param2), SIZEOF(long));
			stream.write(reinterpret_cast<char*>(this->controlKey), SIZEOF(boolean));
			stream.write(reinterpret_cast<char*>(this->shiftKey), SIZEOF(boolean));
		}

		static brogueEvent deserialize(std::istream& stream)
		{
			brogueEvent result;

			stream.read(reinterpret_cast<char*>(&result.eventType), SIZEOF(BrogueEventType));
			stream.read(reinterpret_cast<char*>(&result.param1), SIZEOF(long));
			stream.read(reinterpret_cast<char*>(&result.param2), SIZEOF(long));
			stream.read(reinterpret_cast<char*>(&result.controlKey), SIZEOF(boolean));
			stream.read(reinterpret_cast<char*>(&result.shiftKey), SIZEOF(boolean));

			return result;
		}

	} rogueEvent;

	typedef struct fileEntry {
		char* path;
		char date[100];
	} fileEntry;

	typedef struct brogueScoreEntry {

		long int score;
		time_t dateTime;
		char description[COLS];

	public:

		brogueScoreEntry(long int score, time_t dateTime, char description[COLS])
		{
			this->score = score;
			this->dateTime = dateTime;

			strncpy(this->description, description, COLS);
		}
		~brogueScoreEntry()
		{

		}

		void serialize(ostream& stream)
		{
			// Fields (IN ORDER)
			stream.write(reinterpret_cast<char*>(this->score), SIZEOF(long int));
			stream.write(reinterpret_cast<char*>(this->dateTime), SIZEOF(time_t));
			stream.write(reinterpret_cast<char*>(this->description), SIZEOF(char) * COLS);
		}

		static brogueScoreEntry deserialize(istream& stream)
		{
			brogueScoreEntry result;

			stream.read(reinterpret_cast<char*>(&result.score), SIZEOF(long int));
			stream.read(reinterpret_cast<char*>(&result.dateTime), SIZEOF(time_t));
			stream.read(reinterpret_cast<char*>(&result.description), SIZEOF(char) * COLS);

			return result;
		}

	} brogueScoreEntry;

	/// <summary>
	/// Simple keymap used to customize key processor functionality
	/// </summary>
	typedef struct keyMap {
		char inputCharacter;
		char outputCharacter;
	} keyMap;
}
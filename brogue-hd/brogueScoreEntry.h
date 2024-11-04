#pragma once

#include "simple.h"
#include "simpleExt.h"
#include "simpleString.h"
#include <corecrt.h>

using namespace brogueHd::simple;

namespace brogueHd::backend::model::io
{
	class brogueScoreEntry : hashableObject
	{
	public:

		brogueScoreEntry()
		{
			_score = 0;
			_date = nullptr;
			_description = nullptr;
		}
		brogueScoreEntry(const brogueScoreEntry& copy)
		{
			copyImpl(copy);
		}
		brogueScoreEntry(int score, time_t dateTime, const char* description)
		{
			_score = score;
			_date = new simpleString(simpleExt::formatDate(dateTime, true));
			_description = new simpleString(description);
		}
		~brogueScoreEntry()
		{
			delete _date;
			delete _description;
		}

		void operator=(const brogueScoreEntry& copy)
		{
			delete _date;
			delete _description;

			copyImpl(copy);
		}

		static int compare(const brogueScoreEntry& item1, const brogueScoreEntry& item2)
		{
			return item1.getScore() - item2.getScore();
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(_date, _description, _score);
		}

		int getScore() const
		{
			return _score;
		}

		simpleString* getDate() const
		{
			return _date;
		}

		simpleString* getDescription() const
		{
			return _description;
		}

	private:

		void copyImpl(const brogueScoreEntry& copy)
		{
			_score = copy.getScore();
			_date = copy.getDate();
			_description = copy.getDescription();
		}

	private:

		int _score;
		simpleString* _date;
		simpleString* _description;

	};
}
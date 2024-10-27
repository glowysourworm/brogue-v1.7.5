#pragma once

#include "simpleArray.h"
#include "color.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::model::game
{
	struct colorString : hashable
	{
	public:

		colorString()
		{
			_characters = new simpleArray<char>();
			_colors = new simpleArray<color>();
		}
		colorString(const char* message, color foreColor)
		{
			_characters = new simpleArray<char>(message, strnlen_s(message, COLS * 2));
			_colors = new simpleArray<color>(_characters->count());
		}
		colorString(const colorString& copy)
		{
			copyImpl(copy);
		}
		~colorString()
		{
			delete _characters;
			delete _colors;
		}
		void operator=(const colorString& copy)
		{
			copyImpl(copy);
		}
		bool operator==(const colorString& other)
		{
			return compare(other);
		}
		bool operator!=(const colorString& other)
		{
			return !compare(other);
		}
		char getChar(short index) const
		{
			return _characters->get(index);
		}
		color getColor(short index) const
		{
			return _colors->get(index);
		}
		void set(short index, char character, color color)
		{
			_characters->set(index, character);
			_colors->set(index, color);
		}
		void setColor(short index, color color)
		{
			_colors->set(index, color);
		}
		int getCount() const
		{
			return _characters->count();
		}
		size_t getHash() const override
		{
			size_t characterHash = _characters->getHash();
			size_t colorHash = _colors->getHash();

			return hashGenerator::combineHash(characterHash, colorHash);
		}

	private:

		void copyImpl(const colorString& copy)
		{
			delete _characters;
			delete _colors;

			_characters = new simpleArray<char>(copy.getCount());
			_colors = new simpleArray<color>(copy.getCount());

			for (int index = 0; index < copy.getCount(); index++)
			{
				_characters->set(index, copy.getChar(index));
				_colors->set(index, copy.getColor(index));
			}
		}

		bool compare(const colorString& other)
		{
			if (this->getCount() != other.getCount())
				return false;

			for (int index = 0; index < other.getCount(); index++)
			{
				if (this->getChar(index) != other.getChar(index))
					return false;

				if (this->getColor(index) != other.getColor(index))
					return false;
			}

			return true;
		}

	private:

		simpleArray<char>* _characters;
		simpleArray<color>* _colors;
	};
}
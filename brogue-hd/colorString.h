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

		// Character that corresponds to the empty char and also works with the
		// glyph sheets.
		static const char Empty = ' ';

	public:

		colorString()
		{
			_characters = new simpleArray<int>();
			_colors = new simpleArray<color>();
		}
		colorString(const char* message)
		{
			_characters = new simpleArray<int>(strnlen_s(message, COLS * 2));
			_colors = new simpleArray<color>(_characters->count());

			for (int index = 0; index < _characters->count(); index++)
			{
				// int should include unicode characters
				_characters->set(index, (int)message[index]);
				_colors->set(index, colors::white());
			}
		}
		colorString(const char* message, color foreColor)
		{
			_characters = new simpleArray<int>(strnlen_s(message, COLS * 2));
			_colors = new simpleArray<color>(_characters->count());

			for (int index = 0; index < _characters->count(); index++)
			{
				// int should include unicode characters
				_characters->set(index, (int)message[index]);
				_colors->set(index, foreColor);
			}
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

			_characters = new simpleArray<int>(copy.getCount());
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

		simpleArray<int>* _characters;
		simpleArray<color>* _colors;
	};
}
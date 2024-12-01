#pragma once

#include "brogueGlobal.h"
#include "color.h"
#include "simple.h"
#include "simpleArray.h"
#include "simpleString.h"
#include <cstring>

using namespace brogueHd::simple;

namespace brogueHd::backend::model
{
	struct colorString : simpleStruct
	{
	public:

		colorString()
		{
			_characters = new simpleArray<int>();
			_colors = new simpleArray<color>();
		}
		colorString(int capacity)
		{
			_characters = new simpleArray<int>(capacity);
			_colors = new simpleArray<color>(capacity);
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
			_characters = new simpleArray<int>(copy.getCount());
			_colors = new simpleArray<color>(copy.getCount());

			copyImpl(copy);
		}
		~colorString()
		{
			delete _characters;
			delete _colors;
		}
		void operator=(const colorString& copy)
		{
			if (_characters != nullptr)
			{
				delete _characters;
				delete _colors;
			}

			_characters = new simpleArray<int>(copy.getCount());
			_colors = new simpleArray<color>(copy.getCount());

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
		simpleString getString() const
		{
			simpleString result(_characters->count());

			for (int index = 0; index < _characters->count(); index++)
			{
				result.set(index, (char)_characters->get(index));
			}

			return result;
		}
		simpleString getSubstring(int startIndex, int length) const
		{
			simpleString result = getString();

			return result.subString(startIndex, length);
		}
		char getChar(int index) const
		{
			return _characters->get(index);
		}
		color getColor(int index) const
		{
			return _colors->get(index);
		}
		void set(int index, char character, const color& color)
		{
			_characters->set(index, character);
			_colors->set(index, color);
		}
		void setColor(int index, const color& color)
		{
			_colors->set(index, color);
		}
		void setColorRange(int index, int count, const color& color)
		{
			for (int i = index; i < index + count; i++)
			{
				_colors->set(i, color);
			}
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
			// The arrays must already be new'd in the constructor
			// for this length.

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
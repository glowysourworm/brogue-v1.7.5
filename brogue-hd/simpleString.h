#pragma once

#include "simple.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleList.h"
#include <cctype>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <exception>
#include <iosfwd>
#include <iostream>
#include <string>

namespace brogueHd::simple
{
	class simpleString : public simpleObject
	{
	protected:

		/// <summary>
		/// This should be taken from the std library or boost. I wasn't sure of the limit; but the
		/// strlen_s function asked for one.
		/// </summary>
		static const size_t MAX_LENGTH = 1000000;
		static const size_t MAX_SPLIT_TOKENS = 10;
		static const char NULL_CHAR = '\0';

	public:

		simpleString();
		simpleString(int count);
		simpleString(const char character);
		simpleString(const char* chars);
		simpleString(const std::string& stdStr);
		simpleString(const simpleString& copy);
		~simpleString();

		void operator=(const simpleString& other);
		void operator=(const char* other);

		bool operator==(const simpleString& other) const;
		bool operator!=(const simpleString& other) const;

		//simpleString operator+(const simpleString& other) const;
		//simpleString operator+(const char* other) const;

		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;

		char get(int index) const;
		int count() const;
		bool isEmpty() const;
		bool isEmptyOrWhiteSpace() const;
		int search(const char* search) const;
		int search(const simpleString& search) const;

		bool endsWith(const char* chars) const;

		void set(int index, char value);
		void clear();

		void append(const simpleString& other);
		void append(const char* chars);
		void appendLine(const simpleString& other);
		void appendLine(const char* chars);

		void appendPadding(char padChar, int length);
		void insert(int index, const char* replacement);
		void insert(int index, const simpleString& replacement);
		void remove(int index, int count);
		void replace(const simpleString& string, const simpleString& replacement);
		void replace(const char* string, const char* replacement);
		void replaceFirst(const simpleString& string, const simpleString& replacement);

		simpleString subString(int index, int count) const;
		simpleArray<simpleString> split(char token) const;
		simpleArray<simpleString> split(const char* tokens) const;

		simpleString toUpper() const;

		const char* c_str() const;

	public: // Hashable Object

		size_t getHash() const override;

	public:

		//template<isStringConvertible T, isStringConvertible...Rest>
		//static simpleString format(const simpleString& formatStr, const T& param, Rest...rest);

		friend std::ostream& operator<<(std::ostream& stream, const simpleString& other);
		friend std::istream& operator>>(std::istream& stream, simpleString& other);

		//friend simpleString& operator+(const simpleString& left, const simpleString& right);
		//friend simpleString& operator+(const char* left, const simpleString& right);
		//friend simpleString& operator+(const simpleString& left, const char* right);

		int toInt();
		bool tryToInt(int& result);
		long toLong();
		short toShort();
		float toFloat();
		double toDouble();
		bool toBool();

	private:

		int searchImpl(int startIndex, const simpleString& search) const;

		void copyImpl(const simpleString& copy);
		void copyImpl(const char* copy);

		bool compare(const simpleString& other) const;
		bool compareArray(const char* other) const;

	private:

		simpleArray<char>* _array;
	};

	simpleString::simpleString()
	{
		// Pin down array value
		_array = new simpleArray<char>(0);
	}
	simpleString::simpleString(int count)
	{
		if (count >= this->MAX_LENGTH)
			throw simpleException("Trying to declare simpleString greater than 1MB");

		_array = new simpleArray<char>(count);
	}
	simpleString::simpleString(const char character)
	{
		_array = new simpleArray<char>(1);

		_array->set(0, character);
	}
	simpleString::simpleString(const char* chars)
	{
		// Pin down array value
		_array = new simpleArray<char>(0);

		this->copyImpl(chars);
	}
	simpleString::simpleString(const std::string& stdStr)
	{
		_array = new simpleArray<char>(stdStr.size());

		for (int index = 0; index < _array->count(); index++)
		{
			_array->set(index, stdStr[index]);
		}
	}
	simpleString::simpleString(const simpleString& copy)
	{
		// Pin down array value
		_array = new simpleArray<char>(0);

		this->copyImpl(copy);
	}
	simpleString::~simpleString()
	{
		delete _array;
	}

	void simpleString::operator=(const simpleString& copy)
	{
		// Pin down array value
		_array = new simpleArray<char>(0);

		this->copyImpl(copy);
	}
	void simpleString::operator=(const char* copy)
	{
		// Pin down array value
		_array = new simpleArray<char>(0);

		this->copyImpl(copy);
	}

	bool simpleString::operator==(const simpleString& other) const
	{
		return compare(other);
	}
	bool simpleString::operator!=(const simpleString& other) const
	{
		return !compare(other);
	}
	bool simpleString::operator==(const char* other) const
	{
		return compareArray(other);
	}
	bool simpleString::operator!=(const char* other) const
	{
		return !compareArray(other);
	}

	//simpleString simpleString::operator+(const simpleString& other) const
	//{
	//	simpleString result(_array->getArray());

	//	result.append(other);

	//	return result;
	//}
	//simpleString simpleString::operator+(const char* other) const
	//{
	//	simpleString result(_array->getArray());

	//	result.append(other);

	//	return result;
	//}

	std::ostream& operator<<(std::ostream& stream, const simpleString& other)
	{
		if (other == nullptr)
			return stream;

		stream << other.c_str();

		return stream;
	}
	std::istream& operator>>(std::istream& stream, simpleString& other)
	{
		if (other == nullptr)
			return stream;

		std::string line;

		stream >> line;

		other.append(line.c_str());

		return stream;

		//// Compiler will try to build an instance of simpleString on the stack
		////
		//if (other == default_value<simpleString>::value)
		//	stream.setstate(std::ios::failbit);

		//// Probably nothing to do. Use breakpoint to make sure data made it 
		//// into the simpleString
		////
		//if (true)
		//{
		//	bool isOtherGood;
		//}

		//return stream;
	}

	bool simpleString::endsWith(const char* chars) const
	{
		if (chars == nullptr)
			throw simpleException("Trying to compare a string with a nullptr:  simpleString::endsWith");

		simpleString charsStr(chars);

		if (charsStr.count() > this->count())
			return false;

		int difference = this->count() - charsStr.count();

		for (int index = charsStr.count() - 1; index >= 0; index--)
		{
			if (this->get(index + difference) != charsStr.get(index))
				return false;
		}

		return true;
	}

	bool simpleString::compare(const simpleString& other) const
	{
		// NEED DEFAULT VALUE HANDLERS
		if (other == nullptr)
			return false;

		else if (_array == nullptr)
			return other.count() == 0;

		else if (_array->count() != other.count())
			return false;

		for (int index = 0; index < _array->count(); index++)
		{
			if (_array->get(index) != other.get(index))
				return false;
		}

		return true;
	}
	bool simpleString::compareArray(const char* other) const
	{
		if (_array == nullptr)
			return (other == nullptr);

		else if (other == nullptr)
			return (_array == nullptr);

		else if (strnlen_s(other, _array->count()) != _array->count())
			return false;

		for (int index = 0; index < _array->count(); index++)
		{
			if (_array->get(index) != other[index])
				return false;
		}

		return true;
	}

	void simpleString::append(const simpleString& other)
	{
		this->append(other.c_str());
	}
	void simpleString::append(const char* other)
	{
		int otherCount = strnlen_s(other, this->MAX_LENGTH);

		// Handle default case
		if (this->isEmptyOrWhiteSpace())
		{
			delete _array;

			_array = new simpleArray<char>(other, otherCount);
		}
		else
		{
			int count = _array->count() + otherCount;

			simpleArray<char>* newArray = new simpleArray<char>(count);

			// Copy over data to new array
			for (int index = 0; index < _array->count(); index++)
				newArray->set(index, _array->get(index));

			for (int index = _array->count(); index < count; index++)
				newArray->set(index, other[index - _array->count()]);

			delete _array;

			_array = newArray;
		}
	}
	void simpleString::appendLine(const simpleString& other)
	{
		this->append(other);
		this->appendPadding('\n', 1);
	}
	void simpleString::appendLine(const char* chars)
	{
		this->append(chars);
		this->appendPadding('\n', 1);
	}
	void simpleString::appendPadding(char padChar, int length)
	{
		// TODO: Figure out how to create an empty space string without garbage 
		//		 and / or heap corruption.
		//
		for (int index = 0; index < length; index++)
		{
			this->append(padChar);
		}
	}

	int simpleString::search(const char* search) const
	{
		return this->search(simpleString(search));
	}
	int simpleString::search(const simpleString& search) const
	{
		return this->searchImpl(0, search);
	}
	int simpleString::searchImpl(int startIndex, const simpleString& search) const
	{
		if (search.count() == 0)
			throw simpleException("Trying to search for zero-count string simpleString::search");

		int firstIndex = -1;

		for (int index = startIndex; index < _array->count(); index++)
		{
			if (firstIndex == -1)
			{
				// Found the beginning character
				if (_array->get(index) == search.get(0))
					firstIndex = index;
			}

			// Tracking a possible match
			else
			{
				// Found all characters!
				if (index - firstIndex >= search.count())
				{
					return firstIndex;
				}

				// Found mismatch (reset)
				else if (_array->get(index) != search.get(index - firstIndex))
				{
					firstIndex = -1;
				}

				else
				{
					// Nothing to do
				}
			}
		}

		return firstIndex;
	}

	void simpleString::copyImpl(const simpleString& copy)
	{
		if (copy == nullptr)
			throw simpleException("Trying to declare simpleString from nullptr:  simpleString.h");

		if (copy.count() >= this->MAX_LENGTH)
			throw simpleException("Trying to declare simpleString greater than 1MB");

		delete _array;

		_array = new simpleArray<char>(copy.count());

		for (int index = 0; index < copy.count(); index++)
			_array->set(index, copy.get(index));
	}

	void simpleString::copyImpl(const char* copy)
	{
		if (copy == nullptr)
			throw simpleException("Trying to declare simpleString from nullptr:  simpleString.h");

		size_t length = strnlen_s(copy, this->MAX_LENGTH);

		if (length >= this->MAX_LENGTH)
			throw simpleException("Trying to declare simpleString greater than 1MB");

		delete _array;

		_array = new simpleArray<char>(copy, length);
	}

	void simpleString::insert(int index, const char* replacement)
	{
		this->insert(index, simpleString(replacement));
	}
	void simpleString::insert(int index, const simpleString& replacement)
	{
		if (index >= _array->count())
			throw simpleException("Index for insert was outside the bounds of the array:  simpleString.cpp");

		simpleArray<char>* newArray = new simpleArray<char>(_array->count() + replacement.count());

		// Iterate the new array indices
		for (int i = 0; i < newArray->count(); i++)
		{
			// Inside replacement indices
			if (i >= index && i < index + replacement.count())
			{
				newArray->set(i, replacement.get(i - index));
			}
			else if (i >= index)
			{
				newArray->set(i, _array->get(i - replacement.count()));
			}
			else
				newArray->set(i, _array->get(i));
		}

		delete _array;

		_array = newArray;
	}
	void simpleString::remove(int index, int count)
	{
		if (index + count > _array->count())
			throw simpleException("Index for remove was outside the bounds of the array:  simpleString.cpp");

		simpleArray<char>* newArray = new simpleArray<char>(_array->count() - count);

		// Iterate current array
		for (int i = 0; i < _array->count(); i++)
		{
			// Inside the removed character indices
			if (i >= index && (i - index) < count)
			{
				continue;
			}
			else if (i >= index)
			{
				newArray->set(i - count, _array->get(i));
			}
			else
				newArray->set(i, _array->get(i));
		}

		// Replace with the new array
		delete _array;

		_array = newArray;
	}
	void simpleString::replace(const simpleString& string, const simpleString& replacement)
	{
		int searchIndex = this->search(string);

		while (searchIndex != -1)
		{
			this->remove(searchIndex, string.count());
			this->insert(searchIndex, replacement);

			if (searchIndex + replacement.count() > _array->count())
				break;

			// Increment past the last replacement
			else
				searchIndex = this->searchImpl(searchIndex + replacement.count(), string);
		}
	}
	void simpleString::replace(const char* string, const char* replacement)
	{
		this->replace(simpleString(string), simpleString(replacement));
	}
	void simpleString::replaceFirst(const simpleString& string, const simpleString& replacement)
	{
		int searchIndex = this->search(string);

		if (searchIndex >= 0)
		{
			this->remove(searchIndex, string.count());

			if (searchIndex == this->count())
				this->append(replacement);

			else
				this->insert(searchIndex, replacement);
		}
	}

	char simpleString::get(int index) const
	{
		return _array->get(index);
	}
	int simpleString::count() const
	{
		return _array->count();
	}
	bool simpleString::isEmpty() const
	{
		return _array->count() == 0 ||
			_array->areAll('\0');
	}
	bool simpleString::isEmptyOrWhiteSpace() const
	{
		return _array->count() == 0 ||
			_array->areAllWhere([] (char value)
		{
			return value == '\0' || std::isspace(value);
		});
	}

	void simpleString::set(int index, char value)
	{
		_array->set(index, value);
	}

	void simpleString::clear()
	{
		// Delete and reallocate
		if (_array != nullptr)
		{
			delete _array;
		}

		_array = new simpleArray<char>(0);
	}

	simpleString simpleString::subString(int index, int count) const
	{
		if (index + count > _array->count())
			throw simpleException("Index outside the bounds of the array: simpleArray::subString");

		simpleString result(count);

		for (int i = index; i < index + count; i++)
			result.set(i - index, _array->get(i));

		return result;
	}

	simpleArray<simpleString> simpleString::split(char token) const
	{
		simpleList<simpleString> result;

		int lastIndex = -1;

		for (int strIndex = 0; strIndex < _array->count(); strIndex++)
		{
			// Found token!
			if (_array->get(strIndex) == token)
			{
				simpleString subString = (lastIndex == -1) ? this->subString(0, strIndex) : this->subString(lastIndex + 1, strIndex - lastIndex - 1);

				result.add(subString);

				lastIndex = strIndex;
			}
		}

		// Add the last sub-string
		if (lastIndex != -1)
			result.add(this->subString(lastIndex + 1, _array->count() - lastIndex - 1));

		else
			result.add(this->subString(0, _array->count()));	// Take entire substring on stack

		// Prune for empty strings
		for (int index = result.count() - 1; index >= 0; index--)
		{
			if (result.get(index).isEmptyOrWhiteSpace())
				result.removeAt(index);
		}

		return result.toArray();
	}

	simpleArray<simpleString> simpleString::split(const char* tokens) const
	{
		size_t length = strnlen_s(tokens, this->MAX_SPLIT_TOKENS);

		if (length > this->MAX_SPLIT_TOKENS)
			throw simpleException("Cannot have simpleString::split const char* tokens length greater than 30");

		simpleList<simpleString> result;
		simpleList<simpleString> list;

		result.add(this->subString(0, _array->count()));

		for (int index = 0; index < length; index++)
		{
			char token = tokens[index];

			list.clear();

			// Iterate the result list, swap back to list
			for (int index = 0; index < result.count(); index++)
			{
				simpleArray<simpleString> strings = result.get(index).split(token);

				list.addRange(strings.getArray(), strings.count());
			}

			result.clear();
			result.addRange(list);
			list.clear();
		}

		// Prune for empty strings
		for (int index = result.count() - 1; index >= 0; index--)
		{
			if (result.get(index).isEmptyOrWhiteSpace())
				result.removeAt(index);
		}

		return result.toArray();
	}

	simpleString simpleString::toUpper() const
	{
		simpleString result(_array->count());

		for (int index = 0; index < result.count(); index++)
			result.set(index, toupper(_array->get(index)));

		return result;
	}

	int simpleString::toInt()
	{
		return atoi(_array->getArray());
	}
	bool simpleString::tryToInt(int& result)
	{
		try
		{
			result = this->toInt();

			return true;
		}
		catch (std::exception ex)
		{
			return false;
		}
	}
	long simpleString::toLong()
	{
		return atol(_array->getArray());
	}
	short simpleString::toShort()
	{
		return (short)atoi(_array->getArray());
	}
	float simpleString::toFloat()
	{
		return atof(_array->getArray());
	}
	double simpleString::toDouble()
	{
		return (double)atof(_array->getArray());
	}
	bool simpleString::toBool()
	{
		simpleString upper = this->toUpper();

		if (upper == "TRUE")
			return true;

		return false;
	}

	const char* simpleString::c_str() const
	{
		return _array->getArray();
	}

	size_t simpleString::getHash() const
	{
		return _array->getHash();
	}
}
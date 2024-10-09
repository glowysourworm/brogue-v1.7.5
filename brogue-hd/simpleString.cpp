#pragma once
#include "simpleString.h"

namespace brogueHd::component
{
	simpleString::simpleString()
	{
		_array = NULL;
	}
	simpleString::simpleString(int count)
	{
		if (count >= this->MAX_LENGTH)
			brogueException::show("Trying to declare simpleString greater than 1MB");

		_array = new simpleArray<char>(count);
	}
	simpleString::simpleString(const char* chars)
	{
		if (strnlen_s(chars, this->MAX_LENGTH) >= this->MAX_LENGTH)
			brogueException::show("Trying to declare simpleString greater than 1MB");

		_array = new simpleArray<char>(chars);
	}
	simpleString::simpleString(const simpleString& copy)
	{
		if (copy.count() >= this->MAX_LENGTH)
			brogueException::show("Trying to declare simpleString greater than 1MB");

		_array = new simpleArray<char>(copy.c_str());
	}
	simpleString::~simpleString()
	{
		if (_array != NULL)
			delete _array;
	}

	void simpleString::operator=(const simpleString& copy)
	{
		if (_array != NULL)
			delete _array;

		_array = new simpleArray<char>(copy.c_str());
	}
	void simpleString::operator+=(const simpleString& other)
	{
		if (_array == NULL)
			brogueException::show("Trying to append to a null array:  simpleArray::+=");

		int count = _array->count() + other.count();

		simpleArray<char>* newArray = new simpleArray<char>(count);

		// Copy over data to new array
		for (int index = 0; index < _array->count(); index++)
			newArray->set(index, _array->get(index));

		for (int index = _array->count(); index < count; index++)
			newArray->set(index, other.get(index));

		delete _array;

		_array = newArray;
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

	simpleString simpleString::operator+(const simpleString& other) const
	{
		simpleString result;

		result.append(this->c_str());
		result.append(other);

		return result;
	}
	simpleString simpleString::operator+(const char* other) const
	{
		simpleString result;

		result.append(this->c_str());
		result.append(other);

		return result;
	}

	simpleString& operator+(const simpleString& left, const simpleString& right)
	{
		simpleString result;

		result.append(left);
		result.append(right);

		return result;
	}
	simpleString& operator+(const char* left, const simpleString& right)
	{
		simpleString result;

		result.append(left);
		result.append(right);

		return result;
	}
	simpleString& operator+(const simpleString& left, const char* right)
	{
		simpleString result;

		result.append(left);
		result.append(right);

		return result;
	}
	std::ostream& operator<<(std::ostream& stream, const simpleString& other)
	{
		stream << other.c_str();
	}
	std::istream& operator>>(std::istream& stream, simpleString& other)
	{
		// Compiler will try to build an instance of simpleString on the stack
		//
		if (other == default_value<simpleString>::value)
			stream.setstate(std::ios::failbit);

		// Probably nothing to do. Use breakpoint to make sure data made it 
		// into the simpleString
		//
		if (true)
		{
			bool isOtherGood;
		}

		return stream;
	}

	bool simpleString::compare(const simpleString& other) const
	{
		if (_array == NULL)
			brogueException::show("Trying to operate on a null array:  simpleArray.cpp");

		if (_array->count() != other.count())
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
		if (_array == NULL)
			brogueException::show("Trying to operate on a null array:  simpleArray.cpp");

		if (strnlen_s(other, _array->count()) != _array->count())
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
		if (_array == NULL)
			brogueException::show("Trying to operate on a null array:  simpleArray.cpp");

		this->append(other.c_str());
	}
	void simpleString::append(const char* other)
	{
		if (_array == NULL)
			brogueException::show("Trying to operate on a null array:  simpleArray.cpp");

		int otherCount = strnlen_s(other, this->MAX_LENGTH);

		int count = _array->count() + otherCount;

		simpleArray<char>* newArray = new simpleArray<char>(count);

		// Copy over data to new array
		for (int index = 0; index < _array->count(); index++)
			newArray->set(index, _array->get(index));

		for (int index = _array->count(); index < count; index++)
			newArray->set(index, other[index]);

		delete _array;
	}

	int simpleString::search(const char* search) const
	{
		return this->search(simpleString(search));
	}
	int simpleString::search(const simpleString& search) const
	{
		if (search.count() == 0)
			brogueException::show("Trying to search for zero-count string simpleString::search");

		return this->searchImpl(0, search);
	}
	int simpleString::searchImpl(int startIndex, const simpleString& search) const
	{
		int firstIndex = -1;

		// Can optimize by subtracting the search string count from the array count. May
		// help a little for large strings. (Must add one additional character)
		//
		for (int index = startIndex; index < _array->count() - search.count() + 1; index++)
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

	void simpleString::insert(int index, const char* replacement)
	{
		this->insert(index, simpleString(replacement));
	}
	void simpleString::insert(int index, const simpleString& replacement)
	{
		if (index >= _array->count())
			brogueException::show("Index for insert was outside the bounds of the array:  simpleString.cpp");

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
	}
	void simpleString::remove(int index, int count)
	{
		if (index + count >= _array->count())
			brogueException::show("Index for remove was outside the bounds of the array:  simpleString.cpp");

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
		       _array->areAll([](char value)
				{
					return value == '\0' || std::isspace(value);
				});
	}

	void simpleString::set(int index, char value)
	{
		_array->set(index, value);
	}
	simpleString simpleString::subString(int index, int count) const
	{
		if (index + count > _array->count())
			brogueException::show("Index outside the bounds of the array: simpleArray::subString");

		simpleString result(count);

		for (int i = index; i < index + count; i++)
			result.set(i, _array->get(i));

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

		return result.toArray();
	}
	simpleArray<simpleString> simpleString::split(const simpleString& input, const simpleArray<char>& tokens) const
	{
		simpleList<simpleString> result;
		simpleList<simpleString> list;

		result.add(input);

		for (int index = 0; index < tokens.count(); index++)
		{
			char token = tokens.get(index);

			list.clear();

			// Iterate the result list, swap back to list
			for (int index = 0; index < result.count(); index++)
			{
				simpleArray<simpleString> strings = result.get(index).split(token);

				list.addRange(strings);
			}

			result.clear();
			result.addRange(list);
			list.clear();
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
	char* simpleString::c_str() const
	{
		return _array->getArray();
	}
}
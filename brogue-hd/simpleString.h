#pragma once

#include "brogueException.h"
#include "simpleArray.h"
#include "simpleList.h"

namespace brogueHd::component
{
	class simpleString
	{
	protected:

		/// <summary>
		/// This should be taken from the std library or boost. I wasn't sure of the limit; but the
		/// strlen_s function asked for one.
		/// </summary>
		static const size_t MAX_LENGTH = 1000000;

	public:

		simpleString();
		simpleString(int count);
		simpleString(const char* chars);
		simpleString(const simpleString& copy);
		~simpleString();

		void operator=(const simpleString& copy);
		void operator+=(const simpleString& other);
		bool operator==(const simpleString& other) const;
		bool operator!=(const simpleString& other) const;

		simpleString operator+(const simpleString& other) const;
		simpleString operator+(const char* other) const;		

		bool operator==(const char* other) const;
		bool operator!=(const char* other) const;

		char get(int index) const;
		int count() const;
		bool isEmpty() const;
		bool isEmptyOrWhiteSpace() const;

		void set(int index, char value);

		simpleString subString(int index, int count) const;
		simpleArray<simpleString> split(char token) const;
		simpleArray<simpleString> split(const simpleString& input, const simpleArray<char>& tokens) const;

		simpleString toUpper() const;

		char* c_str() const;

	public:

		friend simpleString& operator+(const simpleString& left, const simpleString& right);
		friend simpleString& operator+(const char* left, const simpleString& right);
		friend simpleString& operator+(const simpleString& left, const char* right);

	private:

		bool compare(const simpleString& other) const;
		bool compareArray(const char* other) const;

		void append(const simpleString& other);
		void append(const char* chars);

	private:

		simpleArray<char>* _array;
	};
}
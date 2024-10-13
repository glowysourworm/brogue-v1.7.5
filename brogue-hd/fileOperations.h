#pragma once

#include "simpleString.h"
#include "simpleException.h"
#include <fstream>
#include <strstream>

using namespace brogueHd::simple;

namespace brogueHd::component
{
	class fileOperations
	{
	public:

		static bool fileExists(const char* filename)
		{
			return fileExists(simpleString(filename));
		}
		static simpleString readFile(const char* filename)
		{
			return readFile(simpleString(filename));
		}
		static void writeFile(const char* filename, const simpleString& contents)
		{
			writeFile(simpleString(filename), contents);
		}
		static bool fileExists(const simpleString& filename)
		{
			bool result = false;

			try
			{
				std::ifstream stream(filename.c_str());

				result = stream.good();

				stream.close();
			}
			catch (std::exception& ex)
			{
				simpleException::show("Error trying to check if file exists:  {}:  {}", filename.c_str(), ex.what());
			}

			return result;
		}
		static simpleString readFile(const simpleString& filename)
		{
			try
			{
				std::ifstream stream(filename.c_str());
				simpleString result;
				std::string line;

				while (std::getline(stream, line))
				{
					result.append(line.c_str());
					line = "";
				}

				stream.close();

				return result;
			}
			catch (std::exception& ex)
			{
				simpleException::show("Error trying to read file:  {}:  {}", filename.c_str(), ex.what());
			}

			return simpleString("");
		}
		static void writeFile(const simpleString& filename, const simpleString& contents)
		{
			try
			{
				std::ofstream stream(filename.c_str());

				stream << contents.c_str();

				stream.close();
			}
			catch (std::exception& ex)
			{
				simpleException::show("Error trying to write file:  {}:  {}", filename.c_str(), ex.what());
			}
		}
	};
}
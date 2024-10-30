#pragma once

#include "simpleBuffer.h"
#include "simpleString.h"
#include "simpleException.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <filesystem>

using namespace brogueHd::simple;

namespace brogueHd::component
{
	class fileOperations
	{
	public:
		
		static simpleString filePathConcat(const simpleString& path1, const simpleString& path2)
		{
			simpleString result(path1);

			if (!result.endsWith("\\"))
				result.append("\\");

			result.append(path2);

			return result;
		}
		static bool fileExists(const char* filename)
		{
			return fileExists(simpleString(filename));
		}
		static simpleBuffer* bufferFile(const simpleString& fileName)
		{
			return bufferFile(fileName.c_str());
		}
		static simpleString readTextFile(const char* filename)
		{
			return readTextFile(simpleString(filename));
		}
		static void writeTextFile(const char* filename, const simpleString& contents)
		{
			writeTextFile(simpleString(filename), contents);
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
		static simpleBuffer* bufferFile(const char* filename)
		{
			try
			{
				size_t fileSize = std::filesystem::file_size(filename);

				if (fileSize <= 0)
					return nullptr;

				char* bufferIn = new char[fileSize];

				std::ifstream stream(filename);

				stream.get(bufferIn, fileSize);
				stream.close();

				return new simpleBuffer(bufferIn, fileSize, false);
			}
			catch (std::exception& ex)
			{
				simpleException::show("Error trying to read file:  {}:  {}", filename, ex.what());
			}

			return nullptr;
		}
		static simpleString readTextFile(const simpleString& filename)
		{
			try
			{
				const char* filenameStr = filename.c_str();
				std::ifstream stream(filenameStr);
				simpleString result;
				std::string line;

				while (std::getline(stream, line))
				{
					line.append("\n");
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
		static void writeTextFile(const simpleString& filename, const simpleString& contents)
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
#pragma once

#include "simpleBuffer.h"
#include "simpleDirectoryEntry.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleString.h"
#include <chrono>
#include <corecrt.h>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <regex>
#include <string>
#include <sys/stat.h>

namespace brogueHd::simple
{
	class simpleFileIO
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
				throw simpleException(simpleExt::format("Error trying to check if file exists:  {}:  {}", filename.c_str(), ex.what()));
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
				throw simpleException(simpleExt::format("Error trying to read file:  {}:  {}", filename, ex.what()));
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
				throw simpleException(simpleExt::format("Error trying to read file:  {}:  {}", filename.c_str(), ex.what()));
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
				throw simpleException(simpleExt::format("Error trying to write file:  {}:  {}", filename.c_str(), ex.what()));
			}
		}
		static simpleDirectoryEntry readDirectory(const char* path)
		{
			return readDirectory(path, "*.*");
		}
		static simpleDirectoryEntry readDirectory(const char* path, const char* search)
		{
			simpleDirectoryEntry result;

			try
			{
				// Using directory iterator
				struct stat entry;

				// Using "experimental" filesystem libraries (also "auto")
				for (const auto& entry : std::filesystem::directory_iterator(path))
				{
					if (!entry.is_directory())
					{
						// Huh.
						auto utcTime = std::chrono::file_clock::to_utc(entry.last_write_time());
						auto sysTime = std::chrono::utc_clock::to_sys(utcTime);
						time_t writeTime = std::chrono::system_clock::to_time_t(sysTime);

						std::string filePath = entry.path().string();
						std::string fileName = entry.path().filename().string();
						std::string fileNameWOExt = entry.path().filename().replace_extension().string();
						std::regex searchRegex(search, strnlen_s(search, 100));

						if (filePath.size() <= 0 ||
							fileName.size() <= 0 ||
							fileNameWOExt.size() <= 0)
							continue;

						if (std::regex_search(fileName, searchRegex))
						{
							result.addFile(filePath.c_str(), fileName.c_str(), fileNameWOExt.c_str(), writeTime, (size_t)entry.file_size());
						}
					}
				}

				return result;
			}
			catch (std::exception& ex)
			{
				throw simpleException(simpleExt::format("Error trying to read directory:  {}", path));
			}

			return result;
		}
	};
}
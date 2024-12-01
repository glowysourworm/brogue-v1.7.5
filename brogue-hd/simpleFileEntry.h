#pragma once

#include "simple.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleString.h"
#include <corecrt.h>
#include <cstdint>

namespace brogueHd::simple
{
	class simpleFileEntry : public simpleObject
	{
	public:

		simpleFileEntry()
		{
			_fileName = nullptr;
			_fileFullPath = nullptr;
			_fileNameWithoutExtension = nullptr;
			_writeTimeLong = nullptr;
			_writeTimeShort = nullptr;
			_fileSize = 0;
		}
		~simpleFileEntry()
		{
			delete _fileName;
			delete _fileFullPath;
			delete _fileNameWithoutExtension;
			delete _writeTimeLong;
			delete _writeTimeShort;
		}
		simpleFileEntry(const simpleFileEntry& copy)
		{
			copy.validate();

			copyImpl(copy);

			validate();
		}
		simpleFileEntry(const char* filePath, const char* afileName, const char* fileNameWithoutExt, time_t writeTime, size_t afileSize)
		{
			_fileFullPath = new simpleString(filePath);
			_fileName = new simpleString(afileName);
			_fileNameWithoutExtension = new simpleString(fileNameWithoutExt);
			_writeTimeLong = new simpleString(simpleExt::formatDate(writeTime, false));
			_writeTimeShort = new simpleString(simpleExt::formatDate(writeTime, true));
			_fileSize = afileSize;

			validate();
		}
		void operator=(const simpleFileEntry& other)
		{
			other.validate();

			delete _fileName;
			delete _fileFullPath;
			delete _fileNameWithoutExtension;
			delete _writeTimeLong;
			delete _writeTimeShort;

			copyImpl(other);

			validate();
		}
		bool operator==(const simpleFileEntry& other) const
		{
			return compare(other);
		}
		bool operator!=(const simpleFileEntry& other) const
		{
			return !compare(other);
		}

		bool compare(const simpleFileEntry& other) const
		{
			return _fileFullPath == other.getFileFullPath() &&
				_fileName == other.getFileName() &&
				_fileNameWithoutExtension == other.getFileNameWithoutExtension() &&
				_writeTimeLong == other.getWriteTimeLong() &&
				_writeTimeShort == other.getWriteTimeShort() &&
				_fileSize == other.getFileSize();
		}

		static int comparePath(const simpleFileEntry& item1, const simpleFileEntry& item2)
		{
			int comparison = 0;
			int cursor = 0;

			while (comparison == 0 &&
				   cursor < item1.getFileFullPath()->count() &&
				   cursor < item2.getFileFullPath()->count())
			{
				// Difference in integer value of the chars (positive => greater than)
				comparison += item1.getFileFullPath()->get(cursor) - item2.getFileFullPath()->get(cursor);

				cursor++;
			}

			// If Equal: Use the file length to compare them
			if (comparison == 0)
				comparison += item1.getFileFullPath()->count() - item2.getFileFullPath()->count();

			return comparison;
		}

		bool validate() const
		{
			if (_fileFullPath->count() == 0 ||
				_fileName->count() == 0 ||
				_fileNameWithoutExtension->count() == 0 ||
				_writeTimeLong->count() == 0 ||
				_writeTimeShort->count() == 0)
				throw simpleException("Invalid file information:  simpleFileEntry.h");

			return true;
		}

		simpleString* getFileFullPath() const
		{
			return _fileFullPath;
		}
		simpleString* getFileName() const
		{
			return _fileName;
		}
		simpleString* getFileNameWithoutExtension() const
		{
			return _fileNameWithoutExtension;
		}
		simpleString* getWriteTimeLong() const
		{
			return _writeTimeLong;
		}
		simpleString* getWriteTimeShort() const
		{
			return _writeTimeShort;
		}
		uint32_t getFileSize() const
		{
			return _fileSize;
		}

		const char* toString() const
		{
			return _fileFullPath->c_str();
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(_fileFullPath);
		}

	private:

		void copyImpl(const simpleFileEntry& copy)
		{
			_fileName = new simpleString(*copy.getFileName());
			_fileFullPath = new simpleString(*copy.getFileFullPath());
			_fileNameWithoutExtension = new simpleString(*copy.getFileNameWithoutExtension());
			_writeTimeLong = new simpleString(*copy.getWriteTimeLong());
			_writeTimeShort = new simpleString(*copy.getWriteTimeShort());
			_fileSize = copy.getFileSize();
		}

	private:

		simpleString* _fileFullPath;
		simpleString* _fileName;
		simpleString* _fileNameWithoutExtension;
		simpleString* _writeTimeLong;
		simpleString* _writeTimeShort;
		uint32_t _fileSize;
	};
}
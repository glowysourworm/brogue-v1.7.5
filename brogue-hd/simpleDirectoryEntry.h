#pragma once

#include "simple.h"
#include "simpleFileEntry.h"
#include "simpleList.h"
#include <corecrt.h>

namespace brogueHd::simple
{
	class simpleDirectoryEntry : hashableObject
	{
	public:

		simpleDirectoryEntry()
		{
			_files = new simpleList<simpleFileEntry*>();
		}
		simpleDirectoryEntry(const simpleDirectoryEntry& copy)
		{
			_files = copy.getFiles();
		}
		~simpleDirectoryEntry()
		{
			delete _files;
		}

		simpleFileEntry* getFile(int index) const
		{
			return _files->get(index);
		}

		void addFile(const char* filePath, const char* fileName, const char* fileNameWithoutExt, time_t writeTime, size_t fileSize)
		{
			_files->add(new simpleFileEntry(filePath, fileName, fileNameWithoutExt, writeTime, fileSize));
		}

		void addFile(const simpleFileEntry& entry)
		{
			_files->add(new simpleFileEntry(entry));
		}

		int fileCount() const
		{
			return _files->count();
		}

		simpleList<simpleFileEntry*> sort() const
		{
			return _files->sort([] (simpleFileEntry* item1, simpleFileEntry* item2)
			{
				return simpleFileEntry::comparePath(*item1, *item2);
			});
		}

	protected:

		simpleList<simpleFileEntry*>* getFiles() const
		{
			return _files;
		}

	private:

		simpleList<simpleFileEntry*>* _files;
	};
}
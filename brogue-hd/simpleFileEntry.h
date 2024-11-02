#pragma once

#include "simpleString.h"

namespace brogueHd::simple
{
	struct simpleFileEntry : hashable
	{
		simpleFileEntry()
		{
			fileName = simpleString("");
			writeTimeLong = simpleString("");
			writeTimeLong = simpleString("");
			fileSize = 0;
		}
		simpleFileEntry(const char* filePath, const char* afileName, const char* fileNameWithoutExt, time_t writeTime, size_t afileSize)
		{
			fileFullPath = simpleString(filePath);
			fileName = simpleString(afileName);
			fileNameWithoutExtension = simpleString(fileNameWithoutExt);
			writeTimeLong = simpleString(simpleExt::formatDate(writeTime, false));
			writeTimeShort = simpleString(simpleExt::formatDate(writeTime, true));
			fileSize = afileSize;
		}

		simpleString fileFullPath;
		simpleString fileName;
		simpleString fileNameWithoutExtension;
		simpleString writeTimeLong;
		simpleString writeTimeShort;
		uint32_t fileSize;
	};
}
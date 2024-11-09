#pragma once

#include <cstdio>
#include <exception>

namespace brogueHd::backend::model
{
	static class fileio
	{

	public:

		static bool fileExists(const char* pathname)
		{
			try
			{
				FILE* file = fopen(pathname, "rb");
				if (file)
				{
					fclose(file);
					return true;
				}
				else
				{
					return false;
				}
			}
			catch (std::exception& ex)
			{
				// TODO

				throw;
			}
		}

		///// <summary>
		///// Returns available file path (file, file 1, ..., file N)
		///// </summary>
		//static void getAvailableFilePath(char* returnPath, const char* defaultPath, const char* suffix)
		//{
		//	char fullPath[BROGUE_FILENAME_MAX];
		//	short fileNameIterator = 2;

		//	strcpy(returnPath, defaultPath);
		//	sprintf(fullPath, "%s%s", returnPath, suffix);
		//	while (fileExists(fullPath)) {
		//		sprintf(returnPath, "%s %i", defaultPath, fileNameIterator);
		//		sprintf(fullPath, "%s%s", returnPath, suffix);
		//		fileNameIterator++;
		//	}
		//}
	};
}


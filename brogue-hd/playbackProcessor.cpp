#include "playbackProcessor.h"
#include <fstream>

using namespace std;

namespace brogueHd::backend::processor
{
	playbackProcessor::playbackProcessor()
	{
		_playbackData = new playbackData();
	}

	playbackProcessor::~playbackProcessor()
	{
		delete _playbackData;
	}

	void playbackProcessor::initialize(const char* filePath)
	{
		// Procedure
		//
		// 1) Load BrogueEvent list (keystrokes)
		// 2) Prepare display data in advance for processing
		//

		try
		{
			fstream fileStream;

			fileStream.open(filePath);

			if (!fileStream.good())
				throw std::runtime_error(std::string("Error opening file:  ") + filePath);

			// Store the path of the playback file
			_playbackData->filePath.assign(filePath);

			// Read the event list from file
			_playbackData->recordedEvents = brogueEventFile::deserialize(fileStream);

			fileStream.close();
		}
		catch (std::exception& ex)
		{
			throw std::runtime_error(std::string("Error opening playback file: ") + ex.what());
		}
	}
}
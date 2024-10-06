#include "playbackProcessor.h"
#include <fstream>

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
			std::fstream fileStream;

			fileStream.open(filePath);

			if (!fileStream.good())
				brogueException::show(simpleString("Error opening file:  ") + filePath);

			// Store the path of the playback file
			_playbackData->filePath = filePath;

			// Read the event list from file
			_playbackData->recordedEvents = brogueEventFile::deserialize(fileStream);

			fileStream.close();
		}
		catch (std::exception& ex)
		{
			throw brogueException::show(simpleString("Error opening playback file: ") + ex.what());
		}
	}
}
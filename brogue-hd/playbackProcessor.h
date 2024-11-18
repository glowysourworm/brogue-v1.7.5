#pragma once

#include "playbackData.h"
#include "simpleException.h"
#include <exception>
#include <fstream>
#include <iosfwd>

using namespace brogueHd::backend::model;

namespace brogueHd::backend
{
	class playbackProcessor
	{
	public:

		playbackProcessor();
		~playbackProcessor();

		void initialize(const char* filePath);

	private:

		playbackData* _playbackData;

		//simpleList<char*>* _annotations;		// the next annotation (char* is 5000 characters)
		unsigned long _annotationTurn;			// the turn number during which to display the next annotation
		unsigned long _annotationIndex;			// how far we've read in the annotations file
		
	};

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
				throw simpleException("Error opening file:  {}", filePath);

			// Store the path of the playback file
			_playbackData->filePath = filePath;

			// Read the event list from file
			//_playbackData->recordedEvents = brogueEventFile::deserialize(fileStream);

			fileStream.close();
		}
		catch (std::exception& ex)
		{
			throw simpleException("Error opening playback file: ", ex.what());
		}
	}
}


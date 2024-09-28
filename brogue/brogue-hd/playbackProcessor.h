#pragma once

#include "playbackData.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::io;

namespace brogueHd::backend::processor
{
	class playbackProcessor
	{
	public:

		playbackProcessor();
		~playbackProcessor();

		void initialize(const char* filePath);

	private:

		playbackData* _playbackData;

		simpleList<char*>* _annotations;		// the next annotation (char* is 5000 characters)
		unsigned long _annotationTurn;			// the turn number during which to display the next annotation
		unsigned long _annotationIndex;			// how far we've read in the annotations file
		
	};
}


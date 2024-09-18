#pragma once

#include "playbackData.h"

using namespace std;

namespace brogueHd
{
	class playbackProcessor
	{
	public:

		playbackProcessor();
		~playbackProcessor();

		void initialize(char* filePath);

	private:

		playbackData* _playbackData;

		std::vector<char*> _annotations;		// the next annotation (char* is 5000 characters)
		unsigned long _annotationTurn;			// the turn number during which to display the next annotation
		unsigned long _annotationIndex;			// how far we've read in the annotations file
		
	};
}


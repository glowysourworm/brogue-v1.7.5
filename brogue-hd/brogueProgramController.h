#pragma once

#include "brogueUIConstants.h"
#include "brogueUIResponseData.h"
#include "programControlData.h"
#include "simpleList.h"

using namespace brogueHd::simple;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueProgramController
	{
	public:

		brogueProgramController()
		{
			_controlData = new simpleList<programControlData*>();
		}
		~brogueProgramController()
		{
			for (int index = 0; index < _controlData->count(); index++)
			{
				delete _controlData->get(index);
			}

			delete _controlData;
		}

		brogueUIProgram getNextProgram(const brogueUIResponseData& response)
		{
			for (int index = 0; index < _controlData->count(); index++)
			{
				if (_controlData->get(index)->currentView == response.sender)
				{
					programControlData* data = _controlData->get(index);

					if (data->exitOnKeyboardAny && response.hotkeyUsed)
						return data->nextProgram;

					else if (data->exitOnKeyboardHotkey && response.hotkeyUsed)
						return data->nextProgram;

					else if (data->exitOnMouseButton && response.mouseUsed && response.mouseLeft)
						return data->nextProgram;

					else
						return data->currentProgram;
				}
			}
		}

		void addViewControl(const programControlData& controlData)
		{
			_controlData->add(new programControlData(controlData));
		}

	private:

		simpleList<programControlData*>* _controlData;
	};
}
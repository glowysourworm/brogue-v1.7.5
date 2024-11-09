#pragma once

#include "brogueUIEvent.h"

namespace brogueHd::backend
{
	class eventController
	{
	public:

		eventController()
		{
			_uiClickEvent = new brogueUIEvent();
			_uiDeactivatedEvent = new brogueUIEvent();
		}
		~eventController()
		{
			delete _uiClickEvent;
			delete _uiDeactivatedEvent;
		}

		brogueUIEvent* getUIClickEvent()
		{
			return _uiClickEvent;
		}
		brogueUIEvent* getUIDeactivatedEvent()
		{
			return _uiDeactivatedEvent;
		}

	private:

		brogueUIEvent* _uiClickEvent;
		brogueUIEvent* _uiDeactivatedEvent;

	};
}
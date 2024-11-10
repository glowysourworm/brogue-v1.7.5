#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "gridRect.h"

#include "eventController.h"

using namespace brogueHd::backend;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueBackground : public brogueViewBase
	{
	public:

		brogueBackground(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueBackground();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override
		{
			// May not need this override (needsUpdate -> false)
		}

		virtual bool needsUpdate() const override
		{
			return false;
		}
	};

	brogueBackground::brogueBackground(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, data, sceneBoundary, viewBoundary)
	{
	}

	brogueBackground::~brogueBackground()
	{
	}

}


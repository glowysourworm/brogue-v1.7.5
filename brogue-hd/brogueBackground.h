#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "gridRect.h"

#include "brogueCellDisplay.h"
#include "eventController.h"
#include "gridDefinitions.h"
#include "simple.h"

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
							int millisecondsLapsed,
							bool forceUpdate) override
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
		brogueBackground* that = this;

		// Initialize to use the background color
		brogueViewBase::iterate([&that] (short column, short row, brogueCellDisplay* cell)
		{
			cell->backColor = that->getBackgroundColor(column, row);
			return iterationCallback::iterate;
		});
	}

	brogueBackground::~brogueBackground()
	{
	}

}


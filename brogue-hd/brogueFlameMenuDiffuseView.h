#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "color.h"
#include "eventController.h"
#include "gridRect.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueFlameMenuDiffuseView : public brogueViewBase
	{
	public:
		brogueFlameMenuDiffuseView(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueFlameMenuDiffuseView();

		void checkUpdate(const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed) override
		{
			// Prevent updating
		}

		void clearUpdate() override {}
		void clearEvents() override {}

		bool needsUpdate() const override
		{
			return false;
		}
		brogueCellDisplay* get(short column, short row) const override
		{
			return _defaultCell;
		}

	private:

		brogueCellDisplay* _defaultCell;
	};

	brogueFlameMenuDiffuseView::brogueFlameMenuDiffuseView(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, data, sceneBoundary, viewBoundary)
	{
		_defaultCell = new brogueCellDisplay();
		_defaultCell->backColor = colors::transparent();
		_defaultCell->noDisplay = false;
	}
	brogueFlameMenuDiffuseView::~brogueFlameMenuDiffuseView()
	{
		delete _defaultCell;
	}
}
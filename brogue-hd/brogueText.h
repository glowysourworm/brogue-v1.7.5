#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "color.h"
#include "eventController.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::component;

namespace brogueHd::frontend
{
	class brogueText : public brogueViewBase
	{
	public:

		brogueText(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueText();

		virtual void update(int millisecondsLapsed,
							bool forceUpdate) override;

		virtual bool needsUpdate() const override;
	};

	brogueText::brogueText(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, data, sceneBoundary, viewBoundary)
	{
		update(0, true);
	}
	brogueText::~brogueText()
	{
	}
	bool brogueText::needsUpdate() const
	{
		// Adding the mouse enter / leave events
		//
		return brogueViewBase::needsUpdate() || this->getMouseLeave() || this->getMouseEnter();
	}
	void brogueText::update(int millisecondsLapsed, bool forceUpdate)
	{
		gridRect bounds = this->getBoundary();
		brogueText* that = this;

		// Iterate THIS Boundary:  apply mouse data
		this->getBoundary().iterate([&that, &bounds] (short column, short row)
		{
			short menuColumn = column - bounds.column;
			short menuRow = row - bounds.row;
			int textOffset = column - bounds.column;

			color nextColor = that->getBackgroundColor(column, row);

			if (that->isHotkey(column, row))
				that->get(column, row)->foreColor = colors::yellow();
			else
				that->get(column, row)->foreColor = that->getForegroundColor(column, row);

			that->get(column, row)->character = that->getText(column, row);
			that->get(column, row)->backColor = nextColor;

			return iterationCallback::iterate;
		});
	}
}
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
using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueButton : public brogueViewBase
	{
	public:

		brogueButton(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueButton();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual bool needsUpdate() const override;

		virtual void iterateChildViews(simpleListCallback<brogueViewBase*> callback) const override
		{
			callback((brogueViewBase*)this);
		}
	};

	brogueButton::brogueButton(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, data, sceneBoundary, viewBoundary)
	{
		// Initialize the view
		//
		update(default_value::value<brogueKeyboardState>(), default_value::value<brogueMouseState>(), 0);
	}
	brogueButton::~brogueButton()
	{
	}
	bool brogueButton::needsUpdate() const
	{
		// Adding the mouse enter / leave events
		//
		return brogueViewBase::needsUpdate() || this->getMouseLeave() || this->getMouseEnter();
	}
	void brogueButton::update(const brogueKeyboardState& keyboardState,
							  const brogueMouseState& mouseState,
							  int millisecondsLapsed)
	{
		gridRect bounds = this->getBoundary();
		brogueButton* that = this;

		// Iterate THIS boundary:  Apply mouse data from the render boundary
		this->getBoundary().iterate([&that, &bounds] (short column, short row)
		{
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
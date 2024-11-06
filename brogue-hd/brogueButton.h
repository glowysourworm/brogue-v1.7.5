#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueView.h"
#include "color.h"
#include "eventController.h"
#include "gridRect.h"
#include "simple.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButton : public brogueView
	{
	public:

		brogueButton(eventController* eventController, brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueButton();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual bool needsUpdate() const override;
	};

	brogueButton::brogueButton(eventController* eventController, brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(eventController, data, sceneBoundary, viewBoundary)
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
		return this->getUIData()->needsUpdate() || this->getUIData()->getMouseLeave() || this->getUIData()->getMouseEnter();
	}
	void brogueButton::update(const brogueKeyboardState& keyboardState,
							  const brogueMouseState& mouseState,
							  int millisecondsLapsed)
	{
		gridRect bounds = this->getBoundary();
		brogueUIData* data = this->getUIData();
		brogueButton* that = this;

		// Iterate THIS boundary:  Apply mouse data from the render boundary
		this->getBoundary().iterate([&that, &data, &bounds] (short column, short row)
		{
			color nextColor = data->calculateGradient(column, row);

			if (data->getIsHotkey(column, row))
				that->get(column, row)->foreColor = colors::yellow();
			else
				that->get(column, row)->foreColor = data->getTextColor(column, row);

			that->get(column, row)->character = data->getText(column, row);
			that->get(column, row)->backColor = nextColor;

			return iterationCallback::iterate;
		});
	}
}
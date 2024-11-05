#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIChildResponse.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIResponseData.h"
#include "brogueView.h"
#include "color.h"
#include "gridRect.h"
#include "simple.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButton : public brogueView
	{
	public:

		brogueButton(brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueButton();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual brogueUIChildResponse checkUpdate(const brogueKeyboardState& keyboardState,
												  const brogueMouseState& mouseState,
												  int millisecondsLapsed) override;
	};

	brogueButton::brogueButton(brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(data, sceneBoundary, viewBoundary)
	{
		// Initialize the view
		//
		update(default_value::value<brogueKeyboardState>(), default_value::value<brogueMouseState>(), 0);
	}
	brogueButton::~brogueButton()
	{
	}
	brogueUIChildResponse brogueButton::checkUpdate(const brogueKeyboardState& keyboardState,
												   const brogueMouseState& mouseState,
												   int millisecondsLapsed)
	{
		brogueUIChildResponse response;

		bool hasInteraction = this->getUIData()->getHasMouseInteraction();

		if (hasInteraction)
		{
			bool mouseOver = this->isMouseOver(mouseState);

			// Update the UI data
			response.actionMet = this->getUIData()->setMouseUpdate(mouseState.getMouseLeft(), mouseOver);
			response.deactivated = !mouseOver && mouseState.getMouseLeft();
			response.needsUpdate = hasInteraction && (mouseOver || mouseState.getMouseLeft());
			response.tag = *this->getUIData()->getAction();
		}

		return response;
	}
	void brogueButton::update(const brogueKeyboardState& keyboardState,
							  const brogueMouseState& mouseState,
							  int millisecondsLapsed)
	{
		// Check mouse hover
		bool mouseHover = this->isMouseOver(mouseState);
		bool mousePressed = mouseHover && mouseState.getMouseLeft();

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
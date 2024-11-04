#pragma once

#include "brogueMouseState.h"
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

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual brogueUIResponseData& checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;
	};

	brogueButton::brogueButton(brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(viewName, data, sceneBoundary, viewBoundary)
	{
		// Initialize the view
		//
		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueButton::~brogueButton()
	{
	}
	brogueUIResponseData& brogueButton::checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueUIResponseData response;

		response.sender = this->getViewName();
		response.mouseHover = this->isMouseOver(mouseState);
		response.mouseUsed = this->getUIData()->getHasMouseInteraction();
		response.mouseLeft = mouseState.getMouseLeft();
		response.shouldUpdate = (response.mouseHover || response.mouseLeft) && response.mouseUsed;

		return response;
	}
	void brogueButton::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		// Check mouse hover
		bool mouseHover = this->isMouseOver(mouseState);

		gridRect bounds = this->getBoundary();
		brogueUIData* data = this->getUIData();
		brogueButton* that = this;

		// Iterate THIS boundary:  Apply mouse data from the render boundary
		this->getBoundary().iterate([&that, &data, &bounds, &mouseState, &mouseHover] (short column, short row)
		{
			color nextColor = data->calculateGradient(column, row, mouseHover);

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
#pragma once

#include "brogueView.h"
#include "gridRect.h"
#include "brogueUIData.h"
#include "brogueMouseState.h"
#include "simpleException.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButton : public brogueView
	{
	public:

		brogueButton(brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueButton();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual bool shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;
	};

	brogueButton::brogueButton(brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(data, sceneBoundary, viewBoundary)
	{
		// Initialize the view
		//
		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueButton::~brogueButton()
	{
	}
	bool brogueButton::shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		return this->isMouseOver(mouseState);
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
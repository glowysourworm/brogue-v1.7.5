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
	class brogueButton : brogueView
	{
	public:

		brogueButton(const brogueUIData& data, gridRect sceneBoundary, gridRect viewBoundary);
		~brogueButton();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual bool shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;

	private:

		brogueUIData* _buttonData;
	};

	brogueButton::brogueButton(const brogueUIData& data, gridRect sceneBoundary, gridRect viewBoundary) : brogueView(sceneBoundary, viewBoundary)
	{
		if (data.text.getCount() > viewBoundary.width)
			simpleException::show("Length of text is greater than button width:  brogueButton.h");

		_buttonData = new brogueUIData(data);

		// Initialize the view
		//
		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueButton::~brogueButton()
	{
		delete _buttonData;
	}
	bool brogueButton::shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		return this->getBoundary().contains(mouseState.getLocation());
	}
	void brogueButton::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		gridRect bounds = this->getBoundary();
		brogueUIData data = *_buttonData;
		brogueButton* that = this;

		// Check mouse hover
		bool mouseHover = this->getBoundary().contains(mouseState.getLocation());

		this->getBoundary().iterate([&that, &data, &bounds, &mouseState, &mouseHover] (short column, short row)
		{
			color nextColor = data.calculateGradient(column, row);
			color highlightColor(0.7, 0.7, 0.7, 0.7);

			nextColor.alpha = 1.0;

			if (mouseHover)
				nextColor.interpolate(highlightColor, 0.5);

			if (data.getIsHotkey(column, row))
				that->get(column, row)->foreColor = colors::yellow();
			else
				that->get(column, row)->foreColor = data.getTextColor(column, row);

			that->get(column, row)->character = data.getText(column, row);
			that->get(column, row)->backColor = nextColor;

			return iterationCallback::iterate;
		});
	}
}
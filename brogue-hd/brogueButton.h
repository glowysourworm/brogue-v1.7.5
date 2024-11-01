#pragma once

#include "brogueView.h"
#include "gridRect.h"
#include "button.h"
#include "simpleMouseState.h"
#include "simpleException.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButton : brogueView
	{
	public:

		brogueButton(const buttonData& data, gridRect sceneBoundary, gridRect viewBoundary);
		~brogueButton();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual bool shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;

	private:

		buttonData* _buttonData;
	};

	brogueButton::brogueButton(const buttonData& data, gridRect sceneBoundary, gridRect viewBoundary) : brogueView(sceneBoundary, viewBoundary)
	{
		if (data.text.getCount() > viewBoundary.width)
			simpleException::show("Length of text is greater than button width:  brogueButton.h");

		_buttonData = new buttonData(data);

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
		buttonData data = *_buttonData;
		brogueButton* that = this;

		// Check mouse hover
		bool mouseHover = this->getBoundary().contains(mouseState.getLocation());

		this->getBoundary().iterate([&that, &data, &bounds, &mouseState, &mouseHover] (short column, short row)
		{
			short menuColumn = column - bounds.column;
			short menuRow = row - bounds.row;
			int textIndex = column - bounds.column;

			int textBeginIndex = 0;

			switch (data.textAlignment)
			{
			case brogueTextAlignment::Left:
				break;
			case brogueTextAlignment::Right:
				textBeginIndex = bounds.right() - data.text.getCount() - bounds.column;
				break;
			case brogueTextAlignment::Center:
				textBeginIndex = ((bounds.right() - data.text.getCount() - bounds.column) / 2) + 1;
				break;
			default:
				simpleException::show("Unhandled brogueTextAlignment:  brogueButtonMenu.h");
			}

			color nextColor;
			color highlightColor(0.7, 0.7, 0.7, 0.7);

			// Create button gradient
			//
			if ((column - bounds.column) < (bounds.width / 2.0f))
				nextColor = color::interpolate(data.background1, data.background2, (bounds.width - (2 * (menuColumn))) / (float)bounds.width);
			else
				nextColor = color::interpolate(data.background1, data.background2, (2 * ((menuColumn)-(bounds.width / 2.0f))) / (float)bounds.width);

			nextColor.alpha = 1.0;

			if (mouseHover)
				nextColor.interpolate(highlightColor, 0.5);

			that->get(column, row)->backColor = nextColor;

			if (textIndex < data.text.getCount())
			{
				if (data.hotkeyIndex == textIndex)
					that->get(column + textBeginIndex, row)->foreColor = colors::yellow();
				else
					that->get(column + textBeginIndex, row)->foreColor = data.text.getColor(textIndex);

				that->get(column + textBeginIndex, row)->character = data.text.getChar(textIndex);
			}

			return iterationCallback::iterate;
		});
	}
}
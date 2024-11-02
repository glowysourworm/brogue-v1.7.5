#pragma once

#include "brogueView.h"
#include "colorString.h"
#include "brogueUIData.h"

namespace brogueHd::frontend::ui
{
	class brogueText : public brogueView
	{
	public:

		brogueText(const brogueUIData& data,
					gridRect sceneBoundary,
					gridRect viewBoundary);
		~brogueText();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;

	private:

		brogueUIData* _data;
		gridRect* _paddedBoundary;
	};

	brogueText::brogueText(const brogueUIData& data,
							gridRect sceneBoundary,
							gridRect viewBoundary)
		: brogueView(sceneBoundary, viewBoundary)
	{
		_data = new brogueUIData(data);
		_paddedBoundary = new gridRect(viewBoundary.column + data.padding,
									   viewBoundary.row + data.padding,
									   viewBoundary.width - (2 * data.padding),
									   viewBoundary.height - (2 * data.padding));
		if (data.text.getCount() > viewBoundary.width - (2 * data.padding))
			simpleException::show("Text too long for the specified view boudndary:  brogueText.h");

		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueText::~brogueText()
	{
		delete _data;
		delete _paddedBoundary;
	}

	void brogueText::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		gridRect bounds = this->getBoundary();
		brogueUIData data = *_data;
		brogueText* that = this;

		// Check mouse hover
		bool mouseHover = this->getBoundary().contains(mouseState.getLocation());

		this->getBoundary().iterate([&that, &data, &bounds, &mouseState, &mouseHover] (short column, short row)
		{
			short menuColumn = column - bounds.column;
			short menuRow = row - bounds.row;
			int textOffset = column - bounds.column;

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
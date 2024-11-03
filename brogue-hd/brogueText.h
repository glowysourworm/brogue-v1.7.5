#pragma once

#include "brogueView.h"
#include "gridRect.h"
#include "colorString.h"
#include "brogueUIData.h"

using namespace brogueHd::component;

namespace brogueHd::frontend::ui
{
	class brogueText : public brogueView
	{
	public:

		brogueText(brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueText();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
	};

	brogueText::brogueText(brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(data, sceneBoundary, viewBoundary)
	{
		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueText::~brogueText()
	{
	}

	void brogueText::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		gridRect bounds = this->getBoundary();
		brogueUIData* data = this->getUIData();
		brogueText* that = this;

		// Check mouse hover
		bool mouseHover = this->getBoundary().contains(mouseState.getLocation());

		this->getBoundary().iterate([&that, &data, &bounds, &mouseState, &mouseHover] (short column, short row)
		{
			short menuColumn = column - bounds.column;
			short menuRow = row - bounds.row;
			int textOffset = column - bounds.column;

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
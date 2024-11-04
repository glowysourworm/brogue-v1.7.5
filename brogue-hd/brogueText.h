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

namespace brogueHd::frontend::ui
{
	class brogueText : public brogueView
	{
	public:

		brogueText(brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueText();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual brogueUIResponseData& checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;
	};

	brogueText::brogueText(brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(viewName, data, sceneBoundary, viewBoundary)
	{
		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueText::~brogueText()
	{
	}
	brogueUIResponseData& brogueText::checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueUIResponseData response;

		response.sender = this->getViewName();
		response.mouseHover = this->isMouseOver(mouseState);
		response.mouseUsed = this->getUIData()->getHasMouseInteraction();
		response.mouseLeft = mouseState.getMouseLeft();
		response.shouldUpdate = (response.mouseHover || response.mouseLeft) && response.mouseUsed;

		return response;
	}
	void brogueText::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		gridRect bounds = this->getBoundary();
		brogueUIData* data = this->getUIData();
		brogueText* that = this;

		// Check mouse hover (Render Boundary)
		bool mouseHover = this->isMouseOver(mouseState);

		// Iterate THIS Boundary:  apply mouse data
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
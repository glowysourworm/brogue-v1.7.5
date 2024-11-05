#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIChildResponse.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
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

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual brogueUIChildResponse checkUpdate(const brogueKeyboardState& keyboardState,
												  const brogueMouseState& mouseState,
												  int millisecondsLapsed) override;
	};

	brogueText::brogueText(brogueUIView viewName, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(data, sceneBoundary, viewBoundary)
	{
		update(default_value::value<brogueKeyboardState>(), default_value::value<brogueMouseState>(), 0);
	}
	brogueText::~brogueText()
	{
	}
	brogueUIChildResponse brogueText::checkUpdate(const brogueKeyboardState& keyboardState,
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
	void brogueText::update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed)
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
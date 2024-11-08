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

namespace brogueHd::frontend::ui
{
	class brogueText : public brogueView
	{
	public:

		brogueText(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueText();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual bool needsUpdate() const override;
	};

	brogueText::brogueText(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(eventController, data, sceneBoundary, viewBoundary)
	{
		update(default_value::value<brogueKeyboardState>(), default_value::value<brogueMouseState>(), 0);
	}
	brogueText::~brogueText()
	{
	}
	bool brogueText::needsUpdate() const
	{
		// Adding the mouse enter / leave events
		//
		return this->getUIData()->needsUpdate() || this->getUIData()->getMouseLeave() || this->getUIData()->getMouseEnter();
	}
	void brogueText::update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed)
	{
		gridRect bounds = this->getBoundary();
		brogueUIData* data = this->getUIData();
		brogueText* that = this;

		// Iterate THIS Boundary:  apply mouse data
		this->getBoundary().iterate([&that, &data, &bounds] (short column, short row)
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
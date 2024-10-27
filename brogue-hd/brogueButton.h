#pragma once

#include "brogueView.h"
#include "gridRect.h"
#include "button.h"
#include "simpleException.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButton : brogueView
	{
		brogueButton(const buttonData& data, gridRect sceneBoundary, gridRect viewBoundary);
		~brogueButton();

		virtual void update(int millisecondsLapsed) override;
	};

	brogueButton::brogueButton(const buttonData& data, gridRect sceneBoundary, gridRect viewBoundary) : brogueView(sceneBoundary, viewBoundary)
	{
		if (data.text.getCount() > viewBoundary.width)
			simpleException::show("Length of text is greater than button width:  brogueButton.h");

		// Initialize the view
		//
		this->iterate([&data] (short column, short row, brogueCellDisplay* cell)
		{
			cell->backColor = data.background1;
			cell->foreColor = data.text.getColor(column);
			cell->character = data.text.getChar(column);

			return iterationCallback::iterate;
		});
	}
	brogueButton::~brogueButton()
	{
	}
	void brogueButton::update(int millisecondsLapsed)
	{

	}
}
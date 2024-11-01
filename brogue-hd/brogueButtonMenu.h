#pragma once

#include "brogueView.h"
#include "brogueViewContainer.h"
#include "brogueButton.h"
#include "simpleMouseState.h"
#include "gridRect.h"
#include "button.h"
#include "simpleHash.h"
#include "simpleException.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButtonMenu : public brogueViewContainer
	{
	public:

		brogueButtonMenu(const simpleList<buttonData>& buttons, int padding, gridRect sceneBoundary, gridRect viewBoundary);
		~brogueButtonMenu();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
	};

	brogueButtonMenu::brogueButtonMenu(const simpleList<buttonData>& buttons, int padding, gridRect sceneBoundary, gridRect viewBoundary) 
		: brogueViewContainer(sceneBoundary, viewBoundary)
	{
		for (int index = 0; index < buttons.count(); index++)
		{
			gridRect boundary(viewBoundary.column + padding, 
							  viewBoundary.row + padding + (index * 2), 
							  viewBoundary.width - (2 * padding), 
							  1);

			if (boundary.width < buttons.get(index).text.getCount())
				simpleException::show("Button text too large for the button boundary:  brogueButtonMenu.h");

			this->addView((brogueView*)new brogueButton(buttons.get(index), sceneBoundary, boundary));
		}

		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueButtonMenu::~brogueButtonMenu()
	{
	}
	void brogueButtonMenu::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueButtonMenu* that = this;

		// Border
		this->getBoundary().iterate([&that] (short column, short row)
		{
			that->get(column, row)->backColor = color(0.1, 0.1, 0.1, 0.8);

			return iterationCallback::iterate;
		});

		// brogueButton -> update()
		this->iterateViews([&mouseState, &millisecondsLapsed] (brogueView* button)
		{
			button->update(mouseState, millisecondsLapsed);
			
			return iterationCallback::iterate;
		});
	}
}
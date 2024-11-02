#pragma once

#include "brogueView.h"
#include "brogueViewContainer.h"
#include "brogueButton.h"
#include "brogueText.h"
#include "brogueMouseState.h"
#include "gridRect.h"
#include "brogueUIData.h"
#include "simpleHash.h"
#include "simpleException.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButtonMenu : public brogueViewContainer
	{
	public:

		brogueButtonMenu(const brogueUIData& menu,
						 const simpleList<brogueUIData>& buttons,
						 const brogueUIData& header,
						 bool useHeader, gridRect sceneBoundary, gridRect viewBoundary);
		~brogueButtonMenu();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;

	private:

		brogueUIData* _menuData;
	};

	brogueButtonMenu::brogueButtonMenu(const brogueUIData& menu,
									   const simpleList<brogueUIData>& buttons,
									   const brogueUIData& header,
									   bool useHeader, gridRect sceneBoundary, gridRect viewBoundary)
		: brogueViewContainer(sceneBoundary, viewBoundary)
	{
		_menuData = new brogueUIData(menu);

		// Header View
		if (useHeader)
			this->addView((brogueView*)new brogueText(header, sceneBoundary, header.boundary));

		// Vertical Buttons
		for (int index = 0; index < buttons.count(); index++)
		{
			// Button View
			this->addView((brogueView*)new brogueButton(buttons.get(index), sceneBoundary, buttons.get(index).boundary));
		}

		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueButtonMenu::~brogueButtonMenu()
	{
		delete _menuData;
	}
	void brogueButtonMenu::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueButtonMenu* that = this;
		brogueUIData menuData = *_menuData;

		// Background
		this->getBoundary().iterate([&that, &menuData] (short column, short row)
		{
			that->get(column, row)->backColor = menuData.calculateGradient(column, row);

			return iterationCallback::iterate;
		});

		// Header + Buttons
		this->iterateViews([&mouseState, &millisecondsLapsed] (brogueView* button)
		{
			button->update(mouseState, millisecondsLapsed);
			
			return iterationCallback::iterate;
		});
	}
}
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

		brogueButtonMenu(brogueUIData* menuData,
						 const simpleList<brogueUIData*>& buttons,
						 brogueUIData* header,
						 bool useHeader,
						 const gridRect& sceneBoundary,
						 const gridRect& viewBoundary);
		~brogueButtonMenu();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
	};

	brogueButtonMenu::brogueButtonMenu(brogueUIData* menuData,
									   const simpleList<brogueUIData*>& buttons,
									   brogueUIData* header,
									   bool useHeader,
									   const gridRect& sceneBoundary,
									   const gridRect& viewBoundary)
		: brogueViewContainer(menuData, sceneBoundary, viewBoundary)
	{
		// Header View
		if (useHeader)
			this->addView((brogueView*)new brogueText(header, sceneBoundary, header->getBounds()));

		// Vertical Buttons
		for (int index = 0; index < buttons.count(); index++)
		{
			// Button View
			this->addView((brogueView*)new brogueButton(buttons.get(index), sceneBoundary, buttons.get(index)->getBounds()));
		}

		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueButtonMenu::~brogueButtonMenu()
	{
	}
	void brogueButtonMenu::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueButtonMenu* that = this;
		brogueUIData* menuData = this->getUIData();

		// Check mouse hover
		bool mouseHover = this->getBoundary().contains(mouseState.getLocation());

		// Background
		this->getBoundary().iterate([&that, &menuData, &mouseHover] (short column, short row)
		{
			that->get(column, row)->backColor = menuData->calculateGradient(column, row, mouseHover);

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
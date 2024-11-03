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
		virtual bool shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;

		// Override needed to check for the header
		virtual brogueCellDisplay* get(short column, short row) const override;

	private:

		brogueText* _header;
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
			_header = new brogueText(header, sceneBoundary, header->getBounds());

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
		delete _header;
	}
	brogueCellDisplay* brogueButtonMenu::get(short column, short row) const
	{
		// Check for header boundaries (header should be left visible)
		if (_header != nullptr && _header->getBoundary().contains(column, row))
		{
			return _header->get(column, row);
		}

		return brogueViewContainer::get(column, row);
	}
	bool brogueButtonMenu::shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		// Check mouse hover
		bool mouseHover = this->getBoundary().contains(mouseState.getLocation());

		// Check mouse scroll
		if (mouseState.getScrollPending() && mouseState.getIsScrollUp() && mouseHover)
			return true;

		brogueButtonMenu* that = this;
		bool result = false;

		// Header + Buttons
		this->iterateViews([&that, &mouseState, &millisecondsLapsed, &result] (brogueView* button)
		{
			result |= button->shouldUpdate(mouseState, millisecondsLapsed);

			if (result)
				return iterationCallback::breakAndReturn;
			else
				return iterationCallback::iterate;
		});

		return result;
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
		this->iterateViews([&that, &mouseState, &millisecondsLapsed] (brogueView* button)
		{
			// Set Visibility
			bool visibility = that->getPaddedBoundary().contains(button->getBoundary());

			button->setVisiblity(visibility);
			button->update(mouseState, millisecondsLapsed);
			
			return iterationCallback::iterate;
		});
	}
}
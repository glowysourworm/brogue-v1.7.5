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
		bool mouseHover = this->isMouseOver(mouseState);

		// Check mouse scroll
		bool scrollUpdate = mouseState.getScrollPending() && mouseHover;

		if (scrollUpdate)
		{
			if (!mouseState.getIsScrollUp())
			{
				// Header Offset
				int headerOffset = _header != nullptr ? 1 : 0;

				// Check scroll bounds
				int overflow = this->getViewCount() - (this->getPaddedBoundary().height - headerOffset);

				// Calculate the number of slots (TODO: USE BOUNDS)
				int nextOverflow = overflow - this->getRenderOffset().row;

				if (nextOverflow > 0)
					this->incrementRenderOffset(0, 1);
			}
			else
			{
				// Make sure you're not at the top
				if (this->getRenderOffset().row > 0)
					this->incrementRenderOffset(0, -1);
			}
		}
			

		// Check just the header, then pass through to the base class
		bool shouldUpdate = _header == nullptr ? false : _header->shouldUpdate(mouseState, millisecondsLapsed);
		
		// Base class (child views)
		bool childUpdates = brogueViewContainer::shouldUpdate(mouseState, millisecondsLapsed);

		return shouldUpdate || childUpdates || scrollUpdate;
	}
	void brogueButtonMenu::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueButtonMenu* that = this;
		brogueUIData* menuData = this->getUIData();

		// Check mouse hover
		bool mouseHover = this->isMouseOver(mouseState);

		// Background
		this->getBoundary().iterate([&that, &menuData, &mouseHover] (short column, short row)
		{
			that->get(column, row)->backColor = menuData->calculateGradient(column, row, mouseHover);

			return iterationCallback::iterate;
		});

		// Header
		if (_header != nullptr)
			_header->update(mouseState, millisecondsLapsed);

		// Buttons
		this->iterateViews([&that, &mouseState, &millisecondsLapsed] (brogueView* button)
		{
			button->update(mouseState, millisecondsLapsed);
			
			return iterationCallback::iterate;
		});
	}
}
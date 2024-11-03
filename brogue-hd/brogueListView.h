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
	class brogueListView : public brogueViewContainer
	{
	public:

		brogueListView(brogueUIData* menuData,
					   const simpleList<brogueUIData*>& items,
					   brogueUIData* header,
					   brogueUIData* footer,
					   const gridRect& sceneBoundary,
					   const gridRect& viewBoundary);
		~brogueListView();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual bool shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;

		// Override needed to check for the header
		virtual brogueCellDisplay* get(short column, short row) const override;

	private:

		brogueText* _header;
		brogueText* _footer;
	};

	brogueListView::brogueListView(brogueUIData* menuData,
									const simpleList<brogueUIData*>& items,
									brogueUIData* header,
									brogueUIData* footer,
									const gridRect& sceneBoundary,
									const gridRect& viewBoundary)
		: brogueViewContainer(menuData, sceneBoundary, viewBoundary)
	{
		// Header View
		if (header != nullptr)
			_header = new brogueText(header, sceneBoundary, header->getBounds());

		// Vertical Buttons
		for (int index = 0; index < items.count(); index++)
		{
			// Button View
			this->addView((brogueView*)new brogueButton(items.get(index), sceneBoundary, items.get(index)->getBounds()));
		}

		if (footer != nullptr)
			_footer = new brogueText(footer, sceneBoundary, footer->getBounds());

		update(default_value::value<brogueMouseState>(), 0);
	}
	brogueListView::~brogueListView()
	{
		delete _header;
		delete _footer;
	}
	brogueCellDisplay* brogueListView::get(short column, short row) const
	{
		// Check for header boundaries (header should be left visible)
		if (_header != nullptr && _header->getBoundary().contains(column, row))
		{
			return _header->get(column, row);
		}

		if (_footer != nullptr && _footer->getBoundary().contains(column, row))
		{
			return _footer->get(column, row);
		}

		return brogueViewContainer::get(column, row);
	}
	bool brogueListView::shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		// No Mouse Interaction
		bool hasMouseInteraction = this->getUIData()->getHasMouseInteraction();

		// Check mouse hover
		bool mouseHover = this->isMouseOver(mouseState);

		// Check mouse scroll
		bool scrollUpdate = hasMouseInteraction && mouseState.getScrollPending() && mouseHover;

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
			

		// Check the header, the footer, then pass through to the base class
		bool shouldUpdate = _header == nullptr ? false : _header->shouldUpdate(mouseState, millisecondsLapsed);

		// Check the footer
		shouldUpdate |= _footer == nullptr ? false : _footer->shouldUpdate(mouseState, millisecondsLapsed);
		
		// Base class (child views)
		bool childUpdates = brogueViewContainer::shouldUpdate(mouseState, millisecondsLapsed);

		return shouldUpdate || childUpdates || scrollUpdate;
	}
	void brogueListView::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueListView* that = this;
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

		// Items
		this->iterateViews([&that, &mouseState, &millisecondsLapsed] (brogueView* item)
		{
			item->update(mouseState, millisecondsLapsed);
			
			return iterationCallback::iterate;
		});

		// Footer
		if (_footer != nullptr)
			_footer->update(mouseState, millisecondsLapsed);
	}
}
#pragma once

#include "brogueButton.h"
#include "brogueCellDisplay.h"
#include "brogueMouseState.h"
#include "brogueText.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIResponseData.h"
#include "brogueView.h"
#include "brogueViewContainer.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueListView : public brogueViewContainer
	{
	public:

		brogueListView(brogueUIView viewName,
					   brogueUIData* menuData,
					   const simpleList<brogueUIData*>& items,
					   brogueUIData* header,
					   brogueUIData* footer,
					   const gridRect& sceneBoundary,
					   const gridRect& viewBoundary);
		~brogueListView();

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual brogueUIResponseData& checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;

		// Override needed to check for the header
		virtual brogueCellDisplay* get(short column, short row) const override;

	private:

		brogueText* _header;
		brogueText* _footer;
	};

	brogueListView::brogueListView(brogueUIView viewName,
								   brogueUIData* menuData,
								   const simpleList<brogueUIData*>& items,
								   brogueUIData* header,
								   brogueUIData* footer,
								   const gridRect& sceneBoundary,
								   const gridRect& viewBoundary)
		: brogueViewContainer(viewName, menuData, sceneBoundary, viewBoundary)
	{
		// Header View
		if (header != nullptr)
			_header = new brogueText(brogueUIView::Unnamed, header, sceneBoundary, header->getBounds());

		// Vertical Buttons
		for (int index = 0; index < items.count(); index++)
		{
			// Button View
			this->addView((brogueView*)new brogueButton(brogueUIView::Unnamed, items.get(index), sceneBoundary, items.get(index)->getBounds()));
		}

		if (footer != nullptr)
			_footer = new brogueText(brogueUIView::Unnamed, footer, sceneBoundary, footer->getBounds());

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
	brogueUIResponseData& brogueListView::checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		brogueUIResponseData response;

		response.sender = this->getViewName();

		// Mouse Interaction
		response.mouseUsed = this->getUIData()->getHasMouseInteraction();

		// Check mouse hover
		response.mouseHover = this->isMouseOver(mouseState);

		// Check mouse scroll
		response.mouseScroll = mouseState.getScrollPending() && response.mouseHover;

		if (response.mouseScroll && response.mouseUsed)
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
		response.shouldUpdate |= _header == nullptr ? false : _header->checkUpdate(mouseState, millisecondsLapsed).shouldUpdate;

		// Check the footer
		response.shouldUpdate |= _footer == nullptr ? false : _footer->checkUpdate(mouseState, millisecondsLapsed).shouldUpdate;

		// Base class (child views)
		response.shouldUpdate = brogueViewContainer::checkUpdate(mouseState, millisecondsLapsed).shouldUpdate;

		return response;
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
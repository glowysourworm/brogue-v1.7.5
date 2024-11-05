#pragma once

#include "brogueButton.h"
#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
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

		brogueListView(brogueView* parentView,
					   brogueUIView viewName,
					   brogueUIData* menuData,
					   const simpleList<brogueUIData*>& items,
					   brogueUIData* header,
					   brogueUIData* footer);
		~brogueListView();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual void checkUpdate(brogueUIResponseData& response, 
								 const brogueKeyboardState& keyboardState, 
								 const brogueMouseState& mouseState, 
								 int millisecondsLapsed) override;

		// Override needed to check for the header
		virtual brogueCellDisplay* get(short column, short row) const override;

	private:

		brogueText* _header;
		brogueText* _footer;
	};

	brogueListView::brogueListView(brogueView* parentView,
								   brogueUIView viewName,
								   brogueUIData* menuData,
								   const simpleList<brogueUIData*>& items,
								   brogueUIData* header,
								   brogueUIData* footer)
		: brogueViewContainer(parentView, viewName, parentView->getSceneBoundary(), parentView->getBoundary())
	{
		// Header View
		if (header != nullptr)
			_header = new brogueText(brogueUIView::Unnamed, header, parentView->getSceneBoundary(), header->getBounds());

		// Vertical Buttons
		for (int index = 0; index < items.count(); index++)
		{
			// Button View
			this->addView((brogueView*)new brogueButton(brogueUIView::Unnamed, items.get(index), parentView->getSceneBoundary(), items.get(index)->getBounds()));
		}

		if (footer != nullptr)
			_footer = new brogueText(brogueUIView::Unnamed, footer, parentView->getSceneBoundary(), footer->getBounds());

		update(default_value::value<brogueKeyboardState>(), default_value::value<brogueMouseState>(), 0);
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
	void brogueListView::checkUpdate(brogueUIResponseData& response,
									 const brogueKeyboardState& keyboardState,
									 const brogueMouseState& mouseState,
									 int millisecondsLapsed)
	{
		/*
			UI Behavior:  This will be an override that takes the base behavior - favoring
						  the child views - but adding the scroll problem.

						  Header / Footer:  There could be a reason to update these, eventually. The
						  behavior would be the same. They must be rendered here becuase of the scroll
						  calculation.
		*/

		// Start with the base (marks the response) (favors the child views; but also gets a response from the parent view)
		brogueViewContainer::checkUpdate(response, keyboardState, mouseState, millisecondsLapsed);

		// Check the header
		bool headerResponse = _header == nullptr ? false : _header->checkUpdate(keyboardState, mouseState, millisecondsLapsed).needsUpdate;

		// Check the footer
		bool footerResponse = _footer == nullptr ? false : _footer->checkUpdate(keyboardState, mouseState, millisecondsLapsed).needsUpdate;

		response.response.needsUpdate |= headerResponse || footerResponse;

		// Check to see if there are child responses
		if (response.response.needsUpdate)
			return;

		// Otherwise, proceed to parent view
		brogueView* parentView = this->getParentView();

		bool mouseOver = parentView->isMouseOver(mouseState);
		bool mousePressed = mouseState.getMouseLeft();
		bool hasInteraction = parentView->getUIData()->getHasMouseInteraction();

		// Parent View -> Mouse
		if (hasInteraction && mouseOver)
		{
			bool scrollEvent = mouseState.getScrollPending();

			// Scrolling
			if (scrollEvent && !mouseState.getIsScrollUp())
			{
				// Header Offset
				int headerOffset = _header != nullptr ? 1 : 0;

				// Check scroll bounds
				int overflow = this->getViewCount() - (parentView->getPaddedBoundary().height - headerOffset);

				// Calculate the number of slots (TODO: USE BOUNDS)
				int nextOverflow = overflow - parentView->getRenderOffset().row;

				if (nextOverflow > 0)
					this->incrementRenderOffset(0, 1);
			}
			else if (scrollEvent)
			{
				// Make sure you're not at the top
				if (parentView->getRenderOffset().row > 0)
					this->incrementRenderOffset(0, -1);
			}

			// Update the UI data
			response.response.actionMet = this->getUIData()->setMouseUpdate(mousePressed, mouseOver);
			response.response.deactivated = !mouseOver && mousePressed;
			response.response.needsUpdate = scrollEvent;
			response.response.tag = *this->getUIData()->getAction();
		}
	}
	void brogueListView::update(const brogueKeyboardState& keyboardState,
								const brogueMouseState& mouseState,
								int millisecondsLapsed)
	{
		brogueView* parentView = this->getParentView();
		brogueUIData* menuData = parentView->getUIData();

		// Check mouse hover
		bool mouseHover = parentView->isMouseOver(mouseState);

		// Background
		parentView->getBoundary().iterate([&parentView, &menuData, &mouseHover] (short column, short row)
		{
			parentView->get(column, row)->backColor = menuData->calculateGradient(column, row);

			return iterationCallback::iterate;
		});

		// Header
		if (_header != nullptr)
			_header->update(keyboardState, mouseState, millisecondsLapsed);

		// Items
		this->iterateChildViews([&keyboardState, &mouseState, &millisecondsLapsed] (brogueView* item)
		{
			item->update(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});

		// Footer
		if (_footer != nullptr)
			_footer->update(keyboardState, mouseState, millisecondsLapsed);
	}
}
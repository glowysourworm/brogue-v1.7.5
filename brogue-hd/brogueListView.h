#pragma once

#include "brogueButton.h"
#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueText.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueView.h"
#include "brogueViewContainer.h"
#include "eventController.h"
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

		brogueListView(eventController* eventController,
					   brogueView* parentView,
					   brogueUIView viewName,
					   brogueUIData* menuData,
					   const simpleList<brogueUIData*>& items,
					   brogueUIData* header,
					   brogueUIData* footer);
		~brogueListView();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

		// Override needed to check for the header
		virtual brogueCellDisplay* get(short column, short row) const override;

	private:

		brogueText* _header;
		brogueText* _footer;
	};

	brogueListView::brogueListView(eventController* eventController,
								   brogueView* parentView,
								   brogueUIView viewName,
								   brogueUIData* menuData,
								   const simpleList<brogueUIData*>& items,
								   brogueUIData* header,
								   brogueUIData* footer)
		: brogueViewContainer(parentView, viewName, parentView->getSceneBoundary(), parentView->getBoundary())
	{
		// Header View
		if (header != nullptr)
			_header = new brogueText(eventController, brogueUIView::Unnamed, header, parentView->getSceneBoundary(), header->getBounds());

		// Vertical Buttons
		for (int index = 0; index < items.count(); index++)
		{
			brogueView* button = new brogueButton(eventController, brogueUIView::Unnamed, items.get(index), parentView->getSceneBoundary(), items.get(index)->getBounds());

			this->addView(button);
		}

		if (footer != nullptr)
			_footer = new brogueText(eventController, brogueUIView::Unnamed, footer, parentView->getSceneBoundary(), footer->getBounds());

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
	void brogueListView::checkUpdate(const brogueKeyboardState& keyboardState,
									 const brogueMouseState& mouseState,
									 int millisecondsLapsed)
	{
		/*
			UI Behavior:  This will add the scroll behavior.

						  Header / Footer:  There could be a reason to update these, eventually. The
						  behavior would be the same. They must be rendered here becuase of the scroll
						  calculation.
		*/

		// Start with the base (marks the response) (favors the child views; but also gets a response from the parent view)
		// Check child views first (for performance) (NOTE:  This function updates mouse related data; and must be run on child views)
		this->iterateChildViews([&mouseState, &keyboardState, &millisecondsLapsed] (brogueView* view)
		{
			view->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});

		// Header
		if (_header != nullptr)
			_header->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

		// Footer
		if (_footer != nullptr)
			_footer->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

		// Parent View (careful of child responses)
		brogueView* parentView = this->getParentView();

		bool mouseOver = parentView->isMouseOver(mouseState);
		bool mousePressed = mouseState.getMouseLeft();
		bool hasInteraction = parentView->getUIData()->getHasMouseInteraction();
		bool scrollEvent = mouseState.getScrollPending();

		// Parent View -> Mouse
		if (hasInteraction && mouseOver)
		{
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
		}

		// Register the parent view state
		parentView->getUIData()->setUpdate(mousePressed, mouseOver, scrollEvent && hasInteraction && mouseOver);
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
		{
			_header->update(keyboardState, mouseState, millisecondsLapsed);
		}

		// Items
		this->iterateChildViews([&keyboardState, &mouseState, &millisecondsLapsed] (brogueView* item)
		{
			item->update(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});

		// Footer
		if (_footer != nullptr)
		{
			_footer->update(keyboardState, mouseState, millisecondsLapsed);
		}
	}
}
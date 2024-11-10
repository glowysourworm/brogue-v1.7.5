#pragma once

#include "brogueCellDisplay.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "color.h"
#include "eventController.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"
#include "simpleOrderedList.h"
#include <functional>

using namespace brogueHd::component;
using namespace brogueHd::simple;
using namespace brogueHd::backend;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueComposedView : public brogueViewBase
	{
	public:

		brogueComposedView(eventController* eventController, const brogueUIData& uiData, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueComposedView();

		// Overrides manage child views
		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

		virtual bool needsUpdate() const override;
		virtual void clearUpdate() override;
		virtual void clearEvents() override;
		virtual brogueCellDisplay* get(short column, short row) const override;

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const override;

	public:

		// Child Views
		void addView(brogueViewBase* view);
		brogueViewBase* getView(int index) const;
		int getViewCount() const;

	private:

		brogueCellDisplay* _defaultCell;
		simpleOrderedList<brogueViewBase*>* _views;

	};

	brogueComposedView::brogueComposedView(eventController* eventController, const brogueUIData& uiData, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, uiData, sceneBoundary, viewBoundary)
	{
		_defaultCell = new brogueCellDisplay();
		_views = new simpleOrderedList<brogueViewBase*>([] (brogueViewBase* view1, brogueViewBase* view2)
		{
			return view1->getZIndex() - view2->getZIndex();
		});

		// This will be the view cell for the padded boundary
		_defaultCell->backColor = colors::transparent();
		_defaultCell->character = brogueGlyphMap::Empty;
		_defaultCell->foreColor = colors::transparent();
	}
	brogueComposedView::~brogueComposedView()
	{
		delete _views;
	}
	void brogueComposedView::addView(brogueViewBase* view)
	{
		_views->add(view);
	}
	brogueViewBase* brogueComposedView::getView(int index) const
	{
		return _views->get(index);
	}
	int brogueComposedView::getViewCount() const
	{
		return _views->count();
	}
	void brogueComposedView::iterate(gridCallback<brogueCellDisplay*> callback) const
	{
		const brogueComposedView* that = this;

		this->getBoundary().iterate([&callback, &that] (short column, short row)
		{
			brogueCellDisplay* cell = that->get(column, row);

			if (cell != nullptr)
				callback(column, row, cell);

			return iterationCallback::iterate;
		});
	}
	brogueCellDisplay* brogueComposedView::get(short column, short row) const
	{
		// Check padding first
		gridRect paddedBoundary = this->getPaddedBoundary();

		if (!paddedBoundary.contains(column, row))
			return _defaultCell;

		// SORTED BY Z-INDEX (Ascending)
		for (int index = _views->count() - 1; index >= 0; index--)
		{
			// This offset should be shared with the container (but it's better to check and be sure)
			gridLocator offset = _views->get(index)->getRenderOffset();

			// Check child view boundaries (this will be an improper child view)
			if (_views->get(index)->getBoundary().contains(offset.column + column, offset.row + row))
			{
				// Retrieve as if from normal boundaries (go ahead an show the improper child)
				return _views->get(index)->get(offset.column + column, offset.row + row);
			}
		}

		return _defaultCell;
	}
	void brogueComposedView::checkUpdate(const brogueKeyboardState& keyboardState,
										 const brogueMouseState& mouseState,
										 int millisecondsLapsed)
	{
		/*
			UI Behavior:  This will add the scroll behavior. (or x-y scrolling behavior)
		*/

		// This View (Behaves as brogueView)
		bool mouseOver = this->getRenderBoundary().contains(mouseState.getLocation());
		bool mousePressed = mouseState.getMouseLeft();
		bool hasInteraction = this->getHasMouseInteraction();
		bool scrollEvent = mouseState.getScrollPendingX() || mouseState.getScrollPendingY();

		gridRect boundary = this->getBoundary();
		gridRect paddedBoundary = this->getPaddedBoundary();
		gridRect childBoundary = default_value::value<gridRect>();

		// Child Views
		_views->forEach([&mouseState, &keyboardState, &millisecondsLapsed, &childBoundary] (brogueViewBase* view)
		{
			view->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			gridRect viewBounds = view->getBoundary();

			if (childBoundary == default_value::value<gridRect>())
				childBoundary = viewBounds;

			else
			{
				childBoundary.expand(viewBounds.left(), viewBounds.top());
				childBoundary.expand(viewBounds.right(), viewBounds.bottom());
			}

			return iterationCallback::iterate;
		});

		// Check scroll bounds
		int scrollX = 0;
		int scrollY = 0;

		// Parent View -> Mouse Over -> Scroll
		if (hasInteraction && mouseOver && scrollEvent)
		{
			if (mouseState.getScrollPendingX())
			{
				if (mouseState.getScrollNegativeX() && childBoundary.left() > paddedBoundary.left())
					scrollX = -1;

				else if (!mouseState.getScrollNegativeX() && childBoundary.right() < paddedBoundary.right())
					scrollX = 1;
			}
			if (mouseState.getScrollPendingY())
			{
				if (mouseState.getScrollNegativeY() && childBoundary.top() > paddedBoundary.top())
					scrollY = -1;

				else if (!mouseState.getScrollNegativeY() && childBoundary.bottom() < paddedBoundary.bottom())
					scrollY = 1;
			}

			this->incrementRenderOffset(scrollX, scrollY);
		}

		// This View -> Update UI Data (no mouse interaction)
		this->setUpdate(mouseState.getMouseLeft(), mouseOver, (scrollX != 0 || scrollY != 0));
	}
	bool brogueComposedView::needsUpdate() const
	{
		bool result = false;

		// Items
		_views->forEach([&result] (brogueViewBase* item)
		{
			result |= item->needsUpdate();

			return iterationCallback::iterate;
		});

		return result;
	}
	void brogueComposedView::update(const brogueKeyboardState& keyboardState,
									const brogueMouseState& mouseState,
									int millisecondsLapsed)
	{
		_views->forEach([&keyboardState, &mouseState, &millisecondsLapsed] (brogueViewBase* item)
		{
			if (item->needsUpdate())
				item->update(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}
	void brogueComposedView::clearUpdate()
	{
		_views->forEach([] (brogueViewBase* item)
		{
			item->clearUpdate();

			return iterationCallback::iterate;
		});
	}
	void brogueComposedView::clearEvents()
	{
		_views->forEach([] (brogueViewBase* item)
		{
			item->clearEvents();

			return iterationCallback::iterate;
		});
	}
}
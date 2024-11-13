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
#include "gridRect.h"
#include "simple.h"
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
		virtual void update(int millisecondsLapsed,
							bool forceUpdate) override;

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

		virtual bool needsUpdate() const override;
		virtual void clearUpdate() override;
		virtual void clearEvents() override;
		virtual brogueCellDisplay* get(short column, short row) const override;

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const override;

	public:		// Child Views

		void addView(brogueViewBase* view);

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
	void brogueComposedView::iterate(gridCallback<brogueCellDisplay*> callback) const
	{
		const brogueComposedView* that = this;
		brogueCellDisplay* defaultCell = _defaultCell;

		this->getBoundary().iterate([&callback, &that, &defaultCell] (short column, short row)
		{
			brogueCellDisplay* cell = that->get(column, row);

			if (cell != nullptr)
				callback(column, row, cell);
			else
				callback(column, row, defaultCell);

			return iterationCallback::iterate;
		});
	}
	brogueCellDisplay* brogueComposedView::get(short column, short row) const
	{
		// SORTED BY Z-INDEX (Ascending)
		for (int index = _views->count() - 1; index >= 0; index--)
		{
			// Check child view boundaries (this will be an improper child view)
			if (_views->get(index)->getBoundary().contains(column, row))
			{
				// Retrieve as if from normal boundaries (go ahead an show the improper child)
				return _views->get(index)->get(column, row);
			}
		}

		return _defaultCell;
	}
	void brogueComposedView::checkUpdate(const brogueKeyboardState& keyboardState,
										 const brogueMouseState& mouseState,
										 int millisecondsLapsed)
	{
		/*
			UI Behavior:  Cater to the child views; and return their cells in z-index order
		*/

		// This View (Behaves as brogueView)
		bool mouseOver = this->getBoundary().contains(mouseState.getLocation());
		bool mousePressed = mouseState.getMouseLeft();
		bool hasInteraction = this->getHasMouseInteraction();
		bool scrollEvent = mouseState.getScrollPendingX() || mouseState.getScrollPendingY();

		// Child Views
		_views->forEach([&mouseState, &keyboardState, &millisecondsLapsed] (brogueViewBase* view)
		{
			view->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});

		// This View -> Update UI Data (no mouse interaction)
		this->setUpdate(mousePressed, mouseOver);
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
	void brogueComposedView::update(int millisecondsLapsed, bool forceUpdate)
	{
		_views->forEach([&millisecondsLapsed, &forceUpdate] (brogueViewBase* item)
		{
			if (item->needsUpdate() || forceUpdate)
				item->update(millisecondsLapsed, forceUpdate);

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
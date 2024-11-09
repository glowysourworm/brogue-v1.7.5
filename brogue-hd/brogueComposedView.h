#pragma once

#include "brogueCellDisplay.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "color.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;
using namespace brogueHd::backend;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueComposedView : public brogueViewBase
	{
	public:

		brogueComposedView(const brogueUIData& uiData);
		~brogueComposedView();

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed) override;

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

		// Override needed to check for the header
		virtual brogueCellDisplay* get(short column, short row) const override;

		virtual brogueUIData* getUIData() const override;
		virtual gridRect getRenderBoundary() const override;

	public:

		// Child Views
		void addView(brogueViewBase* view);
		brogueViewBase* getView(int index) const;
		int getViewCount() const;

	private:

		brogueUIData* _uiData;
		brogueCellDisplay* _defaultCell;
		simpleList<brogueViewBase*>* _views;

	};

	brogueComposedView::brogueComposedView(const brogueUIData& uiData)
	{
		_uiData = new brogueUIData(uiData);
		_defaultCell = new brogueCellDisplay();
		_views = new simpleList<brogueViewBase*>();

		// This will be the view cell for the padded boundary
		_defaultCell->backColor = colors::transparent();
		_defaultCell->character = brogueGlyphMap::Empty;
		_defaultCell->foreColor = colors::transparent();
	}
	brogueComposedView::~brogueComposedView()
	{
		delete _views;
		delete _uiData;
	}
	brogueUIData* brogueComposedView::getUIData() const
	{
		return _uiData;
	}
	gridRect brogueComposedView::getRenderBoundary() const
	{
		gridRect boundary = _uiData->getPaddedBoundary();
		gridLocator offset = _uiData->getRenderOffset();

		boundary.translate(-1 * offset.column, -1 * offset.row);

		return boundary;
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
	brogueCellDisplay* brogueComposedView::get(short column, short row) const
	{
		// Check padding first
		gridRect paddedBoundary = this->getUIData()->getPaddedBoundary();

		if (!paddedBoundary.contains(column, row))
			return _defaultCell;

		for (int index = 0; index < _views->count(); index++)
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
		bool hasInteraction = this->getUIData()->getHasMouseInteraction();
		bool scrollEvent = mouseState.getScrollPendingX() || mouseState.getScrollPendingY();

		gridRect boundary = _uiData->getBounds();
		gridRect paddedBoundary = _uiData->getPaddedBoundary();
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
		_uiData->setUpdate(mouseState.getMouseLeft(), mouseOver, (scrollX != 0 || scrollY != 0));
	}
	void brogueComposedView::update(const brogueKeyboardState& keyboardState,
									const brogueMouseState& mouseState,
									int millisecondsLapsed)
	{
		// Items
		_views->forEach([&keyboardState, &mouseState, &millisecondsLapsed] (brogueViewBase* item)
		{
			item->update(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}
}
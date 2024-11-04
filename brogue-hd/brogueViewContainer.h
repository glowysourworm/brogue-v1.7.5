#pragma once

#include "brogueCellDisplay.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIResponseData.h"
#include "brogueView.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	/// <summary>
	/// This container class will provide overrides of the base class and return / utilize
	/// child views where there are child views defined. Otherwise, it will utilize its own
	/// grid of cells.
	/// </summary>
	class brogueViewContainer : public brogueView
	{
	public:

		brogueViewContainer(brogueUIView viewName, brogueUIData* uiData, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueViewContainer();

		void addView(brogueView* view);

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual brogueUIResponseData& checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;

		virtual brogueCellDisplay* get(short column, short row) const override;

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const override;
		void iterateAdjacent(short column, short row, gridCallbackAdjacent<brogueCellDisplay*> callback) const override;
		void iterateViews(simpleListCallback<brogueView*> callback) const;

	protected:

		virtual void incrementRenderOffset(short columnOffset, short rowOffset) override
		{
			// Increment Child Views
			for (int index = 0; index < _childViews->count(); index++)
			{
				_childViews->get(index)->incrementRenderOffset(columnOffset, rowOffset);
			}

			// Increment This View
			brogueView::incrementRenderOffset(columnOffset, rowOffset);
		}

		int getViewCount() const
		{
			return _childViews->count();
		}

	private:

		simpleList<brogueView*>* _childViews;
	};

	brogueViewContainer::brogueViewContainer(brogueUIView viewName, brogueUIData* uiData, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueView(viewName, uiData, sceneBoundary, viewBoundary)
	{
		_childViews = new simpleList<brogueView*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _childViews;
	}
	brogueCellDisplay* brogueViewContainer::get(short column, short row) const
	{
		// Check padding first
		if (!this->getPaddedBoundary().contains(column, row))
			return brogueView::get(column, row);

		for (int index = 0; index < _childViews->count(); index++)
		{
			// This offset should be shared with the container (but it's better to check and be sure)
			gridLocator offset = _childViews->get(index)->getRenderOffset();

			// Check child view boundaries (this will be an improper child view)
			if (_childViews->get(index)->getBoundary().contains(offset.column + column, offset.row + row))
			{
				// Retrieve as if from normal boundaries (go ahead an show the improper child)
				return _childViews->get(index)->get(offset.column + column, offset.row + row);
			}
		}

		return brogueView::get(column, row);
	}
	void brogueViewContainer::iterate(gridCallback<brogueCellDisplay*> callback) const
	{
		const brogueViewContainer* that = this;

		brogueView::iterate([&that, &callback] (short acolumn, short arow, brogueCellDisplay* cell)
		{
			brogueCellDisplay* childOrContainerCell = that->get(acolumn, arow);

			return callback(acolumn, arow, childOrContainerCell);
		});
	}
	void brogueViewContainer::iterateAdjacent(short column, short row, gridCallbackAdjacent<brogueCellDisplay*> callback) const
	{
		const brogueViewContainer* that = this;

		brogueView::iterateAdjacent(column, row, [&that, &callback] (short acolumn, short arow, brogueCompass direction, brogueCellDisplay* cell)
		{
			brogueCellDisplay* childOrContainerCell = that->get(acolumn, arow);

			return callback(acolumn, arow, direction, childOrContainerCell);
		});
	}
	void brogueViewContainer::addView(brogueView* view)
	{
		_childViews->add(view);
	}
	brogueUIResponseData& brogueViewContainer::checkUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		// Use container's response data to aggregate the child views
		brogueUIResponseData response;

		// Check child views first (for performance) (NOTE:  This function updates mouse related data; and must be run on child views)
		_childViews->forEach([&mouseState, &millisecondsLapsed, &response] (brogueView* view)
		{
			brogueUIResponseData childResponse = view->checkUpdate(mouseState, millisecondsLapsed);

			if (childResponse.shouldUpdate)
				response = childResponse;

			return iterationCallback::iterate;
		});

		// Set the repsonse view name to the container
		response.sender = this->getViewName();

		// Override with this container (wait for the caller to get child data)
		if (this->isMouseOver(mouseState) && this->getUIData()->getHasMouseInteraction())
		{
			response.mouseHover = true;
			response.mouseLeft = mouseState.getMouseLeft();
			response.mouseUsed = mouseState.getMouseLeft();
			response.shouldUpdate = true;
		}

		return response;
	}
	void brogueViewContainer::update(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		// May be overridden.
		//
		brogueView::update(mouseState, millisecondsLapsed);

		// Call update afterwards (logically, we want to overlay the child views; but it may not matter here)
		//
		_childViews->forEach([&mouseState, &millisecondsLapsed] (brogueView* view)
		{
			view->update(mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::iterateViews(simpleListCallback<brogueView*> callback) const
	{
		_childViews->forEach([&callback] (brogueView* view)
		{
			return callback(view);
		});
	}
}
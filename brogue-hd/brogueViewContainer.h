#pragma once

#include "simpleList.h"
#include "brogueView.h"
#include "brogueMouseState.h"

using namespace brogueHd::simple;

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

		brogueViewContainer(gridRect sceneBoundary, gridRect viewBoundary);
		~brogueViewContainer();

		void addView(brogueView* view);

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed) override;
		virtual bool shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed) override;

		virtual brogueCellDisplay* get(short column, short row) const override;

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const override;
		void iterateAdjacent(short column, short row, gridCallbackAdjacent<brogueCellDisplay*> callback) const override;

		void iterateViews(simpleListCallback<brogueView*> callback) const;

	private:

		simpleList<brogueView*>* _childViews;
	};

	brogueViewContainer::brogueViewContainer(gridRect sceneBoundary, gridRect viewBoundary)
		: brogueView(sceneBoundary, viewBoundary)
	{
		_childViews = new simpleList<brogueView*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _childViews;
	}
	brogueCellDisplay* brogueViewContainer::get(short column, short row) const
	{
		for (int index = 0; index < _childViews->count(); index++)
		{
			// Check child view boundaries
			if (_childViews->get(index)->getBoundary().contains(column, row))
			{
				return _childViews->get(index)->get(column, row);
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
	bool brogueViewContainer::shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
	{
		bool childViewInvalidated = false;

		// Check child views first (for performance)
		_childViews->forEach([&mouseState, &millisecondsLapsed, &childViewInvalidated] (brogueView* view)
		{
			childViewInvalidated = view->shouldUpdate(mouseState, millisecondsLapsed);

			if (childViewInvalidated)
				return iterationCallback::breakAndReturn;
			else
				return iterationCallback::iterate;
		});

		return childViewInvalidated || this->getBoundary().contains(mouseState.getLocation());
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
		_childViews->forEach([&callback](brogueView* view)
		{
			return callback(view);
		});
	}
}
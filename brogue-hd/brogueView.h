#pragma once

#include "grid.h"
#include "gridRect.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	class brogueView
	{
	public:
		
		brogueView(gridRect sceneBoundary, gridRect viewBoundary);
		~brogueView();

		brogueCellDisplay* get(short column, short row) const;

		gridRect getParentBoundary() const;
		gridRect getBoundary() const;

		void iterate(gridCallback<brogueCellDisplay*> callback) const;

		virtual void update(int millisecondsLapsed)
		{

		}

	private:

		grid<brogueCellDisplay*>* _view;
	};

	brogueView::brogueView(gridRect sceneBoundary, gridRect viewBoundary)
	{
		_view = new grid<brogueCellDisplay*>(sceneBoundary, viewBoundary);

		grid<brogueCellDisplay*>* grid = _view;

		viewBoundary.iterate([&grid] (short column, short row)
		{
			grid->set(column, row, new brogueCellDisplay());
			return iterationCallback::iterate;
		});
	}

	brogueView::~brogueView()
	{
		delete _view;
	}

	brogueCellDisplay* brogueView::get(short column, short row) const
	{
		return _view->get(column, row);
	}

	gridRect brogueView::getBoundary() const
	{
		return _view->getRelativeBoundary();
	}

	gridRect brogueView::getParentBoundary() const
	{
		return _view->getParentBoundary();
	}

	void brogueView::iterate(gridCallback<brogueCellDisplay*> callback) const
	{
		_view->iterate(callback);
	}
}


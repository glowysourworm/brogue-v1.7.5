#pragma once

#include "grid.h"
#include "gridRect.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	class brogueView
	{
	public:
		
		brogueView(){};
		brogueView(gridRect sceneBoundary, gridRect viewBoundary);
		~brogueView();

		brogueCellDisplay get(short column, short row) const;

		gridRect getSceneBoundary() const;
		gridRect getViewBoundary() const;

		void iterate(gridCallback<brogueCellDisplay> callback) const;

	protected:

		void update(const brogueCellDisplay& cell, short column, short row);

	private:

		grid<brogueCellDisplay>* _view;
	};

	brogueView::brogueView(gridRect sceneBoundary, gridRect viewBoundary)
	{
		_view = new grid<brogueCellDisplay>(sceneBoundary, viewBoundary);
	}
	brogueView::~brogueView()
	{
		delete _view;
	}
	brogueCellDisplay brogueView::get(short column, short row) const
	{
		return _view->get(column, row);
	}
	gridRect brogueView::getSceneBoundary() const
	{
		return _view->getParentBoundary();
	}
	gridRect brogueView::getViewBoundary() const
	{
		return _view->getRelativeBoundary();
	}
	void brogueView::update(const brogueCellDisplay& display, short column, short row)
	{
		_view->get(column, row).update(display);
	}

	void brogueView::iterate(gridCallback<brogueCellDisplay> callback) const
	{
		_view->iterate(callback);
	}
}


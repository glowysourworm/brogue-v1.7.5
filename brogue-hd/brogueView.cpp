#include "brogueView.h"
#include "brogueCellDisplay.h"

#include "gridRect.h"
#include "grid.h"


using namespace brogueHd::component;

namespace brogueHd::frontend::ui
{
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
		_view->get(column,row).update(display);
	}

	void brogueView::iterate(gridCallback<brogueCellDisplay> callback) const
	{
		_view->iterate(callback);
	}
}
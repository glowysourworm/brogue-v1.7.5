#include "brogueView.h"
#include "brogueCellDisplay.h"

#include "gridRect.h"
#include "grid.h"


using namespace brogueHd::component;

namespace brogueHd::frontend::ui
{
	brogueView::brogueView(gridRect boundary)
	{
		_view = new grid<brogueCellDisplay>(boundary, boundary);
	}
	brogueView::~brogueView()
	{
		delete _view;
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
#include "brogueView.h"
#include "gridRect.h"
#include "grid.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model
{
	brogueView::brogueView(gridRect boundary)
	{
		_view = new grid<brogueCellDisplay*>(boundary, boundary);
	}
	brogueView::~brogueView()
	{
		delete _view;
	}

	void brogueView::update(const brogueCellDisplay& display, short column, short row)
	{
		_view->get(column,row)->update(display);
	}
}
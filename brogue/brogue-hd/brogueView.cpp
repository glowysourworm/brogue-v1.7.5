#include "brogueView.h"
#include "gridRect.h"
#include "array2D.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model
{
	brogueView::brogueView(gridRect boundary)
	{
		_view = new array2D<brogueCellDisplay*>(boundary);
	}
	brogueView::~brogueView()
	{
		delete _view;
	}

	void brogueView::update(const brogueCellDisplay& display, short column, short row)
	{
		_view[column][row].update(display);
	}
}
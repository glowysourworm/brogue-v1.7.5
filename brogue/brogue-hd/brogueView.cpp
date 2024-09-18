#include "brogueView.h"
#include "gridRect.h"
#include "array2D.h"
#include "brogueCellDisplay.h"

namespace brogueHd
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
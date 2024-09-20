#pragma once

#include "array2D.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::backend::model::layout;

namespace brogueHd::backend::model
{
	class brogueView
	{
	public:

		brogueView(gridRect boundary);
		~brogueView();

		void update(const brogueCellDisplay& display, short column, short row);

	private:

		array2D<brogueCellDisplay*>* _view;
	};
}


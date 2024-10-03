#pragma once

#include "grid.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	class brogueView
	{
	public:

		brogueView(gridRect boundary);
		~brogueView();

		void update(const brogueCellDisplay& display, short column, short row);

		void iterate(gridCallback<brogueCellDisplay> callback) const;

	private:

		grid<brogueCellDisplay>* _view;
	};
}


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
}


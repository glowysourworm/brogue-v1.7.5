#pragma once

#include "grid.h"
#include "gridRect.h"
#include "brogueCellDisplay.h"
#include "brogueModel.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	template<typename T>
	class brogueView
	{
	public:
		
		brogueView(gridRect sceneBoundary, gridRect viewBoundary);
		~brogueView();

		brogueCellDisplay<T>* get(short column, short row) const;

		gridRect getParentBoundary() const;
		gridRect getBoundary() const;

		void iterate(gridCallback<brogueCellDisplay<T>*> callback) const;

	private:

		grid<brogueCellDisplay<T>*>* _view;
	};

	template<typename T>
	brogueView<T>::brogueView(gridRect sceneBoundary, gridRect viewBoundary)
	{
		_view = new grid<brogueCellDisplay<T>*>(sceneBoundary, viewBoundary);

		grid<brogueCellDisplay<T>*>* grid = _view;

		viewBoundary.iterate([&grid] (short column, short row)
		{
			grid->set(column, row, new brogueCellDisplay<T>());
			return iterationCallback::iterate;
		});
	}

	template<typename T>
	brogueView<T>::~brogueView()
	{
		delete _view;
	}

	template<typename T>
	brogueCellDisplay<T>* brogueView<T>::get(short column, short row) const
	{
		return _view->get(column, row);
	}

	template<typename T>
	gridRect brogueView<T>::getBoundary() const
	{
		return _view->getRelativeBoundary();
	}

	template<typename T>
	gridRect brogueView<T>::getParentBoundary() const
	{
		return _view->getParentBoundary();
	}

	template<typename T>
	void brogueView<T>::iterate(gridCallback<brogueCellDisplay<T>*> callback) const
	{
		_view->iterate(callback);
	}
}


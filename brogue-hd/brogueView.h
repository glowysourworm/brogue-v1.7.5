#pragma once

#include "grid.h"
#include "gridRect.h"
#include "brogueUIData.h"
#include "brogueMouseState.h"
#include "brogueCellDisplay.h"

using namespace brogueHd::frontend::opengl;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	class brogueView
	{
	public:
		
		brogueView(brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueView();

		gridRect getSceneBoundary() const;
		gridRect getBoundary() const;
		gridRect getPaddedBoundary() const;

		/// <summary>
		/// (TODO: MOVE THIS) Calculates the view's boundary is UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		gridRect calculateSceneBoundaryUI() const
		{
			gridRect sceneBoundary = getSceneBoundary();
			gridRect boundaryUI = gridRect(sceneBoundary.left() * brogueCellDisplay::CellWidth,
											sceneBoundary.top() * brogueCellDisplay::CellHeight,
											sceneBoundary.width * brogueCellDisplay::CellWidth,
											sceneBoundary.height * brogueCellDisplay::CellHeight);

			return boundaryUI;
		}
		gridRect calculateViewBoundaryUI() const
		{
			gridRect viewBoundary = getBoundary();
			gridRect boundaryUI = gridRect(viewBoundary.left() * brogueCellDisplay::CellWidth,
											viewBoundary.top() * brogueCellDisplay::CellHeight,
											viewBoundary.width * brogueCellDisplay::CellWidth,
											viewBoundary.height * brogueCellDisplay::CellHeight);

			return boundaryUI;
		}

		virtual brogueCellDisplay* get(short column, short row) const;

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const;
		virtual void iterateAdjacent(short column, short row, gridCallbackAdjacent<brogueCellDisplay*> callback) const;

		virtual void update(const brogueMouseState& mouseState, int millisecondsLapsed)
		{

		}

		virtual bool shouldUpdate(const brogueMouseState& mouseState, int millisecondsLapsed)
		{
			return false;
		}

		void setVisiblity(bool isVisible)
		{
			_uiData->setVisiblity(isVisible);
		}

		bool getVisibility() const
		{
			return _uiData->getVisibility();
		}

	protected:

		brogueUIData* getUIData() const
		{
			return _uiData;
		}

	private:

		grid<brogueCellDisplay*>* _view;

		brogueUIData* _uiData;
	};

	brogueView::brogueView(brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
	{
		_view = new grid<brogueCellDisplay*>(sceneBoundary, viewBoundary);
		_uiData = data;

		grid<brogueCellDisplay*>* grid = _view;

		viewBoundary.iterate([&grid] (short column, short row)
		{
			grid->set(column, row, new brogueCellDisplay());
			return iterationCallback::iterate;
		});
	}

	brogueView::~brogueView()
	{
		delete _view;
		delete _uiData;
	}

	brogueCellDisplay* brogueView::get(short column, short row) const
	{
		return _view->get(column, row);
	}

	gridRect brogueView::getBoundary() const
	{
		return _view->getRelativeBoundary();
	}
	gridRect brogueView::getPaddedBoundary() const
	{
		return _uiData->getPaddedBoundary();
	}
	gridRect brogueView::getSceneBoundary() const
	{
		return _view->getParentBoundary();
	}

	void brogueView::iterate(gridCallback<brogueCellDisplay*> callback) const
	{
		_view->iterate(callback);
	}

	void brogueView::iterateAdjacent(short column, short row, gridCallbackAdjacent<brogueCellDisplay*> callback) const
	{
		_view->iterateAdjacent(column, row, false, callback);
	}
}


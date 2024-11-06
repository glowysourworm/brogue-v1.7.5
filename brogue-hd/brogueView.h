#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIData.h"
#include "brogueUIEvent.h"
#include "brogueViewBase.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"

#include "brogueUITagAction.h"
#include "eventController.h"

using namespace brogueHd::backend::controller;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	class brogueView : public brogueViewBase
	{
	public:

		brogueView(eventController* eventController, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueView();

		gridRect getSceneBoundary() const override;
		gridRect getBoundary() const override;
		gridRect getPaddedBoundary() const override;
		gridRect getRenderBoundary() const override;

		/// <summary>
		/// (TODO: MOVE THIS) Calculates the view's boundary is UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		gridRect calculateSceneBoundaryUI() const override
		{
			gridRect sceneBoundary = getSceneBoundary();
			gridRect boundaryUI = gridRect(sceneBoundary.left() * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   sceneBoundary.top() * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()),
										   sceneBoundary.width * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   sceneBoundary.height * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()));

			return boundaryUI;
		}
		gridRect calculateViewBoundaryUI() const override
		{
			gridRect viewBoundary = getBoundary();
			gridRect boundaryUI = gridRect(viewBoundary.left() * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   viewBoundary.top() * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()),
										   viewBoundary.width * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   viewBoundary.height * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()));

			return boundaryUI;
		}

		virtual void raiseClickEvent(const brogueUITagAction& response) override
		{
			_eventController->getUIClickEvent()->publish(response);
		}

		virtual brogueCellDisplay* get(short column, short row) const override;

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const override;

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override
		{
			_uiData->setUpdate(mouseState.getMouseLeft(), this->isMouseOver(mouseState));

			if (_uiData->getMouseClick())
			{
				if (_uiData->getHasMouseInteraction())
				{
					this->raiseClickEvent(*_uiData->getAction());
				}
				_uiData->clearCapture();
			}
		}
		virtual void clearUpdate() override
		{
			_uiData->clearUpdate();
		}

		virtual bool needsUpdate() const override
		{
			return _uiData->needsUpdate();
		}

		virtual void incrementRenderOffset(short columnOffset, short rowOffset) override
		{
			gridLocator offset = _uiData->getRenderOffset();

			_uiData->setRenderOffset(offset.column + columnOffset, offset.row + rowOffset);
		}

		gridLocator getRenderOffset() const override
		{
			return _uiData->getRenderOffset();
		}

		bool isMouseOver(const brogueMouseState& mouseState) override
		{
			return this->getRenderBoundary().contains(mouseState.getLocation());
		}

		int getZoomLevel() const override
		{
			return _uiData->getZoomLevel();
		}

		brogueUIData* getUIData() const override
		{
			return _uiData;
		}

	private:

		grid<brogueCellDisplay*>* _view;

		brogueUIData* _uiData;

	private:

		eventController* _eventController;
	};

	brogueView::brogueView(eventController* eventController, brogueUIData* data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
	{
		_eventController = eventController;
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
	gridRect brogueView::getRenderBoundary() const
	{
		gridRect boundary = _uiData->getPaddedBoundary();
		gridLocator offset = _uiData->getRenderOffset();

		boundary.translate(-1 * offset.column, -1 * offset.row);

		return boundary;
	}

	void brogueView::iterate(gridCallback<brogueCellDisplay*> callback) const
	{
		_view->iterate(callback);
	}
}


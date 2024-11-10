#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueUITagAction.h"
#include "color.h"
#include "eventController.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"

using namespace brogueHd::backend;

namespace brogueHd::frontend
{
	class brogueViewBase
	{

	public:

		brogueViewBase(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueViewBase();

		virtual gridRect getSceneBoundary() const;
		virtual gridRect getBoundary() const;
		virtual gridRect getPaddedBoundary() const;
		virtual gridRect getRenderBoundary() const;

		/// <summary>
		/// (TODO: MOVE THIS) Calculates the view's scene boundary is UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		gridRect calculateSceneBoundaryUI() const;

		/// <summary>
		/// Calculates the view's boundary in UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		/// <returns></returns>
		gridRect calculateViewBoundaryUI() const;

		/// <summary>
		/// Gets the specified view cell
		/// </summary>
		virtual brogueCellDisplay* get(short column, short row) const;

		/// <summary>
		/// Iterates the view's cells. This will happen in the view's relative boundary.
		/// </summary>
		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const;

		/// <summary>
		/// Overload of the checkUpdate function behaves as though the view is a child of a parent view
		/// </summary>
		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed);

		/// <summary>
		/// Updates the view's grid cells when there has been a change to the view
		/// </summary>
		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed);

		/// <summary>
		/// Clears update flags from the UI tree
		/// </summary>
		virtual void clearUpdate();

		/// <summary>
		/// Clears mouse interaction events from the UI tree
		/// </summary>
		virtual void clearEvents();

		/// <summary>
		/// Checks the tree to see if there are views that need updating.
		/// </summary>
		virtual bool needsUpdate() const;

	public:		// UI Data Functions

		brogueUIProgramPartId getPartId() const;

		void incrementRenderOffset(short columnOffset, short rowOffset);
		gridLocator getRenderOffset() const;
		color getBackgroundColor(short column, short row) const;
		color getForegroundColor(short column, short row) const;
		char getText(short column, short row) const;
		bool isMouseOver(const brogueMouseState& mouseState) const;
		bool isHotkey(short column, short row) const;
		bool getHasMouseInteraction() const;
		bool getMouseEnter() const;
		bool getMouseLeave() const;
		int getZoomLevel() const;
		int getZIndex() const;

	protected:

		void raiseClickEvent(const brogueUITagAction& response);
		virtual void setUpdate(bool mousePressed, bool mouseOver, bool forceUpdate = false);

	private:

		grid<brogueCellDisplay*>* _view;

		brogueUIData* _uiData;

	private:

		eventController* _eventController;
	};

	brogueViewBase::brogueViewBase(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
	{
		_eventController = eventController;
		_view = new grid<brogueCellDisplay*>(sceneBoundary, viewBoundary);
		_uiData = new brogueUIData(data);

		grid<brogueCellDisplay*>* grid = _view;

		viewBoundary.iterate([&grid] (short column, short row)
		{
			grid->set(column, row, new brogueCellDisplay());
			return iterationCallback::iterate;
		});
	}
	brogueViewBase::~brogueViewBase()
	{
		grid<brogueCellDisplay*>* grid = _view;

		_view->getRelativeBoundary().iterate([&grid] (short column, short row)
		{
			delete grid->get(column, row);
			return iterationCallback::iterate;
		});

		delete _view;
		delete _uiData;
	}
	brogueCellDisplay* brogueViewBase::get(short column, short row) const
	{
		return _view->get(column, row);
	}
	gridRect brogueViewBase::calculateSceneBoundaryUI() const
	{
		gridRect sceneBoundary = getSceneBoundary();
		gridRect boundaryUI = gridRect(sceneBoundary.left() * (double)brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
									   sceneBoundary.top() * (double)brogueCellDisplay::CellHeight(_uiData->getZoomLevel()),
									   sceneBoundary.width * (double)brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
									   sceneBoundary.height * (double)brogueCellDisplay::CellHeight(_uiData->getZoomLevel()));

		return boundaryUI;
	}
	gridRect brogueViewBase::calculateViewBoundaryUI() const
	{
		gridRect viewBoundary = getBoundary();
		gridRect boundaryUI = gridRect(viewBoundary.left() * (double)brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
									   viewBoundary.top() * (double)brogueCellDisplay::CellHeight(_uiData->getZoomLevel()),
									   viewBoundary.width * (double)brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
									   viewBoundary.height * (double)brogueCellDisplay::CellHeight(_uiData->getZoomLevel()));

		return boundaryUI;
	}
	gridRect brogueViewBase::getBoundary() const
	{
		return _view->getRelativeBoundary();
	}
	gridRect brogueViewBase::getPaddedBoundary() const
	{
		return _uiData->getPaddedBoundary();
	}
	gridRect brogueViewBase::getSceneBoundary() const
	{
		return _view->getParentBoundary();
	}
	gridRect brogueViewBase::getRenderBoundary() const
	{
		gridRect boundary = _uiData->getPaddedBoundary();
		gridLocator offset = _uiData->getRenderOffset();

		boundary.translate(-1 * offset.column, -1 * offset.row);

		return boundary;
	}
	brogueUIProgramPartId brogueViewBase::getPartId() const
	{
		return _uiData->getId();
	}
	void brogueViewBase::incrementRenderOffset(short columnOffset, short rowOffset)
	{
		gridLocator offset = _uiData->getRenderOffset();

		_uiData->setRenderOffset(offset.column + columnOffset, offset.row + rowOffset);
	}

	gridLocator brogueViewBase::getRenderOffset() const
	{
		return _uiData->getRenderOffset();
	}
	color brogueViewBase::getBackgroundColor(short column, short row) const
	{
		return _uiData->calculateGradient(column, row);
	}
	color brogueViewBase::getForegroundColor(short column, short row) const
	{
		return _uiData->getTextColor(column, row);
	}
	char brogueViewBase::getText(short column, short row) const
	{
		return _uiData->getText(column, row);
	}
	bool brogueViewBase::isHotkey(short column, short row) const
	{
		return _uiData->getIsHotkey(column, row);
	}
	bool brogueViewBase::getHasMouseInteraction() const
	{
		return _uiData->getHasMouseInteraction();
	}
	bool brogueViewBase::isMouseOver(const brogueMouseState& mouseState) const
	{
		return this->getRenderBoundary().contains(mouseState.getLocation());
	}
	bool brogueViewBase::getMouseEnter() const
	{
		return _uiData->getMouseEnter();
	}
	bool brogueViewBase::getMouseLeave() const
	{
		return _uiData->getMouseLeave();
	}
	int brogueViewBase::getZoomLevel() const
	{
		return _uiData->getZoomLevel();
	}
	int brogueViewBase::getZIndex() const
	{
		return _uiData->getZIndex();
	}
	void brogueViewBase::raiseClickEvent(const brogueUITagAction& response)
	{
		_eventController->getUIClickEvent()->publish(_uiData->getId().getName(), response);
	}
	void brogueViewBase::setUpdate(bool mousePressed, bool mouseOver, bool forceUpdate)
	{
		_uiData->setUpdate(mousePressed, mouseOver, forceUpdate);
	}
	void brogueViewBase::clearUpdate()
	{
		_uiData->clearUpdate();
	}
	void brogueViewBase::clearEvents()
	{
		_uiData->clearCapture();
	}
	void brogueViewBase::checkUpdate(const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed)
	{
		// Sets primary real time UI data for the mouse / live updates to the UI.
		_uiData->setUpdate(mouseState.getMouseLeft(), this->isMouseOver(mouseState));

		if (_uiData->getMouseClick() && _uiData->getHasMouseInteraction())
		{
			// UI EVENT:  Mouse Click
			this->raiseClickEvent(_uiData->getAction());

			// Clears UI event data for the mouse
			_uiData->clearCapture();
		}
	}
	void brogueViewBase::update(const brogueKeyboardState& keyboardState,
								const brogueMouseState& mouseState,
								int millisecondsLapsed)
	{
		throw simpleException("brogueViewBase::update must be overridden in the child class");
	}
	bool brogueViewBase::needsUpdate() const
	{
		return _uiData->needsUpdate();
	}
	void brogueViewBase::iterate(gridCallback<brogueCellDisplay*> callback) const
	{
		_view->iterate(callback);
	}
}
#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIMouseData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleHash.h"
#include "simpleList.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	/// <summary>
	/// This container class provide a mechanism to manage views or composed views. 
	/// </summary>
	class brogueViewContainer
	{
	public:

		brogueViewContainer(brogueUIProgram programName, int zoomLevel, bool hasScrollInteraction, bool applyClipping, const gridRect& containerBoundary);
		~brogueViewContainer();

		void addView(brogueViewBase* view);
		brogueViewBase* getView(const brogueUIProgramPartId& programPart) const;
		brogueViewBase* getViewAt(int index) const;
		int getViewCount() const;

	public:

		virtual void initiateStateChange(brogueUIState fromState, brogueUIState toState);
		virtual void clearStateChange();
		virtual bool checkStateChange();

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed);

		virtual void invalidate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState);

		bool needsUpdate(const brogueUIProgramPartId& partId) const;
		void clearUpdate(const brogueUIProgramPartId& partId);
		void clearEvents(const brogueUIProgramPartId& partId);

	public:		// brogueViewBase facade

		gridRect getSceneBoundary() const;
		gridRect getContainerBoundary() const;
		gridLocator getScrollOffset() const;
		vec2 getRenderOffsetUI() const;
		gridRect calculateSceneBoundaryUI() const;
		gridRect calculateViewBoundaryUI() const;
		int getZoomLevel() const;
		bool getClipping() const;
		brogueUIProgram getProgramName() const;

	protected:

		void setScrollOffset(short column, short row);
		void setRenderOffsetUI(int pixelX, float pixelY);
		brogueMouseState getAdjustedMouse(const brogueMouseState& mouseState) const;

	private:

		/// <summary>
		/// Returns aggregate child boundary with scroll offset
		/// </summary>
		gridRect getChildOffsetBoundary() const;

		void updateScroll(const brogueKeyboardState& keyboardState,
						  const brogueMouseState& mouseState,
						  int millisecondsLapsed);

	private:

		brogueUIProgram _programName;
		bool _hasScrollInteraction;
		bool _applyClipping;
		int _zoomLevel;
		brogueUIMouseData* _mouseData;									// Container mouse data (need enter / leave handling)
		gridRect* _containerBoundary;
		gridLocator* _scrollOffset;
		vec2* _renderOffset;											// Hard render offset for shader animation
		simpleHash<brogueUIProgramPartId, brogueViewBase*>* _views;
	};

	brogueViewContainer::brogueViewContainer(brogueUIProgram programName, int zoomLevel, bool hasScrollInteraction, bool applyClipping, const gridRect& containerBoundary)
	{
		_programName = programName;
		_zoomLevel = zoomLevel;
		_mouseData = new brogueUIMouseData();
		_containerBoundary = new gridRect(containerBoundary);
		_scrollOffset = new gridLocator(0,0);
		_renderOffset = new vec2(0, 0);
		_hasScrollInteraction = hasScrollInteraction;
		_applyClipping = applyClipping;
		_views = new simpleHash<brogueUIProgramPartId, brogueViewBase*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _views;
		delete _mouseData;
		delete _containerBoundary;
		delete _renderOffset;
		delete _scrollOffset;
	}
	void brogueViewContainer::addView(brogueViewBase* view)
	{
		_views->add(view->getPartId(), view);
	}
	brogueViewBase* brogueViewContainer::getView(const brogueUIProgramPartId& programPart) const
	{
		return _views->get(programPart);
	}
	brogueViewBase* brogueViewContainer::getViewAt(int index) const
	{
		return _views->getAt(index)->value;
	}
	brogueUIProgram brogueViewContainer::getProgramName() const
	{
		return _programName;
	}
	int brogueViewContainer::getViewCount() const
	{
		return _views->count();
	}
	gridRect brogueViewContainer::getSceneBoundary() const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::getSceneBoundary");

		return _views->getAt(0)->value->getSceneBoundary();
	}
	gridRect brogueViewContainer::getChildOffsetBoundary() const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::getBoundary");

		gridRect boundary = default_value::value<gridRect>();

		boundary = _views->getValues().aggregate<gridRect>(boundary, [] (const gridRect& bounds, brogueViewBase* view)
		{
			gridRect nextBounds = bounds;
			gridRect viewBounds = view->getBoundary();
			if (nextBounds == default_value::value<gridRect>())
				nextBounds = viewBounds;
			else
				nextBounds.expand(viewBounds);
			return nextBounds;
		});

		// Apply scroll offset
		boundary.translate(_scrollOffset->column, _scrollOffset->row);

		return boundary;
	}
	bool brogueViewContainer::getClipping() const
	{
		return _applyClipping;
	}
	gridRect brogueViewContainer::getContainerBoundary() const
	{
		return *_containerBoundary;
	}
	gridLocator brogueViewContainer::getScrollOffset() const
	{
		return *_scrollOffset;
	}
	vec2 brogueViewContainer::getRenderOffsetUI() const
	{
		return *_renderOffset;
	}
	void brogueViewContainer::setScrollOffset(short column, short row)
	{
		_scrollOffset->column = column;
		_scrollOffset->row = row;
	}
	void brogueViewContainer::setRenderOffsetUI(int pixelX, float pixelY)
	{
		_renderOffset->x = pixelX;
		_renderOffset->y = pixelY;
	}
	brogueMouseState brogueViewContainer::getAdjustedMouse(const brogueMouseState& mouseState) const
	{
		// Apply mouse transform to the mouse state for the child views (utilizes scrolling).
		//
		brogueMouseState adjustedMouse(mouseState.getLocation()
												 .subtract(*_scrollOffset)
												 .subtract(_renderOffset->x / brogueCellDisplay::CellWidth(_zoomLevel),
									   _renderOffset->y / brogueCellDisplay::CellHeight(_zoomLevel)),
									   mouseState.getScrollPendingX(),
									   mouseState.getScrollPendingY(),
									   mouseState.getScrollPendingX(),
									   mouseState.getScrollNegativeY(),
									   mouseState.getMouseLeft());

		return adjustedMouse;
	}
	gridRect brogueViewContainer::calculateSceneBoundaryUI() const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::calculateSceneBoundaryUI");

		return _views->getAt(0)->value->calculateSceneBoundaryUI();
	}
	gridRect brogueViewContainer::calculateViewBoundaryUI() const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::calculateViewBoundaryUI");

		return _views->getAt(0)->value->calculateViewBoundaryUI();
	}
	int brogueViewContainer::getZoomLevel() const
	{
		return _zoomLevel;
	}
	void brogueViewContainer::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		_views->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewBase* view)
		{
			view->initiateStateChange(fromState, toState);

			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::clearStateChange()
	{
		_views->forEach([] (const brogueUIProgramPartId& partId, brogueViewBase* view)
		{
			view->clearStateChange();

			return iterationCallback::iterate;
		});
	}
	bool brogueViewContainer::checkStateChange()
	{
		return _views->any([] (const brogueUIProgramPartId& partId, brogueViewBase* view) 
		{ 
			return view->checkStateChange(); 
		});
	}
	void brogueViewContainer::updateScroll(const brogueKeyboardState& keyboardState,
										   const brogueMouseState& mouseState,
										   int millisecondsLapsed)
	{
		if (!_hasScrollInteraction)
			return;

		/*
			UI Behavior:    This will add the scroll behavior. (or x-y scrolling behavior). The
						    getBoundary() method will allow for all child controls. The other
						    getContainerBoundary() method will check the clipping boundary for
						    this view container. 

			Render Offset:  Must add the UI offset to the container boundary to get the proper
							mouse position.
		*/

		// Aggregate child boundary
		gridRect childBoundary = this->getChildOffsetBoundary();
		gridRect boundary = this->getContainerBoundary();
		
		// TODO: CLEAN THIS UP

		// UI Translation:  This is the ultimate position of the mouse pointer
		boundary.translate(_renderOffset->x / brogueCellDisplay::CellWidth(_zoomLevel), 
						   _renderOffset->y / brogueCellDisplay::CellHeight(_zoomLevel));

		bool mouseOver = this->getContainerBoundary().contains(mouseState.getLocation());
		bool mousePressed = mouseState.getMouseLeft();
		bool scrollEvent = mouseState.getScrollPendingX() || mouseState.getScrollPendingY();

		if (!mouseOver || !scrollEvent)
			return;

		// Check scroll bounds
		int scrollX = 0;
		int scrollY = 0;

		// Parent View -> Mouse Over -> Scroll
		if (_hasScrollInteraction && mouseOver && scrollEvent)
		{
			if (mouseState.getScrollPendingX())
			{
				if (!mouseState.getScrollNegativeX() && childBoundary.left() < _containerBoundary->left())
					scrollX = 1;

				else if (mouseState.getScrollNegativeX() && childBoundary.right() > _containerBoundary->right())
					scrollX = -1;
			}
			if (mouseState.getScrollPendingY())
			{
				// Up
				if (mouseState.getScrollNegativeY() && childBoundary.bottom() > _containerBoundary->bottom())
					scrollY = -1;

				// Down
				else if (!mouseState.getScrollNegativeY() && childBoundary.top() < _containerBoundary->top())
					scrollY = 1;
			}
		}

		_scrollOffset->row += scrollY;
		_scrollOffset->column += scrollX;
	}
	void brogueViewContainer::checkUpdate(const brogueKeyboardState& keyboardState,
										  const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::checkUpdate");

		// The view container must track mouse events for specific UI invalidation (and perhaps later,
		// click and drag events)
		//
		bool containerMouseOver = this->getContainerBoundary().contains(mouseState.getLocation());

		// Update View Container:  Keeps track of its own mouse events
		_mouseData->setUpdate(mouseState.getMouseLeft(), containerMouseOver);

		// The view container must apply clipping container to the part to avoid unwanted behavior.
		//
		if (!containerMouseOver && !_mouseData->getMouseEnter() && !_mouseData->getMouseLeave())
			return;

		// (TODO: SOME REDESIGN)
		if (!containerMouseOver)
		{
			// Pre-maturely clearing the view container's mouse data because it's not supposed to be
			// handled by the render loop. The view container is acting like a view.
			_mouseData->clear();

			// The child views will now process their mouse-leave event
		}

		// Scroll Behavior:  The updating of the scroll is done at the container level. So,
		//					 this function must be run once during the normal checkUpdate path
		//					 for this UI program.
		//
		updateScroll(keyboardState, mouseState, millisecondsLapsed);

		// Apply mouse transform to the mouse state for the child views (utilizes scrolling and render offsets)
		//
		brogueMouseState adjustedMouse = getAdjustedMouse(mouseState);

		_views->forEach([&keyboardState, &adjustedMouse, &millisecondsLapsed] (const brogueUIProgramPartId& partId, brogueViewBase* view)
		{
			view->checkUpdate(keyboardState, adjustedMouse, millisecondsLapsed);

			return iterationCallback::iterate;
		});		
	}
	void brogueViewContainer::invalidate(const brogueKeyboardState& keyboardState,
										 const brogueMouseState& mouseState)
	{
		// Apply mouse transform to the mouse state for the child views (utilizes scrolling and render offsets)
		//
		brogueMouseState adjustedMouse = getAdjustedMouse(mouseState);

		_views->forEach([&keyboardState, &adjustedMouse] (const brogueUIProgramPartId& partId, brogueViewBase* view)
		{
			view->invalidate(keyboardState, adjustedMouse);

			return iterationCallback::iterate;
		});
	}
	bool brogueViewContainer::needsUpdate(const brogueUIProgramPartId& partId) const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::needsUpdate");

		return _views->get(partId)->needsUpdate();
	}
	void brogueViewContainer::clearUpdate(const brogueUIProgramPartId& partId)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::clearUpdate");

		_views->get(partId)->clearUpdate();
	}
	void brogueViewContainer::clearEvents(const brogueUIProgramPartId& partId)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::clearEvents");

		// Container mouse data
		_mouseData->clear();

		// Child View mouse data
		_views->get(partId)->clearEvents();
	}
}
#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueImageQuad.h"
#include "brogueKeyboardState.h"
#include "brogueLine.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIMouseData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewCore.h"
#include "brogueViewGridCore.h"
#include "brogueViewPolygonCore.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleHash.h"
#include "simpleList.h"
#include "simpleOpenGL.h"
#include <concepts>
#include <functional>

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	/// <summary>
	/// This container class provide a mechanism to manage views or composed views. 
	/// </summary>
	class brogueViewContainer
	{
	public:

		brogueViewContainer(brogueUIProgram programName,
							int zoomLevel,
							bool hasScrollInteraction,
							bool applyClipping,
							const gridRect& containerBoundary,
							const gridRect& sceneBoundary);
		~brogueViewContainer();

		template<isGLStream TStream>
		void addView(brogueViewCore<TStream>* view);

	private:	// Needed way to get around template specification

		void addViewImpl(brogueViewCore<brogueImageQuad>* view);
		void addViewImpl(brogueViewCore<brogueColorQuad>* view);
		void addViewImpl(brogueViewCore<brogueCellQuad>* view);
		void addViewImpl(brogueViewCore<brogueLine>* view);

		int getViewCount() const;

		template<isGLStream TStream>
		brogueViewCore<TStream> getView(int index);

		template<isGLStream TStream>
		int getViewCount();

		gridRect aggregateChildBoundary() const;

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
		int getZoomLevel() const;
		bool getClipping() const;
		brogueUIProgram getProgramName() const;

	protected:

		void setScrollOffset(int column, int row);
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
		gridRect* _sceneBoundary;
		gridLocator* _scrollOffset;
		vec2* _renderOffset;											// Hard render offset for shader animation

		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueCellQuad>*>* _cellViews;
		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueColorQuad>*>* _colorViews;
		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueImageQuad>*>* _imageViews;
		simpleHash<brogueUIProgramPartId, brogueViewPolygonCore*>* _lineViews;
	};

	brogueViewContainer::brogueViewContainer(brogueUIProgram programName,
											 int zoomLevel,
											 bool hasScrollInteraction,
											 bool applyClipping,
											 const gridRect& containerBoundary,
											 const gridRect& sceneBoundary)
	{
		_programName = programName;
		_zoomLevel = zoomLevel;
		_mouseData = new brogueUIMouseData();
		_containerBoundary = new gridRect(containerBoundary);
		_sceneBoundary = new gridRect(sceneBoundary);
		_scrollOffset = new gridLocator(0, 0);
		_renderOffset = new vec2(0, 0);
		_hasScrollInteraction = hasScrollInteraction;
		_applyClipping = applyClipping;

		_cellViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueCellQuad>*>();
		_colorViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueColorQuad>*>();
		_imageViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueImageQuad>*>();
		_lineViews = new simpleHash<brogueUIProgramPartId, brogueViewPolygonCore*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		_cellViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueCellQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_colorViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueColorQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_imageViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueImageQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_lineViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueLine>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});

		delete _cellViews;
		delete _colorViews;
		delete _imageViews;
		delete _lineViews;
		delete _mouseData;
		delete _sceneBoundary;
		delete _containerBoundary;
		delete _renderOffset;
		delete _scrollOffset;
	}

	template<isGLStream TStream>
	void brogueViewContainer::addView(brogueViewCore<TStream>* view)
	{
		addViewImpl(view);
	}

	brogueUIProgram brogueViewContainer::getProgramName() const
	{
		return _programName;
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueImageQuad>* view)
	{
		_imageViews->add(view->getId(), (brogueViewGridCore<brogueImageQuad>*)view);
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueColorQuad>* view)
	{
		_colorViews->add(view->getId(), (brogueViewGridCore<brogueColorQuad>*)view);
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueCellQuad>* view)
	{
		_cellViews->add(view->getId(), (brogueViewGridCore<brogueCellQuad>*)view);
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueLine>* view)
	{
		_lineViews->add(view->getId(), (brogueViewPolygonCore*)view);
	}
	int brogueViewContainer::getViewCount() const
	{
		return _imageViews->count() + _colorViews->count() + _cellViews->count() + _lineViews->count();
	}

	template<isGLStream TStream>
	int brogueViewContainer::getViewCount<TStream>()
	{
		if (std::same_as<TStream, brogueCellQuad>())
			return _cellViews->count();

		else if (std::same_as<TStream, brogueImageQuad>())
			return _imageViews->count();

		else if (std::same_as<TStream, brogueColorQuad>())
			return _colorViews->count();

		else if (std::same_as<TStream, brogueLine>())
			return _lineViews->count();
		else
			throw simpleException("Unhandled stream type:  brogueViewContainer::getViewCount");
	}
	
	gridRect brogueViewContainer::aggregateChildBoundary() const
	{
		gridRect boundary;

		simpleListAggregator<gridRect, gridRect> callback([] (const gridRect& seed, const gridRect& viewBounds)
		{
			gridRect nextBounds = seed;
			if (nextBounds == default_value::value<gridRect>())
				nextBounds = viewBounds;
			else
				nextBounds.expand(viewBounds);
			return nextBounds;
		});

		// Iterator...
		boundary = _cellViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueCellQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _imageViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueImageQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _colorViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueColorQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _lineViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueLine>* view)
		{
			return callback(seed, view->getBoundary());
		});

		return boundary;
	}
	gridRect brogueViewContainer::getSceneBoundary() const
	{
		return *_sceneBoundary;
	}
	gridRect brogueViewContainer::getChildOffsetBoundary() const
	{
		if (this->getViewCount() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::getBoundary");

		gridRect boundary = aggregateChildBoundary();

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
	void brogueViewContainer::setScrollOffset(int column, int row)
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
	int brogueViewContainer::getZoomLevel() const
	{
		return _zoomLevel;
	}
	void brogueViewContainer::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		_cellViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});

		_imageViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});

		_colorViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});

		_lineViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			//view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::clearStateChange()
	{
		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->clearStateChange();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->clearStateChange();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->clearStateChange();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			//view->clearStateChange();
			return iterationCallback::iterate;
		});
	}
	bool brogueViewContainer::checkStateChange()
	{
		bool result = false;

		result |= _cellViews->any([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			return view->checkStateChange();
		});

		result |= _imageViews->any([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			return view->checkStateChange();
		});

		result |= _colorViews->any([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			return view->checkStateChange();
		});

		result |= _lineViews->any([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			return false; // view->checkStateChange();
		});

		return result;
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
		if (this->getViewCount() == 0)
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
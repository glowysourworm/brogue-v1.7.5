#pragma once

#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
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

		brogueViewContainer(brogueUIProgram programName, bool hasScrollInteraction, bool applyClipping, const gridRect& containerBoundary);
		~brogueViewContainer();

		void addView(brogueViewBase* view);
		brogueViewBase* getView(const brogueUIProgramPartId& programPart) const;
		brogueViewBase* getViewAt(int index) const;
		int getViewCount() const;

	public:

		void updateScroll(const brogueKeyboardState& keyboardState,
						  const brogueMouseState& mouseState,
						  int millisecondsLapsed);

		void checkUpdate(const brogueUIProgramPartId& partId,
						 const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed);

		bool needsUpdate(const brogueUIProgramPartId& partId) const;
		void clearUpdate(const brogueUIProgramPartId& partId);
		void clearEvents(const brogueUIProgramPartId& partId);

	public:		// brogueViewBase facade

		gridRect getSceneBoundary() const;
		gridRect getContainerBoundary() const;
		gridLocator getRenderOffset() const;
		gridRect calculateSceneBoundaryUI() const;
		gridRect calculateViewBoundaryUI() const;
		int getZoomLevel() const;
		bool getClipping() const;
		brogueUIProgram getProgramName() const;

	private:

		/// <summary>
		/// Returns aggregate child boundary with scroll offset
		/// </summary>
		gridRect getChildOffsetBoundary() const;

	private:

		brogueUIProgram _programName;
		bool _hasScrollInteraction;
		bool _applyClipping;
		gridRect* _containerBoundary;
		gridLocator* _renderOffset;
		simpleHash<brogueUIProgramPartId, brogueViewBase*>* _views;
	};

	brogueViewContainer::brogueViewContainer(brogueUIProgram programName, bool hasScrollInteraction, bool applyClipping, const gridRect& containerBoundary)
	{
		_programName = programName;
		_containerBoundary = new gridRect(containerBoundary);
		_renderOffset = new gridLocator(0,0);
		_hasScrollInteraction = hasScrollInteraction;
		_applyClipping = applyClipping;
		_views = new simpleHash<brogueUIProgramPartId, brogueViewBase*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _views;
		delete _containerBoundary;
		delete _renderOffset;
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
		boundary.translate(_renderOffset->column, _renderOffset->row);

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
	gridLocator brogueViewContainer::getRenderOffset() const
	{
		return *_renderOffset;
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
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::getZoomLevel");

		return _views->getAt(0)->value->getZoomLevel();
	}
	void brogueViewContainer::updateScroll(const brogueKeyboardState& keyboardState,
											  const brogueMouseState& mouseState,
											  int millisecondsLapsed)
	{
		if (!_hasScrollInteraction)
			return;

		/*
			UI Behavior:  This will add the scroll behavior. (or x-y scrolling behavior). The
						  getBoundary() method will allow for all child controls. The other
						  getContainerBoundary() method will check the clipping boundary for
						  this view container. 
		*/

		bool mouseOver = this->getContainerBoundary().contains(mouseState.getLocation());
		bool mousePressed = mouseState.getMouseLeft();
		bool scrollEvent = mouseState.getScrollPendingX() || mouseState.getScrollPendingY();

		if (!mouseOver || !scrollEvent)
			return;

		// Aggregate child boundary
		gridRect childBoundary = this->getChildOffsetBoundary();

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

		_renderOffset->row += scrollY;
		_renderOffset->column += scrollX;
	}
	void brogueViewContainer::checkUpdate(const brogueUIProgramPartId& partId,
										  const brogueKeyboardState& keyboardState,
										  const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::checkUpdate");

		// The view container must apply clipping container to the part. (Unless the
		// child view requires an update anyway. Currently, this is not the behavior
		// of child views for the view container; but would be easy to add)
		//
		if (!this->getContainerBoundary().contains(mouseState.getLocation()))
			return;

		// Apply mouse transform to the mouse state for the child views (apply scrolling
		// and clipping.
		//
		brogueMouseState adjustedMouse(mouseState.getLocation().subtract(*_renderOffset), 
									   mouseState.getScrollPendingX(), 
									   mouseState.getScrollPendingY(),
									   mouseState.getScrollPendingX(), 
									   mouseState.getScrollNegativeY(), 
									   mouseState.getMouseLeft());

		_views->get(partId)->checkUpdate(keyboardState, adjustedMouse, millisecondsLapsed);
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

		_views->get(partId)->clearEvents();
	}
}
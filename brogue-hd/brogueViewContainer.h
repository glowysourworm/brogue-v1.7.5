#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
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

		brogueViewContainer(brogueUIProgram programName, bool hasScrollInteraction, const gridRect& containerBoundary);
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

		brogueCellDisplay* get(short column, short row) const;

	public:		// brogueViewBase facade

		gridRect getSceneBoundary() const;
		gridRect getBoundary() const;
		gridRect calculateSceneBoundaryUI() const;
		gridRect calculateViewBoundaryUI() const;
		int getZoomLevel() const;
		brogueUIProgram getProgramName() const;

	private:

		brogueUIProgram _programName;
		bool _hasScrollInteraction;
		gridRect* _containerBoundary;
		simpleHash<brogueUIProgramPartId, brogueViewBase*>* _views;
	};

	brogueViewContainer::brogueViewContainer(brogueUIProgram programName, bool hasScrollInteraction, const gridRect& containerBoundary)
	{
		_programName = programName;
		_containerBoundary = new gridRect(containerBoundary);
		_hasScrollInteraction = hasScrollInteraction;
		_views = new simpleHash<brogueUIProgramPartId, brogueViewBase*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _views;
		delete _containerBoundary;
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
	gridRect brogueViewContainer::getBoundary() const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::getBoundary");

		gridRect boundary = default_value::value<gridRect>();

		return _views->getValues().aggregate<gridRect>(boundary, [] (const gridRect& bounds, brogueViewBase* view)
		{
			gridRect nextBounds = bounds;
			gridRect viewBounds = view->getBoundary();
			nextBounds.expand(viewBounds);
			return nextBounds;
		});
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
	brogueCellDisplay* brogueViewContainer::get(short column, short row) const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::get");

		for (int index = 0; index < _views->count(); index++)
		{
			if (_views->getAt(index)->value->get(column, row) != nullptr)
				return _views->getAt(index)->value->get(column, row);
		}

		return nullptr;
	}
	void brogueViewContainer::updateScroll(const brogueKeyboardState& keyboardState,
											  const brogueMouseState& mouseState,
											  int millisecondsLapsed)
	{
		if (!_hasScrollInteraction)
			return;

		/*
			UI Behavior:  This will add the scroll behavior. (or x-y scrolling behavior)
		*/

		bool mouseOver = this->getBoundary().contains(mouseState.getLocation());
		bool mousePressed = mouseState.getMouseLeft();
		bool scrollEvent = mouseState.getScrollPendingX() || mouseState.getScrollPendingY();

		if (!mouseOver || !scrollEvent)
			return;

		// Aggregate child boundary
		gridRect childBoundary = this->getBoundary();

		// Check scroll bounds
		int scrollX = 0;
		int scrollY = 0;

		// Parent View -> Mouse Over -> Scroll
		if (_hasScrollInteraction && mouseOver && scrollEvent)
		{
			if (mouseState.getScrollPendingX())
			{
				if (mouseState.getScrollNegativeX() && childBoundary.left() > _containerBoundary->left())
					scrollX = -1;

				else if (!mouseState.getScrollNegativeX() && childBoundary.right() < _containerBoundary->right())
					scrollX = 1;
			}
			if (mouseState.getScrollPendingY())
			{
				if (mouseState.getScrollNegativeY() && childBoundary.top() > _containerBoundary->top())
					scrollY = -1;

				else if (!mouseState.getScrollNegativeY() && childBoundary.bottom() < _containerBoundary->bottom())
					scrollY = 1;
			}
		}

		if (scrollX == 0 && scrollY == 0)
			return;

		// Child Views
		_views->forEach([&scrollX, &scrollY] (const brogueUIProgramPartId& partId, brogueViewBase* view)
		{
			// Sets UI Data -> (needsUpdate = true)
			view->incrementRenderOffset(scrollX, scrollY);

			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::checkUpdate(const brogueUIProgramPartId& partId,
										  const brogueKeyboardState& keyboardState,
										  const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::checkUpdate");

		/*
			CAREFUL!  Milliseconds lapsed is for the PART - not the entire view (for this call)
		*/

		_views->get(partId)->checkUpdate(keyboardState, mouseState, millisecondsLapsed);
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
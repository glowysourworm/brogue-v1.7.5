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

		brogueViewContainer(brogueUIProgram programName);
		~brogueViewContainer();

		void addView(brogueViewBase* view);
		brogueViewBase* getView(const brogueUIProgramPartId& programPart) const;
		brogueViewBase* getViewAt(int index) const;
		int getViewCount() const;

	public:

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
		simpleHash<brogueUIProgramPartId, brogueViewBase*>* _views;
	};

	brogueViewContainer::brogueViewContainer(brogueUIProgram programName)
	{
		_programName = programName;
		_views = new simpleHash<brogueUIProgramPartId, brogueViewBase*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _views;
	}
	void brogueViewContainer::addView(brogueViewBase* view)
	{
		// Composed Views:  These will potentially have N number of program parts (buttons, text, background, etc...)
		//					which will be managed by brogueComposedView. Each will have a shader program and data stream
		//					to handle here; but the mouse interaction is the composed view. (brogueViewBase has the 
		//					overload for this function to give the base view.
		//
		for (int index = 0; index < view->getChildViewCount(); index++)
		{
			_views->add(view->getChildView(index)->getPartId(), view->getChildView(index));
		}
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
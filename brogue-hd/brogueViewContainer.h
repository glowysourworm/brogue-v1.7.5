#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueViewBase.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleHash.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	/// <summary>
	/// This container class provide a mechanism to manage views or composed views. 
	/// </summary>
	class brogueViewContainer
	{
	public:

		brogueViewContainer(brogueUIProgram programName);
		~brogueViewContainer();

		void addView(brogueUIProgramPart programPart, brogueViewBase* view);
		brogueViewBase* getView(brogueUIProgramPart programPart) const;
		brogueViewBase* getViewAt(int index) const;
		int getViewCount() const;

	public:

		void checkUpdate(brogueUIProgramPart programPart,
						 const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed);
		bool needsUpdate(brogueUIProgramPart programPart) const;
		void clearUpdate(brogueUIProgramPart programPart);
		void clearEvents(brogueUIProgramPart programPart);

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
		simpleHash<brogueUIProgramPart, brogueViewBase*>* _views;
	};

	brogueViewContainer::brogueViewContainer(brogueUIProgram programName)
	{
		_programName = programName;
		_views = new simpleHash<brogueUIProgramPart, brogueViewBase*>();
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _views;
	}
	void brogueViewContainer::addView(brogueUIProgramPart programPart, brogueViewBase* view)
	{
		_views->add(programPart, view);
	}
	brogueViewBase* brogueViewContainer::getView(brogueUIProgramPart programPart) const
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
	void brogueViewContainer::checkUpdate(brogueUIProgramPart programPart,
										  const brogueKeyboardState& keyboardState,
										  const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::checkUpdate");

		for (int index = 0; index < _views->count(); index++)
		{
			_views->getAt(index)->value->checkUpdate(keyboardState, mouseState, millisecondsLapsed);
		}
	}
	bool brogueViewContainer::needsUpdate(brogueUIProgramPart programPart) const
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::needsUpdate");

		return _views->get(programPart)->needsUpdate();
	}
	void brogueViewContainer::clearUpdate(brogueUIProgramPart programPart)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::clearUpdate");

		_views->get(programPart)->clearUpdate();
	}
	void brogueViewContainer::clearEvents(brogueUIProgramPart programPart)
	{
		if (_views->count() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::clearEvents");

		_views->get(programPart)->clearEvents();
	}
}
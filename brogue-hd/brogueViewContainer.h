#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueView.h"
#include "brogueViewBase.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::ui
{
	/// <summary>
	/// This container class will provide overrides of the base class and return / utilize
	/// child views where there are child views defined. Otherwise, it will utilize its own
	/// grid of cells.
	/// </summary>
	class brogueViewContainer : public brogueViewBase
	{
	public:

		/// <summary>
		/// BrogueViewContainer is the primary brogueUIView for the render path.
		/// </summary>
		brogueViewContainer(brogueView* parentView, brogueUIView parentViewName, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueViewContainer();

		void addView(brogueView* view);

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

		virtual bool needsUpdate() const override;
		virtual void clearUpdate() override;
		virtual void clearEvents() override;

		virtual brogueCellDisplay* get(short column, short row) const override;

		/// <summary>
		/// Iterates child views of the container
		/// </summary>
		void iterateChildViews(simpleListCallback<brogueView*> callback) const;

	public:		// UI Overrides

		virtual gridRect getSceneBoundary() const override
		{
			return _parentView->getSceneBoundary();
		}
		virtual gridRect getBoundary() const override
		{
			return _parentView->getBoundary();
		}
		virtual gridRect getPaddedBoundary() const override
		{
			return _parentView->getPaddedBoundary();
		}
		virtual gridRect getRenderBoundary() const override
		{
			return _parentView->getRenderBoundary();
		}

		virtual gridRect calculateSceneBoundaryUI() const override
		{
			return _parentView->calculateSceneBoundaryUI();
		}
		virtual gridRect calculateViewBoundaryUI() const override
		{
			return _parentView->calculateViewBoundaryUI();
		}

		virtual void incrementRenderOffset(short columnOffset, short rowOffset)
		{
			// Increment Child Views
			for (int index = 0; index < _childViews->count(); index++)
			{
				_childViews->get(index)->incrementRenderOffset(columnOffset, rowOffset);
			}

			// Increment This View
			_parentView->incrementRenderOffset(columnOffset, rowOffset);
		}

		virtual gridLocator getRenderOffset() const
		{
			return _parentView->getRenderOffset();
		}

		virtual bool isMouseOver(const brogueMouseState& mouseState)
		{
			return _parentView->isMouseOver(mouseState);
		}

		virtual int getZoomLevel() const
		{
			return _parentView->getZoomLevel();
		}

		virtual brogueUIData* getUIData() const
		{
			return _parentView->getUIData();
		}

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const
		{
			const brogueViewContainer* that = this;

			_parentView->getBoundary().iterate([&that, &callback] (short column, short row)
			{
				return callback(column, row, that->get(column, row));
			});
		}

	protected:

		int getViewCount() const
		{
			return _childViews->count();
		}

		brogueUIView getViewName() const
		{
			return _parentViewName;
		}

		brogueView* getParentView() const
		{
			return _parentView;
		}

	private:

		brogueView* _parentView;
		brogueUIView _parentViewName;
		simpleList<brogueView*>* _childViews;
	};

	brogueViewContainer::brogueViewContainer(brogueView* parentView,
											 brogueUIView parentViewName,
											 const gridRect& sceneBoundary,
											 const gridRect& viewBoundary)
	{
		_childViews = new simpleList<brogueView*>();
		_parentViewName = parentViewName;
		_parentView = parentView;
	}
	brogueViewContainer::~brogueViewContainer()
	{
		delete _childViews;
	}
	void brogueViewContainer::addView(brogueView* view)
	{
		_childViews->add(view);
	}
	void brogueViewContainer::iterateChildViews(simpleListCallback<brogueView*> callback) const
	{
		_childViews->forEach(callback);
	}
	brogueCellDisplay* brogueViewContainer::get(short column, short row) const
	{
		// Check padding first
		if (!_parentView->getPaddedBoundary().contains(column, row))
			return _parentView->get(column, row);

		for (int index = 0; index < _childViews->count(); index++)
		{
			// This offset should be shared with the container (but it's better to check and be sure)
			gridLocator offset = _childViews->get(index)->getRenderOffset();

			// Check child view boundaries (this will be an improper child view)
			if (_childViews->get(index)->getBoundary().contains(offset.column + column, offset.row + row))
			{
				// Retrieve as if from normal boundaries (go ahead an show the improper child)
				return _childViews->get(index)->get(offset.column + column, offset.row + row);
			}
		}

		return _parentView->get(column, row);
	}
	void brogueViewContainer::checkUpdate(const brogueKeyboardState& keyboardState,
										  const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		// Check child views first (for performance) (NOTE:  This function updates mouse related data; and must be run on child views)
		_childViews->forEach([&mouseState, &keyboardState, &millisecondsLapsed] (brogueView* view)
		{
			view->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});

		// Parent View
		_parentView->checkUpdate(keyboardState, mouseState, millisecondsLapsed);
	}
	bool brogueViewContainer::needsUpdate() const
	{
		bool result = false;

		_childViews->forEach([&result] (brogueView* view)
		{
			result |= view->needsUpdate();

			if (result)
				return iterationCallback::breakAndReturn;
			else
				return iterationCallback::iterate;
		});

		// Parent View
		return result || _parentView->needsUpdate();
	}
	void brogueViewContainer::clearUpdate()
	{
		_childViews->forEach([] (brogueView* view)
		{
			view->clearUpdate();

			return iterationCallback::iterate;
		});

		_parentView->clearUpdate();
	}
	void brogueViewContainer::clearEvents()
	{
		_childViews->forEach([] (brogueView* view)
		{
			view->clearEvents();

			return iterationCallback::iterate;
		});

		_parentView->clearEvents();
	}
}
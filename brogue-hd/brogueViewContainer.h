#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIChildResponse.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIResponseData.h"
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

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed);

		virtual void checkUpdate(brogueUIResponseData& response,
								 const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

		virtual brogueUIChildResponse checkUpdate(const brogueKeyboardState& keyboardState,
												  const brogueMouseState& mouseState,
												  int millisecondsLapsed) override;

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
	brogueUIChildResponse brogueViewContainer::checkUpdate(const brogueKeyboardState& keyboardState,
														   const brogueMouseState& mouseState,
														   int millisecondsLapsed)
	{
		// Treats this view as the child (parent caller)
		brogueUIResponseData thisResponse;

		checkUpdate(thisResponse, keyboardState, mouseState, millisecondsLapsed);

		return thisResponse.response;
	}
	void brogueViewContainer::checkUpdate(brogueUIResponseData& response,
										  const brogueKeyboardState& keyboardState,
										  const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		// Mark the response
		response.signature.view = _parentViewName;

		// Check child views first (for performance) (NOTE:  This function updates mouse related data; and must be run on child views)
		_childViews->forEach([&mouseState, &keyboardState, &millisecondsLapsed, &response] (brogueView* view)
		{
			brogueUIChildResponse childResponse = view->checkUpdate(keyboardState, mouseState, millisecondsLapsed);

			if (childResponse.shouldUpdate)
				response.response = childResponse;

			return iterationCallback::iterate;
		});

		// Nothing set in the child response -> Use this container response
		if (!response.response.shouldUpdate)
		{
			// Parent View
			if (_parentView->isMouseOver(mouseState) && _parentView->getUIData()->getHasMouseInteraction())
			{
				response.response = _parentView->checkUpdate(keyboardState, mouseState, millisecondsLapsed);
			}
		}
	}
	void brogueViewContainer::update(const brogueKeyboardState& keyboardState,
									 const brogueMouseState& mouseState,
									 int millisecondsLapsed)
	{
		// May be overridden.
		//
		_parentView->update(keyboardState, mouseState, millisecondsLapsed);

		// Call update afterwards (logically, we want to overlay the child views; but it may not matter here)
		//
		_childViews->forEach([&keyboardState, &mouseState, &millisecondsLapsed] (brogueView* view)
		{
			view->update(keyboardState, mouseState, millisecondsLapsed);

			return iterationCallback::iterate;
		});
	}
}
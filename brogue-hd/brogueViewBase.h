#pragma once
#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUITagAction.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "simpleException.h"

using namespace brogueHd::backend::model::layout;
using namespace brogueHd::frontend::opengl;


namespace brogueHd::frontend::ui
{
	/// <summary>
	/// Function-oriented (abstract) base class for common function signatures.
	/// </summary>
	class brogueViewBase
	{

		/*
			NOTE:  This class is being shared by both a brogueView and brogueViewContainer. So, there are
				   both overloads of checkUpdate(...).
		*/

	public:

		brogueViewBase()
		{

		}
		~brogueViewBase()
		{

		}

		virtual void raiseClickEvent(const brogueUITagAction& response)
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::raiseClickEvent");
		}

		virtual brogueCellDisplay* get(short column, short row) const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::get");
		}

		virtual gridRect getSceneBoundary() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::getSceneBoundary");
		}
		virtual gridRect getBoundary() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::getBoundary");
		}
		virtual gridRect getPaddedBoundary() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::getPaddedBoundary");
		}
		virtual gridRect getRenderBoundary() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::getRenderBoundary");
		}

		virtual gridRect calculateSceneBoundaryUI() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::calculateSceneBoundaryUI");
		}
		virtual gridRect calculateViewBoundaryUI() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::calculateViewBoundaryUI");
		}

		virtual void iterate(gridCallback<brogueCellDisplay*> callback) const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::iterate");
		}

		virtual void update(const brogueKeyboardState& keyboardState,
							const brogueMouseState& mouseState,
							int millisecondsLapsed)
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::update");
		}

		/// <summary>
		/// Overload of the checkUpdate function behaves as though the view is a child of a parent view
		/// </summary>
		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed)
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::checkUpdate");
		}

		/// <summary>
		/// Clears update flags from the UI tree
		/// </summary>
		virtual void clearUpdate()
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::clearUpdate");
		}

		/// <summary>
		/// Clears mouse interaction events from the UI tree
		/// </summary>
		virtual void clearEvents()
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::clearUpdate");
		}

		/// <summary>
		/// Checks the tree to see if there are views that need updating.
		/// </summary>
		virtual bool needsUpdate() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::needsUpdate");
		}
		virtual void incrementRenderOffset(short columnOffset, short rowOffset)
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::incrementRenderOffset");
		}

		virtual gridLocator getRenderOffset() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::getRenderOffset");
		}

		virtual bool isMouseOver(const brogueMouseState& mouseState)
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::isMouseOver");
		}

		virtual int getZoomLevel() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::getZoomLevel");
		}

		virtual brogueUIData* getUIData() const
		{
			throw simpleException("brogueViewBase function not overriden:  brogueViewBase::getUIData");
		}

	};
}
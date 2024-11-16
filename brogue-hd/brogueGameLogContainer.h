#pragma once
#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueViewContainer.h"
#include "gridRect.h"
#include "simpleException.h"
#include "simpleMath.h"
#include "simplePeriodCounter.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueGameLogContainer : public brogueViewContainer
	{
	public:

		brogueGameLogContainer(brogueUIProgram programName, int zoomLevel, bool hasScrollInteraction, bool applyClipping, const gridRect& containerBoundary);
		~brogueGameLogContainer();

		virtual void initiateStateChange(brogueUIState fromState, brogueUIState toState) override;
		virtual void clearStateChange() override;
		virtual bool checkStateChange() override;

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

	private:

		void setAnimation(float periodValue);

	private:

		simplePeriodCounter* _animationCounter;
		bool _animating;
		bool _closing;

	};

	brogueGameLogContainer::brogueGameLogContainer(brogueUIProgram programName, int zoomLevel, bool hasScrollInteraction, bool applyClipping, const gridRect& containerBoundary)
		: brogueViewContainer(programName, zoomLevel, hasScrollInteraction, applyClipping, containerBoundary)
	{
		_animationCounter = new simplePeriodCounter(300);
		_animating = false;
		_closing = true;

		setAnimation(1);
	}
	brogueGameLogContainer::~brogueGameLogContainer()
	{
		delete _animationCounter;
	}
	void brogueGameLogContainer::setAnimation(float periodValue)
	{
		if (!_closing)
			periodValue = 1 - periodValue;

		// Set a rough offset
		float offset = -1 * periodValue * (this->getContainerBoundary().height - 3);	// Offset from the top of the screen

		// Convert to UI coordinates
		int offsetUI = offset * brogueCellDisplay::CellHeight(this->getZoomLevel());

		this->setRenderOffsetUI(0, offsetUI);
	}
	void brogueGameLogContainer::checkUpdate(const brogueKeyboardState& keyboardState,
											 const brogueMouseState& mouseState,
											 int millisecondsLapsed)
	{
		// Check for state animation
		if (_animating && !_animationCounter->pending())
		{
			_animationCounter->update(millisecondsLapsed, false);

			// Perform the animation
			float periodRatio = _animationCounter->getCounter() / (float)_animationCounter->getPeriod();

			// De-noise
			periodRatio = simpleMath::clamp(periodRatio, 0.0f, 1.0f);

			// Ease-out: Slows the final portion of the rendering
			periodRatio = simpleMath::easeOutCubic(periodRatio);

			setAnimation(periodRatio);
		}

		// Pass through to base class
		brogueViewContainer::checkUpdate(keyboardState, mouseState, millisecondsLapsed);
	}
	void brogueGameLogContainer::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		if (_animating)
			throw simpleException("Trying to initiate a new state change during animation:  brogueGameLogContainer");

		// Open
		if (fromState == brogueUIState::GameNormal &&
			toState == brogueUIState::GameLogOpen)
		{
			_animating = true;
			_closing = false;
		}

		// Closing
		else if (fromState == brogueUIState::GameLogOpen &&
				 toState == brogueUIState::GameNormal)
		{
			_animating = true;
			_closing = true;
		}
	}
	void brogueGameLogContainer::clearStateChange()
	{
		_animationCounter->reset();
		_animating = false;
	}
	bool brogueGameLogContainer::checkStateChange()
	{
		// Pending => period has elapsed. Return true to signal we're still animating.
		return _animating && !_animationCounter->pending();
	}
}
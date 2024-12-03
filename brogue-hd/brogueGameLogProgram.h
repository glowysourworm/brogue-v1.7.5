#pragma once
#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueUIConstants.h"
#include "brogueViewProgram.h"
#include "gridRect.h"
#include "simpleException.h"
#include "simpleKeyboardState.h"
#include "simpleMath.h"
#include "simpleMouseState.h"
#include "simplePeriodCounter.h"

namespace brogueHd::frontend
{
	using namespace simple;

	class brogueGameLogProgram : public brogueViewProgram
	{
	public:
		brogueGameLogProgram(brogueCoordinateConverter* coordinateConverter,
		                     brogueUIProgram programName,
		                     const gridRect& containerBoundary,
		                     const gridRect& sceneBoundary);
		~brogueGameLogProgram();

		virtual void initiateStateChange(brogueUIState fromState, brogueUIState toState) override;
		virtual void clearStateChange() override;
		virtual bool checkStateChange() override;

		virtual void checkUpdate(const simpleKeyboardState& keyboardState,
		                         const simpleMouseState& mouseState,
		                         int millisecondsLapsed) override;

	private:
		void setAnimation(float periodValue);

	private:
		brogueCoordinateConverter* _coordinateConverter;
		simplePeriodCounter* _animationCounter;
		bool _animating;
		bool _closing;
	};

	brogueGameLogProgram::brogueGameLogProgram(brogueCoordinateConverter* coordinateConverter,
	                                           brogueUIProgram programName,
	                                           const gridRect& containerBoundary,
	                                           const gridRect& sceneBoundary)
		: brogueViewProgram(coordinateConverter, programName, containerBoundary, sceneBoundary)
	{
		_coordinateConverter = coordinateConverter;
		_animationCounter = new simplePeriodCounter(300);
		_animating = false;
		_closing = true;

		setAnimation(1);
	}

	brogueGameLogProgram::~brogueGameLogProgram()
	{
		delete _animationCounter;
	}

	void brogueGameLogProgram::setAnimation(float periodValue)
	{
		if (!_closing)
			periodValue = 1 - periodValue;

		// Set a rough offset
		float offset = -1 * periodValue * (this->getContainerBoundary().height - 3);
		// Offset from the top of the screen

		// Convert to UI coordinates
		int offsetUI = offset * brogueCellDisplay::CellHeight(_coordinateConverter->getZoomLevel());

		this->setRenderOffsetUI(0, offsetUI);
	}

	void brogueGameLogProgram::checkUpdate(const simpleKeyboardState& keyboardState,
	                                       const simpleMouseState& mouseState,
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
		brogueViewProgram::checkUpdate(keyboardState, mouseState, millisecondsLapsed);
	}

	void brogueGameLogProgram::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		if (_animating)
			throw simpleException("Trying to initiate a new state change during animation:  brogueGameLogProgram");

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

	void brogueGameLogProgram::clearStateChange()
	{
		_animationCounter->reset();
		_animating = false;
	}

	bool brogueGameLogProgram::checkStateChange()
	{
		// Pending => period has elapsed. Return true to signal we're still animating.
		return _animating && !_animationCounter->pending();
	}
}

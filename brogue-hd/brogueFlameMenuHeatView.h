#pragma once
#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueTitleGrid.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "color.h"
#include "eventController.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simplePeriodCounter.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	/// <summary>
	/// Component responsibility is to provide "heat" constants to the GPU. The data stream of brogueFlameQuad 
	/// elements will provide the constant colors below. This view will keep track of a random sequence to provide
	/// also as uniforms to the GPU shaders to add variability.
	/// </summary>
	class brogueFlameMenuHeatView : public brogueViewBase
	{
	public:
		brogueFlameMenuHeatView(eventController* eventController,
								randomGenerator* randomGenerator,
								int fadePeriodMilliseconds,
								const brogueUIData& data,
								const gridRect& sceneBoundary,
								const gridRect& viewBoundary);
		~brogueFlameMenuHeatView();

		void update(int millisecondsLapsed,
					bool forceUpdate) override;

		void checkUpdate(const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed) override;

		void clearUpdate() override;
		void clearEvents() override;

		bool needsUpdate() const override;

		/// <summary>
		/// Returns a normalized time value for the current fade period
		/// </summary>
		float currentFadePeriod() const
		{
			return _fadePeriodCounter->getCounter() / (float)_fadePeriodCounter->getPeriod();
		}

		int currentFadePeriodRandom1() const
		{
			return _fadePeriodRandom1;
		}
		int currentFadePeriodRandom2() const
		{
			return _fadePeriodRandom2;
		}
		/// <summary>
		/// Returns total number of fade periods so far
		/// </summary>
		int fadePeriodCount() const
		{
			return _fadePeriodCounter->getNumberOfPeriods();
		}

	public:

		bool isTheText(short column, short row) const
		{
			return _titleGrid->isTheText(column, row);
		}

		void iterate(gridCallback<brogueCellDisplay*> callback) const override
		{
			brogueTitleGrid* titleGrid = _titleGrid;
			gridRect sceneBounds = _titleGrid->sceneBounds();

			brogueViewBase::iterate([&callback, &titleGrid, &sceneBounds] (short column, short row, brogueCellDisplay* cell)
			{
				//if (titleGrid->isTheText(column, row) || row == sceneBounds.bottom())
				callback(column, row, cell);

				return iterationCallback::iterate;
			});
		}

	public:

		const color FlameBottomColor1 = color(1.0f, 0.0f, 0.0f, 1);
		const color FlameBottomColor2 = color(1.0f, 1.0f, 0.6f, 1);
		const color FlameBottomColor3 = color(1.0f, 1.0f, 0.8f, 1);

		const color FlameTitleColor1 = color(0.0f, 0.0f, 1.0f, 1);
		const color FlameTitleColor2 = color(0.0f, 0.1f, 0.9f, 1);
		const color FlameTitleColor3 = color(0.0f, 0.0f, 1.0f, 1);

	private:

		randomGenerator* _randomGenerator;
		brogueTitleGrid* _titleGrid;

		simplePeriodCounter* _periodCounter;
		simplePeriodCounter* _fadePeriodCounter;

		int _fadePeriodRandom1;
		int _fadePeriodRandom2;
	};

	brogueFlameMenuHeatView::brogueFlameMenuHeatView(eventController* eventController,
													 randomGenerator* randomGenerator,
													 int fadePeriodMilliseconds,
													 const brogueUIData& data,
													 const gridRect& sceneBoundary,
													 const gridRect& viewBoundary)
		: brogueViewBase(eventController, data, sceneBoundary, viewBoundary)
	{
		_randomGenerator = randomGenerator;
		_fadePeriodRandom1 = randomGenerator->next() * 10000;
		_fadePeriodRandom2 = randomGenerator->next() * 10000;
		_periodCounter = new simplePeriodCounter(5);
		_fadePeriodCounter = new simplePeriodCounter(fadePeriodMilliseconds);
		_titleGrid = new brogueTitleGrid();
	}
	brogueFlameMenuHeatView::~brogueFlameMenuHeatView()
	{
		delete _periodCounter;
		delete _fadePeriodCounter;
	}
	void brogueFlameMenuHeatView::checkUpdate(const brogueKeyboardState& keyboardState,
											  const brogueMouseState& mouseState,
											  int millisecondsLapsed)
	{
		// Heat source fade over the fade period; and then they're cycled to the next
		// color.
		//
		_periodCounter->update(millisecondsLapsed, false);
		_fadePeriodCounter->update(millisecondsLapsed, false);

		// REMOVE THIS
		if (_fadePeriodCounter->pending())
		{
			_fadePeriodRandom2 = _fadePeriodRandom1;
			_fadePeriodRandom1 = _randomGenerator->next() * 10000;
		}

		// Mouse Interaction
		brogueViewBase::checkUpdate(keyboardState, mouseState, millisecondsLapsed);
	}
	bool brogueFlameMenuHeatView::needsUpdate() const
	{
		if (_fadePeriodCounter->pending())
		{
			_fadePeriodCounter->reset();
		}

		// The throttle period (for the actual shader program) is set by the part configuration
		//
		//return _periodCounter->pending();
		return false;
	}
	void brogueFlameMenuHeatView::clearUpdate()
	{
		_periodCounter->reset();


		brogueViewBase::clearUpdate();
	}
	void brogueFlameMenuHeatView::clearEvents()
	{
		brogueViewBase::clearEvents();
	}
	void brogueFlameMenuHeatView::update(int millisecondsLapsed, bool forceUpdate)
	{
	}
}
#pragma once
#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueTitleGrid.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewGridCore.h"
#include "color.h"
#include "eventController.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "resourceController.h"
#include "simple.h"
#include "simplePeriodCounter.h"

namespace brogueHd::frontend
{
	using namespace simple;
	using namespace brogueHd::backend;

	/// <summary>
	/// Component responsibility is to provide "heat" constants to the GPU. The data stream of brogueFlameQuad 
	/// elements will provide the constant colors below. This view will keep track of a random sequence to provide
	/// also as uniforms to the GPU shaders to add variability.
	/// </summary>
	class brogueFlameMenuHeatView : public brogueViewGridCore<brogueCellQuad>
	{
	public:
		brogueFlameMenuHeatView(brogueCoordinateConverter* coordinateConverter,
		                        resourceController* resourceController,
		                        eventController* eventController,
		                        randomGenerator* randomGenerator,
		                        const brogueUIProgramPartId& partId,
		                        const brogueUIData& data,
		                        int fadePeriodMilliseconds);
		~brogueFlameMenuHeatView();

		void update(int millisecondsLapsed,
		            bool forceUpdate) override;

		void checkUpdate(const brogueKeyboardState& keyboardState,
		                 const brogueMouseState& mouseState,
		                 int millisecondsLapsed) override;

		void clearUpdate() override;

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
		bool isTheText(int column, int row) const
		{
			return _titleGrid->isTheText(column, row);
		}

	private:
		void updateImpl(int millisecondsLapsed, int forceUpdate);

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

	brogueFlameMenuHeatView::brogueFlameMenuHeatView(brogueCoordinateConverter* coordinateConverter,
	                                                 resourceController* resourceController,
	                                                 eventController* eventController,
	                                                 randomGenerator* randomGenerator,
	                                                 const brogueUIProgramPartId& partId,
	                                                 const brogueUIData& data,
	                                                 int fadePeriodMilliseconds)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data, true)
	{
		_randomGenerator = randomGenerator;
		_fadePeriodRandom1 = randomGenerator->next() * 10000;
		_fadePeriodRandom2 = randomGenerator->next() * 10000;
		_periodCounter = new simplePeriodCounter(5);
		_fadePeriodCounter = new simplePeriodCounter(fadePeriodMilliseconds);
		_titleGrid = new brogueTitleGrid();

		update(0, true);
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

		// Set Uniforms (where they are defined)
		setUniform<float>("fadePeriodTime", currentFadePeriod());
		setUniform<int>("fadePeriodRandom1", currentFadePeriodRandom1());
		setUniform<int>("fadePeriodRandom2", currentFadePeriodRandom2());
		//setUniform<int>("nextColorNumber", fadePeriodCount() % 3);

		// Mouse Interaction
		brogueViewGridCore::checkUpdate(keyboardState, mouseState, millisecondsLapsed);
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

		brogueViewGridCore::clearUpdate();
	}

	void brogueFlameMenuHeatView::updateImpl(int millisecondsLapsed, int forceUpdate)
	{
		brogueFlameMenuHeatView* that = this;
		brogueTitleGrid* titleGrid = _titleGrid;
		gridRect sceneBounds = _titleGrid->sceneBounds();

		sceneBounds.iterate([&that, &titleGrid, &sceneBounds](int column, int row)
		{
			brogueCellDisplay cell(column, row);

			if (titleGrid->isTheText(column, row))
			{
				cell.backColor = colors::blue();

				that->set(cell);
			}
			else if (row == sceneBounds.bottom())
			{
				cell.backColor = colors::red();

				that->set(cell);
			}
			else
			{
				// TODO: Figure out how to remove excess data w/o messing up the drawing
				cell.backColor = colors::transparent();
				that->set(cell);
			}

			return iterationCallback::iterate;
		});
	}

	void brogueFlameMenuHeatView::update(int millisecondsLapsed, bool forceUpdate)
	{
		// Update data elements in the view
		updateImpl(millisecondsLapsed, forceUpdate);

		// Call the base class -> restream the data
		brogueViewGridCore<brogueCellQuad>::update(millisecondsLapsed, forceUpdate);
	}
}

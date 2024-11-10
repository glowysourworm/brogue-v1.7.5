#pragma once
#include "brogueCellDisplay.h"
#include "brogueGlobal.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueTitleGrid.h"
#include "brogueUIData.h"
#include "brogueViewBase.h"
#include "color.h"
#include "eventController.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleMath.h"
#include "simplePeriodCounter.h"

using namespace brogueHd::backend;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
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

		void update(const brogueKeyboardState& keyboardState,
					const brogueMouseState& mouseState,
					int millisecondsLapsed) override;

		void checkUpdate(const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed) override;

		void clearUpdate() override;
		void clearEvents() override;

		bool needsUpdate() const override;

	public:

		bool isTheText(short column, short row)
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

	protected:

		float calculateHeatEnvelope(short column, short row);

		void cycleHeatSources();
		void nextHeatValues();

	public:

		const float FlameNoise = 0.35f;
		const float FlameFade = 0.35f;

		const color FlameBottomColor1 = color(1.0f, 0.0f, 0.0f, 1);
		const color FlameBottomColor2 = color(1.0f, 0.2f, 0.1f, 1);
		const color FlameBottomColor3 = color(0.1f, 0.0f, 0.0f, 1);

		const color FlameTitleColor1 = color(0.0f, 0.0f, 1.0f, 1);
		const color FlameTitleColor2 = color(0.0f, 0.0f, 0.1f, 1);
		const color FlameTitleColor3 = color(0.8f, 0.8f, 1.0f, 1);

		const color BottomHeatSources[3] = { FlameBottomColor1, FlameBottomColor2, FlameBottomColor3 };
		const color TitleHeatSources[3] = { FlameTitleColor1, FlameTitleColor2, FlameTitleColor3 };

	private:

		grid<color*>* _heatSourceGrid;

		randomGenerator* _randomGenerator;
		brogueTitleGrid* _titleGrid;

		simplePeriodCounter* _periodCounter;
		simplePeriodCounter* _fadePeriodCounter;
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

		_periodCounter = new simplePeriodCounter(10);
		_fadePeriodCounter = new simplePeriodCounter(fadePeriodMilliseconds);
		_titleGrid = new brogueTitleGrid();

		_heatSourceGrid = new grid<color*>(sceneBoundary, viewBoundary);

		// Initialize the heat source grid
		grid<color*>* heatSourceGrid = _heatSourceGrid;

		viewBoundary.iterate([&heatSourceGrid] (short column, short row)
		{
			heatSourceGrid->set(column, row, new color(0, 0, 0, 1), true);
			return iterationCallback::iterate;
		});

		cycleHeatSources();
		nextHeatValues();
	}
	brogueFlameMenuHeatView::~brogueFlameMenuHeatView()
	{
		_heatSourceGrid->iterate([] (short column, short row, color* item)
		{
			delete item;
			return iterationCallback::iterate;
		});

		delete _heatSourceGrid;
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
	}
	bool brogueFlameMenuHeatView::needsUpdate() const
	{
		// The throttle period (for the actual shader program) is set by the part configuration
		//
		return _periodCounter->pending();
	}
	void brogueFlameMenuHeatView::clearUpdate()
	{
		_periodCounter->reset();
	}
	void brogueFlameMenuHeatView::clearEvents()
	{
		// Nothing to do
	}
	void brogueFlameMenuHeatView::update(const brogueKeyboardState& keyboardState,
										 const brogueMouseState& mouseState,
										 int millisecondsLapsed)
	{
		if (_fadePeriodCounter->pending())
			cycleHeatSources();

		nextHeatValues();

		if (_fadePeriodCounter->pending())
			_fadePeriodCounter->reset();
	}

	float brogueFlameMenuHeatView::calculateHeatEnvelope(short column, short row)
	{
		// Column Sine-Wave Pseudo-Heat Function
		//
		float width = (float)this->getBoundary().width;
		float frequency = 7.0f;																		// Number of "candles" across the bottom
		float lowValue = 0.2f;
		float triangleLeft = 2 * (column / width);
		float triangleRight = (triangleLeft * (lowValue - 1)) + (2 - lowValue);
		//float timeArg = 0.3f * simpleMath::sin(2 * simpleMath::Pi * timeValue);
		float timeArg = 0;
		float sinArg = (frequency * ((column / width) * simpleMath::Pi)) + (timeArg);
		float sinAmplitude = (column < (width / 2.0f)) ? triangleLeft : triangleRight;

		// Calculate the heat along the bottom
		float heatValue = sinAmplitude * simpleMath::abs(simpleMath::sin(sinArg));

		return heatValue;
	}

	void brogueFlameMenuHeatView::cycleHeatSources()
	{
		brogueFlameMenuHeatView* that = this;
		randomGenerator* randGenerator = _randomGenerator;

		_heatSourceGrid->iterate([&that, &randGenerator] (short column, short row, color* currentColor)
		{
			color nextColor;

			if (that->isTheText(column, row))
			{
				int nextIndex = randGenerator->randomIndex(0, 3);
				nextColor = randGenerator->nextColorNear(that->TitleHeatSources[nextIndex], that->FlameNoise);
				nextColor.alpha = that->calculateHeatEnvelope(column, row);

				// Mix this color with the current cell color to blend smoothly
				nextColor.interpolate(that->get(column, row)->backColor, 1 - that->FlameFade);
			}
			else if (row == ROWS - 1)
			{
				int nextIndex = randGenerator->randomIndex(0, 3);
				nextColor = randGenerator->nextColorNear(that->BottomHeatSources[nextIndex], that->FlameNoise);
				nextColor.alpha = that->calculateHeatEnvelope(column, row);

				// Mix this color with the current cell color to blend smoothly
				nextColor.interpolate(that->get(column, row)->backColor, 1 - that->FlameFade);
			}
			else
			{
				nextColor = colors::black();
			}

			currentColor->set(nextColor);

			return iterationCallback::iterate;
		});

		// Smooth out the random noise across the bottom row
		//
		for (int index = 1; index < this->getSceneBoundary().width - 1; index++)
		{
			color left = *_heatSourceGrid->get(index - 1, this->getSceneBoundary().height - 1);
			color right = *_heatSourceGrid->get(index + 1, this->getSceneBoundary().height - 1);
			color current = *_heatSourceGrid->get(index, this->getSceneBoundary().height - 1);

			current.averageIn(1, 1, left, right);

			_heatSourceGrid->get(index, this->getSceneBoundary().height - 1)->set(current);
		}
	}
	void brogueFlameMenuHeatView::nextHeatValues()
	{
		brogueFlameMenuHeatView* that = this;
		randomGenerator* randGenerator = _randomGenerator;
		grid<color*>* heatSourceGrid = _heatSourceGrid;

		this->iterate([&that, &randGenerator, &heatSourceGrid] (short column, short row, brogueCellDisplay* cell)
		{
			// Heat Source
			if (that->isTheText(column, row))
			{
				color next = *heatSourceGrid->get(column, row);
				cell->backColor.interpolate(next, that->FlameFade);
			}

			// Heat Source
			else if (row == ROWS - 1)
			{
				color next = *heatSourceGrid->get(column, row);
				cell->backColor.interpolate(next, that->FlameFade);
			}

			// Heat "Diffusion"
			else if (column > 0 && column < COLS - 1)
			{
				color south = that->get(column, row + 1)->backColor;
				color southEast = that->get(column + 1, row + 1)->backColor;
				color southWest = that->get(column - 1, row + 1)->backColor;

				// Order may matter
				cell->backColor.averageIn(1, 1, south, southEast, southWest);
			}
			else
			{
				cell->backColor = color(0, 0, 0, 0);
				cell->noDisplay = true;
			}

			return iterationCallback::iterate;
		});
	}
}
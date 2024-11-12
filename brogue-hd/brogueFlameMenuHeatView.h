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
#include "simpleList.h"
#include "simpleMath.h"
#include "simplePeriodCounter.h"

using namespace brogueHd::simple;
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
					int millisecondsLapsed,
					bool forceUpdate) override;

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
				if (titleGrid->isTheText(column, row) || row == sceneBounds.bottom())
					callback(column, row, cell);

				return iterationCallback::iterate;
			});
		}

	protected:

		float calculateHeatEnvelope(short column, short row);

		void cycleHeatSources();
		void nextHeatValues();

	public:

		const float FlameNoise = 0.2f;
		const float FlameFade = 0.01f;
		const float FlameDiffuse = 0.95f;

		const color FlameBottomColor1 = color(1.0f, 0.0f, 0.0f, 1);
		const color FlameBottomColor2 = color(1.0f, 0.2f, 0.1f, 1);
		const color FlameBottomColor3 = color(0.1f, 0.0f, 0.0f, 1);

		const color FlameTitleColor1 = color(0.0f, 0.0f, 1.0f, 1);
		const color FlameTitleColor2 = color(0.0f, 0.0f, 0.1f, 1);
		const color FlameTitleColor3 = color(0.8f, 0.8f, 1.0f, 1);

		const color BottomHeatSources[3] = { FlameBottomColor1, FlameBottomColor2, FlameBottomColor3 };
		const color TitleHeatSources[3] = { FlameTitleColor1, FlameTitleColor2, FlameTitleColor3 };

	private:

		randomGenerator* _randomGenerator;
		brogueTitleGrid* _titleGrid;

		simplePeriodCounter* _periodCounter;
		simplePeriodCounter* _fadePeriodCounter;

		grid<color*>* _heatSourceGrid;

		bool _firstPass;
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

		_periodCounter = new simplePeriodCounter(5);
		_fadePeriodCounter = new simplePeriodCounter(fadePeriodMilliseconds);
		_titleGrid = new brogueTitleGrid();
		_firstPass = true;

		_heatSourceGrid = new grid<color*>(sceneBoundary, viewBoundary);

		grid<color*>* heatGrid = _heatSourceGrid;

		// Use THIS iterator (which will skip the majority of the grid)
		this->iterate([&heatGrid] (short column, short row, brogueCellDisplay* cell)
		{
			heatGrid->set(column, row, new color(0, 0, 0, 0));
			return iterationCallback::iterate;
		});

		cycleHeatSources();
		nextHeatValues();

		_firstPass = false;
	}
	brogueFlameMenuHeatView::~brogueFlameMenuHeatView()
	{
		grid<color*>* heatGrid = _heatSourceGrid;

		// Use THIS iterator (which will skip the majority of the grid)
		this->iterate([&heatGrid] (short column, short row, brogueCellDisplay* cell)
		{
			delete heatGrid->get(column, row);
			return iterationCallback::iterate;
		});

		delete _periodCounter;
		delete _fadePeriodCounter;
		delete _heatSourceGrid;
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

		// Mouse Interaction
		brogueViewBase::checkUpdate(keyboardState, mouseState, millisecondsLapsed);
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

		brogueViewBase::clearUpdate();
	}
	void brogueFlameMenuHeatView::clearEvents()
	{
		brogueViewBase::clearEvents();
	}
	void brogueFlameMenuHeatView::update(const brogueKeyboardState& keyboardState,
										 const brogueMouseState& mouseState,
										 int millisecondsLapsed,
										 bool forceUpdate)
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
		int timeValue = _fadePeriodCounter->getCounter();
		float width = (float)this->getBoundary().width;
		float frequency = 7.0f;																		// Number of "candles" across the bottom
		float lowValue = 0.2f;
		float triangleLeft = 2 * (column / width);
		float triangleRight = (triangleLeft * (lowValue - 1)) + (2 - lowValue);
		float timeArg = 0.3f * simpleMath::sin(2 * simpleMath::Pi * timeValue);
		//float timeArg = 0;
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
		bool firstPass = _firstPass;
		grid<color*>* heatGrid = _heatSourceGrid;

		this->iterate([&that, &randGenerator, &firstPass, &heatGrid] (short column, short row, brogueCellDisplay* currentColor)
		{
			color nextColor;

			float heatEnvelope = that->calculateHeatEnvelope(column, row);

			if (that->isTheText(column, row))
			{
				int nextIndex = randGenerator->randomIndex(0, 3);
				nextColor = randGenerator->nextColorNear(that->TitleHeatSources[nextIndex], that->FlameNoise);
			}
			else if (row == ROWS - 1)
			{
				int nextIndex = randGenerator->randomIndex(0, 3);
				nextColor = randGenerator->nextColorNear(that->BottomHeatSources[nextIndex], that->FlameNoise);

			}

			// Take color channels and apply heat envelope
			//if (heatEnvelope < 0.25)
			//	nextColor.blue *= heatEnvelope;

			//else if (heatEnvelope < 0.1)
			//{
			//	nextColor.blue *= heatEnvelope;
			//	nextColor.green *= heatEnvelope;
			//}
			//else
			//{
				//nextColor.blue *= heatEnvelope * 4;
				//nextColor.green *= heatEnvelope * 4;
				//nextColor.red *= heatEnvelope * 4;
			//}

			nextColor.alpha = heatEnvelope;

			heatGrid->get(column, row)->set(nextColor);

			return iterationCallback::iterate;
		});

		// Smooth out the random noise across the bottom row
		//
		//for (int index = 1; index < this->getSceneBoundary().width - 1; index++)
		//{
		//	color left = this->get(index - 1, this->getSceneBoundary().height - 1)->backColor;
		//	color right = this->get(index + 1, this->getSceneBoundary().height - 1)->backColor;
		//	color current = this->get(index, this->getSceneBoundary().height - 1)->backColor;

		//	current.averageIn(1, 1, left, right);

		//	this->get(index, this->getSceneBoundary().height - 1)->backColor = current;
		//}
	}
	void brogueFlameMenuHeatView::nextHeatValues()
	{
		brogueFlameMenuHeatView* that = this;
		randomGenerator* randGenerator = _randomGenerator;
		grid<color*>* heatGrid = _heatSourceGrid;

		this->iterate([&that, &randGenerator, &heatGrid] (short column, short row, brogueCellDisplay* cell)
		{
			// Heat Source
			if (that->isTheText(column, row))
			{
				cell->backColor.interpolate(*heatGrid->get(column, row), that->FlameFade);
			}

			// Heat Source
			else if (row == ROWS - 1)
			{
				cell->backColor.interpolate(*heatGrid->get(column, row), that->FlameFade);
			}

			//// Heat "Diffusion"
			//else if (column > 0 && column < COLS - 1)
			//{
			//	color south = that->get(column, row + 1)->backColor;
			//	color southEast = that->get(column + 1, row + 1)->backColor;
			//	color southWest = that->get(column - 1, row + 1)->backColor;

			//	// Order may matter
			//	cell->backColor.averageIn(1, that->FlameDiffuse, south, southEast, southWest);
			//}

			return iterationCallback::iterate;
		});
	}
}
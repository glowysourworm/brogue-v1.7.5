#pragma once
#include "brogueCellDisplay.h"
#include "brogueGlobal.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueTitleGrid.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
#include "color.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleMath.h"

using namespace brogueHd::backend;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueFlameMenuHeatView : public brogueViewBase
	{
	public:
		brogueFlameMenuHeatView(randomGenerator* randomGenerator,
								int fadePeriodMilliseconds,
								int zoomLevel);
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

		gridRect getBoundary() const override
		{
			return _titleGrid->sceneBounds();
		}
		gridRect getSceneBoundary() const override
		{
			return _titleGrid->sceneBounds();
		}
		/// <summary>
		/// (TODO: MOVE THIS) Calculates the view's boundary is UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		gridRect calculateSceneBoundaryUI() const override
		{
			gridRect sceneBoundary = getSceneBoundary();
			gridRect boundaryUI = gridRect(sceneBoundary.left() * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   sceneBoundary.top() * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()),
										   sceneBoundary.width * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   sceneBoundary.height * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()));

			return boundaryUI;
		}
		brogueCellDisplay* get(short column, short row) const override
		{
			return _heatSourceGrid->get(column, row);
		}

		brogueUIData* getUIData() const override
		{
			return _uiData;
		}
		int getZoomLevel() const override
		{
			return _uiData->getZoomLevel();
		}

		bool isTheText(short column, short row)
		{
			return _titleGrid->isTheText(column, row);
		}

		void iterate(gridCallback<brogueCellDisplay*> callback) const override
		{
			brogueTitleGrid* titleGrid = _titleGrid;
			gridRect sceneBounds = _titleGrid->sceneBounds();

			_heatSourceGrid->iterate([&callback, &titleGrid, &sceneBounds] (short column, short row, brogueCellDisplay* cell)
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

		const float FlameNoise = 0.35f;
		const float FlameFade = 0.35f;

		const color FlameBottomColor1 = color(1.0f, 0.0f, 0.0f, 0.5f);
		const color FlameBottomColor2 = color(1.0f, 0.2f, 0.1f, 0.5f);
		const color FlameBottomColor3 = color(0.1f, 0.0f, 0.0f, 0.5f);

		const color FlameTitleColor1 = color(0.0f, 0.0f, 1.0f, 0.3f);
		const color FlameTitleColor2 = color(0.0f, 0.0f, 0.1f, 0.3f);
		const color FlameTitleColor3 = color(0.8f, 0.8f, 1.0f, 0.3f);

		const color BottomHeatSources[3] = { FlameBottomColor1, FlameBottomColor2, FlameBottomColor3 };
		const color TitleHeatSources[3] = { FlameTitleColor1, FlameTitleColor2, FlameTitleColor3 };

	private:

		randomGenerator* _randomGenerator;

		brogueTitleGrid* _titleGrid;
		brogueUIData* _uiData;

		grid<brogueCellDisplay*>* _heatSourceGrid;

		int _fadePeriodMilliconds;
		int _periodCounter;
	};

	brogueFlameMenuHeatView::brogueFlameMenuHeatView(randomGenerator* randomGenerator,
													 int fadePeriodMilliseconds,
													 int zoomLevel) : brogueViewBase()
	{
		gridRect sceneBounds = _titleGrid->sceneBounds();

		_randomGenerator = randomGenerator;
		_fadePeriodMilliconds = fadePeriodMilliseconds;
		_periodCounter = 0;
		_heatSourceGrid = new grid<brogueCellDisplay*>(sceneBounds, sceneBounds);
		_titleGrid = new brogueTitleGrid();

		grid<brogueCellDisplay*>* grid = _heatSourceGrid;

		sceneBounds.iterate([&grid] (short column, short row)
		{
			grid->set(column, row, new brogueCellDisplay(), true);

			return iterationCallback::iterate;
		});

		brogueUIProgramPartId partId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameMenuProgram_HeatSourceProgram, 0);

		_uiData = new brogueUIData(partId, _titleGrid->sceneBounds(), zoomLevel);
		_uiData->setUIParameters(-1, -1, "", brogueUIAction::None, true, 0, zoomLevel);

		cycleHeatSources();
		nextHeatValues();
	}
	brogueFlameMenuHeatView::~brogueFlameMenuHeatView()
	{
		grid<brogueCellDisplay*>* grid = _heatSourceGrid;

		_titleGrid->sceneBounds().iterate([&grid] (short column, short row)
		{
			delete grid->get(column, row);

			return iterationCallback::iterate;
		});

		delete _heatSourceGrid;
		delete _uiData;
	}
	void brogueFlameMenuHeatView::checkUpdate(const brogueKeyboardState& keyboardState,
											  const brogueMouseState& mouseState,
											  int millisecondsLapsed)
	{
		// Heat source fade over the fade period; and then they're cycled to the next
		// color.
		//
		_periodCounter += millisecondsLapsed;
	}
	bool brogueFlameMenuHeatView::needsUpdate() const
	{
		// The throttle period (for the actual shader program) is set by the part configuration
		//
		return true;
	}
	void brogueFlameMenuHeatView::clearUpdate()
	{
		// Nothing to do
	}
	void brogueFlameMenuHeatView::clearEvents()
	{
		// Nothing to do
	}
	void brogueFlameMenuHeatView::update(const brogueKeyboardState& keyboardState,
										 const brogueMouseState& mouseState,
										 int millisecondsLapsed)
	{
		bool cycleSources = _periodCounter >= _fadePeriodMilliconds;

		if (cycleSources)
			cycleHeatSources();

		nextHeatValues();

		if (cycleSources)
			_periodCounter = 0;
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

		_heatSourceGrid->iterate([&that, &randGenerator] (short column, short row, brogueCellDisplay* currentCell)
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

			currentCell->backColor = nextColor;

			return iterationCallback::iterate;
		});

		// Smooth out the random noise across the bottom row
		//
		for (int index = 1; index < this->getSceneBoundary().width - 1; index++)
		{
			color left = _heatSourceGrid->get(index - 1, this->getSceneBoundary().height - 1)->backColor;
			color right = _heatSourceGrid->get(index + 1, this->getSceneBoundary().height - 1)->backColor;
			color current = _heatSourceGrid->get(index, this->getSceneBoundary().height - 1)->backColor;

			current.averageIn(1, 1, left, right);

			// Not using heap memory except for the copy with the grid (grid<>.get returns a copy)
			_heatSourceGrid->get(index, this->getSceneBoundary().height - 1)->backColor = current;
		}
	}
	void brogueFlameMenuHeatView::nextHeatValues()
	{
		brogueFlameMenuHeatView* that = this;
		randomGenerator* randGenerator = _randomGenerator;
		grid<brogueCellDisplay*>* heatSourceGrid = _heatSourceGrid;

		this->iterate([&that, &randGenerator, &heatSourceGrid] (short column, short row, brogueCellDisplay* cell)
		{
			if (that->isTheText(column, row))
			{
				color next = heatSourceGrid->get(column, row)->backColor;
				cell->backColor.interpolate(next, that->FlameFade);
			}

			else if (row == ROWS - 1)
			{
				color next = heatSourceGrid->get(column, row)->backColor;
				cell->backColor.interpolate(next, that->FlameFade);
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
#pragma once

#include "brogueCellDisplay.h"
#include "brogueGlobal.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIChildResponse.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueView.h"
#include "color.h"
#include "grid.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "simple.h"
#include "simpleMath.h"
#include <cctype>

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend::generator;
using namespace brogueHd::backend::model;
using namespace brogueHd::backend::model::layout;
using namespace brogueHd::backend::model::game;

namespace brogueHd::frontend::ui
{
	class brogueFlameMenu : public brogueView
	{
	public:
		brogueFlameMenu(brogueUIView viewName,
						randomGenerator* randomGenerator,
						int fadePeriodMilliseconds,
						int zoomLevel);
		~brogueFlameMenu();

		void update(const brogueKeyboardState& keyboardState,
					const brogueMouseState& mouseState,
					int millisecondsLapsed) override;

		virtual brogueUIChildResponse checkUpdate(const brogueKeyboardState& keyboardState,
												  const brogueMouseState& mouseState,
												  int millisecondsLapsed);

		float calculateHeatEnvelope(short column, short row);

	private:

		void cycleHeatSources();
		void nextHeatValues();

	protected:

		//
		// Brogue v1.7.5 (Brian Walker a.k.a. penderprime) (preserved as well as possible)
		//

		gridRect textBounds()
		{
			// Took one off of this guy because of the odd width of the title.
			// It's just the location of the bounding box - to center the text.
			//
			return   gridRect((COLS - MENU_TITLE_WIDTH) / 2,
							  (ROWS - MENU_TITLE_HEIGHT) / 2,
							  MENU_TITLE_WIDTH,
							  MENU_TITLE_HEIGHT);
		}

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

		bool isTheText(short column, short row)
		{
			gridRect theTextBounds = this->textBounds();

			if (!theTextBounds.contains(column, row))
				return false;

			// Take off the odd column on the right
			if (column == theTextBounds.right())
				return false;

			// THESE INDICIES ARE FLIPPED:  ROW-FIRST
			return !(std::isspace(this->Title[row - theTextBounds.top()][column - theTextBounds.left()]) > 0);
		}

	private:

		randomGenerator* _randomGenerator;

		grid<color>* _heatSourceGrid;

		int _fadePeriodMilliconds;
		int _periodCounter;

		const char Title[MENU_TITLE_HEIGHT][MENU_TITLE_WIDTH] = {
			"########   ########       ######         #######   ####     ###  #########",
			" ##   ###   ##   ###    ##     ###     ##      ##   ##       #    ##     #",
			" ##    ##   ##    ##   ##       ###   ##        #   ##       #    ##     #",
			" ##    ##   ##    ##   #    #    ##   #         #   ##       #    ##      ",
			" ##    ##   ##    ##  ##   ##     ## ##             ##       #    ##    # ",
			" ##   ##    ##   ##   ##   ###    ## ##             ##       #    ##    # ",
			" ######     ## ###    ##   ####   ## ##             ##       #    ####### ",
			" ##    ##   ##  ##    ##   ####   ## ##             ##       #    ##    # ",
			" ##     ##  ##   ##   ##    ###   ## ##      #####  ##       #    ##    # ",
			" ##     ##  ##   ##   ###    ##   ## ###       ##   ##       #    ##      ",
			" ##     ##  ##    ##   ##    #    #   ##       ##   ##       #    ##      ",
			" ##     ##  ##    ##   ###       ##   ###      ##   ###      #    ##     #",
			" ##    ##   ##     ##   ###     ##     ###    ###    ###    #     ##     #",
			"########   ####    ###    ######         #####        ######     #########",
			"                            ##                                            ",
			"                        ##########                                        ",
			"                            ##                                            ",
			"                            ##                                            ",
			"                           ####                                           ",
		};
	};

	brogueFlameMenu::brogueFlameMenu(brogueUIView viewName,
									 randomGenerator* randomGenerator,
									 int fadePeriodMilliseconds,
									 int zoomLevel)

		: brogueView(new brogueUIData(gridRect(0, 0, COLS, ROWS), zoomLevel), gridRect(0, 0, COLS, ROWS), gridRect(0, 0, COLS, ROWS))
	{
		_randomGenerator = randomGenerator;
		_fadePeriodMilliconds = fadePeriodMilliseconds;
		_periodCounter = 0;
		_heatSourceGrid = new grid<color>(this->getSceneBoundary(), this->getSceneBoundary());

		cycleHeatSources();
		nextHeatValues();
	}
	brogueFlameMenu::~brogueFlameMenu()
	{
		delete _heatSourceGrid;
	}
	brogueUIChildResponse brogueFlameMenu::checkUpdate(const brogueKeyboardState& keyboardState,
													   const brogueMouseState& mouseState,
													   int millisecondsLapsed)
	{
		brogueUIChildResponse response;

		bool hasInteraction = this->getUIData()->getHasMouseInteraction();

		if (hasInteraction)
		{
			bool mouseOver = this->isMouseOver(mouseState);

			// Update the UI data
			response.actionMet = this->getUIData()->setMouseUpdate(mouseState.getMouseLeft(), mouseOver);
			response.needsUpdate = hasInteraction && (mouseOver || mouseState.getMouseLeft());
			response.tag = *this->getUIData()->getAction();
		}

		return response;
	}
	void brogueFlameMenu::update(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed)
	{
		// Heat source fade over the fade period; and then they're cycled to the next
		// color.
		//
		_periodCounter += millisecondsLapsed;

		bool cycleSources = _periodCounter >= _fadePeriodMilliconds;

		if (cycleSources)
			cycleHeatSources();

		nextHeatValues();

		if (cycleSources)
			_periodCounter = 0;
	}

	float brogueFlameMenu::calculateHeatEnvelope(short column, short row)
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

	void brogueFlameMenu::cycleHeatSources()
	{
		brogueFlameMenu* that = this;
		randomGenerator* randGenerator = _randomGenerator;
		grid<color>* heatSourceGrid = _heatSourceGrid;

		_heatSourceGrid->iterate([&that, &randGenerator, &heatSourceGrid] (short column, short row, color currentHeatSource)
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

			heatSourceGrid->set(column, row, nextColor, true);

			return iterationCallback::iterate;
		});

		// Smooth out the random noise across the bottom row
		//
		for (int index = 1; index < this->getSceneBoundary().width - 1; index++)
		{
			color left = _heatSourceGrid->get(index - 1, this->getSceneBoundary().height - 1);
			color right = _heatSourceGrid->get(index + 1, this->getSceneBoundary().height - 1);
			color current = _heatSourceGrid->get(index, this->getSceneBoundary().height - 1);

			current.averageIn(1, 1, left, right);

			// Not using heap memory except for the copy with the grid (grid<>.get returns a copy)
			_heatSourceGrid->set(index, this->getSceneBoundary().height - 1, current, true);
		}
	}
	void brogueFlameMenu::nextHeatValues()
	{
		brogueFlameMenu* that = this;
		randomGenerator* randGenerator = _randomGenerator;
		grid<color>* heatSourceGrid = _heatSourceGrid;

		this->iterate([&that, &randGenerator, &heatSourceGrid] (short column, short row, brogueCellDisplay* cell)
		{
			if (that->isTheText(column, row))
			{
				color next = heatSourceGrid->get(column, row);
				cell->backColor.interpolate(next, that->FlameFade);
			}

			else if (row == ROWS - 1)
			{
				color next = heatSourceGrid->get(column, row);
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

/*
#define MENU_FLAME_PRECISION_FACTOR		10
#define MENU_FLAME_RISE_SPEED			50
#define MENU_FLAME_SPREAD_SPEED			20
#define MENU_FLAME_COLOR_DRIFT_SPEED	500
#define MENU_FLAME_FADE_SPEED			20
#define MENU_FLAME_UPDATE_DELAY			50
#define MENU_FLAME_ROW_PADDING			2
#define MENU_TITLE_OFFSET_X				(-4)
#define MENU_TITLE_OFFSET_Y				(-1)

#define MENU_FLAME_COLOR_SOURCE_COUNT	1136

#define MENU_FLAME_DENOMINATOR			(100 + MENU_FLAME_RISE_SPEED + MENU_FLAME_SPREAD_SPEED)

void drawMenuFlames(signed short flames[COLS][(ROWS + MENU_FLAME_ROW_PADDING)][3], unsigned char mask[COLS][ROWS]) {
	short i, j, versionStringLength;
	color tempColor = { 0 };
	const color* maskColor = &black;
	char dchar;

	versionStringLength = strLenWithoutEscapes(BROGUE_VERSION_STRING);

	for (j = 0; j < ROWS; j++) {
		for (i = 0; i < COLS; i++) {
			if (j == ROWS - 1 && i >= COLS - versionStringLength) {
				dchar = BROGUE_VERSION_STRING[i - (COLS - versionStringLength)];
			}
			else {
				dchar = ' ';
			}

			if (mask[i][j] == 100) {
				plotCharWithColor(dchar, i, j, &darkGray, maskColor);
			}
			else {
				tempColor = black;
				tempColor.red = flames[i][j][0] / MENU_FLAME_PRECISION_FACTOR;
				tempColor.green = flames[i][j][1] / MENU_FLAME_PRECISION_FACTOR;
				tempColor.blue = flames[i][j][2] / MENU_FLAME_PRECISION_FACTOR;
				if (mask[i][j] > 0) {
					applyColorAverage(&tempColor, maskColor, mask[i][j]);
				}
				plotCharWithColor(dchar, i, j, &darkGray, &tempColor);
			}
		}
	}
}

void updateMenuFlames(const color* colors[COLS][(ROWS + MENU_FLAME_ROW_PADDING)],
	signed short colorSources[MENU_FLAME_COLOR_SOURCE_COUNT][4],
	signed short flames[COLS][(ROWS + MENU_FLAME_ROW_PADDING)][3]) {

	short i, j, k, l, x, y;
	signed short tempFlames[COLS][3];
	short colorSourceNumber, rand;

	colorSourceNumber = 0;
	for (j = 0; j < (ROWS + MENU_FLAME_ROW_PADDING); j++) {
		// Make a temp copy of the current row.
		for (i = 0; i < COLS; i++) {
			for (k = 0; k < 3; k++) {
				tempFlames[i][k] = flames[i][j][k];
			}
		}

		for (i = 0; i < COLS; i++) {
			// Each cell is the weighted average of the three color values below and itself.
			// Weight of itself: 100
			// Weight of left and right neighbors: MENU_FLAME_SPREAD_SPEED / 2 each
			// Weight of below cell: MENU_FLAME_RISE_SPEED
			// Divisor: 100 + MENU_FLAME_SPREAD_SPEED + MENU_FLAME_RISE_SPEED

			// Itself:
			for (k = 0; k < 3; k++) {
				flames[i][j][k] = 100 * flames[i][j][k] / MENU_FLAME_DENOMINATOR;
			}

			// Left and right neighbors:
			for (l = -1; l <= 1; l += 2) {
				x = i + l;
				if (x == -1) {
					x = COLS - 1;
				}
				else if (x == COLS) {
					x = 0;
				}
				for (k = 0; k < 3; k++) {
					flames[i][j][k] += MENU_FLAME_SPREAD_SPEED * tempFlames[x][k] / 2 / MENU_FLAME_DENOMINATOR;
				}
			}

			// Below:
			y = j + 1;
			if (y < (ROWS + MENU_FLAME_ROW_PADDING)) {
				for (k = 0; k < 3; k++) {
					flames[i][j][k] += MENU_FLAME_RISE_SPEED * flames[i][y][k] / MENU_FLAME_DENOMINATOR;
				}
			}

			// Fade a little:
			for (k = 0; k < 3; k++) {
				flames[i][j][k] = (1000 - MENU_FLAME_FADE_SPEED) * flames[i][j][k] / 1000;
			}

			if (colors[i][j]) {
				// If it's a color source tile:

				// First, cause the color to drift a little.
				for (k = 0; k < 4; k++) {
					colorSources[colorSourceNumber][k] += rand_range(-MENU_FLAME_COLOR_DRIFT_SPEED, MENU_FLAME_COLOR_DRIFT_SPEED);
					colorSources[colorSourceNumber][k] = clamp(colorSources[colorSourceNumber][k], 0, 1000);
				}

				// Then, add the color to this tile's flames.
				rand = colors[i][j]->rand * colorSources[colorSourceNumber][0] / 1000;
				flames[i][j][0] += (colors[i][j]->red + (colors[i][j]->redRand * colorSources[colorSourceNumber][1] / 1000) + rand) * MENU_FLAME_PRECISION_FACTOR;
				flames[i][j][1] += (colors[i][j]->green + (colors[i][j]->greenRand * colorSources[colorSourceNumber][2] / 1000) + rand) * MENU_FLAME_PRECISION_FACTOR;
				flames[i][j][2] += (colors[i][j]->blue + (colors[i][j]->blueRand * colorSources[colorSourceNumber][3] / 1000) + rand) * MENU_FLAME_PRECISION_FACTOR;

				colorSourceNumber++;
			}
		}
	}
}

// Takes a grid of values, each of which is 0 or 100, and fills in some middle values in the interstices.
void antiAlias(unsigned char mask[COLS][ROWS]) {
	short i, j, x, y, dir, nbCount;
	const short intensity[5] = { 0, 0, 35, 50, 60 };

	for (i = 0; i < COLS; i++) {
		for (j = 0; j < ROWS; j++) {
			if (mask[i][j] < 100) {
				nbCount = 0;
				for (dir = 0; dir < 4; dir++) {
					x = i + nbDirs[dir][0];
					y = j + nbDirs[dir][1];
					if (coordinatesAreInWindow(x, y) && mask[x][y] == 100) {
						nbCount++;
					}
				}
				mask[i][j] = intensity[nbCount];
			}
		}
	}
}

#define MENU_TITLE_WIDTH	74
#define MENU_TITLE_HEIGHT	19

void initializeMenuFlames(boolean includeTitle,
	const color* colors[COLS][(ROWS + MENU_FLAME_ROW_PADDING)],
	color colorStorage[COLS],
	signed short colorSources[MENU_FLAME_COLOR_SOURCE_COUNT][4],
	signed short flames[COLS][(ROWS + MENU_FLAME_ROW_PADDING)][3],
	unsigned char mask[COLS][ROWS]) {
	short i, j, k, colorSourceCount;
	const char title[MENU_TITLE_HEIGHT][MENU_TITLE_WIDTH + 1] = {
		"########   ########       ######         #######   ####     ###  #########",
		" ##   ###   ##   ###    ##     ###     ##      ##   ##       #    ##     #",
		" ##    ##   ##    ##   ##       ###   ##        #   ##       #    ##     #",
		" ##    ##   ##    ##   #    #    ##   #         #   ##       #    ##      ",
		" ##    ##   ##    ##  ##   ##     ## ##             ##       #    ##    # ",
		" ##   ##    ##   ##   ##   ###    ## ##             ##       #    ##    # ",
		" ######     ## ###    ##   ####   ## ##             ##       #    ####### ",
		" ##    ##   ##  ##    ##   ####   ## ##             ##       #    ##    # ",
		" ##     ##  ##   ##   ##    ###   ## ##      #####  ##       #    ##    # ",
		" ##     ##  ##   ##   ###    ##   ## ###       ##   ##       #    ##      ",
		" ##     ##  ##    ##   ##    #    #   ##       ##   ##       #    ##      ",
		" ##     ##  ##    ##   ###       ##   ###      ##   ###      #    ##     #",
		" ##    ##   ##     ##   ###     ##     ###    ###    ###    #     ##     #",
		"########   ####    ###    ######         #####        ######     #########",
		"                            ##                                            ",
		"                        ##########                                        ",
		"                            ##                                            ",
		"                            ##                                            ",
		"                           ####                                           ",
	};

const color flameSourceColor = {20, 7, 7, 60, 40, 40, 0, true}; // 8
const color flameSourceColorSecondary = {7, 2, 0, 10, 0, 0, 0, true};

//const color flameTitleColor = {0, 0, 0, 17, 10, 6, 0, true}; // pale orange
//const color flameTitleColor = {0, 0, 0, 7, 7, 10, 0, true}; // *pale blue*
const color flameTitleColor = {0, 0, 0, 9, 9, 15, 0, true}; // *pale blue**

	for (i=0; i<COLS; i++) {
		for (j=0; j<ROWS; j++) {
			mask[i][j] = 0;
		}
	}

	for (i=0; i<COLS; i++) {
		for (j=0; j<(ROWS + MENU_FLAME_ROW_PADDING); j++) {
			colors[i][j] = NULL;
			for (k=0; k<3; k++) {
				flames[i][j][k] = 0;
			}
		}
	}

	// Seed source color random components.
	for (i=0; i<MENU_FLAME_COLOR_SOURCE_COUNT; i++) {
		for (k=0; k<4; k++) {
			colorSources[i][k] = rand_range(0, 1000);
		}
	}

	// Put some flame source along the bottom row.
	colorSourceCount = 0;
	for (i=0; i<COLS; i++) {
		colorStorage[colorSourceCount] = flameSourceColor;
		applyColorAverage(&(colorStorage[colorSourceCount]), &flameSourceColorSecondary, 100 - (smoothHiliteGradient(i, COLS - 1) + 25));

		colors[i][(ROWS + MENU_FLAME_ROW_PADDING)-1] = &(colorStorage[colorSourceCount]);
		colorSourceCount++;
	}

	if (includeTitle) {
		// Wreathe the title in flames, and mask it in black.
		for (i=0; i<MENU_TITLE_WIDTH; i++) {
			for (j=0; j<MENU_TITLE_HEIGHT; j++) {
				if (title[j][i] != ' ') {
					colors[(COLS - MENU_TITLE_WIDTH)/2 + i + MENU_TITLE_OFFSET_X][(ROWS - MENU_TITLE_HEIGHT)/2 + j + MENU_TITLE_OFFSET_Y] = &flameTitleColor;
					colorSourceCount++;
					mask[(COLS - MENU_TITLE_WIDTH)/2 + i + MENU_TITLE_OFFSET_X][(ROWS - MENU_TITLE_HEIGHT)/2 + j + MENU_TITLE_OFFSET_Y] = 100;
				}
			}
		}

		// Anti-alias the mask.
		antiAlias(mask);
	}

	brogueAssert(colorSourceCount <= MENU_FLAME_COLOR_SOURCE_COUNT);

	// Simulate the background flames for a while
	for (i=0; i<100; i++) {
		updateMenuFlames(colors, colorSources, flames);
	}

*/
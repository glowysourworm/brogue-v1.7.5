#pragma once

#include "brogueGlobal.h"
#include "color.h"
#include "grid.h"
#include "brogueView.h"
#include "simpleMath.h"
#include "randomGenerator.h"

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
		brogueFlameMenu(randomGenerator* randomGenerator,
						int padding, 
						int precision, 
						int riseSpeed, 
						int spreadSpeed, 
						int colorDriftSpeed, 
						int fadeSpeed, 
						int updateDelay);
		~brogueFlameMenu();

		void animate(int millisecondsLapsed);

		void update(int millisecondsLapsed) override;

	private:

		randomGenerator* _randomGenerator;

		int _animationTime;
		int _animationPeriod;

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

		const float FlameRedThreshold = 0.2;
		const float FlameBlueThreshold = 0.8;
		const float FlameMaxValue = 1.0;

		const float TextValue = 100.0;

		const float FlamePrecision = 0.01;
		const float FlameRiseSpeed = 0.05;
		const float FlameSpreadSpeed = 0.02;
		const float FlameColorDriftSpeed = 0.5;
		const float FlameFadeSpeed = 0.02;
		const int FlameRowPadding = 2;

		const color FlamePrimaryColor =			color(1.0, 0.1, 0.0, 0.8);
		const color FlameSecondaryColor =		color(1.0, 0.8, 0.0, 0.8);
		const color FlameTertiaryColor =		color(1.0, 0.8, 0.8, 0.8);

		const color FlameTitleColor =			color(0.1, 0.1, 1.0, 0.8);

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

		grid<color*>* _heatSources;

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

	brogueFlameMenu::brogueFlameMenu(randomGenerator* randomGenerator,
									 int padding,
									 int precision,
									 int riseSpeed,
									 int spreadSpeed,
									 int colorDriftSpeed,
									 int fadeSpeed,
									 int updateDelay)

		: brogueView(gridRect(0, 0, COLS, ROWS), gridRect(0, 0, COLS, ROWS))
	{
		_randomGenerator = randomGenerator;
		_animationTime = 0;
		_animationPeriod = 5000;

		_heatSources = new grid<color*>(gridRect(0, 0, COLS, ROWS), gridRect(0, 0, COLS, ROWS));

		brogueFlameMenu* that = this;
		grid<color*>* heatSources = _heatSources;

		color flameColors[3] = { this->FlamePrimaryColor, this->FlameSecondaryColor, this->FlameTertiaryColor };
		color flameTitleColors[2] = { this->FlameTitleColor, this->FlameTertiaryColor };

		// Transfer rendering to the primary view grid
		this->getBoundary().iterate([&that, &heatSources, &randomGenerator, &flameColors, &flameTitleColors] (short column, short row)
		{
			if (that->isTheText(column, row))
			{
				int randIndex = randomGenerator->rand_range(0, 2);
				color nextColor = randomGenerator->nextColorNear(flameTitleColors[randIndex], 0.1);

				heatSources->set(column, row, new color(nextColor), true);
			}

			else if (row == ROWS - 1)
			{
				int randIndex = randomGenerator->rand_range(0, 3);
				color nextColor = randomGenerator->nextColorNear(flameColors[randIndex], 0.1);

				heatSources->set(column, row, new color(nextColor), true);
			}

			else
				heatSources->set(column, row, nullptr, true);

			return iterationCallback::iterate;
		});
	}
	brogueFlameMenu::~brogueFlameMenu()
	{
		
	}
	void brogueFlameMenu::update(int millisecondsLapsed)
	{
		this->animate(millisecondsLapsed);
	}
	void brogueFlameMenu::animate(int millisecondsLapsed)
	{
		brogueFlameMenu* that = this;
		randomGenerator* randGenerator = _randomGenerator;
		grid<color*>* heatSources = _heatSources;

		// Track animation time through the cycle
		if (_animationTime >= _animationPeriod)
			_animationTime = 0;

		_animationTime += millisecondsLapsed;

		float timeValue = _animationTime / (float)_animationPeriod;

		color flameColors[3] = { this->FlamePrimaryColor, this->FlameSecondaryColor, this->FlameTertiaryColor };
		color flameTitleColors[2] = { this->FlameTitleColor, this->FlameTertiaryColor };

		// Treat the heat value of 1.0 as max heat
		this->getBoundary().iterateRowsFirst_BottomToTop([&that, &randGenerator, &heatSources, &timeValue, &flameColors, &flameTitleColors] (short column, short row)
		{
			// Transfer heat upwards to the three cells above this one
			// as an average value
			//
			brogueCellDisplay* cell = that->get(column, row);

			// Animate the Heat Sources
			//
			// 
			// Text Mask:  Heat Sources pulled out by adjacent cells
			//
			if (that->isTheText(column, row))
			{				
				// Take the previous color, and interpolate between that and the next random draw.
				//
				int randIndex = randGenerator->rand_range(0, 2);
				color nextColor = randGenerator->nextColorNear(flameTitleColors[randIndex], 0.1);

				heatSources->get(column, row)->interpolate(nextColor, 0.1);

				// Mask off the text value (leave as black)
				return iterationCallback::iterate;
			}
			else if (row == ROWS - 1)
			{
				int randIndex = randGenerator->rand_range(0, 3);
				color nextColor = randGenerator->nextColorNear(flameColors[randIndex], 0.1);

				heatSources->get(column, row)->interpolate(nextColor, 0.1);
			}

			// Check for heat sources
			color* currentSource = heatSources->get(column, row);

			color* southSource = heatSources->getAdjacentUnsafe(column, row, brogueCompass::S);
			color* southWestSource = heatSources->getAdjacentUnsafe(column, row, brogueCompass::SW);
			color* southEastSource = heatSources->getAdjacentUnsafe(column, row, brogueCompass::SE);

			// Treat the walls and text mask as HOT
			color southWest = southWestSource == nullptr ? colors::black() : *southWestSource;
			color south = southSource == nullptr ? colors::black() : *southSource;
			color southEast = southEastSource == nullptr ? colors::black() : *southEastSource;
			//color east = colors::black();
			//color west = colors::black();

			//// Column Sine-Wave Pseudo-Heat Function
			////
			//float width = (float)that->getBoundary().width;
			//float frequency = 7.0f;																		// Number of "candles" across the bottom
			//float lowValue = 0.2f;
			//float triangleLeft = 2 * (column / width);
			//float triangleRight = (triangleLeft * (lowValue - 1)) + (2 - lowValue);
			////float timeArg = 0.3f * simpleMath::sin(2 * simpleMath::Pi * timeValue);
			//float timeArg = 0;
			//float sinArg = (frequency * ((column / width) * simpleMath::Pi)) + (timeArg);
			//float sinAmplitude = (column < (width / 2.0f)) ? triangleLeft : triangleRight;

			//// Calculate the heat along the bottom
			//float heatValue = sinAmplitude * simpleMath::abs(simpleMath::sin(sinArg));

			// Pulls up heat values from the last row
			//
			if (row < ROWS - 1)
			{
				//color titleColor = that->FlameTitleColor;
				//color titleColor = colors::getGray(randGenerator->gaussian(heatValue, 0.2, 0, 1));

				// "Wreathe the text in flames..." (@penderprime)
				//
				//if (that->isTheText(column, row + 1))
				//	south = titleColor;
				//else
				//	south = that->get(column, row + 1)->backColor;

				if (southSource == nullptr)
					south = that->get(column, row + 1)->backColor;

				if (southWestSource == nullptr && column - 1 >= 0)
				{
					southWest = that->get(column - 1, row + 1)->backColor;
					//west = that->isTheText(column - 1, row) ? *heatSources->get(column - 1, row) : that->get(column - 1, row)->backColor;
				}

				if (southEastSource == nullptr && column + 1 < COLS)
				{
					southEast = that->get(column + 1, row + 1)->backColor;
					//east = that->isTheText(column + 1, row) ? *heatSources->get(column + 1, row) : that->get(column + 1, row)->backColor;
				}				
			}

			// Current sources would be the bottom row (only)
			if (currentSource != nullptr)
				cell->backColor.averageIn(0.95f, 1.0f, *currentSource);

			// Use interpolation to blend the adjacent color values
			//
			else
			{
				cell->backColor.averageIn(0.95f, 1.0f, south, southEast, southWest);
			}

			// 

			//cell->backColor.interpolate(east, 0.1f);
			//cell->backColor.interpolate(west, 0.1f);

			// Add some randomness
			//if (cell->backColor.magnitude() >= 0.1f)
			//	cell->backColor.interpolate(randGenerator->nextColor(that->FlamePrimaryColor, that->FlameSecondaryColor), 0.1);

			if (cell->backColor.magnitude() < 0.05f)
				cell->backColor = colors::black();

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
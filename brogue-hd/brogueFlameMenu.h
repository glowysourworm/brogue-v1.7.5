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

	protected:

		void cycleHeatSources();

	public:

		const int NumberHorizontalCandles = 10;

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

		const color FlamePrimaryColor =			color(1.0, 0.1, 0.0, 0.4);
		const color FlameSecondaryColor =		color(1.0, 0.4, 0.0, 0.4);
		const color FlameTertiaryColor =		color(1.0, 0.8, 0.0, 0.4);

		const color FlameTitleColor1 =			color(0.1, 0.1, 1.0, 0.4);
		const color FlameTitleColor2 =			color(0.1, 0.3, 1.0, 0.3);

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
		grid<color*>* _heatSourcesNext;

		simpleArray<color*>* _titleCandleVector;
		simpleArray<color*>* _bottomCandleVector;

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
		_animationPeriod = 50;

		_heatSources = new grid<color*>(gridRect(0, 0, COLS, ROWS), gridRect(0, 0, COLS, ROWS));
		_heatSourcesNext = new grid<color*>(gridRect(0, 0, COLS, ROWS), gridRect(0, 0, COLS, ROWS));

		_titleCandleVector = new simpleArray<color*>(this->NumberHorizontalCandles);
		_bottomCandleVector = new simpleArray<color*>(this->NumberHorizontalCandles);

		for (int index = 0; index < this->NumberHorizontalCandles; index++)
		{
			_titleCandleVector->set(index, new color(0, 0, 0, 0));
			_bottomCandleVector->set(index, new color(0, 0, 0, 0));
		}

		// Initialize the view
		cycleHeatSources();
		animate(_animationPeriod);
		animate(_animationPeriod);
		animate(_animationPeriod);
	}
	brogueFlameMenu::~brogueFlameMenu()
	{
		// colors new'd in the ctor
		for (int column = 0; column < COLS; column++)
		{
			for (int row = 0; row < ROWS; row++)
			{
				if (_heatSources->isDefined(column, row))
					delete _heatSources->get(column, row);

				if (_heatSourcesNext->isDefined(column, row))
					delete _heatSourcesNext->get(column, row);
			}
		}

		for (int index = 0; index < this->NumberHorizontalCandles; index++)
		{
			delete _titleCandleVector->get(index);
			delete _bottomCandleVector->get(index);
		}

		// Rest of the grid
		delete _heatSources;
		delete _heatSourcesNext;

		// Rest of the candle arrays
		delete _titleCandleVector;
		delete _bottomCandleVector;
	}

	void brogueFlameMenu::cycleHeatSources()
	{
		brogueFlameMenu* that = this;
		randomGenerator* randGenerator = _randomGenerator;
		grid<color*>* heatSources = _heatSources;
		grid<color*>* heatSourcesNext = _heatSourcesNext;

		color titleHeats[2] = { this->FlameTitleColor1,
								this->FlameTitleColor2 };	// Heat source colors wreathing the title

		color bottomHeats[3] = { this->FlamePrimaryColor,
								 this->FlameSecondaryColor,
								 this->FlameTertiaryColor };				// Heat source colors for the candles (bottom row)

		// Cycle the color vector along the bottom
		for (int index = 0; index < this->NumberHorizontalCandles; index++)
		{
			int randIndexTitle = _randomGenerator->randomIndex(0, 2);
			int randIndexBottom = _randomGenerator->randomIndex(0, 3);

			_titleCandleVector->get(index)->set(titleHeats[randIndexTitle]);
			_bottomCandleVector->get(index)->set(bottomHeats[randIndexBottom]);
		}

		simpleArray<color*>* titleVector = _titleCandleVector;
		simpleArray<color*>* bottomVector = _bottomCandleVector;

		// Transfer rendering to the primary view grid
		this->getBoundary().iterate([&that, &heatSources, &heatSourcesNext, &titleVector, &bottomVector, &randGenerator] (short column, short row)
		{	
			int candleSize = that->getBoundary().width / titleVector->count();
			int candleIndex = column / candleSize;

			if (that->isTheText(column, row))
			{
				// Iterate adjacent cells (to the text)
				that->getBoundary().iterateAdjacent(column, row, [&candleIndex, &heatSources, &heatSourcesNext, &titleVector, &randGenerator] (short acol, short arow)
				{
					if (!heatSources->isDefined(acol, arow))
					{
						heatSources->set(acol, arow, new color(0, 0, 0, 0), true);
						heatSourcesNext->set(acol, arow, new color(0, 0, 0, 0), true);
					}

					color nextColor = randGenerator->nextColorNear(*titleVector->get(candleIndex), 0.1);

					heatSources->get(acol, arow)->set(*heatSourcesNext->get(acol, arow));
					heatSourcesNext->get(acol, arow)->set(nextColor);

					return iterationCallback::iterate;
				});
			}

			else if (row == ROWS - 1)
			{
				if (!heatSources->isDefined(column, row))
				{
					heatSources->set(column, row, new color(0, 0, 0, 0), true);
					heatSourcesNext->set(column, row, new color(0, 0, 0, 0), true);
				}

				color nextColor = randGenerator->nextColorNear(*bottomVector->get(candleIndex), 0.1);

				heatSources->get(column, row)->set(*heatSourcesNext->get(column, row));
				heatSourcesNext->get(column, row)->set(nextColor);
			}

			return iterationCallback::iterate;
		});
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
		if (_animationTime > _animationPeriod)
		{
			_animationTime = 0;
			cycleHeatSources();
		}

		_animationTime += millisecondsLapsed;

		// Treat the heat value of 1.0 as max heat
		this->getBoundary().iterateRowsFirst_BottomToTop([&that, &randGenerator, &heatSources] (short column, short row)
		{
			// Transfer heat upwards to the three cells above this one
			// as an average value
			//
			brogueCellDisplay* cell = that->get(column, row);

			// Text Mask:  Just skip this one
			if (that->isTheText(column, row))
				return iterationCallback::iterate;

			// Check for heat sources
			color* currentSource = heatSources->get(column, row);

			color* southSource = heatSources->getAdjacentUnsafe(column, row, brogueCompass::S);
			color* southWestSource = heatSources->getAdjacentUnsafe(column, row, brogueCompass::SW);
			color* southEastSource = heatSources->getAdjacentUnsafe(column, row, brogueCompass::SE);

			// Check current values before sources
			//
			color southWest = southWestSource == nullptr ? colors::black() : *southWestSource;
			color south = southSource == nullptr ? colors::black() : *southSource;
			color southEast = southEastSource == nullptr ? colors::black() : *southEastSource;

			// Check current values before sources
			//
			if (row < ROWS - 1)
			{
				if (southSource == nullptr)
					south = that->get(column, row + 1)->backColor;

				if (southWestSource == nullptr && column - 1 >= 0)
				{
					southWest = that->get(column - 1, row + 1)->backColor;
				}

				if (southEastSource == nullptr && column + 1 < COLS)
				{
					southEast = that->get(column + 1, row + 1)->backColor;
				}
			}

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



			// Current Source:  Interpolate this value for the next several frames
			//
			if (currentSource != nullptr)
				cell->backColor.interpolate(*currentSource, 0.3);

			// Diffusion:  Pull up the values from the adjacent cells - 3 below the current - 
			//			   using an average.
			//
			else
			{
				cell->backColor.averageIn(0.7f, 1.0f, south, southEast, southWest);
			}

			// Diffusion Threshold:  This value would be the lower limit of the "heat" in the
			//					     cell - further away from the heat sources.
			//
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
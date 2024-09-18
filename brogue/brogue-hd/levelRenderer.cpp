#include "levelRenderer.h"
#include "brogueLevel.h"
#include "brogueMessageQueue.h"
#include "gridMethods.h"
#include "brogueView.h"
#include "color.h"
#include "colorConstants.h"

namespace brogueHd
{
	levelRenderer::levelRenderer(brogueLevel* level, brogueMessageQueue* messageQueue, randomGenerator* randomGenerator)
	{
		brogueAssert(randomGenerator->checkId(RANDOM_GENERATOR_COSMETIC))

		_level = level;
		_messageQueue = messageQueue;
		_randomGenerator = randomGenerator;

		_levelView = new brogueView(level->levelGrid->getBoundary());
		_rememberedView = new brogueView(level->levelGrid->getBoundary());

		// TODO: Figure out where penderprime hid the boundaries for each of the view segments
		//
		_sidebarView = new brogueView(level->levelGrid->getBoundary());
		_footerMenuView = new brogueView(level->levelGrid->getBoundary());
		_inventoryView = new brogueView(level->levelGrid->getBoundary());
		_messageView = new brogueView(level->levelGrid->getBoundary());
		_flavorMessageView = new brogueView(level->levelGrid->getBoundary());
	}

	levelRenderer::~levelRenderer()
	{
		delete _levelView;
		delete _rememberedView;
		delete _sidebarView;
		delete _footerMenuView;
		delete _inventoryView;
		delete _messageView;
		delete _flavorMessageView;
	}

	brogueView* levelRenderer::getLevelView() const
	{
		return _levelView;
	}
	brogueView* levelRenderer::getRememberedView() const
	{
		return _rememberedView;
	}
	brogueView* levelRenderer::getSidebarView() const
	{
		return _sidebarView;
	}
	brogueView* levelRenderer::getFooterMenuView() const
	{
		return _footerMenuView;
	}
	brogueView* levelRenderer::getInventoryView() const
	{
		return _inventoryView;
	}
	brogueView* levelRenderer::getMessageView() const
	{
		return _messageView;
	}
	brogueView* levelRenderer::getFlavorMessageView() const
	{
		return _flavorMessageView;
	}

	//void levelRenderer::plotCharWithColor(uchar inputChar, short xLoc, short yLoc, const color* cellForeColor, const color* cellBackColor) 
	//{
	//	short oldRNG;

	//	short foreRed = cellForeColor->red,
	//		  foreGreen = cellForeColor->green,
	//		  foreBlue = cellForeColor->blue,

	//		  backRed = cellBackColor->red,
	//		  backGreen = cellBackColor->green,
	//		  backBlue = cellBackColor->blue,

	//		  foreRand, backRand;

	//	// brogueAssert(coordinatesAreInWindow(xLoc, yLoc));

	//	//if (rogue.gameHasEnded || rogue.playbackFastForward) {
	//	//	return;
	//	//}

	//	//assureCosmeticRNG;
	//	//oldRNG = rogue.RNG;
	//	//rogue.RNG = RNG_COSMETIC;

	//	foreRand = _randomGenerator->rand_range(0, cellForeColor->rand);
	//	backRand = _randomGenerator->rand_range(0, cellBackColor->rand);
	//	foreRed += _randomGenerator->rand_range(0, cellForeColor->redRand) + foreRand;
	//	foreGreen += _randomGenerator->rand_range(0, cellForeColor->greenRand) + foreRand;
	//	foreBlue += _randomGenerator->rand_range(0, cellForeColor->blueRand) + foreRand;
	//	backRed += _randomGenerator->rand_range(0, cellBackColor->redRand) + backRand;
	//	backGreen += _randomGenerator->rand_range(0, cellBackColor->greenRand) + backRand;
	//	backBlue += _randomGenerator->rand_range(0, cellBackColor->blueRand) + backRand;

	//	foreRed = min(100, max(0, foreRed));
	//	foreGreen = min(100, max(0, foreGreen));
	//	foreBlue = min(100, max(0, foreBlue));
	//	backRed = min(100, max(0, backRed));
	//	backGreen = min(100, max(0, backGreen));
	//	backBlue = min(100, max(0, backBlue));

	//	if (inputChar != ' '
	//		&& foreRed == backRed
	//		&& foreGreen == backGreen
	//		&& foreBlue == backBlue) {

	//		inputChar = ' ';
	//	}

	//	brogueCellDisplay display = _level->levelGrid->getCell(xLoc, yLoc)->display;

	//	if (inputChar != display.character
	//		|| foreRed != display.foreColor.red
	//		|| foreGreen != display.foreColor.green
	//		|| foreBlue != display.foreColor.blue
	//		|| backRed != display.backColor.red
	//		|| backGreen != display.backColor.green
	//		|| backBlue != display.backColor.blue) {

	//		display.needsUpdate = true;

	//		display.character = inputChar;
	//		display.foreColor.red = foreRed;
	//		display.foreColor.green = foreGreen;
	//		display.foreColor.blue = foreBlue;
	//		display.foreColor.red = backRed;
	//		display.foreColor.green = backGreen;
	//		display.foreColor.blue = backBlue;
	//	}
	//}

	//// Highlight the portion indicated by hiliteCharGrid with the hiliteColor at the hiliteStrength -- both latter arguments are optional.
	//void levelRenderer::hiliteGrid(color* hiliteColor, short hiliteStrength)
	//{
	//	short x, y;
	//	color hCol;

	//	if (hiliteColor) {
	//		hCol = *hiliteColor;
	//	}
	//	else {
	//		hCol = yellow;
	//	}

	//	bakeColor(&hCol);

	//	if (!hiliteStrength) {
	//		hiliteStrength = 75;
	//	}

	//	brogueGrid* grid = _level->levelGrid;

	//	iterate(_level->levelGrid->getBoundary(), [grid, &x, &y, &hCol, &hiliteStrength](short i, short j)
	//	{
	//		if (grid->isDefined(i, j))
	//		{
	//			x = mapToWindowX(i);
	//			y = mapToWindowY(j);

	//			brogueCellDisplay display = grid->getCell(i, j)->display;

	//			display.needsUpdate = true;
	//			display.backColor.red = clamp(display.backColor.red + hCol.red * hiliteStrength / 100, 0, 100);
	//			display.backColor.green = clamp(display.backColor.green + hCol.green * hiliteStrength / 100, 0, 100);
	//			display.backColor.blue = clamp(display.backColor.blue + hCol.blue * hiliteStrength / 100, 0, 100);
	//			display.foreColor.red = clamp(display.foreColor.red + hCol.red * hiliteStrength / 100, 0, 100);
	//			display.foreColor.green = clamp(display.foreColor.green + hCol.green * hiliteStrength / 100, 0, 100);
	//			display.foreColor.blue = clamp(display.foreColor.blue + hCol.blue * hiliteStrength / 100, 0, 100);
	//		}
	//	});
	//}

	//void levelRenderer::bakeColor(color* theColor) 
	//{
	//	short rand;
	//	rand = _randomGenerator->rand_range(0, theColor->rand);
	//	theColor->red += _randomGenerator->rand_range(0, theColor->redRand) + rand;
	//	theColor->green += _randomGenerator->rand_range(0, theColor->greenRand) + rand;
	//	theColor->blue += _randomGenerator->rand_range(0, theColor->blueRand) + rand;
	//	theColor->redRand = theColor->greenRand = theColor->blueRand = theColor->rand = 0;
	//}

	//void levelRenderer::displayMessageArchive()
	//{
		//short i, j, k, reverse, fadePercent, totalMessageCount, currentMessageCount;
		//boolean fastForward;

		////brogueCellDisplay dbuf[COLS][ROWS], rbuf[COLS][ROWS];

		////for (totalMessageCount = 0;
		////	totalMessageCount < MESSAGE_ARCHIVE_LINES && messageArchive[totalMessageCount][0];
		////	totalMessageCount++);

		//if (totalMessageCount > MESSAGE_LINES) 
		//{
		//	copyDisplayBuffer(rbuf, displayBuffer);

		//	// Pull-down/pull-up animation:
		//	for (reverse = 0; reverse <= 1; reverse++) {
		//		fastForward = false;
		//		for (currentMessageCount = (reverse ? totalMessageCount : MESSAGE_LINES);
		//			(reverse ? currentMessageCount >= MESSAGE_LINES : currentMessageCount <= totalMessageCount);
		//			currentMessageCount += (reverse ? -1 : 1)) {

		//			clearDisplayBuffer(dbuf);

		//			// Print the message archive text to the dbuf.
		//			for (j = 0; j < currentMessageCount && j < ROWS; j++) {
		//				printString(messageArchive[(messageArchivePosition - currentMessageCount + MESSAGE_ARCHIVE_LINES + j) % MESSAGE_ARCHIVE_LINES],
		//					mapToWindowX(0), j, &whiteBrogue, &black, dbuf);
		//			}

		//			// Set the dbuf opacity, and do a fade from bottom to top to make it clear that the bottom messages are the most recent.
		//			for (j = 0; j < currentMessageCount && j < ROWS; j++) {
		//				fadePercent = 50 * (j + totalMessageCount - currentMessageCount) / totalMessageCount + 50;
		//				for (i = 0; i < DCOLS; i++) {
		//					dbuf[mapToWindowX(i)][j].opacity = INTERFACE_OPACITY;
		//					if (dbuf[mapToWindowX(i)][j].character != ' ') {
		//						for (k = 0; k < 3; k++) {
		//							dbuf[mapToWindowX(i)][j].foreColorComponents[k] = dbuf[mapToWindowX(i)][j].foreColorComponents[k] * fadePercent / 100;
		//						}
		//					}
		//				}
		//			}

		//			// Display.
		//			overlayDisplayBuffer(rbuf, 0);
		//			overlayDisplayBuffer(dbuf, 0);

		//			if (!fastForward && pauseBrogue(reverse ? 1 : 2)) {
		//				fastForward = true;
		//				dequeueEvent();
		//				currentMessageCount = (reverse ? MESSAGE_LINES + 1 : totalMessageCount - 1); // skip to the end
		//			}
		//		}

		//		if (!reverse) {
		//			displayMoreSign();
		//		}
		//	}
		//	overlayDisplayBuffer(rbuf, 0);
		//	updateFlavorText();
		//	confirmMessages();
		//	updateMessageDisplay();
		//}
	//}

	// Clears the message area and prints the given message in the area.
	// It will disappear when messages are refreshed and will not be archived.
	// This is primarily used to display prompts.
	void temporaryMessage(char* msg, boolean requireAcknowledgment) 
	{
		//char message[COLS];
		//short i, j;

		//assureCosmeticRNG;
		//strcpy(message, msg);

		//for (i = 0; message[i] == COLOR_ESCAPE; i += 4) {
		//	upperCase(&(message[i]));
		//}

		//refreshSideBar(-1, -1, false);

		//for (i = 0; i < MESSAGE_LINES; i++) {
		//	for (j = 0; j < DCOLS; j++) {
		//		plotCharWithColor(' ', mapToWindowX(j), i, &black, &black);
		//	}
		//}
		//printString(message, mapToWindowX(0), mapToWindowY(-1), &whiteBrogue, &black, 0);
		//if (requireAcknowledgment) {
		//	waitForAcknowledgment();
		//	updateMessageDisplay();
		//}
		//restoreRNG;
	}

	//void flavorMessage(char* msg) 
	//{
	//	short i;
	//	char text[COLS * 20];

	//	for (i = 0; i < COLS * 2 && msg[i] != '\0'; i++) {
	//		text[i] = msg[i];
	//	}
	//	text[i] = '\0';

	//	for (i = 0; text[i] == COLOR_ESCAPE; i += 4);
	//	upperCase(&(text[i]));

	//	printString(text, mapToWindowX(0), ROWS - 2, &flavorTextColor, &black, 0);
	//	for (i = strLenWithoutEscapes(text); i < DCOLS; i++) {
	//		plotCharWithColor(' ', mapToWindowX(i), ROWS - 2, &black, &black);
	//	}
	//}

	//void messageWithoutCaps(char* msg, boolean requireAcknowledgment) 
	//{
	//	short i;
	//	if (!msg[0]) {
	//		return;
	//	}

	//	// need to confirm the oldest message? (Disabled!)
	//	/*if (!messageConfirmed[MESSAGE_LINES - 1]) {
	//		//refreshSideBar(-1, -1, false);
	//		displayMoreSign();
	//		for (i=0; i<MESSAGE_LINES; i++) {
	//			messageConfirmed[i] = true;
	//		}
	//	}*/

	//	for (i = MESSAGE_LINES - 1; i >= 1; i--) {
	//		messageConfirmed[i] = messageConfirmed[i - 1];
	//		strcpy(displayedMessage[i], displayedMessage[i - 1]);
	//	}
	//	messageConfirmed[0] = false;
	//	strcpy(displayedMessage[0], msg);

	//	// Add the message to the archive.
	//	strcpy(messageArchive[messageArchivePosition], msg);
	//	messageArchivePosition = (messageArchivePosition + 1) % MESSAGE_ARCHIVE_LINES;

	//	// display the message:
	//	updateMessageDisplay();

	//	if (requireAcknowledgment || rogue.cautiousMode) {
	//		displayMoreSign();
	//		confirmMessages();
	//		rogue.cautiousMode = false;
	//	}

	//	if (rogue.playbackMode) {
	//		rogue.playbackDelayThisTurn += rogue.playbackDelayPerTurn * 5;
	//	}
	//}


	//void message(const char* msg, boolean requireAcknowledgment) 
	//{
	//	char text[COLS * 20], * msgPtr;
	//	short i, lines;

	//	assureCosmeticRNG;

	//	rogue.disturbed = true;
	//	if (requireAcknowledgment) {
	//		refreshSideBar(-1, -1, false);
	//	}
	//	displayCombatText();

	//	lines = wrapText(text, msg, DCOLS);
	//	msgPtr = &(text[0]);

	//	// Implement the American quotation mark/period/comma ordering rule.
	//	for (i = 0; text[i] != '\0' && text[i + 1] != '\0'; i++) {
	//		if (text[i] == COLOR_ESCAPE) {
	//			i += 4;
	//		}
	//		else if (text[i] == '"'
	//			&& (text[i + 1] == '.' || text[i + 1] == ',')) {

	//			text[i] = text[i + 1];
	//			text[i + 1] = '"';
	//		}
	//	}

	//	for (i = 0; text[i] == COLOR_ESCAPE; i += 4);
	//	upperCase(&(text[i]));

	//	if (lines > 1) {
	//		for (i = 0; text[i] != '\0'; i++) {
	//			if (text[i] == '\n') {
	//				text[i] = '\0';

	//				messageWithoutCaps(msgPtr, false);
	//				msgPtr = &(text[i + 1]);
	//			}
	//		}
	//	}

	//	messageWithoutCaps(msgPtr, requireAcknowledgment);
	//	restoreRNG;
	//}

	//// Only used for the "you die..." message, to enable posthumous inventory viewing.
	//void displayMoreSignWithoutWaitingForAcknowledgment() {
	//	if (strLenWithoutEscapes(displayedMessage[0]) < DCOLS - 8 || messageConfirmed[0]) {
	//		printString("--MORE--", COLS - 8, MESSAGE_LINES - 1, &black, &whiteBrogue, 0);
	//	}
	//	else {
	//		printString("--MORE--", COLS - 8, MESSAGE_LINES, &black, &whiteBrogue, 0);
	//	}
	//}

	//void displayMoreSign() {
	//	short i;

	//	if (rogue.autoPlayingLevel) {
	//		return;
	//	}

	//	if (strLenWithoutEscapes(displayedMessage[0]) < DCOLS - 8 || messageConfirmed[0]) {
	//		printString("--MORE--", COLS - 8, MESSAGE_LINES - 1, &black, &whiteBrogue, 0);
	//		waitForAcknowledgment();
	//		printString("        ", COLS - 8, MESSAGE_LINES - 1, &black, &black, 0);
	//	}
	//	else {
	//		printString("--MORE--", COLS - 8, MESSAGE_LINES, &black, &whiteBrogue, 0);
	//		waitForAcknowledgment();
	//		for (i = 1; i <= 8; i++) {
	//			refreshDungeonCell(DCOLS - i, 0);
	//		}
	//	}
	//}

	//// Inserts a four-character color escape sequence into a string at the insertion point.
	//// Does NOT check string lengths, so it could theoretically write over the null terminator.
	//// Returns the new insertion point.
	//short levelRenderer::encodeMessageColor(char* msg, short i, const color* theColor)
	//{
	//	//boolean needTerminator = false;
	//	//color col(*theColor);

	//	//bakeColor(&col);

	//	//col.red = clamp(col.red, 0, 100);
	//	//col.green = clamp(col.green, 0, 100);
	//	//col.blue = clamp(col.blue, 0, 100);

	//	//needTerminator = !msg[i] || !msg[i + 1] || !msg[i + 2] || !msg[i + 3];

	//	//msg[i++] = COLOR_ESCAPE;
	//	//msg[i++] = (char)(COLOR_VALUE_INTERCEPT + col.red);
	//	//msg[i++] = (char)(COLOR_VALUE_INTERCEPT + col.green);
	//	//msg[i++] = (char)(COLOR_VALUE_INTERCEPT + col.blue);

	//	//if (needTerminator) {
	//	//	msg[i] = '\0';
	//	//}

	//	//return i;

	//	return -1;
	//}

	//// Call this when the i'th character of msg is COLOR_ESCAPE.
	//// It will return the encoded color, and will advance i past the color escape sequence.
	//short decodeMessageColor(const char* msg, short i, color* returnColor) {

	//	if (msg[i] != COLOR_ESCAPE) {
	//		printf("\nAsked to decode a color escape that didn't exist!");
	//		*returnColor = whiteBrogue;
	//	}
	//	else {
	//		i++;
	//		*returnColor = black;
	//		returnColor->red = (short)(msg[i++] - COLOR_VALUE_INTERCEPT);
	//		returnColor->green = (short)(msg[i++] - COLOR_VALUE_INTERCEPT);
	//		returnColor->blue = (short)(msg[i++] - COLOR_VALUE_INTERCEPT);

	//		returnColor->red = clamp(returnColor->red, 0, 100);
	//		returnColor->green = clamp(returnColor->green, 0, 100);
	//		returnColor->blue = clamp(returnColor->blue, 0, 100);
	//	}
	//	return i;
	//}

	//void updateMessageDisplay() {
	//	short i, j, m;
	//	color messageColor;

	//	for (i = 0; i < MESSAGE_LINES; i++) {
	//		messageColor = whiteBrogue;

	//		if (messageConfirmed[i]) {
	//			applyColorAverage(&messageColor, &black, 50);
	//			applyColorAverage(&messageColor, &black, 75 * i / MESSAGE_LINES);
	//		}

	//		for (j = m = 0; displayedMessage[i][m] && j < DCOLS; j++, m++) {

	//			while (displayedMessage[i][m] == COLOR_ESCAPE) {
	//				m = decodeMessageColor(displayedMessage[i], m, &messageColor); // pulls the message color out and advances m
	//				if (messageConfirmed[i]) {
	//					applyColorAverage(&messageColor, &black, 50);
	//					applyColorAverage(&messageColor, &black, 75 * i / MESSAGE_LINES);
	//				}
	//			}

	//			plotCharWithColor(displayedMessage[i][m], mapToWindowX(j), MESSAGE_LINES - i - 1,
	//				&messageColor,
	//				&black);
	//		}
	//		for (; j < DCOLS; j++) {
	//			plotCharWithColor(' ', mapToWindowX(j), MESSAGE_LINES - i - 1, &black, &black);
	//		}
	//	}
	//}

	

	//// Does NOT clear the message archive.
	//void deleteMessages() {
	//	short i;
	//	for (i = 0; i < MESSAGE_LINES; i++) {
	//		displayedMessage[i][0] = '\0';
	//	}
	//	confirmMessages();
	//}

	//void confirmMessages() {
	//	short i;
	//	for (i = 0; i < MESSAGE_LINES; i++) {
	//		messageConfirmed[i] = true;
	//	}
	//	updateMessageDisplay();
	//}
}
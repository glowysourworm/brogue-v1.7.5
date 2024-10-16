#pragma once

#include "brogueGlobal.h"
#include "openglRenderer.h"
#include "gameConstants.h"
#include "resourceController.h"
#include "randomGenerator.h"
#include "brogueFlameMenu.h"
#include "brogueSceneBuilder.h"
#include "brogueFlameMenuProgram.h"
#include <functional>
#include <stdlib.h>

using namespace brogueHd::backend::controller;
using namespace brogueHd::frontend::opengl;

namespace brogueHd::backend::controller
{
	class renderingController
	{
	public:

		renderingController(resourceController* resourceController, randomGenerator* randomGenerator);
		~renderingController();

		/// <summary>
		/// Sets the video output to the specific view
		/// </summary>
		void setViewMode(BrogueGameMode mode);

	private:

		brogueFlameMenuProgram* createMenuView();
		brogueProgram* createHighScoresView();
		brogueProgram* createSetSeedView();
		brogueProgram* createOpenGameView();
		brogueProgram* createGameView();

	private:

		BrogueGameMode _mode;

		randomGenerator* _randomGenerator;
		resourceController* _resourceController;
		openglRenderer* _openglRenderer;

	};

	renderingController::renderingController(resourceController* resourceController, randomGenerator* randomGenerator)
	{
		_openglRenderer = new openglRenderer();
		_resourceController = resourceController;
		_randomGenerator = randomGenerator;
		_mode = BrogueGameMode::Menu;
	}
	renderingController::~renderingController()
	{
		delete _openglRenderer;
	}

	void renderingController::setViewMode(BrogueGameMode mode)
	{
		_mode = mode;

		if (!_openglRenderer->isInitializedGL())
			_openglRenderer->initializeOpenGL();

		switch (_mode)
		{
		case BrogueGameMode::Menu:
		case BrogueGameMode::MenuHighScores:
		case BrogueGameMode::MenuOpenGame:
		case BrogueGameMode::MenuSetSeed:
		case BrogueGameMode::Game:
		case BrogueGameMode::Playback:
		{
			// Shuts down thread, deletes our program memory
			_openglRenderer->terminateProgram();
			
			brogueFlameMenuProgram* program = createMenuView();

			_openglRenderer->setProgram((brogueProgram*)program);
			_openglRenderer->startProgram();
		}
		break;
		case BrogueGameMode::Scum:
		case BrogueGameMode::Quit:
		{
			_openglRenderer->terminateProgram();
		}
		break;
		default:
			break;
		}
	}

	brogueFlameMenuProgram* renderingController::createMenuView()
	{
		// Prepare pieces of the render program
		brogueFlameMenu* mainMenu = new brogueFlameMenu(_randomGenerator, 0, 1, 1, 1, 1, 1, 1);

		// Calculate converted scene boundary
		gridRect sceneBoundaryUI = brogueSceneBuilder::calculateSceneBoundary(mainMenu);

		// Calculate scene's GL data stream
		simpleDataStream<float>* sceneDataStream = brogueSceneBuilder::prepareSceneDataStream(mainMenu);
		simpleDataStream<float>* frameDataStream = brogueSceneBuilder::prepareFrameDataStream(mainMenu);
		simpleDataStream<float>* frameBlendDataStream = brogueSceneBuilder::prepareFrameDataStream(mainMenu);

		// Read shaders from the resource cache
		shaderData vertexShaderData = _resourceController->getShader(shaderResource::brogueFlameMenuVert);
		shaderData fragmentShaderData = _resourceController->getShader(shaderResource::brogueFlameMenuFrag);
		shaderData frameVertexShaderData = _resourceController->getShader(shaderResource::brogueFrameVert);
		shaderData frameFragmentShaderData = _resourceController->getShader(shaderResource::brogueFrameFrag);
		shaderData frameBlendVertexShaderData = _resourceController->getShader(shaderResource::brogueFrameBlendVert);
		shaderData frameBlendFragmentShaderData = _resourceController->getShader(shaderResource::brogueFrameBlendFrag);

		// (MEMORY!) Create view for the renderer as shader program
		simpleShaderProgram* sceneProgram = brogueSceneBuilder::createSceneShaderProgram(sceneDataStream, vertexShaderData, fragmentShaderData);
		simpleShaderProgram* frameProgram = brogueSceneBuilder::createFrameShaderProgram(frameDataStream, frameVertexShaderData, frameFragmentShaderData);
		simpleShaderProgram* frameBlendProgram = brogueSceneBuilder::createSceneShaderProgram(frameBlendDataStream, frameBlendVertexShaderData, frameBlendFragmentShaderData);

		int textureIndex = 0;

		simpleFrameBuffer* frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);
		simpleTexture frameTexture(NULL, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_BGRA, GL_UNSIGNED_BYTE);
		simpleTexture frameBlendTexture(NULL, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_BGRA, GL_UNSIGNED_BYTE);

		// THESE ARE STATICALLY DEFINED:  They depend on the shader file.
		//
		simpleUniform<int> frameFragUniform(frameFragmentShaderData.uniforms1i->get(0));
		simpleUniform<int> frameBlendFragUniform(frameBlendFragmentShaderData.uniforms1i->get(0));

		return new brogueFlameMenuProgram(sceneProgram, 
										  frameProgram, 
										  frameBlendProgram, 
										  frameBuffer,
										  mainMenu,
										  frameTexture,
										  frameBlendTexture, 
										  frameFragUniform, 
										  frameBlendFragUniform,
										  sceneBoundaryUI);

		//signed short flames[COLS][(ROWS + MENU_FLAME_ROW_PADDING)][3]; // red, green and blue
		//signed short colorSources[MENU_FLAME_COLOR_SOURCE_COUNT][4]; // red, green, blue, and rand, one for each color source (no more than MENU_FLAME_COLOR_SOURCE_COUNT).
		//const color* colors[COLS][(ROWS + MENU_FLAME_ROW_PADDING)];
		//color colorStorage[COLS];
		//unsigned char mask[COLS][ROWS];
		//boolean controlKeyWasDown = false;

		//short i, b, x, y, button;
		//buttonState state;
		//brogueButton buttons[6];
		//char whiteColorEscape[10] = "";
		//char goldColorEscape[10] = "";
		//char newGameText[100] = "", customNewGameText[100] = "";
		//rogueEvent theEvent;
		//enum NGCommands buttonCommands[6] = { NG_NEW_GAME, NG_OPEN_GAME, NG_VIEW_RECORDING, NG_HIGH_SCORES, NG_QUIT };

		//cellDisplayBuffer shadowBuf[COLS][ROWS];

		// Initialize the RNG so the flames aren't always the same.

		//seedRandomGenerator(0);

		// Empty nextGamePath and nextGameSeed so that the buttons don't try to load an old game path or seed.
		//rogue.nextGamePath[0] = '\0';
		//rogue.nextGameSeed = 0;

		// Initialize the title menu buttons.
		//encodeMessageColor(whiteColorEscape, 0, &whiteBrogue);
		//encodeMessageColor(goldColorEscape, 0, KEYBOARD_LABELS ? &itemMessageColor : &whiteBrogue);
		//sprintf(newGameText, "      %sN%sew Game      ", goldColorEscape, whiteColorEscape);
		//sprintf(customNewGameText, " %sN%sew Game (custom) ", goldColorEscape, whiteColorEscape);
		//b = 0;
		//button = -1;

		//initializeButton(&(buttons[b]));
		//strcpy(buttons[b].text, newGameText);
		//buttons[b].hotkey[0] = 'n';
		//buttons[b].hotkey[1] = 'N';
		//b++;

		//initializeButton(&(buttons[b]));
		//sprintf(buttons[b].text, "     %sO%spen Game      ", goldColorEscape, whiteColorEscape);
		//buttons[b].hotkey[0] = 'o';
		//buttons[b].hotkey[1] = 'O';
		//b++;

		//initializeButton(&(buttons[b]));
		//sprintf(buttons[b].text, "   %sV%siew Recording   ", goldColorEscape, whiteColorEscape);
		//buttons[b].hotkey[0] = 'v';
		//buttons[b].hotkey[1] = 'V';
		//b++;

		//initializeButton(&(buttons[b]));
		//sprintf(buttons[b].text, "    %sH%sigh Scores     ", goldColorEscape, whiteColorEscape);
		//buttons[b].hotkey[0] = 'h';
		//buttons[b].hotkey[1] = 'H';
		//b++;

		//initializeButton(&(buttons[b]));
		//sprintf(buttons[b].text, "        %sQ%suit        ", goldColorEscape, whiteColorEscape);
		//buttons[b].hotkey[0] = 'q';
		//buttons[b].hotkey[1] = 'Q';
		//b++;

		//x = COLS - 1 - 20 - 2;
		//y = ROWS - 1;
		//for (i = b - 1; i >= 0; i--) {
		//	y -= 2;
		//	buttons[i].x = x;
		//	buttons[i].y = y;
		//	buttons[i].buttonColor = titleButtonColor;
		//	buttons[i].flags |= B_WIDE_CLICK_AREA;
		//}

		//blackOutScreen();
		//clearDisplayBuffer(shadowBuf);
		//initializeButtonState(&state, buttons, b, x, y, 20, b * 2 - 1);
		//rectangularShading(x, y, 20, b * 2 - 1, &black, INTERFACE_OPACITY, shadowBuf);
		//drawButtonsInState(&state);

		//initializeMenuFlames(true, colors, colorStorage, colorSources, flames, mask);
		//rogue.creaturesWillFlashThisTurn = false; // total unconscionable hack

		//do {
		//	if (isApplicationActive()) {
		//		 Revert the display.
		//		overlayDisplayBuffer(state.rbuf, NULL);

		//		if (!controlKeyWasDown && controlKeyIsDown()) {
		//			strcpy(state.buttons[0].text, customNewGameText);
		//			drawButtonsInState(&state);
		//			buttonCommands[0] = NG_NEW_GAME_WITH_SEED;
		//			controlKeyWasDown = true;
		//		}
		//		else if (controlKeyWasDown && !controlKeyIsDown()) {
		//			strcpy(state.buttons[0].text, newGameText);
		//			drawButtonsInState(&state);
		//			buttonCommands[0] = NG_NEW_GAME;
		//			controlKeyWasDown = false;
		//		}

		//		 Update the display.
		//		updateMenuFlames(colors, colorSources, flames);
		//		drawMenuFlames(flames, mask);
		//		overlayDisplayBuffer(shadowBuf, NULL);
		//		overlayDisplayBuffer(state.dbuf, NULL);

		//		 Pause briefly.
		//		if (pauseBrogue(MENU_FLAME_UPDATE_DELAY)) {
		//			 There was input during the pause! Get the input.
		//			nextBrogueEvent(&theEvent, true, false, true);

		//			 Process the input.
		//			button = processButtonInput(&state, NULL, &theEvent);
		//		}

		//	}
		//	else {
		//		pauseBrogue(64);
		//	}
		//} while (button == -1 && rogue.nextGame == NG_NOTHING);
		//drawMenuFlames(flames, mask);
		//if (button != -1) {
		//	if (button == 0 && controlKeyIsDown()) {
		//		 Should fix an issue with Linux/Windows ports that require moving the mouse after
		//		 pressing control to get the button to change.
		//		rogue.nextGame = NG_NEW_GAME_WITH_SEED;
		//	}
		//	else {
		//		rogue.nextGame = buttonCommands[button];
		//	}
		//}
	}
	brogueProgram* renderingController::createHighScoresView()
	{
		return nullptr;
	}
	brogueProgram* renderingController::createOpenGameView()
	{
		return nullptr;
	}
	brogueProgram* renderingController::createGameView()
	{
		return nullptr;
	}
	brogueProgram* renderingController::createSetSeedView()
	{
		return nullptr;
		//// Prompt for seed; default is the previous game's seed.
		//sprintf(maxSeed, "%lu", ULONG_MAX);

		//if (previousGameSeed == 0)
		//{
		//	seedDefault[0] = '\0';
		//}
		//else
		//{
		//	sprintf(seedDefault, "%lu", previousGameSeed);
		//}
		//if (getInputTextString(buf, "Generate dungeon with seed number:",
		//	log10(ULONG_MAX) + 1,
		//	seedDefault,
		//	"",
		//	TEXT_INPUT_NUMBERS,
		//	true)
		//	&& buf[0] != '\0') {
		//	seedTooBig = false;
		//	if (strlen(buf) > strlen(maxSeed)) {
		//		seedTooBig = true;
		//	}
		//	else if (strlen(buf) == strlen(maxSeed)) {
		//		for (i = 0; maxSeed[i]; i++) {
		//			if (maxSeed[i] > buf[i]) {
		//				break; // we're good
		//			}
		//			else if (maxSeed[i] < buf[i]) {
		//				seedTooBig = true;
		//				break;
		//			}
		//		}
		//	}
		//	if (seedTooBig) {
		//		rogue.nextGameSeed = ULONG_MAX;
		//	}
		//	else {
		//		sscanf(buf, "%lu", &rogue.nextGameSeed);
		//	}
		//}
		//else {
		//	rogue.nextGame = NG_NOTHING;
		//	break; // Don't start a new game after all.
		//}
	}
}


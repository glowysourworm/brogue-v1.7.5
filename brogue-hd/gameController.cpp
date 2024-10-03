#include "gameController.h"
#include "gameData.h"
#include "keyProcessor.h"
#include "randomGenerator.h"
#include "broguefile.h"
#include "brogueGlobal.h"

#include <time.h>
#include <fstream>

#include <GLFW/glfw3.h>

using namespace std;

using namespace brogueHd::backend::model::game;

namespace brogueHd::backend::controller
{
	gameController::gameController()
	{
		_gameData = NULL;
		_keyProcessor = new keyProcessor();
		_playbackProcessor = new playbackProcessor();
		_randomMain = new randomGenerator(RANDOM_GENERATOR_MAIN);
		_randomCosmetic = new randomGenerator(RANDOM_GENERATOR_COSMETIC);
		_renderingController = new renderingController();
	}

	gameController::~gameController()
	{
		if (_gameData != NULL)
		{
			delete _gameData;

			_gameData = NULL;
		}

		delete _keyProcessor;
		delete _randomMain;
		delete _randomCosmetic;
		delete _playbackProcessor;
		delete _renderingController;
	}

	void gameController::loadKeymap()
	{

	}

	void gameController::setMode(BrogueGameMode gameMode)
	{
		_gameMode = gameMode;

		//if (SDL_Init(SDL_INIT_VIDEO)) 
		//{
		//	printf("Could not start SDL.\n");
		//	return;
		//}

		//loadFont(true);
		//rogueMain();

		//TCOD_console_delete(NULL);

		//rogueEvent theEvent;
		//char path[BROGUE_FILENAME_MAX], buf[100], seedDefault[100];
		//char maxSeed[40];
		//short i, j, k;
		//boolean seedTooBig;

		// (RENDERING CODE)
		// 
		/*
		// clear screen and display buffer
		for (i = 0; i < COLS; i++)
		{
			for (j = 0; j < ROWS; j++)
			{
				displayBuffer[i][j].character = 0;
				displayBuffer[i][j].needsUpdate = false;
				displayBuffer[i][j].opacity = 100;

				for (k = 0; k < 3; k++)
				{
					displayBuffer[i][j].foreColorComponents[k] = 0;
					displayBuffer[i][j].backColorComponents[k] = 0;
				}
				plotCharWithColor(' ', i, j, &black, &black);
			}
		}*/



	}

	void gameController::closeGame()
	{
		//if (_gameData != NULL)
		//{ 
		//	delete _gameData;
		//	_gameData = NULL;
		//}

		gameController::closeWindow();
	}

	void gameController::initNewGame(unsigned long seed)
	{
		//if (_gameData != NULL)
		//	brogueException::show("Trying to initialize game while a current one is loaded:  call closeGame() first");

		//_randomMain->reset(seed);

		//// Reset Game Data
		//if (_gameData != NULL)
		//{
		//	delete _gameData;
		//}

		//_gameData = new gameData();

		gameController::openWindow();
	}

	void gameController::renderWindow()
	{
		// RENDERING CODE:  Using static renderingController
	}

	void gameController::closeWindow()
	{
		glfwDestroyWindow(_gameWindow);
		glfwTerminate();
	}

	/// <summary>
	/// Opens game window for rendering target
	/// </summary>
	void gameController::openWindow()
	{
		if (_gameWindow != NULL)
			brogueException::show("Trying to reopen game window:  gameController.cpp");

		if (glfwInit() == GLFW_TRUE)
		{
			glfwSetErrorCallback([](int error, const char* messasge)
			{
				// log error
			});

			_gameWindow = glfwCreateWindow(640, 480, "My Title", NULL, NULL);

			glfwMakeContextCurrent(_gameWindow);

			// GLFW Event for user wanting to close the window
			//
			// https://www.glfw.org/docs/latest/quick.html#quick_steps


			//while (!glfwWindowShouldClose(window))
			//{
			//	// Keep running
			//}

			// Also:   glfwSetWindowCloseCallback

			/*
				static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
				{
					if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
						glfwSetWindowShouldClose(window, GLFW_TRUE);
				}

				glfwSetKeyCallback(window, key_callback);
			*/

			/*
				OpenGL Rendering

				int width, height;
				glfwGetFramebufferSize(window, &width, &height);
				glViewport(0, 0, width, height);
				glfwSetFramebufferSizeCallback
			*/

		}
		else
		{
			// error
		}

		//glutInit(0, NULL);
		//glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
		//glutInitWindowSize(1024, 768);
		//glutInitWindowPosition(100, 100);
		//glutCreateWindow("Brogue Game Window");
		//glutDisplayFunc(&gameController::renderWindow);
		//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		//glutMainLoop();
		//glClear(GL_COLOR_BUFFER_BIT);
		//glutSwapBuffers();
	}

	void gameController::initGame(gameData* data)
	{
		if (_gameData != NULL)
			brogueException::show("Trying to initialize game while a current one is loaded:  call closeGame() first");

		// unsigned long gameSeed (GET FROM GAME DATA)

		_randomMain->reset(0);

		// Reset Game Data
		if (_gameData != NULL)
		{
			delete _gameData;
		}

		_gameData = data;

		//if (rogue.nextGamePath[0])
		//{
		//	strcpy(path, rogue.nextGamePath);
		//	rogue.nextGamePath[0] = '\0';
		//}
		//else {
		//	dialogChooseFile(path, GAME_SUFFIX, "Open saved game:");
		//	//chooseFile(path, "Open saved game: ", "Saved game", GAME_SUFFIX);
		//}

		//if (openFile(path)) {
		//	loadSavedGame();
		//	mainInputLoop();
		//	freeEverything();
		//}
		//else {
		//	//dialogAlert("File not found.");
		//}
		//rogue.playbackMode = false;
		//rogue.playbackOOS = false;
	}

	void gameController::initPlayback(const char* recordingPath)
	{
		if (_gameData != NULL)
			brogueException::show("Trying to initialize playback while a current one is loaded:  call closeGame() first");

		if (recordingPath == NULL)
			brogueException::show("Recording path not specified");

		try
		{
			_playbackProcessor->initialize(recordingPath);
		}
		catch (std::exception ex)
		{
			throw ex;
		}

		//	// Procedure
		//	//
		//	// 1) Load BrogueEvent list (keystrokes)
		//	// 2) Prepare display data in advance for processing
		//	//
		//	

		//	randomNumbersGenerated = 0;
		//	rogue.playbackMode = true;
		//	initializeRogue(0); // Seed argument is ignored because we're in playback.
		//	if (!rogue.gameHasEnded) {
		//		startLevel(rogue.depthLevel, 1);
		//		rogue.playbackPaused = true;
		//		displayAnnotation(); // in case there's an annotation for turn 0
		//	}

		//	while (!rogue.gameHasEnded && rogue.playbackMode) {
		//		if (rogue.playbackPaused) {
		//			rogue.playbackPaused = false;
		//			pausePlayback();
		//		}

		//		rogue.RNG = RNG_COSMETIC; // dancing terrain colors can't influence recordings
		//		rogue.playbackBetweenTurns = true;
		//		nextBrogueEvent(&theEvent, false, true, false);
		//		rogue.RNG = RNG_SUBSTANTIVE;

		//		executeEvent(&theEvent);
		//	}

		//	freeEverything();
		//}

		////dialogChooseFile(path, RECORDING_SUFFIX, "View recording:");
		////chooseFile(path, "View recording: ", "Recording", RECORDING_SUFFIX);

		//if (openFile(path)) 
		//{

		//}
		//else 
		//{
		//	// announce file not found
		//}
		////rogue.playbackMode = false;
		////rogue.playbackOOS = false;
	}

	bool gameController::runMenu()
	{
		return false;
	}

	bool gameController::runGame()
	{
		return false;
	}

	bool gameController::runSetSeed()
	{
		return false;
	}

	bool gameController::runOpenGame()
	{
		//if (rogue.nextGamePath[0]) 
		//{
		//	strcpy(path, rogue.nextGamePath);
		//	rogue.nextGamePath[0] = '\0';
		//}
		//else {
		//	dialogChooseFile(path, GAME_SUFFIX, "Open saved game:");
		//	//chooseFile(path, "Open saved game: ", "Saved game", GAME_SUFFIX);
		//}

		//if (openFile(path)) {
		//	loadSavedGame();
		//	mainInputLoop();
		//	freeEverything();
		//}
		//else {
		//	//dialogAlert("File not found.");
		//}
		//rogue.playbackMode = false;
		//rogue.playbackOOS = false;

		return false;
	}

	bool gameController::runPlayback()
	{
		//rogue.nextGame = NG_NOTHING;

		//path[0] = '\0';
		//if (rogue.nextGamePath[0]) {
		//	strcpy(path, rogue.nextGamePath);
		//	rogue.nextGamePath[0] = '\0';
		//}
		//else {
		//	dialogChooseFile(path, RECORDING_SUFFIX, "View recording:");
		//	//chooseFile(path, "View recording: ", "Recording", RECORDING_SUFFIX);
		//}

		//if (openFile(path)) {
		//	randomNumbersGenerated = 0;
		//	rogue.playbackMode = true;
		//	initializeRogue(0); // Seed argument is ignored because we're in playback.
		//	if (!rogue.gameHasEnded) {
		//		startLevel(rogue.depthLevel, 1);
		//		rogue.playbackPaused = true;
		//		displayAnnotation(); // in case there's an annotation for turn 0
		//	}

		//	while (!rogue.gameHasEnded && rogue.playbackMode) {
		//		if (rogue.playbackPaused) {
		//			rogue.playbackPaused = false;
		//			pausePlayback();
		//		}

		//		rogue.RNG = RNG_COSMETIC; // dancing terrain colors can't influence recordings
		//		rogue.playbackBetweenTurns = true;
		//		nextBrogueEvent(&theEvent, false, true, false);
		//		rogue.RNG = RNG_SUBSTANTIVE;

		//		executeEvent(&theEvent);
		//	}

		//	freeEverything();
		//}
		//else {
		//	// announce file not found
		//}
		//rogue.playbackMode = false;
		//rogue.playbackOOS = false;

		return false;
	}

	BrogueGameMode gameController::getMode()
	{
		return BrogueGameMode::Game;
	}

	bool gameController::run()
	{
		switch (_gameMode)
		{
		case BrogueGameMode::Menu:
			break;
		case BrogueGameMode::MenuHighScores:
			break;
		case BrogueGameMode::MenuOpenGame:
			break;
		case BrogueGameMode::MenuSetSeed:
			break;
		case BrogueGameMode::Game:
			break;
		case BrogueGameMode::Playback:
			break;
		case BrogueGameMode::Scum:
			break;
		case BrogueGameMode::Quit:
			break;
		default:
			break;
		}

		return true;
	}

	bool gameController::runHighScores()
	{
		//rogue.nextGame = NG_NOTHING;
		//printHighScores(false);

		return false;
	}

	bool gameController::runScum()
	{
		//rogue.nextGame = NG_NOTHING;
		//scum(1, 1000, 5);

		return false;
	}
}
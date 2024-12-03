#pragma once

#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueLevel.h"
#include "brogueMainProgram.h"
#include "brogueMouseState.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueViewProgram.h"
#include "eventController.h"
#include "gridRect.h"
#include "openglRenderer.h"
#include "randomGenerator.h"
#include "resourceController.h"
#include "simpleList.h"
#include "simpleString.h"

namespace brogueHd::backend
{
	using namespace brogueHd::frontend;

	class renderingController
	{
	public:
		renderingController(eventController* eventController, resourceController* resourceController,
		                    randomGenerator* randomGenerator, int zoomLevel);
		~renderingController();

		/// <summary>
		/// Sets the video output to the specific view
		/// </summary>
		void setGameModeAsync(BrogueGameMode mode);

		/// <summary>
		/// Creates program containers with active UI components
		/// </summary>
		void initialize();

		brogueKeyboardState getKeyboardState() const;
		brogueMouseState getMouseState() const;
		BrogueGameMode getGameModeRequest(bool& newGame, bool& openGame, simpleString& fileName) const;
		BrogueGameMode getRenderingGameMode() const;

		/// <summary>
		/// Primary rendering data hand-over to the rendering thread
		/// </summary>
		void updateGameData(const brogueLevel* level);

	private:
		brogueMainProgram* _mainProgram;
		brogueUIBuilder* _uiBuilder;
		randomGenerator* _randomGenerator;
		eventController* _eventController;
		resourceController* _resourceController;
		openglRenderer* _openglRenderer;
		brogueGlyphMap* _glyphMap;
	};

	renderingController::renderingController(eventController* eventController, resourceController* resourceController,
	                                         randomGenerator* randomGenerator, int zoomLevel)
	{
		_glyphMap = new brogueGlyphMap();
		_openglRenderer = new openglRenderer(eventController);
		_resourceController = resourceController;
		_eventController = eventController;
		_randomGenerator = randomGenerator;
		_uiBuilder = new brogueUIBuilder(eventController, resourceController, randomGenerator, zoomLevel);

		// Title Screen:  Build program parts, load the container

		brogueViewProgram* flameView = _uiBuilder->buildProgramView(brogueUIProgram::FlameMenuProgram);
		brogueViewProgram* titleMask = _uiBuilder->buildProgramView(brogueUIProgram::FlameMenuTitleMaskProgram);
		brogueViewProgram* mainMenu = _uiBuilder->buildProgramView(brogueUIProgram::MainMenuProgram);
		brogueViewProgram* openMenu = _uiBuilder->buildProgramView(brogueUIProgram::OpenMenuProgram);
		brogueViewProgram* playbackMenu = _uiBuilder->buildProgramView(brogueUIProgram::PlaybackMenuProgram);
		brogueViewProgram* highScoresMenu = _uiBuilder->buildProgramView(brogueUIProgram::HighScoresProgram);

		// Game Screen:  Build program parts

		brogueViewProgram* gameView = _uiBuilder->buildProgramView(brogueUIProgram::GameProgram);
		brogueViewProgram* gameMenu = _uiBuilder->buildProgramView(brogueUIProgram::GameMenuProgram);
		brogueViewProgram* gameLog = _uiBuilder->buildProgramView(brogueUIProgram::GameLogProgram);
		brogueViewProgram* gameInventory = _uiBuilder->buildProgramView(brogueUIProgram::GameInventoryProgram);
		brogueViewProgram* gameObjectList = _uiBuilder->buildProgramView(brogueUIProgram::GameObjectListProgram);
		brogueViewProgram* flavorTextPanel = _uiBuilder->buildProgramView(brogueUIProgram::FlavorTextPanelProgram);
		brogueViewProgram* bottomMenuBar = _uiBuilder->buildProgramView(brogueUIProgram::BottomBarMenuProgram);

		simpleList<brogueViewProgram*> viewList;

		// Title Views (Use Reverse Z-Ordering to check UI events in that order)
		viewList.add(mainMenu); // Largest Z-Index
		viewList.add(openMenu);
		viewList.add(playbackMenu);
		viewList.add(highScoresMenu);
		viewList.add(flameView);
		viewList.add(titleMask);

		// Game Views
		viewList.add(gameLog); // Largest Z-Index
		viewList.add(gameInventory);
		viewList.add(gameMenu);
		viewList.add(gameView);
		viewList.add(gameObjectList);
		viewList.add(flavorTextPanel);
		viewList.add(bottomMenuBar);

		gridRect sceneBoundaryUI = _uiBuilder->getBrogueSceneBoundaryUI();

		_mainProgram = new brogueMainProgram(_uiBuilder, resourceController, eventController, _glyphMap,
		                                     sceneBoundaryUI, zoomLevel, viewList);
	}

	renderingController::~renderingController()
	{
		delete _openglRenderer;
		delete _glyphMap;
		delete _mainProgram;
		delete _uiBuilder;
	}

	void renderingController::initialize()
	{
		_openglRenderer->setProgram(_mainProgram, BrogueGameMode::Title);
		_openglRenderer->startProgram();
	}

	brogueKeyboardState renderingController::getKeyboardState() const
	{
		return _openglRenderer->getKeyboardState();
	}

	brogueMouseState renderingController::getMouseState() const
	{
		return _openglRenderer->getMouseState();
	}

	BrogueGameMode renderingController::getGameModeRequest(bool& newGame, bool& openGame, simpleString& fileName) const
	{
		return _openglRenderer->getRequestedMode(newGame, openGame, fileName);
	}

	void renderingController::updateGameData(const brogueLevel* level)
	{
		_openglRenderer->setGameData(level);
	}

	BrogueGameMode renderingController::getRenderingGameMode() const
	{
		return _openglRenderer->getRenderingMode();
	}

	void renderingController::setGameModeAsync(BrogueGameMode mode)
	{
		switch (mode)
		{
		case BrogueGameMode::Title:
		case BrogueGameMode::Game:
		case BrogueGameMode::Playback:
			_openglRenderer->setGameMode(mode);
			break;
		default:
			break;
		}
	}
}

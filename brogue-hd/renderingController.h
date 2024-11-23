#pragma once

#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueLevel.h"
#include "brogueMouseState.h"
#include "brogueProgramContainer.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueViewContainer.h"
#include "eventController.h"
#include "gridRect.h"
#include "openglRenderer.h"
#include "randomGenerator.h"
#include "resourceController.h"
#include "simpleList.h"
#include "simpleString.h"

using namespace brogueHd::frontend;

namespace brogueHd::backend
{
	class renderingController
	{
	public:

		renderingController(eventController* eventController, resourceController* resourceController, randomGenerator* randomGenerator);
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

		brogueProgramContainer* _programContainer;
		brogueUIBuilder* _uiBuilder;
		randomGenerator* _randomGenerator;
		eventController* _eventController;
		resourceController* _resourceController;
		openglRenderer* _openglRenderer;
		brogueGlyphMap* _glyphMap;
	};

	renderingController::renderingController(eventController* eventController, resourceController* resourceController, randomGenerator* randomGenerator)
	{
		int zoomLevel = 10;

		_glyphMap = new brogueGlyphMap();
		_openglRenderer = new openglRenderer(eventController);
		_resourceController = resourceController;
		_eventController = eventController;
		_randomGenerator = randomGenerator;
		_uiBuilder = new brogueUIBuilder(eventController, resourceController, randomGenerator, zoomLevel);

		// Title Screen:  Build program parts, load the container

		brogueViewContainer* flameView = _uiBuilder->buildProgramView(brogueUIProgram::FlameMenuProgram);
		brogueViewContainer* titleMask = _uiBuilder->buildProgramView(brogueUIProgram::FlameMenuTitleMaskProgram);
		brogueViewContainer* mainMenu = _uiBuilder->buildProgramView(brogueUIProgram::MainMenuProgram);
		brogueViewContainer* openMenu = _uiBuilder->buildProgramView(brogueUIProgram::OpenMenuProgram);
		brogueViewContainer* openMenuBackground = _uiBuilder->buildProgramView(brogueUIProgram::OpenMenuBackgroundProgram);
		brogueViewContainer* playbackMenu = _uiBuilder->buildProgramView(brogueUIProgram::PlaybackMenuProgram);
		brogueViewContainer* playbackMenuBackground = _uiBuilder->buildProgramView(brogueUIProgram::PlaybackMenuBackgroundProgram);
		brogueViewContainer* highScoresMenu = _uiBuilder->buildProgramView(brogueUIProgram::HighScoresProgram);

		// Game Screen:  Build program parts

		brogueViewContainer* gameView = _uiBuilder->buildProgramView(brogueUIProgram::GameProgram);
		brogueViewContainer* gameMenu = _uiBuilder->buildProgramView(brogueUIProgram::GameMenuProgram);
		brogueViewContainer* gameLog = _uiBuilder->buildProgramView(brogueUIProgram::GameLogProgram);
		brogueViewContainer* gameInventory = _uiBuilder->buildProgramView(brogueUIProgram::GameInventoryProgram);
		brogueViewContainer* gameObjectList = _uiBuilder->buildProgramView(brogueUIProgram::GameObjectListProgram);
		brogueViewContainer* flavorTextPanel = _uiBuilder->buildProgramView(brogueUIProgram::FlavorTextPanelProgram);
		brogueViewContainer* bottomMenuBar = _uiBuilder->buildProgramView(brogueUIProgram::BottomBarMenuProgram);

		simpleList<brogueViewContainer*> viewList;

		// Title Views
		viewList.add(flameView);
		viewList.add(titleMask);
		viewList.add(mainMenu);
		viewList.add(openMenuBackground);
		viewList.add(openMenu);
		viewList.add(playbackMenuBackground);
		viewList.add(playbackMenu);
		viewList.add(highScoresMenu);

		// Game Views
		viewList.add(gameView);
		viewList.add(gameMenu);
		viewList.add(gameLog);
		viewList.add(gameInventory);
		viewList.add(gameObjectList);
		viewList.add(flavorTextPanel);
		viewList.add(bottomMenuBar);

		gridRect sceneBoundaryUI = flameView->calculateSceneBoundaryUI();

		_programContainer = new brogueProgramContainer(_uiBuilder, resourceController, eventController, _glyphMap, sceneBoundaryUI, zoomLevel, viewList);
	}
	renderingController::~renderingController()
	{
		delete _openglRenderer;
		delete _glyphMap;
		delete _programContainer;
		delete _uiBuilder;
	}

	void renderingController::initialize()
	{
		_openglRenderer->setProgram(_programContainer, BrogueGameMode::Title);
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


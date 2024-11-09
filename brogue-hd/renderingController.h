#pragma once

#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
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
#include "simpleDirectoryEntry.h"
#include "simpleFileIO.h"
#include "simpleList.h"

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
		void setGameMode(BrogueGameMode mode);

		/// <summary>
		/// Creates program containers with active UI components
		/// </summary>
		void initialize();

		brogueKeyboardState getKeyboardState() const;
		brogueMouseState getMouseState() const;
		BrogueGameMode getGameModeRequest() const;

	private:

		brogueProgramContainer* _programContainer;

		randomGenerator* _randomGenerator;
		eventController* _eventController;
		resourceController* _resourceController;
		openglRenderer* _openglRenderer;
		brogueGlyphMap* _glyphMap;
	};

	renderingController::renderingController(eventController* eventController, resourceController* resourceController, randomGenerator* randomGenerator)
	{
		_glyphMap = new brogueGlyphMap();
		_openglRenderer = new openglRenderer(eventController);
		_resourceController = resourceController;
		_eventController = eventController;
		_randomGenerator = randomGenerator;

		int zoomLevel = 9;

		// Title Screen:  Build program parts, load the container

		simpleDirectoryEntry gameFiles = simpleFileIO::readDirectory(_resourceController->getGamesDirectory()->c_str(), ".broguesave");
		simpleDirectoryEntry recordingFiles = simpleFileIO::readDirectory(_resourceController->getPlaybackDirectory()->c_str(), ".broguerec");

		brogueViewContainer* titleView = brogueUIBuilder::createFlameMenu(_eventController, _randomGenerator, zoomLevel);
		brogueViewContainer* mainMenu = brogueUIBuilder::createMainMenuButtons(_eventController, zoomLevel);
		brogueViewContainer* openMenu = brogueUIBuilder::createMainMenuSelector(brogueUIProgram::OpenMenuProgram, _eventController, gameFiles, zoomLevel);
		brogueViewContainer* playbackMenu = brogueUIBuilder::createMainMenuSelector(brogueUIProgram::PlaybackMenuProgram, _eventController, recordingFiles, zoomLevel);
		brogueViewContainer* highScoresMenu = brogueUIBuilder::createHighScoresView(_eventController, _resourceController, zoomLevel);

		simpleList<brogueViewContainer*> viewList;

		viewList.add(titleView);
		viewList.add(mainMenu);
		viewList.add(openMenu);
		viewList.add(playbackMenu);
		viewList.add(highScoresMenu);

		gridRect sceneBoundaryUI = titleView->getSceneBoundary();

		_programContainer = new brogueProgramContainer(resourceController, eventController, _glyphMap, sceneBoundaryUI, zoomLevel, viewList);
	}
	renderingController::~renderingController()
	{
		delete _openglRenderer;
		delete _glyphMap;
		delete _programContainer;
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
	BrogueGameMode renderingController::getGameModeRequest() const
	{
		return _openglRenderer->getRequestedMode();
	}

	void renderingController::setGameMode(BrogueGameMode mode)
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


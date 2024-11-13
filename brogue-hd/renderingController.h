#pragma once

#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueProgramContainer.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueViewContainer.h"
#include "color.h"
#include "colorString.h"
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

		brogueViewContainer* titleView1 = _uiBuilder->buildProgramView(brogueUIProgram::FlameMenuProgram1);
		brogueViewContainer* titleView2 = _uiBuilder->buildProgramView(brogueUIProgram::FlameMenuProgram2);
		brogueViewContainer* mainMenu = _uiBuilder->buildProgramView(brogueUIProgram::MainMenuProgram);
		brogueViewContainer* openMenu = _uiBuilder->buildProgramView(brogueUIProgram::OpenMenuProgram);
		brogueViewContainer* openMenuBackground = _uiBuilder->buildProgramView(brogueUIProgram::OpenMenuBackgroundProgram);
		brogueViewContainer* playbackMenu = _uiBuilder->buildProgramView(brogueUIProgram::PlaybackMenuProgram);
		brogueViewContainer* playbackMenuBackground = _uiBuilder->buildProgramView(brogueUIProgram::PlaybackMenuBackgroundProgram);
		brogueViewContainer* highScoresMenu = _uiBuilder->buildProgramView(brogueUIProgram::HighScoresProgram);

		simpleList<brogueViewContainer*> viewList;

		viewList.add(titleView1);
		viewList.add(titleView2);
		viewList.add(mainMenu);
		viewList.add(openMenuBackground);
		viewList.add(openMenu);
		viewList.add(playbackMenuBackground);
		viewList.add(playbackMenu);
		viewList.add(highScoresMenu);

		gridRect sceneBoundaryUI = titleView1->calculateSceneBoundaryUI();

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


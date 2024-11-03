#pragma once

#include "brogueGlobal.h"
#include "openglRenderer.h"
#include "gameConstants.h"
#include "resourceController.h"
#include "randomGenerator.h"
#include "brogueFlameMenu.h"
#include "brogueFlameMenuProgram.h"
#include "brogueUIBuilder.h"
#include "brogueDataStream.h"
#include "simpleOpenGl.h"
#include <functional>
#include <stdlib.h>
#include "simpleFileIO.h"

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

		BrogueGameMode _mode;

		brogueProgramContainer* _container;

		randomGenerator* _randomGenerator;
		resourceController* _resourceController;
		openglRenderer* _openglRenderer;
		brogueGlyphMap* _glyphMap;
	};

	renderingController::renderingController(resourceController* resourceController, randomGenerator* randomGenerator)
	{
		_openglRenderer = new openglRenderer();
		_glyphMap = new brogueGlyphMap();
		_resourceController = resourceController;
		_randomGenerator = randomGenerator;
		_mode = BrogueGameMode::Menu;

		_container = new brogueProgramContainer();
	}
	renderingController::~renderingController()
	{
		delete _openglRenderer;
		delete _glyphMap;

		for (int index = 0; index < _container->getUIProgramCount(); index++)
		{
			delete _container->getUIProgram(index);
		}

		delete _container->getBackgroundProgram();
		delete _container;
	}

	void renderingController::setViewMode(BrogueGameMode mode)
	{
		_mode = mode;

		for (int index = 0; index < _container->getUIProgramCount(); index++)
		{
			delete _container->getUIProgram(index);
		}

		if (_container->getBackgroundProgram() != nullptr)
			delete _container->getBackgroundProgram();

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
			
			int zoomLevel = 9;

			simpleDirectoryEntry gameFiles = simpleFileIO::readDirectory(_resourceController->getGamesDirectory()->c_str(), ".broguesave");
			simpleDirectoryEntry recordingFiles = simpleFileIO::readDirectory(_resourceController->getPlaybackDirectory()->c_str(), ".broguerec");

			brogueFlameMenu* titleView = new brogueFlameMenu(_randomGenerator, 100, zoomLevel);
			brogueListView* mainMenu = brogueUIBuilder::createMainMenuButtons(zoomLevel);
			brogueListView* openMenu = brogueUIBuilder::createMainMenuSelector(gameFiles, zoomLevel);
			brogueListView* playbackMenu = brogueUIBuilder::createMainMenuSelector(recordingFiles, zoomLevel);
			brogueListView* highScoresMenu = brogueUIBuilder::createHighScoresView(_resourceController, zoomLevel);

			// Main Menu:  brogueCellQuad, full scene (its view coordinates)
			brogueDataStream<brogueListView>* mainMenuStream =
				new brogueDataStream<brogueListView>(_resourceController,
					_glyphMap,
					openglDataStreamType::brogueCellQuad,
					openglBrogueCellOutputSelector::Display,
					false);

			// Open Menu:  brogueCellQuad, full scene (its view coordinates)
			brogueDataStream<brogueListView>* openMenuStream =
				new brogueDataStream<brogueListView>(_resourceController,
					_glyphMap,
					openglDataStreamType::brogueCellQuad,
					openglBrogueCellOutputSelector::Display,
					false);

			// Playback Menu:  brogueCellQuad, full scene (its view coordinates)
			brogueDataStream<brogueListView>* playbackMenuStream =
				new brogueDataStream<brogueListView>(_resourceController,
					_glyphMap,
					openglDataStreamType::brogueCellQuad,
					openglBrogueCellOutputSelector::Display,
					false);

			// High Scores Menu:  brogueCellQuad, full scene (its view coordinates)
			brogueDataStream<brogueListView>* highScoresStream =
				new brogueDataStream<brogueListView>(_resourceController,
					_glyphMap,
					openglDataStreamType::brogueCellQuad,
					openglBrogueCellOutputSelector::Display,
					false);
			
			brogueViewProgram<brogueListView>* mainMenuProgram =
				new brogueViewProgram<brogueListView>(mainMenu, _resourceController, _glyphMap,
														shaderResource::brogueCellDisplayVert,
														shaderResource::brogueCellDisplayFrag,
														mainMenuStream,
														true);

			brogueViewProgram<brogueListView>* openMenuProgram =
				new brogueViewProgram<brogueListView>(openMenu, _resourceController, _glyphMap,
					shaderResource::brogueCellDisplayVert,
					shaderResource::brogueCellDisplayFrag,
					openMenuStream,
					true);

			brogueViewProgram<brogueListView>* playbackMenuProgram =
				new brogueViewProgram<brogueListView>(playbackMenu, _resourceController, _glyphMap,
					shaderResource::brogueCellDisplayVert,
					shaderResource::brogueCellDisplayFrag,
					playbackMenuStream,
					true);

			brogueViewProgram<brogueListView>* highScoresProgram =
				new brogueViewProgram<brogueListView>(highScoresMenu, _resourceController, _glyphMap,
					shaderResource::brogueCellDisplayVert,
					shaderResource::brogueCellDisplayFrag,
					highScoresStream,
					true);

			_container->setBackground(new brogueFlameMenuProgram(titleView, mainMenu, _resourceController, _glyphMap));
			_container->addUIProgram(mainMenuProgram, false);
			_container->addUIProgram(openMenuProgram, false);
			_container->addUIProgram(playbackMenuProgram, false);
			_container->addUIProgram(highScoresProgram, true);

			_openglRenderer->setProgram(_container);
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
}


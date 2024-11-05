#pragma once

#include "brogueDataStream.h"
#include "brogueFlameMenu.h"
#include "brogueFlameMenuProgram.h"
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueListView.h"
#include "brogueProgramContainer.h"
#include "brogueProgramSignature.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueViewProgram.h"
#include "openglRenderer.h"
#include "randomGenerator.h"
#include "resourceController.h"
#include "simpleDirectoryEntry.h"
#include "simpleFileIO.h"

using namespace brogueHd::frontend;
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
		_glyphMap = new brogueGlyphMap();
		_openglRenderer = new openglRenderer();
		_resourceController = resourceController;
		_randomGenerator = randomGenerator;
		_mode = BrogueGameMode::Title;

		_container = new brogueProgramContainer(brogueUIContainer::TitleContainer);
	}
	renderingController::~renderingController()
	{
		delete _openglRenderer;
		delete _glyphMap;

		for (int index = 0; index < _container->getUIProgramCount(); index++)
		{
			delete _container->getUIProgramAt(index);
		}

		delete _container->getBackgroundProgram();
		delete _container;
	}

	void renderingController::setViewMode(BrogueGameMode mode)
	{
		_mode = mode;

		for (int index = 0; index < _container->getUIProgramCount(); index++)
		{
			delete _container->getUIProgramAt(index);
		}

		if (_container->getBackgroundProgram() != nullptr)
			delete _container->getBackgroundProgram();

		if (!_openglRenderer->isInitializedGL())
			_openglRenderer->initializeOpenGL();

		switch (_mode)
		{
			case BrogueGameMode::Title:
			{
				// Shuts down thread, deletes our program memory
				_openglRenderer->terminateProgram();

				int zoomLevel = 9;

				simpleDirectoryEntry gameFiles = simpleFileIO::readDirectory(_resourceController->getGamesDirectory()->c_str(), ".broguesave");
				simpleDirectoryEntry recordingFiles = simpleFileIO::readDirectory(_resourceController->getPlaybackDirectory()->c_str(), ".broguerec");

				brogueFlameMenu* titleView = new brogueFlameMenu(brogueUIView::Unnamed, _randomGenerator, 100, zoomLevel);
				brogueListView* mainMenu = brogueUIBuilder::createMainMenuButtons(zoomLevel);
				brogueListView* openMenu = brogueUIBuilder::createMainMenuSelector(brogueUIView::OpenGameSelector, gameFiles, zoomLevel);
				brogueListView* playbackMenu = brogueUIBuilder::createMainMenuSelector(brogueUIView::PlaybackSelector, recordingFiles, zoomLevel);
				brogueListView* highScoresMenu = brogueUIBuilder::createHighScoresView(_resourceController, zoomLevel);

				// Main Menu:  brogueCellQuad, full scene (its view coordinates)
				brogueDataStream* mainMenuStream =
					new brogueDataStream(_resourceController,
										 _glyphMap,
										 brogueOpenglDataStream::BrogueView,
										 openglDataStreamType::brogueCellQuad,
										 openglBrogueCellOutputSelector::Display,
										 false);

				// Open Menu:  brogueCellQuad, full scene (its view coordinates)
				brogueDataStream* openMenuStream =
					new brogueDataStream(_resourceController,
										 _glyphMap,
										 brogueOpenglDataStream::BrogueView,
										 openglDataStreamType::brogueCellQuad,
										 openglBrogueCellOutputSelector::Display,
										 false);

				// Playback Menu:  brogueCellQuad, full scene (its view coordinates)
				brogueDataStream* playbackMenuStream =
					new brogueDataStream(_resourceController,
										 _glyphMap,
										 brogueOpenglDataStream::BrogueView,
										 openglDataStreamType::brogueCellQuad,
										 openglBrogueCellOutputSelector::Display,
										 false);

				// High Scores Menu:  brogueCellQuad, full scene (its view coordinates)
				brogueDataStream* highScoresStream =
					new brogueDataStream(_resourceController,
										 _glyphMap,
										 brogueOpenglDataStream::BrogueView,
										 openglDataStreamType::brogueCellQuad,
										 openglBrogueCellOutputSelector::Display,
										 false);

				brogueViewProgram* mainMenuProgram =
					new brogueViewProgram(brogueUIProgram::MainMenuProgram,
										  mainMenu, _resourceController, _glyphMap,
										  shaderResource::brogueCellDisplayVert,
										  shaderResource::brogueCellDisplayFrag,
										  mainMenuStream,
										  true);

				brogueViewProgram* openMenuProgram =
					new brogueViewProgram(brogueUIProgram::OpenMenuProgram,
										  openMenu, _resourceController, _glyphMap,
										  shaderResource::brogueCellDisplayVert,
										  shaderResource::brogueCellDisplayFrag,
										  openMenuStream,
										  true);

				brogueViewProgram* playbackMenuProgram =
					new brogueViewProgram(brogueUIProgram::PlaybackMenuProgram,
										  playbackMenu, _resourceController, _glyphMap,
										  shaderResource::brogueCellDisplayVert,
										  shaderResource::brogueCellDisplayFrag,
										  playbackMenuStream,
										  true);

				brogueViewProgram* highScoresProgram =
					new brogueViewProgram(brogueUIProgram::HighScoresProgram,
										  highScoresMenu, _resourceController, _glyphMap,
										  shaderResource::brogueCellDisplayVert,
										  shaderResource::brogueCellDisplayFrag,
										  highScoresStream,
										  true);

				_container->setBackground(new brogueFlameMenuProgram(titleView, _resourceController, _glyphMap));
				_container->addUIProgram(mainMenuProgram);
				_container->addUIProgram(openMenuProgram);
				_container->addUIProgram(playbackMenuProgram);
				_container->addUIProgram(highScoresProgram);

				_container->activateUIProgram(mainMenuProgram->getProgramName());

				_openglRenderer->setProgram(_container);
				_openglRenderer->startProgram();
			}
			break;
			case BrogueGameMode::Game:
			case BrogueGameMode::Playback:
			{
				_openglRenderer->terminateProgram();
			}
			break;
			default:
				break;
		}
	}
}


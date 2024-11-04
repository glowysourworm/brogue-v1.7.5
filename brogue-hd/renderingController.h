#pragma once

#include "brogueDataStream.h"
#include "brogueFlameMenu.h"
#include "brogueFlameMenuProgram.h"
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueListView.h"
#include "brogueProgramContainer.h"
#include "brogueProgramController.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueViewProgram.h"
#include "openglRenderer.h"
#include "programControlData.h"
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
		brogueProgramController* _programController;
		openglRenderer* _openglRenderer;
		brogueGlyphMap* _glyphMap;
	};

	renderingController::renderingController(resourceController* resourceController, randomGenerator* randomGenerator)
	{
		_glyphMap = new brogueGlyphMap();
		_programController = new brogueProgramController();
		_openglRenderer = new openglRenderer(_programController);
		_resourceController = resourceController;
		_randomGenerator = randomGenerator;
		_mode = BrogueGameMode::Title;

		_container = new brogueProgramContainer();

		// Initialize:  NOTE:  Still need key codes. So, 'E' => "Escape Key". Also, need "select item behavior" for the program control framework (so, more to do)
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::MainMenuProgram, brogueUIProgram::OpenMenuProgram, brogueUIView::MainMenuSelector, brogueUIView::OpenGameSelector, false, true, false, false, false, true, 'o'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::MainMenuProgram, brogueUIProgram::PlaybackMenuProgram, brogueUIView::MainMenuSelector, brogueUIView::PlaybackSelector, false, true, false, false, false, true, 'v'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::MainMenuProgram, brogueUIProgram::HighScoresProgram, brogueUIView::MainMenuSelector, brogueUIView::HighScoresView, false, true, false, false, false, true, 'h'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::MainMenuProgram, brogueUIProgram::ModalProgram, brogueUIView::MainMenuSelector, brogueUIView::QuitModal, false, true, false, false, false, true, 'q'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::OpenMenuProgram, brogueUIProgram::MainMenuProgram, brogueUIView::OpenGameSelector, brogueUIView::MainMenuSelector, false, true, true, false, true, true, 'E'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::PlaybackMenuProgram, brogueUIProgram::MainMenuProgram, brogueUIView::PlaybackSelector, brogueUIView::MainMenuSelector, false, true, true, false, true, true, 'E'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::HighScoresProgram, brogueUIProgram::MainMenuProgram, brogueUIView::HighScoresView, brogueUIView::MainMenuSelector, false, true, true, false, true, true, 'E'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::ModalProgram, brogueUIProgram::MainMenuProgram, brogueUIView::QuitModal, brogueUIView::MainMenuSelector, false, true, true, false, true, true, 'E'));
		_programController->addViewControl(programControlData(brogueUIContainer::TitleContainer, brogueUIProgram::ModalProgram, brogueUIProgram::MainMenuProgram, brogueUIView::QuitModal, brogueUIView::Unnamed, true, true, false, false, false, true, 'y'));
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
		delete _programController;
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

				_container->setBackground(new brogueFlameMenuProgram(titleView, mainMenu, _resourceController, _glyphMap));
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


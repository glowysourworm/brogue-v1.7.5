#pragma once

#include "brogueGlobal.h"
#include "openglRenderer.h"
#include "gameConstants.h"
#include "resourceController.h"
#include "randomGenerator.h"
#include "brogueFlameMenu.h"
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
			
			simpleList<buttonData> buttons;

			// Pulled from Brogue v1.7.5
			color gradient1(0.22, 0.14, 0.29, 0.5);
			color gradient2(0.11, 0.06, 0.15, 0.5);
			

			buttons.add(buttonData("New Game", gradient1, gradient2, brogueTextAlignment::Center, 0));
			buttons.add(buttonData("Open Game", gradient1, gradient2, brogueTextAlignment::Center, 0));
			buttons.add(buttonData("View Recording", gradient1, gradient2, brogueTextAlignment::Center, 0));
			buttons.add(buttonData("High Scores", gradient1, gradient2, brogueTextAlignment::Center, 0));
			buttons.add(buttonData("Quit", gradient1, gradient2, brogueTextAlignment::Center, 0));

			brogueFlameMenu* titleView = new brogueFlameMenu(_randomGenerator, 100);
			brogueButtonMenu* mainMenu = new brogueButtonMenu(buttons, 1, titleView->getSceneBoundary(), gridRect(COLS - 26, ROWS - 12, 24, 11));

			// Main Menu:  brogueCellQuad, full scene (its view coordinates)
			brogueDataStream<brogueButtonMenu>* mainMenuStream =
				new brogueDataStream<brogueButtonMenu>(_resourceController,
					_glyphMap,
					openglDataStreamType::brogueCellQuad,
					openglBrogueCellOutputSelector::Display,
					false);
			
			brogueViewProgram<brogueButtonMenu>* mainMenuProgram = 
				new brogueViewProgram<brogueButtonMenu>(mainMenu, _resourceController, _glyphMap,
														shaderResource::brogueCellDisplayVert,
														shaderResource::brogueCellDisplayFrag,
														mainMenuStream,
														true);

			_container->setBackground(new brogueFlameMenuProgram(titleView, mainMenu, _resourceController, _glyphMap));
			_container->addUIProgram(mainMenuProgram);

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


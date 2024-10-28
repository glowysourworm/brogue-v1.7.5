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

		brogueProgram* _currentProgram;
		simpleList<brogueView*>* _currentViews;

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

		_currentViews = new simpleList<brogueView*>();
		_currentProgram = nullptr;
	}
	renderingController::~renderingController()
	{
		delete _openglRenderer;
		delete _glyphMap;

		for (int index = 0; index < _currentViews->count(); index++)
		{
			delete _currentViews->get(index);
		}

		delete _currentViews;
		delete _currentProgram;
	}

	void renderingController::setViewMode(BrogueGameMode mode)
	{
		_mode = mode;

		for (int index = 0; index < _currentViews->count(); index++)
		{
			delete _currentViews->get(index);
		}

		if (_currentProgram != nullptr)
			delete _currentProgram;

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
			

			buttons.add(buttonData("New Game", gradient1, gradient2));
			buttons.add(buttonData("Open Game", gradient1, gradient2));
			buttons.add(buttonData("View Recording", gradient1, gradient2));
			buttons.add(buttonData("High Scores", gradient1, gradient2));
			buttons.add(buttonData("Quit", gradient1, gradient2));

			brogueFlameMenu* titleView = new brogueFlameMenu(_randomGenerator, 100);
			brogueButtonMenu* mainMenu = new brogueButtonMenu(buttons, 1, titleView->getParentBoundary(), gridRect(COLS - 26, ROWS - 12, 24, 11));

			_currentProgram = new brogueFlameMenuProgram(titleView, mainMenu, _resourceController, _glyphMap);

			_currentViews->add(titleView);
			_currentViews->add(mainMenu);

			_openglRenderer->setProgram(_currentProgram);
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


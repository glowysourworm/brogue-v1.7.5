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
		brogueView* _currentView;

		randomGenerator* _randomGenerator;
		resourceController* _resourceController;
		openglRenderer* _openglRenderer;

	};

	renderingController::renderingController(resourceController* resourceController, randomGenerator* randomGenerator)
	{
		_openglRenderer = new openglRenderer();
		_resourceController = resourceController;
		_randomGenerator = randomGenerator;
		_mode = BrogueGameMode::Menu;

		_currentView = nullptr;
		_currentProgram = nullptr;
	}
	renderingController::~renderingController()
	{
		delete _openglRenderer;

		if (_currentView != nullptr)
			delete _currentView;

		if (_currentProgram != nullptr)
			delete _currentProgram;
	}

	void renderingController::setViewMode(BrogueGameMode mode)
	{
		_mode = mode;

		if (_currentView != nullptr)
			delete _currentView;

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
			
			_currentView = new brogueFlameMenu(_randomGenerator, 0, 0, 0, 0, 0, 0, 0);
			_currentProgram = new brogueFlameMenuProgram(dynamic_cast<brogueFlameMenu*>(_currentView), _resourceController);

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


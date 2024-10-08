#pragma once

#include "brogueGlobal.h"
#include "openglRenderer.h"
#include "gameConstants.h"
#include "resourceController.h"
#include <functional>
#include <stdlib.h>

using namespace brogueHd::backend::controller;
using namespace brogueHd::frontend::opengl;

namespace brogueHd::backend::controller
{
	class renderingController
	{
	public:

		renderingController(resourceController* resourceController);
		~renderingController();

		/// <summary>
		/// Sets the video output to the specific view
		/// </summary>
		void setViewMode(BrogueGameMode mode);

		/// <summary>
		/// Renders view to device output
		/// </summary>
		void renderView();

	private:

		void renderMenuView();
		void renderHighScoresView();
		void renderSetSeedView();
		void renderOpenGameView();
		void renderGameView();

	private:

		BrogueGameMode _mode;

		resourceController* _resourceController;
		openglRenderer* _openglRenderer;

	};
}


#pragma once

#include "command.h"
#include "openglRenderer.h"

using namespace brogueHd::frontend::opengl;

namespace brogueHd::backend::controller
{
	class renderingController
	{
	public:

		renderingController();
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

		openglRenderer* _renderer;

	};
}


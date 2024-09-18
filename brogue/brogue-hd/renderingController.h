#pragma once

#include "command.h"
#include "openglRenderer.h"

namespace brogueHd
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

		void renderMenuView();
		void renderHighScoresView();
		void renderSetSeedView();
		void renderOpenGameView();
		void renderGameView();

		/// <summary>
		/// Renders view to device output
		/// </summary>
		void drawView();

	private:

		BrogueGameMode _mode;

		openglRenderer* _renderer;

	};
}


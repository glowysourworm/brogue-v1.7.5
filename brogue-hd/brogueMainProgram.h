#pragma once
#include "brogueBackground.h"
#include "brogueCellDisplay.h"
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueProgramBuilder.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartConfiguration.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewContainer.h"
#include "brogueViewProgram.h"
#include "color.h"
#include "eventController.h"
#include "gl.h"
#include "gridRect.h"
#include "openglHelper.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleBitmap.h"
#include "simpleDataStream.h"
#include "simpleExt.h"
#include "simpleFrameBuffer.h"
#include "simpleGlData.h"
#include "simpleHash.h"
#include "simpleKeyboardState.h"
#include "simpleList.h"
#include "simpleMouseState.h"
#include "simpleShaderProgram.h"
#include "simpleTexture.h"
#include <SDL_image.h>
#include <SDL_surface.h>

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueMainProgram
	{
	public:

		brogueMainProgram(resourceController* resourceController,
						  eventController* eventController,
						  brogueGlyphMap* glyphMap,
						  const gridRect& sceneBoundaryUI,
						  int zoomLevel,
						  const simpleList<brogueViewContainer*>& viewList);
		~brogueMainProgram();

		brogueKeyboardState calculateKeyboardState(const simpleKeyboardState& keyboard);
		brogueMouseState calculateMouseState(const simpleMouseState& mouse);
		gridRect getSceneBoundaryUI() const;

		void activateUIProgram(brogueUIProgram programName);
		void deactivateUIProgram(brogueUIProgram programName);
		bool isProgramActive(brogueUIProgram programName) const;

		void checkUpdate(const simpleKeyboardState& keyboardState,
						 const simpleMouseState& mouseState,
						 int millisecondsLapsed);
		bool needsUpdate();
		void clearUpdate();
		void clearEvents();
		void update(const simpleKeyboardState& keyboardState,
					const simpleMouseState& mouseState,
					int millisecondsLapsed,
					bool forceUpdate);

		void initialize();
		void run(int millisecondsElapsed);
		void setMode(BrogueGameMode mode);
		void showErrors() const;
		bool hasErrors() const;

	private:

		resourceController* _resourceController;
		brogueGlyphMap* _glyphMap;

		simpleList<brogueViewContainer*>* _uiViews;
		simpleHash<brogueUIProgram, brogueViewProgram*>* _uiPrograms;

		simpleShaderProgram* _frameProgram;

		simpleFrameBuffer* _frameBuffer;

		simpleTexture* _frameTexture0;				// GL_TEXTURE0, Color Attachment 0, (same thing), and will carry the flame menu background
		simpleTexture* _frameTexture1;				// GL_TEXTURE1, Color Attachment 1, carries the UI components
		simpleTexture* _openMenuTexture;			// GL_TEXTURE2 (Open Menu Selector) (Clipping / Scrolling Behavior)
		simpleTexture* _playbackMenuTexture;		// GL_TEXTURE3 (Playback Menu Selector) (Clipping / Scrolling Behavior)
		simpleTexture* _fontTexture;				// GL_TEXTURE4 (Glyph Map texture) (Clipping / Scrolling Behavior)
		gridRect* _sceneBoundaryUI;

		BrogueGameMode _gameMode;
		int _zoomLevel;
	};

	brogueMainProgram::brogueMainProgram(resourceController* resourceController,
										 eventController* eventController,
										 brogueGlyphMap* glyphMap,
										 const gridRect& sceneBoundaryUI,
										 int zoomLevel,
										 const simpleList<brogueViewContainer*>& viewList)
	{
		_uiPrograms = new simpleHash<brogueUIProgram, brogueViewProgram*>();
		_uiViews = new simpleList<brogueViewContainer*>(viewList);
		_resourceController = resourceController;
		_glyphMap = glyphMap;
		_gameMode = BrogueGameMode::Title;
		_sceneBoundaryUI = new gridRect(sceneBoundaryUI);
		_zoomLevel = zoomLevel;

		// Grid Coordinates...
		gridRect sceneBounds = brogueUIBuilder::getBrogueSceneBoundary();

		brogueUIProgramPartId partId(brogueUIProgram::ContainerControlledProgram, brogueUIProgramPart::ViewCompositor, 0);
		brogueUIData backgroundData(partId, sceneBounds, zoomLevel);
		brogueProgramBuilder builder(resourceController, glyphMap);
		brogueBackground background(eventController,
									  backgroundData,
									  sceneBounds,
									  sceneBounds);

		brogueUIProgramPartConfiguration frameConfiguration(brogueUIProgramPart::Background,
															shaderResource::mixFrameTexturesVert,
															shaderResource::mixFrameTexturesFrag,
															openglDataStreamType::brogueImageQuad,
															openglBrogueCellOutputSelector::DisplayCurrentFrame,
															0,
															false,
															true);

		_frameProgram = builder.buildProgram(&background, frameConfiguration);

		// (CRITICAL!) Keep track of this, MUST MATCH:  Uniform location (layout index in the shader); shaderData->index; texture attachment GL_TEXTURE(N) index.
		//
		int textureIndex = 0;

		// We're not sure about how this is handling the data format: The specifics aren't solving the texture sampling issue which is showing
		//															  on the title screen.
		// 
		// https://www.khronos.org/opengl/wiki/Common_Mistakes#Image_precision
		//

		_frameTexture0 = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);
		_frameTexture1 = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE1, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);
		_openMenuTexture = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE2, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);
		_playbackMenuTexture = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE3, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);

		// Font Glyphs:  Going to load the max zoom for now
		simpleBitmap* glyphSheet = resourceController->getFontGlyphs(zoomLevel);

		// TODO: FIX THIS
		const char* fileFormat = "C:\\Backup\\_Source\\Git\\brogue-v1.7.5\\resources\\fonts\\font-{}-RGBA24-unsigned.bmp";

		// TODO: Decide whether to fully incorporate SDL and replace GLFW.
		SDL_Surface* glyphSurface = IMG_Load(simpleExt::format(fileFormat, zoomLevel).c_str());

		// TODO: Fix this
		openglHelper::flipSurface(glyphSurface);

		// void* @_@ (No really good way to own this one until we do it ourselves for each and every data type, that WE own)
		_fontTexture = new simpleTexture(glyphSurface, glyphSheet->pixelWidth(), glyphSheet->pixelHeight(), textureIndex++, GL_TEXTURE4, GL_RGBA, GL_RGBA, 4, GL_UNSIGNED_BYTE);

		_frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);
	}
	brogueMainProgram::~brogueMainProgram()
	{
		delete _frameProgram;
		delete _frameTexture0;
		delete _frameTexture1;
		delete _openMenuTexture;
		delete _playbackMenuTexture;
		delete _fontTexture;
		delete _frameBuffer;
		delete _sceneBoundaryUI;
		delete _uiViews;
		delete _uiPrograms;
	}

	brogueKeyboardState brogueMainProgram::calculateKeyboardState(const simpleKeyboardState& keyboard)
	{
		return _uiPrograms->firstValue()->calculateKeyboardState(keyboard);
	}
	brogueMouseState brogueMainProgram::calculateMouseState(const simpleMouseState& mouse)
	{
		return _uiPrograms->firstValue()->calculateMouseState(mouse);
	}
	void brogueMainProgram::activateUIProgram(brogueUIProgram programName)
	{
		_uiPrograms->get(programName)->activate();
	}
	void brogueMainProgram::deactivateUIProgram(brogueUIProgram programName)
	{
		_uiPrograms->get(programName)->deactivate();
	}
	bool brogueMainProgram::isProgramActive(brogueUIProgram programName) const
	{
		return _uiPrograms->get(programName)->isActive();
	}
	gridRect brogueMainProgram::getSceneBoundaryUI() const
	{
		return *_sceneBoundaryUI;
	}
	void brogueMainProgram::run(int millisecondsElapsed)
	{
		// Have to wait until the FrameBuffer is ready (skip draw passes)
		if (!_frameBuffer->isReady())
			return;

		// Procedure:
		//
		// Frame Texture 0:  Feedback texture (flame menu)
		// Frame Texture 1:  Component texture
		// 
		// During gameplay, the feedback texture is not drawn. The component texture is still drawn and holds
		// the components for that game mode
		// 
		// 1) Bind Frame Texture
		// 2) Cycle View Programs -> Draw to Texture 1 (in pre-defined order)
		// 3) Bind Framebuffer -> Output is now the screen (or the next opengl buffer - for swapping later) 
		// 4) Run Frame Program (Blend Texture 0 / Texture 1 -> Output Rendering)
		//

		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0, 0, 0, 0);

		_frameBuffer->bind();

		// Frame Buffer -> GL_TEXTURE0
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		switch (_gameMode)
		{
			case BrogueGameMode::Title:

				if (_uiPrograms->get(brogueUIProgram::FlameMenuProgram)->isActive())
					_uiPrograms->get(brogueUIProgram::FlameMenuProgram)->run(millisecondsElapsed);

				break;
			case BrogueGameMode::Game:
				break;
			case BrogueGameMode::Playback:
				break;
			default:
				break;
		}

		// Frame Buffer -> GL_TEXTURE1 (Holds UI component output)
		glDrawBuffer(GL_COLOR_ATTACHMENT1);

		// Runs a GL function to sub texture image data with zeros.
		_frameTexture1->bind();
		_frameTexture1->clearColor(colors::transparent());

		// Draw UI components
		switch (_gameMode)
		{
			case BrogueGameMode::Title:

				// Main Menu
				if (_uiPrograms->get(brogueUIProgram::MainMenuProgram)->isActive())
					_uiPrograms->get(brogueUIProgram::MainMenuProgram)->run(millisecondsElapsed);

				// Open Menu Header
				if (_uiPrograms->get(brogueUIProgram::OpenMenuProgram_HeaderProgram)->isActive())
					_uiPrograms->get(brogueUIProgram::OpenMenuProgram_HeaderProgram)->run(millisecondsElapsed);

				// Put the open menu (scrollable area) on a separate texture to control rendering
				glDrawBuffer(GL_COLOR_ATTACHMENT2);
				_openMenuTexture->bind();
				_openMenuTexture->clearColor(colors::transparent());

				// Open Menu
				if (_uiPrograms->get(brogueUIProgram::OpenMenuProgram)->isActive())
					_uiPrograms->get(brogueUIProgram::OpenMenuProgram)->run(millisecondsElapsed);

				glDrawBuffer(GL_COLOR_ATTACHMENT1);

				// Playback Menu Header
				if (_uiPrograms->get(brogueUIProgram::PlaybackMenuProgram_HeaderProgram)->isActive())
					_uiPrograms->get(brogueUIProgram::PlaybackMenuProgram_HeaderProgram)->run(millisecondsElapsed);

				// Put the open menu (scrollable area) on a separate texture to control rendering
				glDrawBuffer(GL_COLOR_ATTACHMENT3);
				_playbackMenuTexture->bind();
				_playbackMenuTexture->clearColor(colors::transparent());

				// Playback Menu
				if (_uiPrograms->get(brogueUIProgram::PlaybackMenuProgram)->isActive())
					_uiPrograms->get(brogueUIProgram::PlaybackMenuProgram)->run(millisecondsElapsed);

				glDrawBuffer(GL_COLOR_ATTACHMENT1);

				// High Scores
				if (_uiPrograms->get(brogueUIProgram::HighScoresProgram)->isActive())
					_uiPrograms->get(brogueUIProgram::HighScoresProgram)->run(millisecondsElapsed);

				break;
			case BrogueGameMode::Game:
				break;
			case BrogueGameMode::Playback:
				break;
			default:
				break;
		}

		// Unbind the frame buffer -> render output to normal GL output
		_frameBuffer->unBind();

		// Get frame program uniforms
		vec4 openMenuClipXY = _uiPrograms->get(brogueUIProgram::OpenMenuProgram)->getClipXY();
		vec4 playbackMenuClipXY = _uiPrograms->get(brogueUIProgram::PlaybackMenuProgram)->getClipXY();
		vec2 openMenuScrollUV = _uiPrograms->get(brogueUIProgram::OpenMenuProgram)->getScrollUV();
		vec2 playbackMenuScrollUV = _uiPrograms->get(brogueUIProgram::PlaybackMenuProgram)->getScrollUV();

		// Output the frame buffer
		_frameProgram->bind();
		_frameProgram->bindUniform4("openMenuClipXY", openMenuClipXY);
		_frameProgram->bindUniform4("playbackMenuClipXY", playbackMenuClipXY);
		_frameProgram->bindUniform2("openMenuScrollUV", openMenuScrollUV);
		_frameProgram->bindUniform2("playbackMenuScrollUV", playbackMenuScrollUV);
		_frameProgram->draw();

		glFlush();
		glFinish();
	}

	void brogueMainProgram::initialize()
	{
		/*
			Brogue Main Program:  This program has the responsibility of running and rendering the UI Programs using
								  a frame buffer; and handling the global textures.
		*/

		// Create the textures:  Texture 1 is used for the direct drawing, Texture 0 for the "color diffusion"
		_frameTexture0->glCreate(-1);		// Textures don't automatically associate w/ a program
		_frameTexture1->glCreate(-1);
		_openMenuTexture->glCreate(-1);
		_playbackMenuTexture->glCreate(-1);
		_fontTexture->glCreate(-1);

		//Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		_frameBuffer->glCreate(-1);			// Frame buffers don't automatically associate w/ a program

		// Attach texture to frame buffer
		_frameBuffer->bind();
		_frameBuffer->attachTexture(_frameTexture0->getHandle(), GL_COLOR_ATTACHMENT0);
		_frameBuffer->attachTexture(_frameTexture1->getHandle(), GL_COLOR_ATTACHMENT1);
		_frameBuffer->attachTexture(_openMenuTexture->getHandle(), GL_COLOR_ATTACHMENT2);
		_frameBuffer->attachTexture(_playbackMenuTexture->getHandle(), GL_COLOR_ATTACHMENT3);
		_frameBuffer->attachRenderBuffer();
		_frameBuffer->unBind();

		_frameProgram->compile();

		/*
			Brogue UI Programs:  This will call routines to setup all the "shader programs" that compose UI programs,
								 one per brogueViewProgram, which are the primary UI components, have names, interact
								 with the mouse, may be activated and deactivated, and will stay in memory until the
								 end of the application.
		*/

		// (MEMORY!) Create the brogueViewProgram* (see destructor)
		for (int index = 0; index < _uiViews->count(); index++)
		{
			brogueViewContainer* view = _uiViews->get(index);
			brogueViewProgram* program = new brogueViewProgram(view->getProgramName(),
															   view,
															   _resourceController,
															   _glyphMap);
			// Loads GPU (OpenGL backend)
			program->initialize();

			// Maintain UI program collection
			_uiPrograms->add(view->getProgramName(), program);
		}
	}

	void brogueMainProgram::checkUpdate(const simpleKeyboardState& keyboardState,
										const simpleMouseState& mouseState,
										int millisecondsLapsed)
	{
		// Iterate UI Programs
		_uiPrograms->iterate([&keyboardState, &mouseState, &millisecondsLapsed] (brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
			{
				program->checkUpdate(keyboardState, mouseState, millisecondsLapsed);
			}

			return iterationCallback::iterate;
		});
	}
	bool brogueMainProgram::needsUpdate()
	{
		bool result = false;

		_uiPrograms->iterate([&result] (brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
			{
				result |= program->needsUpdate();
			}

			if (result)
				return iterationCallback::breakAndReturn;
			else
				return iterationCallback::iterate;
		});

		return result;
	}
	void brogueMainProgram::clearUpdate()
	{
		_uiPrograms->iterate([] (brogueUIProgram programName, brogueViewProgram* program)
		{
			//if (program->isActive())
				program->clearUpdate();

			return iterationCallback::iterate;
		});
	}
	void brogueMainProgram::clearEvents()
	{
		_uiPrograms->iterate([] (brogueUIProgram programName, brogueViewProgram* program)
		{
			//if (program->isActive())
				program->clearEvents();

			return iterationCallback::iterate;
		});
	}
	void brogueMainProgram::update(const simpleKeyboardState& keyboardState,
								   const simpleMouseState& mouseState,
								   int millisecondsLapsed,
								   bool forceUpdate)
	{
		// Take Framebuffer Offline
		_frameBuffer->unBind();

		_uiPrograms->iterate([&keyboardState, &mouseState, &millisecondsLapsed, &forceUpdate] (brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
			{
				program->update(keyboardState, mouseState, millisecondsLapsed, forceUpdate);
			}

			return iterationCallback::iterate;
		});
	}
	bool brogueMainProgram::hasErrors() const
	{
		bool hasErrors = false;

		_uiPrograms->iterate([&hasErrors] (brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				hasErrors |= program->hasErrors();

			return iterationCallback::iterate;
		});

		return hasErrors;
	}
	void brogueMainProgram::showErrors() const
	{
		_uiPrograms->iterate([] (brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				program->outputStatus();

			return iterationCallback::iterate;
		});
	}
	void brogueMainProgram::setMode(BrogueGameMode mode)
	{
		// Deactivate All
		for (int index = 0; index < _uiPrograms->count(); index++)
		{
			_uiPrograms->getAt(index)->value->deactivate();
		}

		switch (mode)
		{
			case BrogueGameMode::Title:
				_uiPrograms->get(brogueUIProgram::FlameMenuProgram)->activate();
				_uiPrograms->get(brogueUIProgram::MainMenuProgram)->activate();
				break;
			case BrogueGameMode::Game:
				break;
			case BrogueGameMode::Playback:
				break;
			case BrogueGameMode::Exit:
			default:
				break;
		}
	}
}
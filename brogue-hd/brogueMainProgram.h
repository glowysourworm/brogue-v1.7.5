#pragma once
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueProgramBuilder.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartConfiguration.h"
#include "brogueUIProgramPartId.h"
#include "brogueView.h"
#include "brogueViewContainer.h"
#include "brogueViewProgram.h"
#include "eventController.h"
#include "gl.h"
#include "gridRect.h"
#include "openglHelper.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleBitmap.h"
#include "simpleExt.h"
#include "simpleFrameBuffer.h"
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
					int millisecondsLapsed);

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
		simpleTexture* _frameTexture1;				// GL_TEXTURE1, will carry all the rest of the components (overwriting / blending with texture 0)
		simpleTexture* _fontTexture;				// Glyph Map texture
		gridRect* _sceneBoundaryUI;

		BrogueGameMode _gameMode;
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

		brogueUIData backgroundData(brogueUIProgramPartId(brogueUIProgram::ContainerControlledProgram, brogueUIProgramPart::ViewCompositor, 0), sceneBoundaryUI, zoomLevel);
		brogueProgramBuilder builder(resourceController, glyphMap);
		brogueView background(eventController,
							  backgroundData,
							  sceneBoundaryUI,
							  sceneBoundaryUI);

		brogueUIProgramPartConfiguration frameConfiguration(brogueUIProgramPart::Background,
															shaderResource::mixFrameTexturesVert,
															shaderResource::mixFrameTexturesFrag,
															openglDataStreamType::brogueImageQuad,
															openglBrogueCellOutputSelector::DisplayCurrentFrame,
															0,
															false,
															true);

		_frameProgram = builder.buildProgram(&background, frameConfiguration);

		int textureIndex = 0;

		_frameTexture0 = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_RGBA, 4, GL_FLOAT);
		_frameTexture1 = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_RGBA, 4, GL_FLOAT);

		// Font Glyphs:  Going to load the max zoom for now
		simpleBitmap* glyphSheet = resourceController->getFontGlyphs(zoomLevel);

		// TODO: FIX THIS
		const char* fileFormat = "C:\\Backup\\_Source\\Git\\brogue-v1.7.5\\resources\\fonts\\font-{}-RGBA24-unsigned.bmp";

		// TODO: Decide whether to fully incorporate SDL and replace GLFW.
		SDL_Surface* glyphSurface = IMG_Load(simpleExt::format(fileFormat, zoomLevel).c_str());

		// TODO: Fix this
		openglHelper::flipSurface(glyphSurface);

		// void* @_@ (No really good way to own this one until we do it ourselves for each and every data type, that WE own)
		_fontTexture = new simpleTexture(glyphSurface, glyphSheet->pixelWidth(), glyphSheet->pixelHeight(), textureIndex++, GL_TEXTURE1, GL_RGBA, GL_RGBA, 4, GL_UNSIGNED_BYTE);

		_frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);
	}
	brogueMainProgram::~brogueMainProgram()
	{
		delete _frameProgram;
		delete _frameTexture0;
		delete _frameTexture1;
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
		glClearColor(0, 0, 0, 0);

		_frameBuffer->bind();

		switch (_gameMode)
		{
			case BrogueGameMode::Title:


				glDrawBuffer(GL_COLOR_ATTACHMENT0);
				_uiPrograms->get(brogueUIProgram::FlameMenuProgram)->run(millisecondsElapsed);

				glDrawBuffer(GL_COLOR_ATTACHMENT1);
				_uiPrograms->get(brogueUIProgram::MainMenuProgram)->run(millisecondsElapsed);
				break;
			case BrogueGameMode::Game:
				break;
			case BrogueGameMode::Playback:
				break;
			default:
				break;
		}

		_frameBuffer->unBind();

		// Blend the textures and output the result
		_frameProgram->bind();
		_frameProgram->draw();

		glFlush();
	}

	void brogueMainProgram::initialize()
	{
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

		/*
			Brogue Main Program:  This program has the responsibility of running and rendering the UI Programs using
								  a frame buffer; and handling the global textures.
		*/

		// Need a cell quad for one of the shaders
		//simpleQuad cellQuad = _heatDiffuseProgram->getCellSizeUV();

		_frameProgram->compile();

		// Create the textures:  Texture 1 is used for the direct drawing, Texture 0 for the "color diffusion"
		_frameTexture0->glCreate(-1);		// Textures don't automatically associate w/ a program
		_frameTexture1->glCreate(-1);
		_fontTexture->glCreate(-1);

		//Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		_frameBuffer->glCreate(-1);			// Frame buffers don't automatically associate w/ a program

		//// Heat Source Uniforms
		//_heatSourceProgram->getProgram()->bind();
		//_heatSourceProgram->getProgram()->bindUniform1i("frame0Texture", 0);       // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		//_heatSourceProgram->getProgram()->bindUniform1i("fontTexture", 1);		 // GL_TEXTURE1 "texture unit" Required! USE INDEXES!! (During the run)

		//// Heat Diffuse Uniform
		//// DON'T CHANGE THIS!!! NEED TO FIND A WAY TO GET THE UNIFORM VALUE FOR THE GL_TEXTURE0!!!
		//_heatDiffuseProgram->getProgram()->bind();
		//_heatDiffuseProgram->getProgram()->bindUniform1i("frame0Texture", 0);  // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		//_heatDiffuseProgram->getProgram()->bindUniform1i("fontTexture", 1);    // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		//_heatDiffuseProgram->getProgram()->bindUniform2("cellSizeUV", vec2(cellQuad.getWidth(), cellQuad.getHeight()));		// Gives the size of one cell
		//_heatDiffuseProgram->getProgram()->bindUniform1("weight", 0.2f);

		// Attach texture to frame buffer
		_frameBuffer->bind();
		_frameBuffer->attachTexture(_frameTexture0->getHandle(), GL_COLOR_ATTACHMENT0);
		_frameBuffer->attachTexture(_frameTexture0->getHandle(), GL_COLOR_ATTACHMENT1);
		_frameBuffer->attachRenderBuffer();
		_frameBuffer->unBind();
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
			program->clearUpdate();

			return iterationCallback::iterate;
		});
	}
	void brogueMainProgram::clearEvents()
	{
		_uiPrograms->iterate([] (brogueUIProgram programName, brogueViewProgram* program)
		{
			program->clearEvents();

			return iterationCallback::iterate;
		});
	}
	void brogueMainProgram::update(const simpleKeyboardState& keyboardState,
								   const simpleMouseState& mouseState,
								   int millisecondsLapsed)
	{
		// Take frame buffer offline
		_frameBuffer->unBind();

		_uiPrograms->iterate([&keyboardState, &mouseState, &millisecondsLapsed] (brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
			{
				program->update(keyboardState, mouseState, millisecondsLapsed);
			}

			return iterationCallback::iterate;
		});
	}
	bool brogueMainProgram::hasErrors() const
	{
		bool hasErrors = false;

		_uiPrograms->iterate([&hasErrors] (brogueUIProgram programName, brogueViewProgram* program)
		{
			hasErrors |= program->hasErrors();

			return iterationCallback::iterate;
		});

		return hasErrors;
	}
	void brogueMainProgram::showErrors() const
	{
		_uiPrograms->iterate([] (brogueUIProgram programName, brogueViewProgram* program)
		{
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
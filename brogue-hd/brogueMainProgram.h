#pragma once
#include "brogueCellDisplay.h"
#include "brogueColorQuad.h"
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueImageQuad.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIBuilder.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewProgram.h"
#include "color.h"
#include "eventController.h"
#include "gl.h"
#include "gridLocator.h"
#include "gridLocatorEdge.h"
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
#include "simpleLine.h"
#include "simpleList.h"
#include "simpleLogger.h"
#include "simpleMouseState.h"
#include "simpleShader.h"
#include "simpleShaderProgram.h"
#include "simpleTexture.h"
#include "simpleVertexArray.h"
#include "simpleVertexBuffer.h"
#include <SDL_image.h>
#include <SDL_surface.h>

namespace brogueHd::frontend
{
	using namespace simple;

	class brogueMainProgram
	{
	public:
		brogueMainProgram(brogueUIBuilder* uiBuilder,
		                  resourceController* resourceController,
		                  eventController* eventController,
		                  brogueGlyphMap* glyphMap,
		                  const gridRect& sceneBoundaryUI,
		                  int zoomLevel,
		                  const simpleList<brogueViewProgram*>& viewList);
		~brogueMainProgram();

		brogueKeyboardState calculateKeyboardState(const simpleKeyboardState& keyboard);
		brogueMouseState calculateMouseState(const simpleMouseState& mouse);
		gridRect getSceneBoundaryUI() const;

		void activateUIProgram(brogueUIProgram programName);
		void deactivateUIProgram(brogueUIProgram programName);
		void deactivateUIAll();
		bool isProgramActive(brogueUIProgram programName) const;

		void initiateStateChange(brogueUIState fromState, brogueUIState toState);
		void clearStateChange();
		bool checkStateChange();

		void checkUpdate(const simpleKeyboardState& keyboardState,
		                 const simpleMouseState& mouseState,
		                 int millisecondsLapsed);
		bool needsUpdate();
		void clearUpdate();
		void clearEvents();
		void update(int millisecondsLapsed, bool forceUpdate);

		void initializeUIPrograms();
		void initialize();
		void run(int millisecondsElapsed);
		void setMode(BrogueGameMode mode);
		void showErrors() const;
		bool hasErrors() const;

		void invalidate(const simpleMouseState& mouse, const simpleKeyboardState& keyboard);

		/// <summary>
		/// Primary game data update (see program container)
		/// </summary>
		void setGameUpdate(const brogueCellDisplay& data);

		/// <summary>
		/// Sets the debug polygon data for the level room graph
		/// </summary>
		void setDebugPolygonUpdate(const gridLocatorEdge<gridLocator>& roomConnection);

	private:
		void clearAllTextures();

	private:
		brogueUIBuilder* _uiBuilder;
		resourceController* _resourceController;
		brogueGlyphMap* _glyphMap;

		simpleHash<brogueUIProgram, brogueViewProgram*>* _uiPrograms;

		simpleShaderProgram* _frameProgram;

		simpleFrameBuffer* _frameBuffer;

		simpleTexture* _frameTexture;
		// GL_TEXTURE0, Color Attachment 0, (same thing), will carry the primary frame output (background(s))
		simpleTexture* _flameTexture; // GL_TEXTURE1, Blue Flame Background
		simpleTexture* _titleMaskTexture; // GL_TEXTURE2, Title Mask - separated due to frame buffer thrashing.
		simpleTexture* _uiTexture; // GL_TEXTURE3, Color Attachment 1, carries the UI components
		simpleTexture* _gameLogTexture; // GL_TEXTURE6 (Game Log) (Clipping / Scrolling Behavior)
		simpleTexture* _fontTexture; // GL_TEXTURE7 (Glyph Map texture) (Clipping / Scrolling Behavior)
		gridRect* _sceneBoundaryUI;

		BrogueGameMode _gameMode;
		int _zoomLevel;
	};

	brogueMainProgram::brogueMainProgram(brogueUIBuilder* uiBuilder,
	                                     resourceController* resourceController,
	                                     eventController* eventController,
	                                     brogueGlyphMap* glyphMap,
	                                     const gridRect& sceneBoundaryUI,
	                                     int zoomLevel,
	                                     const simpleList<brogueViewProgram*>& viewList)
	{
		_uiPrograms = new simpleHash<brogueUIProgram, brogueViewProgram*>();
		_resourceController = resourceController;
		_glyphMap = glyphMap;
		_gameMode = BrogueGameMode::Title;
		_sceneBoundaryUI = new gridRect(sceneBoundaryUI);
		_zoomLevel = zoomLevel;
		_uiBuilder = uiBuilder;

		// Add view/programs to the program collection
		for (int index = 0; index < viewList.count(); index++)
		{
			// Each view container contains collections of UI program parts - each with 
			// their own shader program for the GPU.
			_uiPrograms->add(viewList.get(index)->getProgramName(), viewList.get(index));
		}

		// Create a program for the frame buffer
		simpleShader vertexShader(resourceController->getShader(shaderResource::mixFrameTexturesVert));
		simpleShader fragmentShader(resourceController->getShader(shaderResource::mixFrameTexturesFrag));
		brogueCellDisplay emptyCell(0, 0, colors::transparent());
		simpleQuad frameQuadXY = uiBuilder->getCoordinateConverter()->createFrameQuadXY();
		simpleQuad frameQuadUV = uiBuilder->getCoordinateConverter()->createFrameQuadUV();
		brogueImageQuad element(emptyCell, frameQuadXY, frameQuadUV);
		simpleDataStream* frameDataStream = new simpleDataStream(1, element.getElementVertexSize(GL_TRIANGLES),
		                                                         element.getStreamSize(GL_TRIANGLES));

		// Buffer the stream (for our one frame element)
		element.streamBuffer(GL_TRIANGLES, frameDataStream);

		int vertexBufferIndex = 0; // GL VBO index is STATIC!

		// (MEMORY!) These are managed by the simple shader program
		simpleVertexBuffer<float>* programVBO = new simpleVertexBuffer<float>(
			vertexBufferIndex++, frameDataStream, vertexShader.getVertexAttributes());
		simpleVertexArray<float>* programVAO = new simpleVertexArray<float>(GL_TRIANGLES, programVBO);

		// (MEMORY!)
		_frameProgram = new simpleShaderProgram(vertexShader, fragmentShader, programVAO);

		// (CRITICAL!) Keep track of this, MUST MATCH:  Uniform location (layout index in the shader); shaderData->index; texture attachment GL_TEXTURE(N) index.
		//
		int textureIndex = 0;

		// We're not sure about how this is handling the data format: The specifics aren't solving the texture sampling issue which is showing
		//															  on the title screen.
		// 
		// https://www.khronos.org/opengl/wiki/Common_Mistakes#Image_precision
		//

		_frameTexture = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++,
		                                  GL_TEXTURE0, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);
		_flameTexture = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++,
		                                  GL_TEXTURE1, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);
		_titleMaskTexture = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++,
		                                      GL_TEXTURE2, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);
		_uiTexture = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++,
		                               GL_TEXTURE3, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);
		_gameLogTexture = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++,
		                                    GL_TEXTURE4, GL_RGBA, GL_RGBA8, 4, GL_FLOAT);

		// Font Glyphs:  Going to load the max zoom for now
		simpleBitmap* glyphSheet = resourceController->getFontGlyphs(zoomLevel);

		// TODO: FIX THIS
		const char* fileFormat =
			"C:\\Backup\\_Source\\Git\\brogue-v1.7.5\\resources\\fonts\\font-{}-RGBA24-unsigned.bmp";

		// TODO: Decide whether to fully incorporate SDL and replace GLFW.
		SDL_Surface* glyphSurface = IMG_Load(simpleExt::format(fileFormat, zoomLevel).c_str());

		// TODO: Fix this
		openglHelper::flipSurface(glyphSurface);

		// void* @_@ (No really good way to own this one until we do it ourselves for each and every data type, that WE own)
		_fontTexture = new simpleTexture(glyphSurface, glyphSheet->pixelWidth(), glyphSheet->pixelHeight(),
		                                 textureIndex++, GL_TEXTURE5, GL_RGBA, GL_RGBA, 4, GL_UNSIGNED_BYTE);

		_frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);
	}

	brogueMainProgram::~brogueMainProgram()
	{
		delete _frameProgram;
		delete _frameTexture;
		delete _flameTexture;
		delete _titleMaskTexture;
		delete _uiTexture;
		delete _gameLogTexture;
		delete _fontTexture;
		delete _frameBuffer;
		delete _sceneBoundaryUI;
		delete _uiPrograms; // TODO: Need entire teardown / delete procedure for the backend.
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

	void brogueMainProgram::deactivateUIAll()
	{
		_uiPrograms->iterate([](brogueUIProgram name, brogueViewProgram* program)
		{
			program->deactivate();
			return iterationCallback::iterate;
		});
	}

	void brogueMainProgram::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		_uiPrograms->iterate([&fromState, &toState](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				program->initiateStateChange(fromState, toState);

			return iterationCallback::iterate;
		});
	}

	void brogueMainProgram::clearStateChange()
	{
		_uiPrograms->iterate([](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				program->clearStateChange();

			return iterationCallback::iterate;
		});
	}

	bool brogueMainProgram::checkStateChange()
	{
		bool result = false;

		_uiPrograms->iterate([&result](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				result |= program->checkStateChange();

			return iterationCallback::iterate;
		});

		return result;
	}

	bool brogueMainProgram::isProgramActive(brogueUIProgram programName) const
	{
		return _uiPrograms->get(programName)->isActive();
	}

	gridRect brogueMainProgram::getSceneBoundaryUI() const
	{
		return *_sceneBoundaryUI;
	}

	void brogueMainProgram::invalidate(const simpleMouseState& mouse, const simpleKeyboardState& keyboard)
	{
		_uiPrograms->iterate([&mouse, &keyboard](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				program->invalidate(keyboard, mouse);

			return iterationCallback::iterate;
		});
	}

	void brogueMainProgram::setGameUpdate(const brogueCellDisplay& data)
	{
		brogueUIProgramPartId partId(brogueUIProgram::GameProgram, brogueUIProgramPart::GameSurface, 0);

		_uiPrograms->get(brogueUIProgram::GameProgram)->setGridProgram(partId, data);
	}

	void brogueMainProgram::setDebugPolygonUpdate(const gridLocatorEdge<gridLocator>& roomConnection)
	{
		brogueUIProgramPartId partId(brogueUIProgram::GameProgram, brogueUIProgramPart::Polygon, 0);

		simpleLine<int> line = _uiBuilder->getCoordinateConverter()->convertToUI(roomConnection.node1.getLocator(), roomConnection.node2.getLocator(), true);

		_uiPrograms->get(brogueUIProgram::GameProgram)->setPolygonProgram(partId, line);
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

		//glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0, 0, 0, 0);

		_frameBuffer->bind();

		// Frame Buffer -> GL_TEXTURE0
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		_frameTexture->bind();
		_frameTexture->clearColor(color(0, 0, 0, 0)); // Runs GL sub data function to clear color from texture

		switch (_gameMode)
		{
		case BrogueGameMode::Title:

			glDrawBuffer(GL_COLOR_ATTACHMENT1);

		// Flame Texture
			if (_uiPrograms->get(brogueUIProgram::FlameMenuProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::FlameMenuProgram)->run();

			glDrawBuffer(GL_COLOR_ATTACHMENT2);

		// Title Mask
			if (_uiPrograms->get(brogueUIProgram::FlameMenuTitleMaskProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::FlameMenuTitleMaskProgram)->run();

			break;
		case BrogueGameMode::Game:
			break;
		case BrogueGameMode::Playback:
			break;
		default:
			break;
		}

		// UI  Texture
		glDrawBuffer(GL_COLOR_ATTACHMENT3);

		_uiTexture->bind();
		_uiTexture->clearColor(colors::transparent());

		// Draw UI components
		switch (_gameMode)
		{
		case BrogueGameMode::Title:

			// Main Menu
			if (_uiPrograms->get(brogueUIProgram::MainMenuProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::MainMenuProgram)->run();

		// Open Menu
			if (_uiPrograms->get(brogueUIProgram::OpenMenuProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::OpenMenuProgram)->run();

		// Playback Menu
			if (_uiPrograms->get(brogueUIProgram::PlaybackMenuProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::PlaybackMenuProgram)->run();

		// High Scores
			if (_uiPrograms->get(brogueUIProgram::HighScoresProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::HighScoresProgram)->run();

			break;
		case BrogueGameMode::Game:

			// Currently, all on the UI texture
			//
			if (_uiPrograms->get(brogueUIProgram::GameProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::GameProgram)->run();

			if (_uiPrograms->get(brogueUIProgram::GameObjectListProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::GameObjectListProgram)->run();

			if (_uiPrograms->get(brogueUIProgram::FlavorTextPanelProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::FlavorTextPanelProgram)->run();

			if (_uiPrograms->get(brogueUIProgram::BottomBarMenuProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::BottomBarMenuProgram)->run();

			if (_uiPrograms->get(brogueUIProgram::GameMenuProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::GameMenuProgram)->run();

			if (_uiPrograms->get(brogueUIProgram::GameInventoryProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::GameInventoryProgram)->run();

		// Put the game log (scrollable area) on a separate texture to control rendering
			glDrawBuffer(GL_COLOR_ATTACHMENT4);
			_gameLogTexture->bind();
			_gameLogTexture->clearColor(colors::transparent());

			if (_uiPrograms->get(brogueUIProgram::GameLogProgram)->isActive())
				_uiPrograms->get(brogueUIProgram::GameLogProgram)->run();

			break;
		case BrogueGameMode::Playback:
			break;
		default:
			break;
		}

		//glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// Unbind the frame buffer -> render output to normal GL output
		_frameBuffer->unBind();

		glFlush();
		glFinish();


		// Get frame program uniforms
		vec2 gameLogOffsetUV = _uiPrograms->get(brogueUIProgram::GameLogProgram)->getOffsetUV();

		// Output the frame buffer
		_frameProgram->bind();
		_frameProgram->bindUniform2("gameLogOffsetUV", gameLogOffsetUV);
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
		_frameTexture->glCreate(-1); // Textures don't automatically associate w/ a program
		_flameTexture->glCreate(-1);
		_titleMaskTexture->glCreate(-1);
		_uiTexture->glCreate(-1);
		_gameLogTexture->glCreate(-1);
		_fontTexture->glCreate(-1);

		//Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		_frameBuffer->glCreate(-1); // Frame buffers don't automatically associate w/ a program

		// Attach texture to frame buffer
		_frameBuffer->bind();
		_frameBuffer->attachTexture(_frameTexture->getHandle(), GL_COLOR_ATTACHMENT0);
		_frameBuffer->attachTexture(_flameTexture->getHandle(), GL_COLOR_ATTACHMENT1);
		_frameBuffer->attachTexture(_titleMaskTexture->getHandle(), GL_COLOR_ATTACHMENT2);
		_frameBuffer->attachTexture(_uiTexture->getHandle(), GL_COLOR_ATTACHMENT3);
		_frameBuffer->attachTexture(_gameLogTexture->getHandle(), GL_COLOR_ATTACHMENT4);
		_frameBuffer->attachRenderBuffer();
		_frameBuffer->unBind();

		_frameProgram->compile();

		// Sets uniforms on UI programs
		initializeUIPrograms();

		// Initialize the textures by clearing
		clearAllTextures();
	}

	void brogueMainProgram::initializeUIPrograms()
	{
		/*
			Brogue UI Programs:  This will call routines to setup all the shader programs that compose "UI programs",
								 one (UI program) per brogueViewProgram, which are the primary UI components, have names,
								 interact with the mouse, may be activated and deactivated, and will stay in memory until the
								 end of the application. These must be initialized before calling "draw"; but there are
								 lazy compilations of the brogueViewCore that allow data to be set after the constructor.
		*/

		_uiPrograms->iterate([](const brogueUIProgram& programName, brogueViewProgram* viewProgram)
		{
			// Compile the UI Programs
			viewProgram->initialize();

			return iterationCallback::iterate;
		});
	}

	void brogueMainProgram::checkUpdate(const simpleKeyboardState& keyboardState,
	                                    const simpleMouseState& mouseState,
	                                    int millisecondsLapsed)
	{
		// Iterate UI Programs
		_uiPrograms->iterate(
			[&keyboardState, &mouseState, &millisecondsLapsed](brogueUIProgram programName, brogueViewProgram* program)
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

		_uiPrograms->iterate([&result](brogueUIProgram programName, brogueViewProgram* program)
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
		_uiPrograms->iterate([](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				program->clearUpdate();

			return iterationCallback::iterate;
		});
	}

	void brogueMainProgram::clearEvents()
	{
		_uiPrograms->iterate([](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				program->clearEvents();

			return iterationCallback::iterate;
		});
	}

	void brogueMainProgram::update(int millisecondsLapsed, bool forceUpdate)
	{
		// Take Framebuffer Offline
		_frameBuffer->unBind();

		_uiPrograms->iterate(
			[&millisecondsLapsed, &forceUpdate](brogueUIProgram programName, brogueViewProgram* program)
			{
				if (program->isActive() && program->needsUpdate())
				{
					program->update(millisecondsLapsed, forceUpdate);
				}

				return iterationCallback::iterate;
			});
	}

	bool brogueMainProgram::hasErrors() const
	{
		bool hasErrors = false;

		_uiPrograms->iterate([&hasErrors](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				hasErrors |= program->hasErrors();

			return iterationCallback::iterate;
		});

		return hasErrors;
	}

	void brogueMainProgram::showErrors() const
	{
		_uiPrograms->iterate([](brogueUIProgram programName, brogueViewProgram* program)
		{
			if (program->isActive())
				program->showErrors();

			return iterationCallback::iterate;
		});
	}

	void brogueMainProgram::setMode(BrogueGameMode mode)
	{
		_gameMode = mode;

		// Deactivate All
		for (int index = 0; index < _uiPrograms->count(); index++)
		{
			_uiPrograms->getAt(index)->getValue()->deactivate();
		}

		// Clear All Textures
		clearAllTextures();

		// Set "gameMode" uniform (see frame mixing texture)
		_frameProgram->bind();
		_frameProgram->bindUniform1i("gameMode", (int)mode);

		switch (mode)
		{
		case BrogueGameMode::Title:
			_uiPrograms->get(brogueUIProgram::FlameMenuProgram)->activate();
			_uiPrograms->get(brogueUIProgram::FlameMenuTitleMaskProgram)->activate();
			_uiPrograms->get(brogueUIProgram::MainMenuProgram)->activate();
			break;
		case BrogueGameMode::Game:
			_uiPrograms->get(brogueUIProgram::BottomBarMenuProgram)->activate();
			_uiPrograms->get(brogueUIProgram::GameObjectListProgram)->activate();
			_uiPrograms->get(brogueUIProgram::FlavorTextPanelProgram)->activate();
			_uiPrograms->get(brogueUIProgram::GameLogProgram)->activate();
			_uiPrograms->get(brogueUIProgram::GameProgram)->activate();
			break;
		case BrogueGameMode::Playback:
			break;
		case BrogueGameMode::Exit:
		default:
			break;
		}
	}

	void brogueMainProgram::clearAllTextures()
	{
		_frameTexture->bind();
		_frameTexture->clearColor(colors::transparent());

		_flameTexture->bind();
		_flameTexture->clearColor(colors::transparent());

		_titleMaskTexture->bind();
		_titleMaskTexture->clearColor(colors::transparent());

		_uiTexture->bind();
		_uiTexture->clearColor(colors::transparent());

		_gameLogTexture->bind();
		_gameLogTexture->clearColor(colors::transparent());
	}
}

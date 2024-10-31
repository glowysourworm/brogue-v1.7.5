#pragma once

#include "brogueCellDisplay.h"
#include "brogueProgram.h"
#include "simpleBitmap.h"
#include "simpleShaderProgram.h"
#include "simpleTexture.h"
#include "simpleFrameBuffer.h"
#include "brogueFlameMenu.h"
#include "brogueButtonMenu.h"
#include "brogueProgramBuilder.h"
#include "resourceController.h"
#include <SDL_surface.h>

using namespace brogueHd::simple;
using namespace brogueHd::backend::controller;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueFlameMenuProgram : public brogueProgram
	{
	public:
		
		brogueFlameMenuProgram(brogueFlameMenu* titleView,
								brogueButtonMenu* mainMenu,
								resourceController* resourceController,
								brogueGlyphMap* glyphMap);
		~brogueFlameMenuProgram();

		void initialize() override;
		void update(int millisecondsLapsed) override;
		void run(int millisecondsElapsed) override;
		
		bool hasErrors() const override
		{
			bool errors = _heatSourceProgram->hasErrors();

			errors |= _heatDiffuseProgram->hasErrors();
			errors |= _titleMaskProgram->hasErrors();
			errors |= _frameProgram->hasErrors();
			errors |= _mainMenuProgram->hasErrors();

			return errors;
		}

		void outputStatus() const override
		{
			_heatSourceProgram->showErrors();
			_heatDiffuseProgram->showErrors();
			_titleMaskProgram->showErrors();
			_frameProgram->showErrors();
			_mainMenuProgram->showErrors();
		}

		bool isCompiled() const override
		{
			return _frameProgram->isCreated() &&
				   _heatDiffuseProgram->isCreated() &&
				   _titleMaskProgram->isCreated() &&
				   _frameProgram->isCreated() &&
				   _mainMenuProgram->isCreated();
		}

		gridRect getSceneBoundaryUI() const override
		{
			return _programBuilder->calculateBoundaryUI(_titleView);
		}

	private:

		void init();

	private:

		brogueFlameMenu* _titleView;
		brogueButtonMenu* _mainMenu;

		simpleShaderProgram* _mainMenuProgram;
		simpleShaderProgram* _heatSourceProgram;
		simpleShaderProgram* _heatDiffuseProgram;
		simpleShaderProgram* _titleMaskProgram;
		simpleShaderProgram* _frameProgram;		

		simpleFrameBuffer* _frameBuffer;

		simpleTexture* _frameTexture0;
		simpleTexture* _fontTexture;

		int _diffusePeriodMilliseconds;
		int _diffusePeriodCounterMilliseconds;
		int _updatePeriodCounterMilliseconds;
		int _updatePeriodMilliseconds;
		
		resourceController* _resourceController;

		brogueProgramBuilder* _programBuilder;
	};

	brogueFlameMenuProgram::brogueFlameMenuProgram(brogueFlameMenu* titleView,
												   brogueButtonMenu* mainMenu,
												   resourceController* resourceController,
												   brogueGlyphMap* glyphMap)
	{
		_resourceController = resourceController;
		_titleView = titleView;
		_mainMenu = mainMenu;
		_diffusePeriodCounterMilliseconds = 0;
		_diffusePeriodMilliseconds = 10;
		_updatePeriodMilliseconds = 30;
		_updatePeriodCounterMilliseconds = 0;

		_programBuilder = new brogueProgramBuilder(resourceController, glyphMap);

		init();
	}

	brogueFlameMenuProgram::~brogueFlameMenuProgram()
	{
		delete _heatSourceProgram;
		delete _heatDiffuseProgram;
		delete _titleMaskProgram;
		delete _frameProgram;
		delete _mainMenuProgram;
		delete _frameTexture0;
		delete _fontTexture;
		delete _frameBuffer;
		delete _programBuilder;
	}

	void flipSurface(SDL_Surface* surface)
	{
		SDL_LockSurface(surface);

		int pitch = surface->pitch; // row size
		char* temp = new char[pitch]; // intermediate buffer
		char* pixels = (char*)surface->pixels;

		for (int i = 0; i < surface->h / 2; ++i) {
			// get pointers to the two rows to swap
			char* row1 = pixels + i * pitch;
			char* row2 = pixels + (surface->h - i - 1) * pitch;

			// swap rows
			memcpy(temp, row1, pitch);
			memcpy(row1, row2, pitch);
			memcpy(row2, temp, pitch);
		}

		delete[] temp;

		SDL_UnlockSurface(surface);
	}

	void brogueFlameMenuProgram::init()
	{
		gridRect sceneBoundaryUI = getSceneBoundaryUI();

		brogueCoordinateConverter coordinateConverter = _programBuilder->createCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height);

		int textureIndex = 0;

		brogueFlameMenu* titleView = _titleView;
		brogueButtonMenu* mainMenu = _mainMenu;

		// This is for the uniform cellSize variable (see fragment shaders)
		//
		simpleQuad cellSizeUV = coordinateConverter.getViewConverter().createQuadNormalizedUV(0, 0, brogueCellDisplay::CellWidth, brogueCellDisplay::CellHeight);

		shaderData* brogueCellVert = _resourceController->getShader(shaderResource::brogueCellDisplayVert);
		shaderData* brogueCellFrag = _resourceController->getShader(shaderResource::brogueCellDisplayFrag);

		// Heat Source
		//
		simpleDataStream* heatSourceDataStream = _programBuilder->createSceneDataStream(_titleView, openglDataStreamType::brogueCellQuad, openglBrogueCellOutputSelector::DisplayCurrentFrame,
		[&titleView] (short column, short row, brogueCellDisplay* cell)
		{
			return titleView->isTheText(column, row) || row == titleView->getBoundary().bottom();
		});

		shaderData* colorMaskVert = _resourceController->getShader(shaderResource::colorMaskVert);
		shaderData* colorMaskFrag = _resourceController->getShader(shaderResource::colorMaskFrag);

		// Title Text Mask
		//
		simpleDataStream* colorMaskDataStream = _programBuilder->createSceneDataStream(titleView, openglDataStreamType::brogueColorQuad, openglBrogueCellOutputSelector::NoDisplay,
		[&titleView] (short column, short row, brogueCellDisplay* cell)
		{
			return titleView->isTheText(column, row);
		});

		shaderData* diffuseUpwardVert = _resourceController->getShader(shaderResource::diffuseColorUpwardVert);
		shaderData* diffuseUpwardFrag = _resourceController->getShader(shaderResource::diffuseColorUpwardFrag);
		simpleDataStream* diffuseUpwardDataStream = _programBuilder->createSceneDataStream(titleView, openglDataStreamType::brogueCellQuad, openglBrogueCellOutputSelector::NoDisplay);

		shaderData* mixTexturesVert = _resourceController->getShader(shaderResource::mixFrameTexturesVert);
		shaderData* mixTexturesFrag = _resourceController->getShader(shaderResource::mixFrameTexturesFrag);
		simpleDataStream* mixTexturesDataStream = _programBuilder->createFrameDataStream(titleView, openglDataStreamType::brogueImageQuad);

		// Reuse background color shaders - make a new frame data stream for the frame copy
		simpleDataStream* backgroundColorFrameCopyStream = _programBuilder->createFrameDataStream(titleView, openglDataStreamType::brogueColorQuad);

		// Main Menu:
		simpleDataStream* mainMenuDataStream = _programBuilder->createSceneDataStream(mainMenu, openglDataStreamType::brogueCellQuad, openglBrogueCellOutputSelector::DisplayCurrentFrame);

		// (MEMORY!)
		_mainMenuProgram = _programBuilder->createShaderProgram(mainMenuDataStream, brogueCellVert, brogueCellFrag);
		_heatSourceProgram = _programBuilder->createShaderProgram(heatSourceDataStream, brogueCellVert, brogueCellFrag);
		_heatDiffuseProgram = _programBuilder->createShaderProgram(diffuseUpwardDataStream, diffuseUpwardVert, diffuseUpwardFrag);
		_titleMaskProgram = _programBuilder->createShaderProgram(colorMaskDataStream, colorMaskVert, colorMaskFrag);
		_frameProgram = _programBuilder->createShaderProgram(mixTexturesDataStream, mixTexturesVert, mixTexturesFrag);

		_frameTexture0 = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_RGBA, 4, GL_FLOAT);

		// Font Glyphs:  Going to load the max zoom for now
		simpleBitmap* glyphSheet = _resourceController->getFontGlyphs(MAX_ZOOM);
		
		SDL_Surface* glyphSurface = SDL_LoadBMP("C:\\Backup\\_Source\\Git\\brogue-v1.7.5\\resources\\fonts\\font-13-RGBA24-unsigned.bmp");

		flipSurface(glyphSurface);

		// void* @_@ (No really good way to own this one until we do it ourselves for each and every data type, that WE own)
		_fontTexture = new simpleTexture(glyphSurface, glyphSheet->pixelWidth(), glyphSheet->pixelHeight(), textureIndex++, GL_TEXTURE1, GL_RGBA, GL_RGBA8, 4, GL_UNSIGNED_BYTE);

		_frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);
	}

	void brogueFlameMenuProgram::run(int millisecondsElapsed)
	{
		if (!this->isCompiled())
			simpleException::show("Must first call IGLProgram.Compile() before using the GL program");

		// Have to wait until the FrameBuffer is ready (skip draw passes)
		
		if (!_frameBuffer->isReady())
			return;

		// Procedure:  Framebuffer Feedback
		//
		// 0) Load next heat source data (already done during this->update(int))
		// 1) Bind Framebuffer -> Texture 0 (contains last frame)
		// 2) Draw heat source data 
		// 3) "Diffuse" the colors onto Texture 0
		// 4) Apply Title Text Mask
		// 5) Bind Framebuffer(0) -> Output is now the screen (or the next opengl buffer - for swapping later) 
		//

		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0, 0, 0, 0);

		_frameBuffer->bind();
		
		// Render the heat sources as usual -> Color Attachment 0
		_heatSourceProgram->bind();
		_heatSourceProgram->draw();

		// Throttle the diffusion rate to be consistent / separated from the 
		// screen refresh rate.
		//
		if (_diffusePeriodCounterMilliseconds > _diffusePeriodMilliseconds)
		{
			_heatDiffuseProgram->bind();
			_heatDiffuseProgram->draw();

			_diffusePeriodCounterMilliseconds = 0;
		}
		else
			_diffusePeriodCounterMilliseconds += millisecondsElapsed;

		_titleMaskProgram->bind();
		_titleMaskProgram->draw();

		// Switch back to default GL frame buffer
		_frameBuffer->unBind();

		_frameProgram->bind();
		_frameProgram->draw();

		// TODO:  Fix transparency for the rest of the frame buffer (above)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		_mainMenuProgram->bind();
		_mainMenuProgram->bindUniform1i("frame0Texture", 0);
		_mainMenuProgram->bindUniform1i("fontTexture", 1);
		_mainMenuProgram->draw();

		glDisable(GL_BLEND);

		glFlush();
	}

	void brogueFlameMenuProgram::initialize()
	{
		gridRect sceneBoundaryUI = getSceneBoundaryUI();

		brogueCoordinateConverter coordinateConverter = _programBuilder->createCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height);

		simpleQuad cellQuad = coordinateConverter.getViewConverter().createQuadNormalizedUV(0, 0, brogueCellDisplay::CellWidth, brogueCellDisplay::CellHeight);

		_mainMenuProgram->compile();
		_frameProgram->compile();
		_heatSourceProgram->compile();
		_heatDiffuseProgram->compile();
		_titleMaskProgram->compile();

		_mainMenuProgram->bind();
		_frameProgram->bind();
		_heatSourceProgram->bind();
		_heatDiffuseProgram->bind();
		_titleMaskProgram->bind();

		// Create the textures:  Texture 1 is used for the direct drawing, Texture 0 for the "color diffusion"
		_frameTexture0->glCreate(-1);		// Textures don't automatically associate w/ a program
		_fontTexture->glCreate(-1);

		openglHelper::outputMaxTextureUnits();

		//Copy data over from the font glyph sheet
		simpleBitmap* glyphSheet = _resourceController->getFontGlyphs(MAX_ZOOM);

		//Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		_frameBuffer->glCreate(-1);			// Frame buffers don't automatically associate w/ a program

		// Main Menu Uniforms
		_mainMenuProgram->bind();
		_mainMenuProgram->bindUniform1i("frame0Texture", 0);     // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_mainMenuProgram->bindUniform1i("fontTexture", 1);		 // GL_TEXTURE1 "texture unit" Required! USE INDEXES!! (During the run)

		// Heat Source Uniforms
		_heatSourceProgram->bind();
		_heatSourceProgram->bindUniform1i("frame0Texture", 0);       // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_heatSourceProgram->bindUniform1i("fontTexture", 1);		 // GL_TEXTURE1 "texture unit" Required! USE INDEXES!! (During the run)

		// Heat Diffuse Uniform
		// DON'T CHANGE THIS!!! NEED TO FIND A WAY TO GET THE UNIFORM VALUE FOR THE GL_TEXTURE0!!!
		_heatDiffuseProgram->bind();
		_heatDiffuseProgram->bindUniform1i("frame0Texture", 0);  // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_heatDiffuseProgram->bindUniform1i("fontTexture", 1);    // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_heatDiffuseProgram->bindUniform2("cellSizeUV", vec2(cellQuad.getWidth(), cellQuad.getHeight()));		// Gives the size of one cell
		_heatDiffuseProgram->bindUniform1("weight", 0.2f);

		// Frame Program Uniforms
		_frameProgram->bind();
		_frameProgram->bindUniform1i("frame0Texture", 0);		   // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)

		// Attach texture to frame buffer
		_frameBuffer->bind();
		_frameBuffer->attachTexture(_frameTexture0->getHandle(), GL_COLOR_ATTACHMENT0);
		_frameBuffer->attachRenderBuffer();
		_frameBuffer->unBind();
	}

	void brogueFlameMenuProgram::update(int millisecondsLapsed)
	{
		_updatePeriodCounterMilliseconds += millisecondsLapsed;

		// Wait until the period has elapsed
		//
		if (_updatePeriodCounterMilliseconds < _updatePeriodMilliseconds)
			return;

		brogueFlameMenu* titleView = _titleView;

		_frameBuffer->unBind();

		// Update the rendering
		_titleView->update(_updatePeriodCounterMilliseconds);

		// Heat Source
		//
		simpleDataStream* heatSourceDataStream = _programBuilder->createSceneDataStream(titleView,
																						openglDataStreamType::brogueCellQuad, 
																						openglBrogueCellOutputSelector::DisplayCurrentFrame,
		[&titleView] (short column, short row, brogueCellDisplay* cell)
		{
			return titleView->isTheText(column, row) || row == titleView->getBoundary().bottom();
		});

		// (MEMORY! deletes old buffer)
		_heatSourceProgram->bind();
		_heatSourceProgram->reBuffer(heatSourceDataStream);

		_updatePeriodCounterMilliseconds = 0;
	}
}
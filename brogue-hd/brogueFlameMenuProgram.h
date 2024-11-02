#pragma once

#include "brogueView.h"
#include "brogueCellDisplay.h"
#include "brogueProgram.h"
#include "simpleBitmap.h"
#include "simpleMouseState.h"
#include "simpleShaderProgram.h"
#include "simpleTexture.h"
#include "simpleFrameBuffer.h"
#include "simplePeriodCounter.h"
#include "brogueFlameMenu.h"
#include "brogueButtonMenu.h"
#include "brogueProgramBuilder.h"
#include "brogueViewProgram.h"
#include "resourceController.h"
#include "brogueDataStream.h"

#include <SDL_image.h>
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
		void update(const simpleMouseState& mouseState, int millisecondsLapsed) override;
		bool shouldUpdate(const simpleMouseState& mouseState, int millisecondsLapsed) override;
		void run(int millisecondsElapsed) override;
		bool hasErrors() const override;
		void outputStatus() const override;
		bool isCompiled() const override;

		gridRect getSceneBoundaryUI() const override
		{
			return _frameProgram->getSceneBoundaryUI();
		}

	private:

		brogueViewProgram<brogueFlameMenu>* _heatSourceProgram;
		brogueViewProgram<brogueFlameMenu>* _heatDiffuseProgram;
		brogueViewProgram<brogueFlameMenu>* _titleMaskProgram;
		brogueViewProgram<brogueFlameMenu>* _frameProgram;

		simpleFrameBuffer* _frameBuffer;

		simpleTexture* _frameTexture0;
		simpleTexture* _fontTexture;

		simplePeriodCounter* _diffuseCounter;
		simplePeriodCounter* _updateCounter;
	};

	brogueFlameMenuProgram::brogueFlameMenuProgram(brogueFlameMenu* titleView,
												   brogueButtonMenu* mainMenu,
												   resourceController* resourceController,
												   brogueGlyphMap* glyphMap)
	{
		_diffuseCounter = new simplePeriodCounter(10);
		_updateCounter = new simplePeriodCounter(30);

		heatSourceDataStream* heatSourceStream =
			new heatSourceDataStream(resourceController,
				glyphMap,
				openglDataStreamType::brogueCellQuad,
				openglBrogueCellOutputSelector::DisplayCurrentFrame,
				false);

		brogueDataStream<brogueFlameMenu>* heatDiffuseStream =
			new brogueDataStream<brogueFlameMenu>(resourceController,
				glyphMap,
				openglDataStreamType::brogueCellQuad,
				openglBrogueCellOutputSelector::Display,
				false);

		titleMaskDataStream* titleMaskStream =
			new titleMaskDataStream(resourceController,
				glyphMap,
				openglDataStreamType::brogueColorQuad,
				openglBrogueCellOutputSelector::NoDisplay,
				false);

		brogueDataStream<brogueFlameMenu>* frameStream =
			new brogueDataStream<brogueFlameMenu>(resourceController, glyphMap,
				openglDataStreamType::brogueImageQuad,
				openglBrogueCellOutputSelector::NoDisplay,
				true);


		// Heat Sources:  brogueCellQuad, full scene (its view coordinates), using inclusion predicate mask
		_heatSourceProgram = new brogueViewProgram<brogueFlameMenu>(titleView, resourceController, glyphMap,
																	shaderResource::brogueCellDisplayVert,
																	shaderResource::brogueCellDisplayFrag,
																	heatSourceStream,
																	false);

		// Heat Diffusion:  brogueCellDisplay, full scene (...), Heat Diffusion Shaders
		_heatDiffuseProgram = new brogueViewProgram<brogueFlameMenu>(titleView, resourceController, glyphMap,
													shaderResource::diffuseColorUpwardVert,
													shaderResource::diffuseColorUpwardFrag,													
													heatDiffuseStream,
													false);

		// Title Mask:  brogueColorQuad, full scene, using title mask inclusion predicate
		_titleMaskProgram = new brogueViewProgram<brogueFlameMenu>(titleView, resourceController, glyphMap,
												  shaderResource::colorMaskVert,
												  shaderResource::colorMaskFrag,
												  titleMaskStream,
												  false);

		// Frame Program:  brogueImageQuad, full scene, frame-type data stream
		_frameProgram = new brogueViewProgram<brogueFlameMenu>(titleView, resourceController, glyphMap,
											  shaderResource::mixFrameTexturesVert, 
											  shaderResource::mixFrameTexturesFrag,
											  frameStream,
											  false);

		// Some final initialization of the other opengl objects
		gridRect sceneBoundaryUI = titleView->calculateSceneBoundaryUI();
		int textureIndex = 0;

		_frameTexture0 = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_RGBA, 4, GL_FLOAT);

		// Font Glyphs:  Going to load the max zoom for now
		simpleBitmap* glyphSheet = resourceController->getFontGlyphs(MAX_ZOOM);

		// TODO: Decide whether to fully incorporate SDL and replace GLFW.
		SDL_Surface* glyphSurface = IMG_Load("C:\\Backup\\_Source\\Git\\brogue-v1.7.5\\resources\\fonts\\font-13-RGBA24-unsigned.bmp");

		// TODO: Fix this
		openglHelper::flipSurface(glyphSurface);

		// void* @_@ (No really good way to own this one until we do it ourselves for each and every data type, that WE own)
		_fontTexture = new simpleTexture(glyphSurface, glyphSheet->pixelWidth(), glyphSheet->pixelHeight(), textureIndex++, GL_TEXTURE1, GL_RGBA, GL_RGBA, 4, GL_UNSIGNED_BYTE);

		_frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);
	}

	brogueFlameMenuProgram::~brogueFlameMenuProgram()
	{
		delete _heatSourceProgram;
		delete _heatDiffuseProgram;
		delete _titleMaskProgram;
		delete _frameProgram;
		delete _frameTexture0;
		delete _fontTexture;
		delete _frameBuffer;
		delete _updateCounter;
		delete _diffuseCounter;
	}

	void brogueFlameMenuProgram::run(int millisecondsElapsed)
	{
		if (!this->isCompiled())
			simpleException::show("Must first call initialize() before using the GL program");

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
		_heatSourceProgram->run(millisecondsElapsed);

		// Throttle the diffusion rate to be consistent / separated from the 
		// screen refresh rate.
		//
		if (_diffuseCounter->update(millisecondsElapsed))
			_heatDiffuseProgram->run(millisecondsElapsed);

		_titleMaskProgram->run(millisecondsElapsed);

		// Switch back to default GL frame buffer
		_frameBuffer->unBind();

		_frameProgram->run(millisecondsElapsed);

		glFlush();
	}

	void brogueFlameMenuProgram::initialize()
	{
		// Need a cell quad for one of the shaders
		simpleQuad cellQuad = _heatDiffuseProgram->getCellSizeUV();

		//_mainMenuProgram->initialize();
		_frameProgram->initialize();
		_heatSourceProgram->initialize();
		_heatDiffuseProgram->initialize();
		_titleMaskProgram->initialize();

		// Create the textures:  Texture 1 is used for the direct drawing, Texture 0 for the "color diffusion"
		_frameTexture0->glCreate(-1);		// Textures don't automatically associate w/ a program
		_fontTexture->glCreate(-1);

		//Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		_frameBuffer->glCreate(-1);			// Frame buffers don't automatically associate w/ a program

		// Heat Source Uniforms
		_heatSourceProgram->getProgram()->bind();
		_heatSourceProgram->getProgram()->bindUniform1i("frame0Texture", 0);       // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_heatSourceProgram->getProgram()->bindUniform1i("fontTexture", 1);		 // GL_TEXTURE1 "texture unit" Required! USE INDEXES!! (During the run)

		// Heat Diffuse Uniform
		// DON'T CHANGE THIS!!! NEED TO FIND A WAY TO GET THE UNIFORM VALUE FOR THE GL_TEXTURE0!!!
		_heatDiffuseProgram->getProgram()->bind();
		_heatDiffuseProgram->getProgram()->bindUniform1i("frame0Texture", 0);  // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_heatDiffuseProgram->getProgram()->bindUniform1i("fontTexture", 1);    // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_heatDiffuseProgram->getProgram()->bindUniform2("cellSizeUV", vec2(cellQuad.getWidth(), cellQuad.getHeight()));		// Gives the size of one cell
		_heatDiffuseProgram->getProgram()->bindUniform1("weight", 0.2f);

		// Frame Program Uniforms
		_frameProgram->getProgram()->bind();
		_frameProgram->getProgram()->bindUniform1i("frame0Texture", 0);		   // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)

		// Attach texture to frame buffer
		_frameBuffer->bind();
		_frameBuffer->attachTexture(_frameTexture0->getHandle(), GL_COLOR_ATTACHMENT0);
		_frameBuffer->attachRenderBuffer();
		_frameBuffer->unBind();
	}

	bool brogueFlameMenuProgram::shouldUpdate(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		// Wait until the period has elapsed
		//
		return _updateCounter->update(millisecondsLapsed);
	}
	bool brogueFlameMenuProgram::hasErrors() const
	{
		return _heatSourceProgram->hasErrors() ||
				_heatDiffuseProgram->hasErrors() ||
				_titleMaskProgram->hasErrors() ||
				_frameProgram->hasErrors();
	}
	void brogueFlameMenuProgram::outputStatus() const
	{
		_heatSourceProgram->outputStatus();
		_heatDiffuseProgram->outputStatus();
		_titleMaskProgram->outputStatus();
		_frameProgram->outputStatus();
	}
	bool brogueFlameMenuProgram::isCompiled() const
	{
		return _heatSourceProgram->isCompiled() &&
			   _heatDiffuseProgram->isCompiled() &&
			   _titleMaskProgram->isCompiled() &&
			   _frameProgram->isCompiled();
	}

	void brogueFlameMenuProgram::update(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		// Take frame buffer offline
		_frameBuffer->unBind();

		// Check programs
		if (_heatSourceProgram->shouldUpdate(mouseState, millisecondsLapsed))
			_heatSourceProgram->update(mouseState, millisecondsLapsed);
	}
}
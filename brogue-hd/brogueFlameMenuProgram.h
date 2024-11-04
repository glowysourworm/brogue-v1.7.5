#pragma once

#include "brogueDataStream.h"
#include "brogueFlameMenu.h"
#include "brogueListView.h"
#include "brogueProgram.h"
#include "brogueUIConstants.h"
#include "brogueUIResponseData.h"
#include "brogueView.h"
#include "brogueViewProgram.h"
#include "resourceController.h"
#include "simpleBitmap.h"
#include "simpleFrameBuffer.h"
#include "simpleMouseState.h"
#include "simplePeriodCounter.h"
#include "simpleShaderProgram.h"
#include "simpleTexture.h"

#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "gl.h"
#include "gridRect.h"
#include "openglHelper.h"
#include "simpleException.h"
#include "simpleGlData.h"
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
			brogueListView* mainMenu,
			resourceController* resourceController,
			brogueGlyphMap* glyphMap);
		~brogueFlameMenuProgram();

		void initialize() override;
		void update(const simpleMouseState& mouseState, int millisecondsLapsed) override;
		brogueUIResponseData& checkUpdate(const simpleMouseState& mouseState, int millisecondsLapsed) override;
		void run(int millisecondsElapsed) override;
		bool hasErrors() const override;
		void outputStatus() const override;
		bool isCompiled() const override;

		gridRect getSceneBoundaryUI() const override
		{
			return _frameProgram->getSceneBoundaryUI();
		}

	private:

		brogueViewProgram* _heatSourceProgram;
		brogueViewProgram* _heatDiffuseProgram;
		brogueViewProgram* _titleMaskProgram;
		brogueViewProgram* _frameProgram;

		simpleFrameBuffer* _frameBuffer;

		simpleTexture* _frameTexture0;
		simpleTexture* _fontTexture;

		simplePeriodCounter* _diffuseCounter;
		simplePeriodCounter* _updateCounter;
	};

	brogueFlameMenuProgram::brogueFlameMenuProgram(brogueFlameMenu* titleView,
		brogueListView* mainMenu,
		resourceController* resourceController,
		brogueGlyphMap* glyphMap)
		: brogueProgram(brogueUIProgram::FlameMenuProgram, false)
	{
		_diffuseCounter = new simplePeriodCounter(10);
		_updateCounter = new simplePeriodCounter(30);

		brogueDataStream* heatSourceStream =
			new brogueDataStream(resourceController,
				glyphMap,
				brogueOpenglDataStream::HeatSourceStream,
				openglDataStreamType::brogueCellQuad,
				openglBrogueCellOutputSelector::DisplayCurrentFrame,
				false);

		brogueDataStream* heatDiffuseStream =
			new brogueDataStream(resourceController,
				glyphMap,
				brogueOpenglDataStream::BrogueView,
				openglDataStreamType::brogueCellQuad,
				openglBrogueCellOutputSelector::Display,
				false);

		brogueDataStream* titleMaskStream =
			new brogueDataStream(resourceController,
				glyphMap,
				brogueOpenglDataStream::TitleMaskStream,
				openglDataStreamType::brogueColorQuad,
				openglBrogueCellOutputSelector::NoDisplay,
				false);

		brogueDataStream* frameStream =
			new brogueDataStream(resourceController, glyphMap,
				brogueOpenglDataStream::BrogueView,
				openglDataStreamType::brogueImageQuad,
				openglBrogueCellOutputSelector::NoDisplay,
				true);


		// Heat Sources:  brogueCellQuad, full scene (its view coordinates), using inclusion predicate mask
		_heatSourceProgram = new brogueViewProgram(brogueUIProgram::ContainerControlledProgram,
			titleView, resourceController, glyphMap,
			shaderResource::brogueCellDisplayVert,
			shaderResource::brogueCellDisplayFrag,
			heatSourceStream,
			false);

		// Heat Diffusion:  brogueCellDisplay, full scene (...), Heat Diffusion Shaders
		_heatDiffuseProgram = new brogueViewProgram(brogueUIProgram::ContainerControlledProgram,
			titleView, resourceController, glyphMap,
			shaderResource::diffuseColorUpwardVert,
			shaderResource::diffuseColorUpwardFrag,
			heatDiffuseStream,
			false);

		// Title Mask:  brogueColorQuad, full scene, using title mask inclusion predicate
		_titleMaskProgram = new brogueViewProgram(brogueUIProgram::ContainerControlledProgram,
			titleView, resourceController, glyphMap,
			shaderResource::colorMaskVert,
			shaderResource::colorMaskFrag,
			titleMaskStream,
			false);

		// Frame Program:  brogueImageQuad, full scene, frame-type data stream
		_frameProgram = new brogueViewProgram(brogueUIProgram::ContainerControlledProgram,
			titleView, resourceController, glyphMap,
			shaderResource::mixFrameTexturesVert,
			shaderResource::mixFrameTexturesFrag,
			frameStream,
			false);

		// Some final initialization of the other opengl objects
		gridRect sceneBoundaryUI = titleView->calculateSceneBoundaryUI();
		int textureIndex = 0;

		_frameTexture0 = new simpleTexture(nullptr, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_RGBA, 4, GL_FLOAT);

		// Font Glyphs:  Going to load the max zoom for now
		simpleBitmap* glyphSheet = resourceController->getFontGlyphs(MAX_ZOOM - 1);

		// TODO: Decide whether to fully incorporate SDL and replace GLFW.
		SDL_Surface* glyphSurface = IMG_Load("C:\\Backup\\_Source\\Git\\brogue-v1.7.5\\resources\\fonts\\font-12-RGBA24-unsigned.bmp");

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

	brogueUIResponseData& brogueFlameMenuProgram::checkUpdate(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		// Wait until the period has elapsed
		//
		bool elapsed = _updateCounter->update(millisecondsLapsed, false);

		brogueUIResponseData response(brogueUIProgram::FlameMenuProgram, brogueUIView::Unnamed, elapsed);

		return response;
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
		if (_heatSourceProgram->checkUpdate(mouseState, millisecondsLapsed).shouldUpdate)
			_heatSourceProgram->update(mouseState, millisecondsLapsed);
	}
}
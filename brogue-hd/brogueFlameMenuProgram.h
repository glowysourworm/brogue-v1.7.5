#pragma once

#include "brogueProgram.h"
#include "simpleShaderProgram.h"
#include "simpleTexture.h"
#include "simpleFrameBuffer.h"
#include "brogueFlameMenu.h"
#include "brogueProgramBuilder.h"
#include "resourceController.h"

using namespace brogueHd::backend::controller;
using namespace brogueHd::frontend::opengl;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueFlameMenuProgram : public brogueProgram
	{
	public:
		
		brogueFlameMenuProgram(brogueFlameMenu* mainMenu,
							   resourceController* resourceController);
		~brogueFlameMenuProgram();

		void initialize() override;
		void update(int millisecondsLapsed) override;
		void run() override;
		
		bool hasErrors() const override
		{
			bool errors = _heatSourceProgram->hasErrors();

			errors |= _heatDiffuseProgram->hasErrors();
			errors |= _titleMaskProgram->hasErrors();
			errors |= _frameProgram->hasErrors();

			return errors;
		}

		void outputStatus() const override
		{
			_heatSourceProgram->showErrors();
			_heatDiffuseProgram->showErrors();
			_titleMaskProgram->showErrors();
			_frameProgram->showErrors();
		}

		bool isCompiled() const override
		{
			return _heatSourceProgram->isCreated() &&
				   _heatDiffuseProgram->isCreated() &&
				   _titleMaskProgram->isCreated() &&
				   _frameProgram->isCreated();
		}

		gridRect getSceneBoundaryUI() const override
		{
			return *_sceneBoundaryUI;
		}

	private:

		brogueFlameMenu* _renderedView;

		simpleShaderProgram* _heatSourceProgram;
		simpleShaderProgram* _heatDiffuseProgram;
		simpleShaderProgram* _titleMaskProgram;
		simpleShaderProgram* _frameProgram;		

		simpleFrameBuffer* _frameBuffer;

		simpleTexture* _frameTexture0;

		vec2* _cellSizeUV;
		ivec2* _cellSizeUI;
		ivec2* _sceneSizeUI;

		int _periodCounter;
		int _periodMilliseconds;
		
		gridRect* _sceneBoundaryUI;

		// TODO: Get rid of this dependency
		resourceController* _resourceController;
	};

	brogueFlameMenuProgram::brogueFlameMenuProgram(brogueFlameMenu* mainMenu,
												   resourceController* resourceController)
	{
		_resourceController = resourceController;
		_renderedView = mainMenu;
		_periodCounter = 0;
		_periodMilliseconds = 100;

		int textureIndex = 0;

		gridRect sceneBoundaryUI = brogueProgramBuilder::calculateBoundaryUI(mainMenu);

		// This is for the uniform cellSize variable (see fragment shaders)
		//
		simpleQuad cellSizeUV = coordinateConverter::createQuadNormalizedUVScene(0, 0, brogueCellDisplay::CellWidth, brogueCellDisplay::CellHeight, sceneBoundaryUI.width, sceneBoundaryUI.height);

		_cellSizeUV = new vec2(cellSizeUV.bottomRight.x - cellSizeUV.topLeft.x, cellSizeUV.topLeft.y - cellSizeUV.bottomRight.y);
		_cellSizeUI = new ivec2(brogueCellDisplay::CellWidth, brogueCellDisplay::CellHeight);
		_sceneSizeUI = new ivec2(sceneBoundaryUI.width, sceneBoundaryUI.height);

		shaderData* brogueCellVert = resourceController->getShader(shaderResource::brogueCellDisplayVert);
		shaderData* brogueCellFrag = resourceController->getShader(shaderResource::brogueCellDisplayFrag);

		// Heat Source
		//
		simpleDataStream* heatSourceDataStream = brogueProgramBuilder::createSceneDataStream(mainMenu, openglDataStreamType::brogueCellQuad, openglBrogueCellOutputSelector::DisplayCurrentFrame,
		[&mainMenu] (short column, short row, brogueCellDisplay* cell)
		{
			return true; /*mainMenu->isTheText(column, row) || row == mainMenu->getBoundary().bottom();*/
		});

		shaderData* colorMaskVert = resourceController->getShader(shaderResource::colorMaskVert);
		shaderData* colorMaskFrag = resourceController->getShader(shaderResource::colorMaskFrag);

		// Title Text Mask
		//
		simpleDataStream* colorMaskDataStream = brogueProgramBuilder::createSceneDataStream(mainMenu, openglDataStreamType::brogueColorQuad, openglBrogueCellOutputSelector::NoDisplay,
		[&mainMenu](short column, short row, brogueCellDisplay* cell)
		{
			return mainMenu->isTheText(column, row);
		});

		shaderData* diffuseUpwardVert = resourceController->getShader(shaderResource::diffuseColorUpwardVert);
		shaderData* diffuseUpwardFrag = resourceController->getShader(shaderResource::diffuseColorUpwardFrag);
		simpleDataStream* diffuseUpwardDataStream = brogueProgramBuilder::createSceneDataStream(mainMenu, openglDataStreamType::brogueCellQuad, openglBrogueCellOutputSelector::NoDisplay);

		shaderData* mixTexturesVert = resourceController->getShader(shaderResource::mixFrameTexturesVert);
		shaderData* mixTexturesFrag = resourceController->getShader(shaderResource::mixFrameTexturesFrag);
		simpleDataStream* mixTexturesDataStream = brogueProgramBuilder::createFrameDataStream(mainMenu, openglDataStreamType::brogueImageQuad);

		// Reuse background color shaders - make a new frame data stream for the frame copy
		simpleDataStream* backgroundColorFrameCopyStream = brogueProgramBuilder::createFrameDataStream(mainMenu, openglDataStreamType::brogueColorQuad);

		// (MEMORY!)
		_heatSourceProgram = brogueProgramBuilder::createShaderProgram(heatSourceDataStream, brogueCellVert, brogueCellFrag);
		_heatDiffuseProgram = brogueProgramBuilder::createShaderProgram(diffuseUpwardDataStream, diffuseUpwardVert, diffuseUpwardFrag);
		_titleMaskProgram = brogueProgramBuilder::createShaderProgram(colorMaskDataStream, colorMaskVert, colorMaskFrag);
		_frameProgram = brogueProgramBuilder::createShaderProgram(mixTexturesDataStream, mixTexturesVert, mixTexturesFrag);

		_frameTexture0 = new simpleTexture(NULL, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_FLOAT);

		_frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);

		_sceneBoundaryUI = new gridRect(sceneBoundaryUI);
	}

	brogueFlameMenuProgram::~brogueFlameMenuProgram()
	{
		delete _heatSourceProgram;
		delete _heatDiffuseProgram;
		delete _titleMaskProgram;
		delete _frameProgram;
		delete _frameTexture0;
		delete _frameBuffer;
		delete _sceneBoundaryUI;
		delete _cellSizeUV;
		delete _cellSizeUI;
		delete _sceneSizeUI;
	}

	void brogueFlameMenuProgram::run()
	{
		if (!this->isCompiled())
			simpleException::show("Must first call IGLProgram.Compile() before using the GL program");

		// Have to wait until the FrameBuffer is ready (skip draw passes)
		//
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

		_heatDiffuseProgram->bind();
		_heatDiffuseProgram->draw();

		_titleMaskProgram->bind();
		_titleMaskProgram->draw();

		_frameBuffer->unBind();

		_frameProgram->bind();
		_frameProgram->draw();

		glFlush();
	}

	void brogueFlameMenuProgram::initialize()
	{
		_heatSourceProgram->compile();
		_heatDiffuseProgram->compile();
		_titleMaskProgram->compile();
		_frameProgram->compile();

		_heatSourceProgram->bind();
		_heatDiffuseProgram->bind();
		_titleMaskProgram->bind();
		_frameProgram->bind();

		// Create the textures:  Texture 1 is used for the direct drawing, Texture 0 for the "color diffusion"
		_frameTexture0->glCreate(-1);		// Textures don't automatically associate w/ a program

		// Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		_frameBuffer->glCreate(-1);			// Frame buffers don't automatically associate w/ a program

		// Heat Source Uniforms
		_heatSourceProgram->bind();
		_heatSourceProgram->bindUniform1i("frame0Texture", 0);

		// Heat Diffuse Uniform
		// DON'T CHANGE THIS!!! NEED TO FIND A WAY TO GET THE UNIFORM VALUE FOR THE GL_TEXTURE0!!!
		_heatDiffuseProgram->bind();
		_heatDiffuseProgram->bindUniform1i("frame0Texture", 0);  // GL_TEXTURE0 "texture unit" Required! USE INDEXES!! (During the run)
		_heatDiffuseProgram->bindUniform2("cellSizeUV", *_cellSizeUV);
		_heatDiffuseProgram->bindUniform1i("cellWidthUI", (int)_cellSizeUI->x);
		_heatDiffuseProgram->bindUniform1i("cellHeightUI", (int)_cellSizeUI->y);
		_heatDiffuseProgram->bindUniform2i("sceneSizeUI", *_sceneSizeUI);
		_heatDiffuseProgram->bindUniform1("weight", 0.1f);

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
		_periodCounter += millisecondsLapsed;

		// Wait until the period has elapsed
		//
		if (_periodCounter < _periodMilliseconds)
			return;

		_periodCounter = 0;

		brogueFlameMenu* mainMenu = _renderedView;

		// Update the rendering
		_renderedView->update(millisecondsLapsed);

		// Heat Source
		//
		simpleDataStream* heatSourceDataStream = brogueProgramBuilder::createSceneDataStream(mainMenu, 
																							 openglDataStreamType::brogueCellQuad, 
																							 openglBrogueCellOutputSelector::DisplayCurrentFrame,
		[&mainMenu] (short column, short row, brogueCellDisplay* cell)
		{
			return true; /*mainMenu->isTheText(column, row) || row == mainMenu->getBoundary().bottom();*/
		});

		// (MEMORY! deletes old buffer)
		_heatSourceProgram->bind();
		_heatSourceProgram->reBuffer(heatSourceDataStream);
	}
}
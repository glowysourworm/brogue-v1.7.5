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

		void compile() override;

		void update(int millisecondsLapsed) override;

		void run() override;
		bool hasErrors() override
		{
			bool errors = _heatSourceProgram->hasErrors();

			errors |= _heatDiffuseProgram->hasErrors();
			//errors |= _titleMaskProgram->hasErrors();
			//errors |= _frameProgram->hasErrors();
			//errors |= _frameCopyProgram->hasErrors();

			return errors;
		}

		gridRect getSceneBoundaryUI() const override
		{
			return *_sceneBoundaryUI;
		}

	private:

		void createStencilBuffer();
		void drawInsideStencilBuffer();
		void blendFrameBuffers();

	private:

		brogueFlameMenu* _renderedView;

		simpleShaderProgram* _frameCopyProgram;
		simpleShaderProgram* _heatSourceProgram;
		simpleShaderProgram* _heatDiffuseProgram;
		simpleShaderProgram* _titleMaskProgram;
		simpleShaderProgram* _frameProgram;		

		simpleFrameBuffer* _frameBuffer;

		simpleTexture* _frameTexture0;
		simpleTexture* _frameTexture1;

		vec2* _cellSizeUV;
		ivec2* _cellSizeUI;
		ivec2* _sceneSizeUI;

		gridRect* _sceneBoundaryUI;
	};

	brogueFlameMenuProgram::brogueFlameMenuProgram(brogueFlameMenu* mainMenu,
												   resourceController* resourceController)
	{
		_renderedView = mainMenu;

		int textureIndex = 0;

		gridRect sceneBoundaryUI = brogueProgramBuilder::calculateBoundaryUI(mainMenu);

		// This is for the uniform cellSize variable (see fragment shaders)
		//
		simpleQuad cellSizeUV = coordinateConverter::createQuadNormalizedUVScene(0, 0, brogueCellDisplay::CellWidth, brogueCellDisplay::CellHeight, sceneBoundaryUI.width, sceneBoundaryUI.height);

		_cellSizeUV = new vec2(cellSizeUV.bottomRight.x - cellSizeUV.topLeft.x, cellSizeUV.topLeft.y - cellSizeUV.bottomRight.y);
		_cellSizeUI = new ivec2(brogueCellDisplay::CellWidth, brogueCellDisplay::CellHeight);
		_sceneSizeUI = new ivec2(sceneBoundaryUI.width, sceneBoundaryUI.height);

		shaderData* backgroundColorVert = resourceController->getShader(shaderResource::backgroundColorVert);
		shaderData* backgroundColorFrag = resourceController->getShader(shaderResource::backgroundColorFrag);

		// Heat Source
		//
		simpleDataStream* heatSourceDataStream = brogueProgramBuilder::createSceneDataStream(mainMenu, openglDataStreamType::brogueColorQuad,
		[&mainMenu] (short column, short row, brogueCellDisplay* cell)
		{
			return mainMenu->isTheText(column, row) || row == mainMenu->getBoundary().bottom();
		});

		//shaderData* colorMaskVert = resourceController->getShader(shaderResource::colorMaskVert);
		//shaderData* colorMaskFrag = resourceController->getShader(shaderResource::colorMaskFrag);

		//// Title Text Mask
		////
		//simpleDataStream* colorMaskDataStream = brogueProgramBuilder::createSceneDataStream(mainMenu, openglDataStreamType::brogueColorQuad,
		//[&mainMenu](short column, short row, brogueCellDisplay* cell)
		//{
		//	return mainMenu->isTheText(column, row);
		//});

		shaderData* diffuseUpwardVert = resourceController->getShader(shaderResource::diffuseColorUpwardVert);
		shaderData* diffuseUpwardFrag = resourceController->getShader(shaderResource::diffuseColorUpwardFrag);
		simpleDataStream* diffuseUpwardDataStream = brogueProgramBuilder::createSceneDataStream(mainMenu, openglDataStreamType::brogueCellQuad);

		//shaderData* mixTexturesVert = resourceController->getShader(shaderResource::mixFrameTexturesVert);
		//shaderData* mixTexturesFrag = resourceController->getShader(shaderResource::mixFrameTexturesFrag);
		//simpleDataStream* mixTexturesDataStream = brogueProgramBuilder::createFrameDataStream(mainMenu, openglDataStreamType::brogueImageQuad);

		//// Reuse background color shaders - make a new frame data stream for the frame copy
		//simpleDataStream* backgroundColorFrameCopyStream = brogueProgramBuilder::createFrameDataStream(mainMenu, openglDataStreamType::brogueColorQuad);

		// (MEMORY!)
		//_frameCopyProgram = brogueProgramBuilder::createShaderProgram(backgroundColorFrameCopyStream, backgroundColorVert, backgroundColorFrag);
		_heatSourceProgram = brogueProgramBuilder::createShaderProgram(heatSourceDataStream, backgroundColorVert, backgroundColorFrag);
		_heatDiffuseProgram = brogueProgramBuilder::createShaderProgram(diffuseUpwardDataStream, diffuseUpwardVert, diffuseUpwardFrag);
		//_titleMaskProgram = brogueProgramBuilder::createShaderProgram(colorMaskDataStream, colorMaskVert, colorMaskFrag);
		//_frameProgram = brogueProgramBuilder::createShaderProgram(mixTexturesDataStream, mixTexturesVert, mixTexturesFrag);

		_frameTexture0 = new simpleTexture(NULL, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
		//_frameTexture1 = new simpleTexture(NULL, sceneBoundaryUI.width, sceneBoundaryUI.height, textureIndex++, GL_TEXTURE1, GL_RGBA, GL_UNSIGNED_BYTE);

		_frameBuffer = new simpleFrameBuffer(sceneBoundaryUI.width, sceneBoundaryUI.height);

		_sceneBoundaryUI = new gridRect(sceneBoundaryUI);
	}

	brogueFlameMenuProgram::~brogueFlameMenuProgram()
	{
		delete _frameCopyProgram;
		delete _heatSourceProgram;
		delete _heatDiffuseProgram;
		delete _titleMaskProgram;
		delete _frameProgram;
		delete _frameTexture0;
		delete _frameTexture1;
		delete _frameBuffer;
		delete _sceneBoundaryUI;
		delete _cellSizeUV;
		delete _cellSizeUI;
		delete _sceneSizeUI;
	}

	void brogueFlameMenuProgram::run()
	{
		if (!this->isCompiled)
			simpleException::show("Must first call IGLProgram.Compile() before using the GL program");

		if (!_frameBuffer->isReady())
		{
			//simpleException::show("Framebuffer status not ready for drawing!");
			return;
		}
			

		// Procedure:  
		//
		// 1) Copy data from the previous output into Texture 1
		//		-> glReadBuffer
		//		-> glClear(...)
		// 2) Draw heat source data over the top (Texture 1)
		// 3) "Diffuse" the colors onto Texture 0 (using Texture 1)
		// 4) Apply Title Text Mask (Texture 0)
		// 5) "Mix" the two textures to the frame buffer output (Texture 0)
		//

		// Pre-Draw the current rendering onto Color Attachment 1
		//glDrawBuffer(GL_COLOR_ATTACHMENT1);

		//_frameCopyProgram->bind(true);
		//_frameCopyProgram->drawAll();

		// Render the scene -> Color Attachment 0
		//_sceneProgram->bind(true);
		//_sceneProgram->drawAll();

		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		//GL.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		// Pre-draw using the heat diffuse program
		//_heatDiffuseProgram->bind(true);
		//_heatDiffuseProgram->drawAll();

		// Switch to Frame Buffer -> Color Attachment 0
		_frameBuffer->bind(true);

		// Activate Color Attachment 0
		glDrawBuffer(GL_COLOR_ATTACHMENT0);

		// Draw the entire frame -> Color Attachment 0
		//_frameProgram->bind(true);
		//_frameProgram->drawAll();

		// Render the heat sources as usual -> Color Attachment 0
		_heatSourceProgram->bind(true);
		_heatSourceProgram->drawAll();

		// Sample Color Attachment 0:  Create the "Diffusion" effect
		//_heatDiffuseProgram->bind(true);
		//_heatDiffuseProgram->drawAll();

		//glDrawBuffer(0);

		// Redirect output to show the result
		_frameBuffer->bind(false);

		// Sample Color Attachment 0:  Create the "Diffusion" effect
		_heatDiffuseProgram->bind(true);
		_heatDiffuseProgram->bindUniform2("cellSizeUV", *_cellSizeUV);
		_heatDiffuseProgram->bindUniform1i("cellWidthUI", (int)_cellSizeUI->x);
		_heatDiffuseProgram->bindUniform1i("cellHeightUI", (int)_cellSizeUI->y);
		_heatDiffuseProgram->bindUniform2i("sceneSizeUI", *_sceneSizeUI);
		_heatDiffuseProgram->bindUniform1("weight", 0.1f);
		_heatDiffuseProgram->drawAll();

		// Render Output
		//_frameProgram->bind(true);
		////_frameProgram->bindUniform1("weight", 0.9f);
		//_frameProgram->drawAll();

		//// Mask Output
		//_titleMaskProgram->bind(true);
		//_titleMaskProgram->bindUniform4("maskColor", vec4(0, 0, 0, 0));
		//_titleMaskProgram->drawAll();


		//glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		////GL.ClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		//glDisable(GL_DEPTH_TEST);

		//// Enable the frame buffer
		//_frameBuffer->bind(true);

		//// Activate Color Attachment 0
		//glDrawBuffer(GL_COLOR_ATTACHMENT0);

		//// Render the VISIBLE scene -> Color Attachment 0
		//_sceneProgram->bind(true);
		//_sceneProgram->drawAll();

		//// STENCIL TEST
		//glEnable(GL_STENCIL_TEST);

		//createStencilBuffer();

		//// Draw lighting to Color Attachment 1
		//glDrawBuffer(GL_COLOR_ATTACHMENT1);

		//drawInsideStencilBuffer();

		//// STENCIL TEST - DISABLE
		//glDisable(GL_STENCIL_TEST);

		////// Re-render BASE scene overwriting VISIBLE scene in Color Attachment 0

		////// Activate Color Attachment 0
		////glDrawBuffer(GL_COLOR_ATTACHMENT0);

		////_sceneProgram->bind(true);
		////_sceneProgram->drawAll();

		//// Blend Color Attachment 1 -> Color Attachment 0
		//blendFrameBuffers();

		//// Render the frame buffer contents
		//_frameBuffer->bind(false);
		//_frameProgram->bind(true);
		//_frameProgram->drawAll();
	}

	void brogueFlameMenuProgram::compile()
	{
		//_frameCopyProgram->compile();
		_heatSourceProgram->compile();
		_heatDiffuseProgram->compile();
		//_titleMaskProgram->compile();
		//_frameProgram->compile();

		//_frameCopyProgram->bind(true);
		_heatSourceProgram->bind(true);
		_heatDiffuseProgram->bind(true);
		//_titleMaskProgram->bind(true);
		//_frameProgram->bind(true);

		// Frame Copy Program:  Draws data directly from the previous draw pass.

		// Create the textures:  Texture 1 is used for the direct drawing, Texture 0 for the "color diffusion"
		_frameTexture0->glCreate(-1);		// Textures don't automatically associate w/ a program
		////_frameTexture1->glCreate(-1);

		// Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		_frameBuffer->glCreate(-1);			// Frame buffers don't automatically associate w/ a program

		// Cell Size Uniform
		_heatDiffuseProgram->bind(true);
		//_heatDiffuseProgram->bindUniform1i("frame0Texture", _frameTexture0->getHandle());  // GL_TEXTURE0 "texture unit" Required!
		_heatDiffuseProgram->bindUniform2("cellSizeUV", *_cellSizeUV);
		_heatDiffuseProgram->bindUniform1i("cellWidthUI", (int)_cellSizeUI->x);
		_heatDiffuseProgram->bindUniform1i("cellHeightUI", (int)_cellSizeUI->y);
		_heatDiffuseProgram->bindUniform2i("sceneSizeUI", *_sceneSizeUI);
		_heatDiffuseProgram->bindUniform1("weight", 0.1f);

		//_frameProgram->bind(true);
		//_frameProgram->bindUniform1i("frame0Texture", _frameTexture0->getHandle());		   // GL_TEXTURE0 "texture unit" Required!


		//_frameProgram->bindUniform1i("frame1Texture", _frameTexture1->getHandle());

		// Attach texture to frame buffer
		_frameBuffer->attachTexture(_frameTexture0->getHandle(), GL_COLOR_ATTACHMENT0);
		//_frameBuffer->attachTexture(_frameTexture1->getHandle(), GL_COLOR_ATTACHMENT1);
		_frameBuffer->attachRenderBuffer();

		//_frameBuffer1->attachTexture(_frameBufferTexture1->getHandle(), GL_COLOR_ATTACHMENT1);
		//_frameBuffer1->attachRenderBuffer();

		this->isCompiled = true;
	}

	void brogueFlameMenuProgram::createStencilBuffer()
	{
		//// Disable the color writes for the frame buffer
		//glColorMask(false, false, false, false);

		//// Setup stencil buffer:  REPLACE on PASS (per pixel) with ref = 0.
		//// 
		//// https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glStencilFunc.xhtml
		////
		//glStencilFunc(GL_ALWAYS, 0, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glStencilMask(0xFF);

		//// Draw the mask to create the stencil buffer (0's where there is any visible color)
		//_frameBlendProgram->bind(true);
		//_frameBlendProgram->drawAll();

		//// Reset the color buffer and set up the next rendering pass to
		//// render just where the test passed
		//glColorMask(true, true, true, true);
		//glStencilMask(0x00);
	}

	void brogueFlameMenuProgram::drawInsideStencilBuffer()
	{
		//// Draw the base scene INSIDE THE STENCIL! (We're going to be deleting)
		//glStencilFunc(GL_EQUAL, 0, 0xFF);

		//// Draw the frame blend program
		//_frameBlendProgram->bind(true);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA);
		//glBlendEquation(GL_MAX);

		//glDisable(GL_BLEND);
	}

	void brogueFlameMenuProgram::blendFrameBuffers()
	{
		//_frameBlendProgram->bind(true);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendEquation(GL_FUNC_ADD);

		//_frameBlendProgram->drawAll();

		//glDisable(GL_BLEND);
	}

	void brogueFlameMenuProgram::update(int millisecondsLapsed)
	{
		// Update the rendering
		//_renderedView->update(millisecondsLapsed);

		// Stream the new data
		//simpleDataStream<float>* sceneDataStream = brogueSceneBuilder::prepareSceneDataStream(_renderedView);
		//simpleDataStream<float>* frameDataStream = brogueSceneBuilder::prepareFrameDataStream(_renderedView);
		//simpleDataStream<float>* frameBlendDataStream = brogueSceneBuilder::prepareFrameDataStream(_renderedView);

		// Get rid of old stream / buffer new stream
		//_sceneProgram->bind(false);
		//_frameProgram->bind(false);
		//_frameBlendProgram->bind(false);

		//_sceneProgram->reBuffer(0, sceneDataStream);
		//_frameProgram->reBuffer(0, frameDataStream);
		//_frameBlendProgram->reBuffer(0, frameBlendDataStream);

		// SPECIFIC! (re-design) (MEMORY! deletes old buffer)
		//_sceneProgram->bind(true);
		//_frameProgram->bind(true);
		//_frameBlendProgram->bind(true);
	}
}
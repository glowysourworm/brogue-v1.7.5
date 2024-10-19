#pragma once

#include "brogueProgram.h"
#include "simpleShaderProgram.h"
#include "simpleTexture.h"
#include "simpleFrameBuffer.h"
#include "brogueFlameMenu.h"
#include "brogueSceneBuilder.h"

using namespace brogueHd::frontend::opengl;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueFlameMenuProgram : brogueProgram
	{
	public:
		
		brogueFlameMenuProgram(simpleShaderProgram* sceneProgram,
								//simpleShaderProgram* frameProgram,
								//simpleShaderProgram* frameBlendProgram,
								//simpleFrameBuffer* frameBuffer,
								brogueFlameMenu* renderedView,
								//const simpleTexture& frameTexture,
								//const simpleTexture& frameBlendTexture,
								//const simpleUniform<int>& frameTextureUniform,
								//const simpleUniform<int>& frameBlendTextureUniform,
								const gridRect& sceneBoundaryUI);
		~brogueFlameMenuProgram();

		void compile() override;

		void update(int millisecondsLapsed) override;

		void run() override;
		bool hasErrors() override
		{
			return _sceneProgram->hasErrors();
		}

		gridRect getSceneBoundaryUI() const override
		{
			return _sceneBoundaryUI;
		}

	private:

		brogueFlameMenu* _renderedView;

		simpleShaderProgram* _sceneProgram;
		simpleShaderProgram* _frameProgram;
		simpleShaderProgram* _frameBlendProgram;
		simpleFrameBuffer* _frameBuffer;

		simpleTexture _frameTexture;
		simpleTexture _frameBlendTexture;

		simpleUniform<int> _frameTextureUniform;
		simpleUniform<int> _frameBlendTextureUniform;

		gridRect _sceneBoundaryUI;
	};

	brogueFlameMenuProgram::brogueFlameMenuProgram(simpleShaderProgram* sceneProgram,
												   //simpleShaderProgram* frameProgram,
												   //simpleShaderProgram* frameBlendProgram,
												   //simpleFrameBuffer* frameBuffer,
												   brogueFlameMenu* renderedView,
												   //const simpleTexture& frameTexture,
												   //const simpleTexture& frameBlendTexture,
												   //const simpleUniform<int>& frameTextureUniform,
												   //const simpleUniform<int>& frameBlendTextureUniform,
												   const gridRect& sceneBoundaryUI)
	{
		_sceneProgram = sceneProgram;
		//_frameProgram = frameProgram;
		//_frameBlendProgram = frameBlendProgram;

		//_frameBuffer = frameBuffer;

		//_frameTexture = frameTexture;
		//_frameBlendTexture = frameBlendTexture;

		//_frameBlendTextureUniform = frameBlendTextureUniform;
		//_frameTextureUniform = frameTextureUniform;

		_renderedView = renderedView;

		_sceneBoundaryUI = sceneBoundaryUI;
	}

	brogueFlameMenuProgram::~brogueFlameMenuProgram()
	{
	}

	void brogueFlameMenuProgram::run()
	{
		if (!this->isCompiled)
			simpleException::show("Must first call IGLProgram.Compile() before using the GL program");

		//if (!_frameBuffer->isReady())
		//	simpleException::show("Framebuffer status not ready for drawing!");

		// Procedure:  Two color attachments to the frame buffer - color attachment 1 used for blending
		//			
		// NOTE:       This routine should be easily modified for the actual brogue game. It was used in
		//			   another program to blend the back buffer with the fore buffer. We could try the same
		//			   here to see if it helps smooth out the flame drawing.
		//
		// 1) Bind the frame buffer
		// 2) Render scene to frame texture
		// 3) BLEND frame texture buffer with frame buffer
		// 4) De-activate frame buffer
		// 5) Render to screen (using frame program)
		//

		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

		//// Enable the frame buffer
		//_frameBuffer->bind(true);

		//// Activate Color Attachment 1 (Color Attachment 0 was used for blending)
		//glDrawBuffer(GL_COLOR_ATTACHMENT1);

		// Render the VISIBLE scene -> Color Attachment 0
		_sceneProgram->bind(true);
		_sceneProgram->drawAll();

		//// Blend Color Attachment 1 -> Color Attachment 0
		//_frameBlendProgram->bind(true);

		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendEquation(GL_FUNC_ADD);

		//_frameBlendProgram->drawAll();

		//glDisable(GL_BLEND);

		//// Render the frame buffer contents
		//_frameBuffer->bind(false);
		//_frameProgram->bind(true);
		//_frameProgram->drawAll();
	}

	void brogueFlameMenuProgram::compile()
	{
		_sceneProgram->compile();
		//_frameProgram->compile();
		//_frameBlendProgram->compile();

		//// Create the textures:  (NOTE** Program handle not used currently)
		//_frameTexture.glCreate(_frameProgram->getHandle());
		//_frameBlendTexture.glCreate(_frameBlendProgram->getHandle());

		//// Declare Uniforms
		//_frameProgram->bind(true);
		//_frameProgram->bindUniform(_frameTextureUniform);

		//_frameBlendProgram->bind(true);
		//_frameBlendProgram->bindUniform(_frameBlendTextureUniform);

		//// Create Frame buffer:  Uses scene program to render to the frame buffer attached texture
		//_frameBuffer->glCreate(_sceneProgram->getHandle());

		//// Attach texture to frame buffer
		//_frameBuffer->attachTexture(_frameTexture.getHandle(), GL_COLOR_ATTACHMENT0);
		//_frameBuffer->attachTexture(_frameBlendTexture.getHandle(), GL_COLOR_ATTACHMENT1);
		//_frameBuffer->attachRenderBuffer();

		this->isCompiled = true;
	}

	void brogueFlameMenuProgram::update(int millisecondsLapsed)
	{
		// Update the rendering
		_renderedView->update(millisecondsLapsed);

		// Stream the new data
		simpleDataStream<float>* sceneDataStream = brogueSceneBuilder::prepareSceneDataStream(_renderedView);
		//simpleDataStream<float>* frameDataStream = brogueSceneBuilder::prepareFrameDataStream(_renderedView);
		//simpleDataStream<float>* frameBlendDataStream = brogueSceneBuilder::prepareFrameDataStream(_renderedView);

		// Get rid of old stream / buffer new stream
		_sceneProgram->bind(false);
		//_frameProgram->bind(false);
		//_frameBlendProgram->bind(false);

		_sceneProgram->reBuffer(0, sceneDataStream);
		//_frameProgram->reBuffer(0, frameDataStream);
		//_frameBlendProgram->reBuffer(0, frameBlendDataStream);

		// SPECIFIC! (re-design) (MEMORY! deletes old buffer)
		_sceneProgram->bind(true);
		//_frameProgram->bind(true);
		//_frameBlendProgram->bind(true);
	}
}
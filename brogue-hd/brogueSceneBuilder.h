#pragma once

#include "gridDefinitions.h"
#include "simpleArrayExtension.h"
#include "brogueCellDisplay.h"
#include "brogueView.h"

#include "coordinateConverter.h"
#include "simpleShader.h"
#include "shaderData.h"
#include "simpleShaderProgram.h"
#include "simpleVertexArray.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueSceneBuilder
	{
	public:

		/// <summary>
		/// The scene data is the data from a brogueView. Using quads, this data is streamed out to our simpleDataStream
		/// object to hold for the GL backend calls.
		/// </summary>
		static simpleDataStream<float> prepareSceneDataStream(const brogueView& view)
		{
			// Starting with the raw data, build a simpleQuad data vector to pass to the simpleDataStream<float>
			//
			simpleList<simpleQuad> cellQuads;
			int cellWidth = 10;
			int cellHeight = 15;

			view.iterate([&cellQuads, &cellWidth, &cellHeight](short column, short row, brogueCellDisplay cell)
			{
				cellQuads.add(simpleQuad(column * cellWidth, row * cellHeight, (column + 1) * cellWidth, (row + 1) * cellHeight));

				return iterationCallback::iterate;
			});

			// Create Scene Data Streams

			// Element Size:   Total number of primitives (floats) to commit to the stream
			// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
			//

			// Scene Base: Must declare before streaming the data onto it
			simpleDataStream<float> dataStream(cellQuads.count(), cellQuads.first().getElementSize(GL_QUADS), cellQuads.first().getStreamSize());

			// Stream the data for output
			cellQuads.forEach([&dataStream](simpleQuad quad)
			{
				quad.streamBuffer(dataStream);
				return iterationCallback::iterate;
			});

			return dataStream;
		}

		/// <summary>
		/// The Frame of the scene is essentially the buffer that owns the scene's rendering output. It must be declared before 
		/// it is used during rendering.
		/// </summary>
		static simpleDataStream<float> prepareFrameDataStream(const brogueView& view)
		{
			gridRect sceneBoundary = view.getSceneBoundary();

			// Frame Data (for the scene)
			simpleQuad frameQuad = coordinateConverter::createQuadNormalizedXYScene(0, 0, sceneBoundary.width, sceneBoundary.height, sceneBoundary.width, sceneBoundary.height);

			// Create Scene Data Streams

			// Element Size:   Total number of primitives (floats) to commit to the stream
			// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
			//

			// Frame
			simpleDataStream<float> dataStream(1, frameQuad.getElementSize(GL_QUADS), frameQuad.getStreamSize());

			// Transfer data to the stream
			frameQuad.streamBuffer(dataStream);

			return dataStream;
		}
		
		/// <summary>
		/// Creates the program for working with the frame VAO on the GL backend. This should be used to render scene data to before
		/// calling routines to show the frame buffer.
		/// </summary>
		static simpleShaderProgram* createFrameShaderProgram(const simpleDataStream<float>& frameDataStream,
															 const shaderData& vertexData,
															 const shaderData& fragmentData)
		{
			// Create Shaders
			simpleShader vertexShader(vertexData.source);
			simpleShader fragmentShader(fragmentData.source);

			// Shaders / Vertex Attributes
			
			int vertexBufferIndex = 0;

			// These are very similar data structures; but please leave this transfer until the GL backend is better understood
			//
			simpleArray<simpleVertexAttribute> frameVertexAttributes = simpleArrayExtension::select<vertexAttributeData, simpleVertexAttribute>(
				*vertexData.attributes, [](vertexAttributeData data)
				{
					return simpleVertexAttribute(data.index, data.name, data.type);
				});

			simpleArray<simpleVertexAttribute> frameFragmentAttributes = simpleArrayExtension::select<vertexAttributeData, simpleVertexAttribute>(
				*fragmentData.attributes, [](vertexAttributeData data) 
				{
					return simpleVertexAttribute(data.index, data.name, data.type);
				});


			simpleVertexBuffer<float> frameVBO(vertexBufferIndex++, frameDataStream, frameVertexAttributes);
			simpleVertexArray<float> frameVAO(GL_QUADS, frameVBO);

			simpleShaderProgram* program = new simpleShaderProgram(vertexShader, fragmentShader);

			program->declareVAO(frameVAO);

			return program;
		}

        
		/// <summary>
		/// Creates program for rendering the scene's data to a buffer on the GL backend
		/// </summary>
		static simpleShaderProgram* createSceneShaderProgram(const simpleDataStream<float>& sceneDataStream,
															 const shaderData& vertexData,
															 const shaderData& fragmentData)
		{
			// Create Shaders
			simpleShader vertexShader(vertexData.source);
			simpleShader fragmentShader(fragmentData.source);

			// Shaders / Vertex Attributes

			int vertexBufferIndex = 0;

			// These are very similar data structures; but please leave this transfer until the GL backend is better understood
			//
			simpleArray<simpleVertexAttribute> vertexAttributes = simpleArrayExtension::select<vertexAttributeData, simpleVertexAttribute>(
				*vertexData.attributes, [](vertexAttributeData data)
				{
					return simpleVertexAttribute(data.index, data.name, data.type);
				});

			simpleArray<simpleVertexAttribute> fragmentAttributes = simpleArrayExtension::select<vertexAttributeData, simpleVertexAttribute>(
				*fragmentData.attributes, [](vertexAttributeData data)
				{
					return simpleVertexAttribute(data.index, data.name, data.type);
				});

			simpleVertexBuffer<float> sceneVBO(vertexBufferIndex++, sceneDataStream, vertexAttributes);
			simpleVertexArray<float> sceneVAO(GL_QUADS, sceneVBO);

			simpleShaderProgram* program = new simpleShaderProgram(vertexShader, fragmentShader);

			program->declareVAO(sceneVAO);

			return program;
		}
	};
}
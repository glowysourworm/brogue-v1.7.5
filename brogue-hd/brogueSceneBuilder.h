#pragma once

#include "gridDefinitions.h"
#include "simpleArrayExtension.h"
#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueImageQuad.h"
#include "brogueView.h"

#include "coordinateConverter.h"
#include "simpleShader.h"
#include "shaderData.h"
#include "brogueValueQuad.h"
#include "simpleShaderProgram.h"
#include "simpleVertexArray.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::model::layout;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueSceneBuilder
	{
	public:

		/// <summary>
		/// Calculates the scene boundary is UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		static gridRect calculateSceneBoundary(const brogueView* view)
		{
			gridRect viewBoundary = view->getParentBoundary();
			gridRect sceneBoundaryUI = gridRect(viewBoundary.left() * brogueCellDisplay::CellWidth,
												viewBoundary.right() * brogueCellDisplay::CellHeight,
												viewBoundary.width * brogueCellDisplay::CellWidth,
												viewBoundary.height * brogueCellDisplay::CellHeight);

			return sceneBoundaryUI;
		}

		/// <summary>
		/// (MEMORY!) The scene data is the data from a brogueView. Using quads, this data is streamed out to our simpleDataStream
		/// object to hold for the GL backend calls.
		/// </summary>
		static simpleDataStream<float>* prepareSceneDataStream(const brogueView* view)
		{
			// Starting with the raw data, build a simpleQuad data vector to pass to the simpleDataStream<float>
			//
			gridRect sceneBoundary = calculateSceneBoundary(view);

			simpleList<brogueCellQuad> cellQuads;

			view->iterate([&sceneBoundary, &cellQuads](short column, short row, brogueCellDisplay* cell)
			{
				simpleQuad quad = coordinateConverter::createQuadNormalizedXYScene(GL_TRIANGLES, column * brogueCellDisplay::CellWidth,
																						 row * brogueCellDisplay::CellHeight,
																						 brogueCellDisplay::CellWidth,
																						 brogueCellDisplay::CellHeight,
																						 sceneBoundary.width, 
																						 sceneBoundary.height);

				cellQuads.add(brogueCellQuad(*cell));

				return iterationCallback::iterate;
			});

			// Create Scene Data Streams

			// Element Size:   Total number of primitives (floats) to commit to the stream
			// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
			//

			// Scene Base: Must declare before streaming the data onto it
			simpleDataStream<float>* dataStream = new simpleDataStream<float>(cellQuads.count(), 
																			  cellQuads.first().getElementSize(GL_TRIANGLES), 
																			  cellQuads.first().getStreamSize(GL_TRIANGLES));

			// Stream the data for output
			cellQuads.forEach([&dataStream](brogueCellQuad quad)
			{
				quad.streamBuffer(GL_TRIANGLES, *dataStream);
				return iterationCallback::iterate;
			});

			return dataStream;
		}

		/// <summary>
		/// (MEMORY!) The Frame of the scene is essentially the buffer that owns the scene's rendering output. It must be declared before 
		/// it is used during rendering.
		/// </summary>
		static simpleDataStream<float>* prepareFrameDataStream(const brogueView* view)
		{
			gridRect sceneBoundary = calculateSceneBoundary(view);

			// Create Scene Data Streams

			// Element Size:   Total number of primitives (floats) to commit to the stream
			// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
			//

			simpleQuad vertexQuad = coordinateConverter::createQuadNormalizedXYScene(GL_TRIANGLES, 0, 0, sceneBoundary.width, sceneBoundary.height, sceneBoundary.width, sceneBoundary.height);
			simpleQuad textureQuad = coordinateConverter::createQuadNormalizedUVScene(GL_TRIANGLES, 0, 0, sceneBoundary.width, sceneBoundary.height, sceneBoundary.width, sceneBoundary.height);

			// Use image quad to prepare the stream
			brogueImageQuad imageQuad(vertexQuad, textureQuad);

			// Frame
			simpleDataStream<float>* dataStream = new simpleDataStream<float>(1, imageQuad.getElementSize(GL_TRIANGLES), imageQuad.getStreamSize(GL_TRIANGLES));

			// Transfer data to the stream
			imageQuad.streamBuffer(GL_TRIANGLES, *dataStream);

			return dataStream;
		}
		
		/// <summary>
		/// (MEMORY!) Creates the program for working with the frame VAO on the GL backend. This should be used to render scene data to before
		/// calling routines to show the frame buffer.
		/// </summary>
		static simpleShaderProgram* createFrameShaderProgram(simpleDataStream<float>* frameDataStream,
															 const shaderData& vertexData,
															 const shaderData& fragmentData)
		{
			// Create Shaders
			simpleShader vertexShader(vertexData);
			simpleShader fragmentShader(fragmentData);

			// Shaders / Vertex Attributes
			
			int vertexBufferIndex = 0;

			// These are very similar data structures; but please leave this transfer until the GL backend is better understood
			//
			simpleList<simpleVertexAttribute> frameVertexAttributes = vertexData.attributes->select<simpleVertexAttribute>([](vertexAttributeData data)
			{
				return simpleVertexAttribute(data.index, data.name, data.type);
			});

			simpleList<simpleVertexAttribute> frameFragmentAttributes = fragmentData.attributes->select<simpleVertexAttribute>([](vertexAttributeData data)
			{
				return simpleVertexAttribute(data.index, data.name, data.type);
			});


			simpleVertexBuffer<float>* frameVBO = new simpleVertexBuffer<float>(vertexBufferIndex++, frameDataStream, frameVertexAttributes);
			simpleVertexArray<float>* frameVAO = new simpleVertexArray<float>(GL_TRIANGLES, frameVBO);

			simpleShaderProgram* program = new simpleShaderProgram(vertexShader, fragmentShader);

			program->declareVAO(frameVAO);

			return program;
		}
		/// <summary>
		/// (MEMORY!) Creates program for rendering the scene's data to a buffer on the GL backend
		/// </summary>
		static simpleShaderProgram* createSceneShaderProgram(simpleDataStream<float>* sceneDataStream,
															 const shaderData& vertexData,
															 const shaderData& fragmentData)
		{
			// Create Shaders
			simpleShader vertexShader(vertexData);
			simpleShader fragmentShader(fragmentData);

			// Shaders / Vertex Attributes

			int vertexBufferIndex = 0;

			// These are very similar data structures; but please leave this transfer until the GL backend is better understood
			//
			simpleList<simpleVertexAttribute> vertexAttributes = vertexData.attributes->select<simpleVertexAttribute>([](vertexAttributeData data)
			{
				return simpleVertexAttribute(data.index, data.name, data.type);
			});

			simpleList<simpleVertexAttribute> fragmentAttributes = fragmentData.attributes->select<simpleVertexAttribute>([](vertexAttributeData data)
			{
				return simpleVertexAttribute(data.index, data.name, data.type);
			});

			simpleVertexBuffer<float>* sceneVBO = new simpleVertexBuffer<float>(vertexBufferIndex++, sceneDataStream, vertexAttributes);
			simpleVertexArray<float>* sceneVAO = new simpleVertexArray<float>(GL_TRIANGLES, sceneVBO);

			simpleShaderProgram* program = new simpleShaderProgram(vertexShader, fragmentShader);

			program->declareVAO(sceneVAO);

			return program;
		}
	};
}
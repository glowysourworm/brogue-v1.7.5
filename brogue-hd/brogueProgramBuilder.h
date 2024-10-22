#pragma once

#include "gridDefinitions.h"
#include "simpleArrayExtension.h"
#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueImageQuad.h"
#include "brogueView.h"
#include "gridDefinitions.h"

#include "coordinateConverter.h"
#include "simpleShader.h"
#include "shaderData.h"
#include "simpleShaderProgram.h"
#include "simpleVertexArray.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;
using namespace brogueHd::frontend::ui;

namespace brogueHd::frontend::opengl
{
	class brogueProgramBuilder
	{
	public:

		/// <summary>
		/// Calculates the scene boundary is UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		static gridRect calculateBoundaryUI(const brogueView* view)
		{
			gridRect viewBoundary = view->getParentBoundary();
			gridRect boundaryUI = gridRect(viewBoundary.left() * brogueCellDisplay::CellWidth,
											viewBoundary.right() * brogueCellDisplay::CellHeight,
											viewBoundary.width * brogueCellDisplay::CellWidth,
											viewBoundary.height * brogueCellDisplay::CellHeight);

			return boundaryUI;
		}

		/// <summary>
		/// (MEMORY!) The scene data is the data from a brogueView. Using quads, this data is streamed out to our simpleDataStream
		/// object to hold for the GL backend calls.
		/// </summary>
		static simpleDataStream<float>* createSceneDataStream(const brogueView* view, openglDataStreamType dataType)
		{
			// Starting with the raw data, build a simpleQuad data vector to pass to the simpleDataStream<float>
			//
			gridRect sceneBoundary = calculateBoundaryUI(view);

			// Problems with polymorphism:  Copy constructors not working for child structs. May be a struct issue.
			simpleList<brogueImageQuad> imageQuads;
			simpleList<brogueCellQuad> cellQuads;
			simpleList<brogueColorQuad> colorQuads;

			// Iterator scope could be removed; but want to be able to handle the root issue. Should be able to copy data
			// up the stack.
			//
			view->iterate([&sceneBoundary, &cellQuads, &colorQuads, &imageQuads, &view, &dataType](short column, short row, brogueCellDisplay* cell)
			{
				switch (dataType)
				{
				case openglDataStreamType::brogueImageQuad:
					imageQuads.add(coordinateConverter::createBrogueImageQuadScene(*cell, column, row, sceneBoundary.width, sceneBoundary.height));
					break;
				case openglDataStreamType::brogueCellQuad:
					cellQuads.add(coordinateConverter::createBrogueCellQuadScene(*cell, column, row, sceneBoundary.width, sceneBoundary.height));
					break;
				case openglDataStreamType::brogueColorQuad:
					colorQuads.add(coordinateConverter::createBrogueColorQuadScene(*cell, column, row, sceneBoundary.width, sceneBoundary.height));
					break;
				default:
					simpleException::show("Unhandled openglDataStreamType:  brogueProgramBuilder.h");
				}

				return iterationCallback::iterate;
			});

			// Create Scene Data Streams

			// Element Size:   Total number of primitives (floats) to commit to the stream
			// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
			//

			simpleDataStream<float>* dataStream = nullptr;

			switch (dataType)
			{
			case openglDataStreamType::brogueImageQuad:
			{
				// (MEMORY!) Scene Base: Must declare before streaming the data onto it
				dataStream = new simpleDataStream<float>(imageQuads.count(),
														 imageQuads.first().getElementSize(GL_TRIANGLES),
														 imageQuads.first().getStreamSize(GL_TRIANGLES));

				imageQuads.forEach([&dataStream] (brogueImageQuad quad) 
				{
					quad.streamBuffer(GL_TRIANGLES, dataStream);
					return iterationCallback::iterate;
				});
			}
			break;
			case openglDataStreamType::brogueCellQuad:
			{
				// (MEMORY!) Scene Base: Must declare before streaming the data onto it
				dataStream = new simpleDataStream<float>(cellQuads.count(),
														cellQuads.first().getElementSize(GL_TRIANGLES),
														cellQuads.first().getStreamSize(GL_TRIANGLES));

				cellQuads.forEach([&dataStream] (brogueCellQuad quad)
				{
					quad.streamBuffer(GL_TRIANGLES, dataStream);
					return iterationCallback::iterate;
				});
			}
			break;
			case openglDataStreamType::brogueColorQuad:
			{
				// (MEMORY!) Scene Base: Must declare before streaming the data onto it
				dataStream = new simpleDataStream<float>(colorQuads.count(),
					colorQuads.first().getElementSize(GL_TRIANGLES),
					colorQuads.first().getStreamSize(GL_TRIANGLES));

				colorQuads.forEach([&dataStream] (brogueColorQuad quad)
				{
					quad.streamBuffer(GL_TRIANGLES, dataStream);
					return iterationCallback::iterate;
				});
			}
			break;
			default:
				simpleException::show("Unhandled openglDataStreamType:  brogueProgramBuilder.h");
				break;
			}

			return dataStream;
		}

		/// <summary>
		/// (MEMORY!) The Frame of the scene is essentially the buffer that owns the scene's rendering output. It must be declared before 
		/// it is used during rendering.
		/// </summary>
		static simpleDataStream<float>* createFrameDataStream(const brogueView* view, openglDataStreamType dataType)
		{
			gridRect sceneBoundary = calculateBoundaryUI(view);

			simpleGlData frameData;

			switch (dataType)
			{
			case openglDataStreamType::brogueImageQuad:
				frameData = coordinateConverter::createBrogueImageQuadFrame(0, 0, sceneBoundary.width, sceneBoundary.height);
				break;
			case openglDataStreamType::brogueColorQuad:
				frameData = coordinateConverter::createBrogueColorQuadFrame(colors::black(), 0, 0, sceneBoundary.width, sceneBoundary.height);
				break;
			case openglDataStreamType::brogueCellQuad:
			default:
				simpleException::show("Unhandled openglDataStreamType:  brogueProgramBuilder.h");
			}

			// Create Scene Data Streams

			// Element Size:   Total number of primitives (floats) to commit to the stream
			// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
			//
			
			simpleDataStream<float>* dataStream = new simpleDataStream<float>(1, frameData.getElementSize(GL_TRIANGLES), frameData.getStreamSize(GL_TRIANGLES));

			// Transfer data to the stream
			frameData.streamBuffer(GL_TRIANGLES, dataStream);

			return dataStream;
		}

		/// <summary>
		/// (MEMORY!) Creates program for rendering the scene's data to a buffer on the GL backend
		/// </summary>
		static simpleShaderProgram* createShaderProgram(simpleDataStream<float>* sceneDataStream,
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
			simpleList<simpleVertexAttribute> vertexAttributes = vertexData.attributes.select<simpleVertexAttribute>([](vertexAttributeData data)
			{
				return simpleVertexAttribute(data.index, data.name, data.type);
			});

			simpleList<simpleVertexAttribute> fragmentAttributes = fragmentData.attributes.select<simpleVertexAttribute>([](vertexAttributeData data)
			{
				return simpleVertexAttribute(data.index, data.name, data.type);
			});

			simpleVertexBuffer<float>* programVBO = new simpleVertexBuffer<float>(vertexBufferIndex++, sceneDataStream, vertexAttributes);
			simpleVertexArray<float>* programVAO = new simpleVertexArray<float>(GL_TRIANGLES, programVBO);

			simpleShaderProgram* program = new simpleShaderProgram(vertexShader, fragmentShader);

			program->declareVAO(programVAO);

			return program;
		}
	};
}
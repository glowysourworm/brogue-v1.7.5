#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueGlyphMap.h"
#include "brogueImageQuad.h"
#include "gridDefinitions.h"
#include "openglQuadConverter.h"
#include "resourceController.h"

#include "brogueAdjacencyColorQuad.h"
#include "brogueColorQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueUIConstants.h"
#include "brogueViewBase.h"
#include "color.h"
#include "gl.h"
#include "gridRect.h"
#include "shaderData.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleList.h"
#include "simpleShader.h"
#include "simpleShaderProgram.h"
#include "simpleVertexArray.h"
#include "simpleVertexAttribute.h"
#include "simpleVertexBuffer.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend;

namespace brogueHd::frontend
{
	class brogueDataStreamBuilder
	{
	public:

		brogueDataStreamBuilder(resourceController* resourceController, brogueGlyphMap* glyphMap);
		~brogueDataStreamBuilder();

		/// <summary>
		/// (MEMORY!) The scene data is the data from a brogueView. Using quads, this data is streamed out to our simpleDataStream
		/// object to hold for the GL backend calls.
		/// </summary>
		simpleDataStream* createSceneDataStream(const brogueViewBase* view, openglDataStreamType dataType, openglBrogueCellOutputSelector noDisplayOutputSelector);

		/// <summary>
		/// (MEMORY!) The Frame of the scene is essentially the buffer that owns the scene's rendering output. It must be declared before 
		/// it is used during rendering.
		/// </summary>
		simpleDataStream* createFrameDataStream(const brogueViewBase* view, openglDataStreamType dataType);

		/// <summary>
		/// Recreates scene data stream into the original buffer. MUST BE SAME SIZE / FORMAT!
		/// </summary>
		void recreateSceneDataStream(const brogueViewBase* view,
									 simpleDataStream* dataStream,
									 openglDataStreamType dataType,
									 openglBrogueCellOutputSelector noDisplayOutputSelector);

		/// <summary>
		/// Recreates frame data stream into the original buffer. MUST BE SAME SIZE / FORMAT!
		/// </summary>
		void recreateFrameDataStream(const brogueViewBase* view,
									 simpleDataStream* dataStream,
									 openglDataStreamType dataType);


		/// <summary>
		/// (MEMORY!) Creates program for rendering the scene's data to a buffer on the GL backend
		/// </summary>
		simpleShaderProgram* createShaderProgram(simpleDataStream* sceneDataStream,
												 shaderData* vertexData,
												 shaderData* fragmentData);

		/// <summary>
		/// Creates a coordinate converter for openGL backend coordinate transformations
		/// </summary>
		brogueCoordinateConverter createCoordinateConverter(int sceneWidth, int sceneHeight, int zoomLevel) const;

	private:

		simpleDataStream* initializeSceneDataStream(const brogueViewBase* view, openglDataStreamType dataType, openglBrogueCellOutputSelector noDisplayOutputSelector);
		simpleDataStream* initializeFrameDataStream(const brogueViewBase* view, openglDataStreamType dataType);

	private:

		resourceController* _resourceController;
		brogueGlyphMap* _glyphMap;

	};

	brogueDataStreamBuilder::brogueDataStreamBuilder(resourceController* resourceController, brogueGlyphMap* glyphMap)
	{
		_resourceController = resourceController;
		_glyphMap = glyphMap;
	}
	brogueDataStreamBuilder::~brogueDataStreamBuilder()
	{
	}

	brogueCoordinateConverter brogueDataStreamBuilder::createCoordinateConverter(int sceneWidth, int sceneHeight, int zoomLevel) const
	{
		int glyphSheetWidth = _resourceController->getFontGlyphs(zoomLevel)->pixelWidth();
		int glyphSheetHeight = _resourceController->getFontGlyphs(zoomLevel)->pixelHeight();

		float glyphWidth = glyphSheetWidth / (float)_glyphMap->GlyphSheetColumns;
		float glyphHeight = glyphSheetHeight / (float)(_glyphMap->GlyphSheetRows + _glyphMap->GlyphSheetRowOffset);

		openglQuadConverter glyphMap(glyphWidth, glyphHeight, glyphSheetWidth, glyphSheetHeight);
		openglQuadConverter viewMap(brogueCellDisplay::CellWidth(zoomLevel), brogueCellDisplay::CellHeight(zoomLevel), sceneWidth, sceneHeight);

		return brogueCoordinateConverter(glyphMap, viewMap);
	}

	simpleDataStream* brogueDataStreamBuilder::initializeSceneDataStream(const brogueViewBase* view, openglDataStreamType dataType, openglBrogueCellOutputSelector noDisplayOutputSelector)
	{
		simpleDataStream* result;
		int cellCount = 0;

		// Pre-count the cells
		view->iterate([&cellCount] (short column, short row, brogueCellDisplay* cell)
		{
			cellCount++;

			return iterationCallback::iterate;
		});
		
		// Create the pre-allocated data stream
		switch (dataType)
		{
			case openglDataStreamType::brogueImageQuad:
			{
				brogueImageQuad quad;
				result = new simpleDataStream(cellCount, quad.getElementVertexSize(GL_TRIANGLES), quad.getStreamSize(GL_TRIANGLES));
			}
			break;
			case openglDataStreamType::brogueCellQuad:
			{
				brogueCellQuad quad;
				result = new simpleDataStream(cellCount, quad.getElementVertexSize(GL_TRIANGLES), quad.getStreamSize(GL_TRIANGLES));
			}
			break;
			case openglDataStreamType::brogueColorQuad:
			{
				brogueColorQuad quad;
				result = new simpleDataStream(cellCount, quad.getElementVertexSize(GL_TRIANGLES), quad.getStreamSize(GL_TRIANGLES));
			}
			break;
			case openglDataStreamType::brogueAdjacencyColorQuad:
			{
				brogueAdjacencyColorQuad quad;
				result = new simpleDataStream(cellCount, quad.getElementVertexSize(GL_TRIANGLES), quad.getStreamSize(GL_TRIANGLES));
			}
			break;
			default:
				throw simpleException("Unknown openglDataStreamType:  brogueDataStreamBuilder::initializeDataStream");
		}

		return result;
	}

	simpleDataStream* brogueDataStreamBuilder::initializeFrameDataStream(const brogueViewBase* view, openglDataStreamType dataType)
	{
		simpleDataStream* result;

		// Create the pre-allocated data stream
		switch (dataType)
		{
			case openglDataStreamType::brogueImageQuad:
			{
				brogueImageQuad quad;
				result = new simpleDataStream(1, quad.getElementVertexSize(GL_TRIANGLES), quad.getStreamSize(GL_TRIANGLES));
			}
			break;
			case openglDataStreamType::brogueColorQuad:
			{
				brogueColorQuad quad;
				result = new simpleDataStream(1, quad.getElementVertexSize(GL_TRIANGLES), quad.getStreamSize(GL_TRIANGLES));
			}
			break;
			case openglDataStreamType::brogueAdjacencyColorQuad:
			case openglDataStreamType::brogueCellQuad:
			default:
				throw simpleException("Invalid openglDataStreamType (for frame):  brogueDataStreamBuilder::initializeDataStream");
		}

		return result;
	}

	simpleDataStream* brogueDataStreamBuilder::createSceneDataStream(const brogueViewBase* view,
																	 openglDataStreamType dataType,
																	 openglBrogueCellOutputSelector noDisplayOutputSelector)
	{
		// Sets size of the stream
		simpleDataStream* stream = initializeSceneDataStream(view, dataType, noDisplayOutputSelector);

		// Fills in the data
		recreateSceneDataStream(view, stream, dataType, noDisplayOutputSelector);

		return stream;
	}

	simpleDataStream* brogueDataStreamBuilder::createFrameDataStream(const brogueViewBase* view, openglDataStreamType dataType)
	{
		// Sets size of the stream
		simpleDataStream* stream = initializeFrameDataStream(view, dataType);

		recreateFrameDataStream(view, stream, dataType);

		return stream;
	}

	void brogueDataStreamBuilder::recreateSceneDataStream(const brogueViewBase* view,
														  simpleDataStream* dataStream,
														  openglDataStreamType dataType,
														  openglBrogueCellOutputSelector noDisplayOutputSelector)
	{
		// Starting with the raw data, build a simpleQuad data vector to pass to the simpleDataStream<float>
		//
		gridRect sceneBoundaryUI = view->calculateSceneBoundaryUI();
		gridRect viewBoundary = view->getBoundary();

		// Create coordinate converter for this view
		//
		brogueCoordinateConverter coordinateConverter = createCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height, view->getZoomLevel());

		// Problems with polymorphism:  Copy constructors not working for child structs. May be a struct issue.
		simpleList<brogueImageQuad> imageQuads;
		simpleList<brogueCellQuad> cellQuads;
		simpleList<brogueColorQuad> colorQuads;
		simpleList<brogueAdjacencyColorQuad> adjColorQuads;

		// Iterator scope could be removed; but want to be able to handle the root issue. Should be able to copy data
		// up the stack.
		//
		view->iterate(
			[&sceneBoundaryUI, &viewBoundary, &cellQuads, &colorQuads, &adjColorQuads, &imageQuads, &view, &dataType, &coordinateConverter, &noDisplayOutputSelector] (short column, short row, brogueCellDisplay* cell)
		{
			switch (dataType)
			{
				case openglDataStreamType::brogueImageQuad:
					imageQuads.add(coordinateConverter.createBrogueImageQuadScene(*cell, column, row));
					break;
				case openglDataStreamType::brogueCellQuad:
					if (cell->noDisplay)
						cellQuads.add(coordinateConverter.createBrogueCellQuadScene(*cell, column, row, noDisplayOutputSelector));
					else
						cellQuads.add(coordinateConverter.createBrogueCellQuadScene(*cell, column, row, openglBrogueCellOutputSelector::Display));
					break;
				case openglDataStreamType::brogueAdjacencyColorQuad:
					adjColorQuads.add(coordinateConverter.createBrogueAdjacencyColorQuadScene(*cell, column, row));
					break;
				case openglDataStreamType::brogueColorQuad:
					colorQuads.add(coordinateConverter.createBrogueColorQuadScene(*cell, column, row));
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

		// RESET CURSOR:  This sends the data stream cursor back to zero; and allows overwriting the existing data.
		dataStream->resetCursor();

		switch (dataType)
		{
			case openglDataStreamType::brogueImageQuad:
			{
				imageQuads.forEach([&dataStream] (const brogueImageQuad& quad)
				{
					quad.streamBuffer(GL_TRIANGLES, dataStream);
					return iterationCallback::iterate;
				});
			}
			break;
			case openglDataStreamType::brogueCellQuad:
			{
				cellQuads.forEach([&dataStream] (const brogueCellQuad& quad)
				{
					quad.streamBuffer(GL_TRIANGLES, dataStream);
					return iterationCallback::iterate;
				});
			}
			break;
			case openglDataStreamType::brogueColorQuad:
			{
				colorQuads.forEach([&dataStream] (const brogueColorQuad& quad)
				{
					quad.streamBuffer(GL_TRIANGLES, dataStream);
					return iterationCallback::iterate;
				});
			}
			break;
			case openglDataStreamType::brogueAdjacencyColorQuad:
			{
				adjColorQuads.forEach([&dataStream] (const brogueAdjacencyColorQuad& quad)
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
	}

	void brogueDataStreamBuilder::recreateFrameDataStream(const brogueViewBase* view,
														 simpleDataStream* dataStream,
														 openglDataStreamType dataType)
	{
		gridRect sceneBoundaryUI = view->calculateSceneBoundaryUI();

		// Create coordinate converter for this view
		//
		brogueCoordinateConverter coordinateConverter = createCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height, view->getZoomLevel());

		// Create Scene Data Streams

		// Element Size:   Total number of primitives (floats) to commit to the stream
		// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
		//

		// Reset Datastream Cursor:  Allows recreating the data on the stream
		dataStream->resetCursor();

		switch (dataType)
		{
			case openglDataStreamType::brogueImageQuad:
			{
				brogueImageQuad imageQuad = coordinateConverter.createBrogueImageQuadFrame();
				imageQuad.streamBuffer(GL_TRIANGLES, dataStream);
			}
			break;
			case openglDataStreamType::brogueColorQuad:
			{
				brogueColorQuad colorQuad = coordinateConverter.createBrogueColorQuadFrame(colors::black());
				colorQuad.streamBuffer(GL_TRIANGLES, dataStream);
			}
			break;
			case openglDataStreamType::brogueAdjacencyColorQuad:
			case openglDataStreamType::brogueCellQuad:
			default:
				simpleException::show("Unhandled openglDataStreamType:  brogueProgramBuilder.h");
		}
	}

	simpleShaderProgram* brogueDataStreamBuilder::createShaderProgram(simpleDataStream* sceneDataStream,
																      shaderData* vertexData,
																      shaderData* fragmentData)
	{
		// Create Shaders
		simpleShader vertexShader(vertexData);
		simpleShader fragmentShader(fragmentData);

		// Shaders / Vertex Attributes

		int vertexBufferIndex = 0;

		// These are very similar data structures; but please leave this transfer until the GL backend is better understood
		//
		simpleList<simpleVertexAttribute> vertexAttributes = vertexData->attributes.select<simpleVertexAttribute>([] (vertexAttributeData data)
		{
			return simpleVertexAttribute(data.index, data.name, data.type);
		});

		simpleList<simpleVertexAttribute> fragmentAttributes = fragmentData->attributes.select<simpleVertexAttribute>([] (vertexAttributeData data)
		{
			return simpleVertexAttribute(data.index, data.name, data.type);
		});

		simpleVertexBuffer<float>* programVBO = new simpleVertexBuffer<float>(vertexBufferIndex++, sceneDataStream, vertexAttributes);
		simpleVertexArray<float>* programVAO = new simpleVertexArray<float>(GL_TRIANGLES, programVBO);

		simpleShaderProgram* program = new simpleShaderProgram(vertexShader, fragmentShader, programVAO);

		return program;
	}
}
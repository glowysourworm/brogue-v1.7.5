#pragma once
#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueDataStreamBuilder.h"
#include "brogueGlyphMap.h"
#include "brogueImageQuad.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartConfiguration.h"
#include "brogueViewBase.h"
#include "gl.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "resourceController.h"
#include "shaderData.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleShaderProgram.h"
#include "brogueFlameQuad.h"

using namespace brogueHd::backend;
using namespace brogueHd::backend::model;
using namespace brogueHd::component;

namespace brogueHd::frontend
{
	class brogueProgramBuilder
	{
	public:

		brogueProgramBuilder(resourceController* resourceController,
							 brogueGlyphMap* glyphMap);
		~brogueProgramBuilder();

		// (MEMORY!) -> deleted by simpleVertexBuffer
		simpleDataStream* buildDataStream(const brogueViewBase* view,
										  const brogueUIProgramPartConfiguration& configuration);

		// (MEMORY!) -> deleted by brogueViewProgram
		simpleShaderProgram* buildProgram(const brogueViewBase* view,
										  const brogueUIProgramPartConfiguration& configuration);

		void rebuildDataStream(const brogueViewBase* view,
							   const brogueUIProgramPartConfiguration& configuration,
							   simpleDataStream* stream);

		/// <summary>
		/// Calculates a datastream range for the specified stream configuration. Returns true if
		/// there is a contiguous stream block; false otherwise.
		/// </summary>
		bool calculateStreamRange(const brogueViewBase* source,
									const brogueViewBase* dest,
									const brogueUIProgramPartConfiguration& configuration,
									const gridLocator& start,
									const gridLocator& end,
									bool copyView,
									int& offsetStart,
									int& offsetEnd);

		brogueCoordinateConverter buildCoordinateConverter(int sceneWidth, int sceneHeight, int zoomLevel);

	private:

		simpleDataStream* createStream(const brogueViewBase* view,
									   openglDataStreamType type,
									   openglBrogueCellOutputSelector noDisplaySelector,
									   bool isFrameType);

		void recreateDataStream(const brogueViewBase* view,
								simpleDataStream* stream,
								openglDataStreamType type,
								openglBrogueCellOutputSelector noDisplaySelector,
								bool isFrameType);

	private:

		brogueDataStreamBuilder* _dataStreamBuilder;
		resourceController* _resourceController;
		brogueGlyphMap* _glyphMap;
	};

	brogueProgramBuilder::brogueProgramBuilder(resourceController* resourceController, brogueGlyphMap* glyphMap)
	{
		_resourceController = resourceController;
		_glyphMap = glyphMap;
		_dataStreamBuilder = new brogueDataStreamBuilder(resourceController, glyphMap);
	}
	brogueProgramBuilder::~brogueProgramBuilder()
	{
		delete _dataStreamBuilder;
	}

	brogueCoordinateConverter brogueProgramBuilder::buildCoordinateConverter(int sceneWidth, int sceneHeight, int zoomLevel)
	{
		return _dataStreamBuilder->createCoordinateConverter(sceneWidth, sceneHeight, zoomLevel);
	}

	simpleShaderProgram* brogueProgramBuilder::buildProgram(const brogueViewBase* view,
															const brogueUIProgramPartConfiguration& configuration)
	{
		simpleDataStream* dataStream = this->buildDataStream(view, configuration);

		shaderData* vertexShader = _resourceController->getShader(configuration.vertexShaderName);
		shaderData* fragmentShader = _resourceController->getShader(configuration.fragmentShaderName);

		// (MEMORY!) -> Deleted by the simpleVertexBuffer
		return _dataStreamBuilder->createShaderProgram(dataStream, vertexShader, fragmentShader);
	}

	simpleDataStream* brogueProgramBuilder::buildDataStream(const brogueViewBase* view,
															const brogueUIProgramPartConfiguration& configuration)
	{
		switch (configuration.programPartName)
		{
			case brogueUIProgramPart::FlameDisplay:
			case brogueUIProgramPart::ColorMask:
			case brogueUIProgramPart::Background:
			case brogueUIProgramPart::MenuBackground:
			case brogueUIProgramPart::Button:
			case brogueUIProgramPart::Text:
				return createStream(view, configuration.dataStreamType, configuration.noDisplaySelector, configuration.isFrameType);
			default:
				throw simpleException("Unhandled brogueUIProgram type:  brogueProgramBuilder.h");
		}
	}

	void brogueProgramBuilder::rebuildDataStream(const brogueViewBase* view,
												 const brogueUIProgramPartConfiguration& configuration,
												 simpleDataStream* stream)
	{
		switch (configuration.programPartName)
		{
			case brogueUIProgramPart::FlameDisplay:
			case brogueUIProgramPart::ColorMask:
			case brogueUIProgramPart::Background:
			case brogueUIProgramPart::MenuBackground:
			case brogueUIProgramPart::Button:
			case brogueUIProgramPart::Text:
				return recreateDataStream(view, stream, configuration.dataStreamType, configuration.noDisplaySelector, configuration.isFrameType);
			default:
				throw simpleException("Unhandled brogueUIProgram type:  brogueProgramBuilder.h");
		}
	}

	simpleDataStream* brogueProgramBuilder::createStream(const brogueViewBase* view,
														 openglDataStreamType type,
														 openglBrogueCellOutputSelector noDisplaySelector,
														 bool isFrameType)
	{
		if (!isFrameType)
			return _dataStreamBuilder->createSceneDataStream(view, type, noDisplaySelector);

		else
			return _dataStreamBuilder->createFrameDataStream(view, type);
	}

	void brogueProgramBuilder::recreateDataStream(const brogueViewBase* view,
												  simpleDataStream* stream,
												  openglDataStreamType type,
												  openglBrogueCellOutputSelector noDisplaySelector,
												  bool isFrameType)
	{
		if (!isFrameType)
			return _dataStreamBuilder->recreateSceneDataStream(view, stream, type, noDisplaySelector);

		else
			return _dataStreamBuilder->recreateFrameDataStream(view, stream, type);
	}

	bool brogueProgramBuilder::calculateStreamRange(const brogueViewBase* source,
													const brogueViewBase* dest,
													const brogueUIProgramPartConfiguration& configuration,
													const gridLocator& start,
													const gridLocator& end,
													bool copyView,
													int& offsetStart,
													int& offsetEnd)
	{
		if (source->getBoundary() != dest->getBoundary())
			throw simpleException("Invalid view boundaries:  cannot perform stream copy.");

		// The stream is built as the iterator of brogueViewBase iterates its cells. So, any breaks in the 
		// chain will result in an offset error for the data stream.

		gridRect boundary = source->getBoundary();
		bool result = true;

		source->iterateFrom(start, end, [&configuration, &source, &dest, &result, &copyView] (int column, int row, brogueCellDisplay* cell)
		{
			if (cell == nullptr || dest->get(column, row) == nullptr)
			{
				result = false;
				return iterationCallback::breakAndReturn;
			}

			// Check Copy:
			if (copyView)
				cell->setUI(*dest->get(column, row));

			return iterationCallback::iterate;
		});

		int cellStreamSizeBytes = 0;

		switch (configuration.dataStreamType)
		{
			case openglDataStreamType::brogueCellQuad:
			{
				brogueCellQuad quad;
				cellStreamSizeBytes = quad.getStreamSize(GL_TRIANGLES);
			}
				break;
			case openglDataStreamType::brogueColorQuad:
			{
				brogueColorQuad quad;
				cellStreamSizeBytes = quad.getStreamSize(GL_TRIANGLES);
			}
				break;
			case openglDataStreamType::brogueFlameQuad:
			{
				brogueFlameQuad quad;
				cellStreamSizeBytes = quad.getStreamSize(GL_TRIANGLES);
			}
			break;
			case openglDataStreamType::brogueImageQuad:
			{
				brogueImageQuad quad;
				cellStreamSizeBytes = quad.getStreamSize(GL_TRIANGLES);
			}
				break;
			default:
				throw simpleException("Unhandled data stream type:  brogueProgramBuilder::calculateStreamRange");
		}

		// Calculate data stream offsets
		int startRangeCells = start.row > 0 ? ((start.row - 1) * boundary.width) + start.column : start.column;
		int endRangeCells = end.row > 0 ? ((end.row - 1) * boundary.width) + end.column : end.column;

		offsetStart = startRangeCells * cellStreamSizeBytes;
		offsetEnd = endRangeCells * cellStreamSizeBytes;

		return result;
	}
}
#pragma once
#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueDataStreamBuilder.h"
#include "brogueGlyphMap.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartConfiguration.h"
#include "brogueViewBase.h"
#include "gridDefinitions.h"
#include "resourceController.h"
#include "shaderData.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleShaderProgram.h"

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
			case brogueUIProgramPart::FlameMenuProgram_HeatSourceProgram:
			case brogueUIProgramPart::FlameMenuProgram_TitleMaskProgram:
			case brogueUIProgramPart::FlameMenuProgram_HeatDiffuseProgram:
			case brogueUIProgramPart::Background:
			case brogueUIProgramPart::MenuBackground:
			case brogueUIProgramPart::MenuButton:
			case brogueUIProgramPart::Text:
				return createStream(view, configuration.dataStreamType, configuration.noDisplaySelector, configuration.isFrameType);
			case brogueUIProgramPart::ViewCompositor:
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
			case brogueUIProgramPart::FlameMenuProgram_HeatSourceProgram:
			case brogueUIProgramPart::FlameMenuProgram_TitleMaskProgram:
			case brogueUIProgramPart::FlameMenuProgram_HeatDiffuseProgram:
			case brogueUIProgramPart::Background:
			case brogueUIProgramPart::MenuBackground:
			case brogueUIProgramPart::MenuButton:
			case brogueUIProgramPart::Text:
				return recreateDataStream(view, stream, configuration.dataStreamType, configuration.noDisplaySelector, configuration.isFrameType);
			case brogueUIProgramPart::ViewCompositor:
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
}
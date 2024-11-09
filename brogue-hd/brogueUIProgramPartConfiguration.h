#pragma once
#include "brogueGlobal.h"
#include "brogueUIConstants.h"
#include <cstdint>

namespace brogueHd::frontend
{
	struct brogueUIProgramPartConfiguration
	{
		brogueUIProgramPartConfiguration(brogueUIProgramPart programPart,
										 shaderResource vertexShader,
										 shaderResource fragmentShader,
										 openglDataStreamType streamType,
										 openglBrogueCellOutputSelector noDisplayOutput,
										 uint32_t minUpdatePeriodMilliseconds,
										 bool alphaBlendingEnable,
										 bool frameType)
		{
			programPartName = programPart;
			vertexShaderName = vertexShader;
			fragmentShaderName = fragmentShader;
			dataStreamType = streamType;
			noDisplaySelector = noDisplayOutput;
			minimumUpdatePeriodMilliseconds = minUpdatePeriodMilliseconds;
			useAlphaBlending = alphaBlendingEnable;
			isFrameType = frameType;
		}
		brogueUIProgramPartConfiguration(const brogueUIProgramPartConfiguration& copy)
		{
			programPartName = copy.programPartName;
			vertexShaderName = copy.vertexShaderName;
			fragmentShaderName = copy.fragmentShaderName;
			dataStreamType = copy.dataStreamType;
			noDisplaySelector = copy.noDisplaySelector;
			minimumUpdatePeriodMilliseconds = copy.minimumUpdatePeriodMilliseconds;
			useAlphaBlending = copy.useAlphaBlending;
			isFrameType = copy.isFrameType;
		}
		~brogueUIProgramPartConfiguration() {};
		void operator=(const brogueUIProgramPartConfiguration& copy)
		{
			programPartName = copy.programPartName;
			vertexShaderName = copy.vertexShaderName;
			fragmentShaderName = copy.fragmentShaderName;
			dataStreamType = copy.dataStreamType;
			noDisplaySelector = copy.noDisplaySelector;
			minimumUpdatePeriodMilliseconds = copy.minimumUpdatePeriodMilliseconds;
			useAlphaBlending = copy.useAlphaBlending;
			isFrameType = copy.isFrameType;
		}

		brogueUIProgramPart programPartName;
		shaderResource vertexShaderName;
		shaderResource fragmentShaderName;
		openglDataStreamType dataStreamType;
		openglBrogueCellOutputSelector noDisplaySelector;
		uint32_t minimumUpdatePeriodMilliseconds;
		bool useAlphaBlending;
		bool isFrameType;
	};
}
#pragma once
#include "brogueGlobal.h"
#include "brogueUIConstants.h"
#include "gl.h"
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
										 GLenum openglPrimitiveType,
										 bool alphaBlendingEnable,
										 bool frameType)
		{
			programPartName = programPart;
			vertexShaderName = vertexShader;
			fragmentShaderName = fragmentShader;
			dataStreamType = streamType;
			noDisplaySelector = noDisplayOutput;
			minimumUpdatePeriodMilliseconds = minUpdatePeriodMilliseconds;
			glPrimitiveType = openglPrimitiveType;
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
			glPrimitiveType = copy.glPrimitiveType;
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
			glPrimitiveType = copy.glPrimitiveType;
			useAlphaBlending = copy.useAlphaBlending;
			isFrameType = copy.isFrameType;
		}

		brogueUIProgramPart programPartName;
		shaderResource vertexShaderName;
		shaderResource fragmentShaderName;
		openglDataStreamType dataStreamType;
		openglBrogueCellOutputSelector noDisplaySelector;
		uint32_t minimumUpdatePeriodMilliseconds;
		GLenum glPrimitiveType;
		bool useAlphaBlending;
		bool isFrameType;
	};
}
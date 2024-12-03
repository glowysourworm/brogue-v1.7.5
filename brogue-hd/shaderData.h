#pragma once

#include "brogueGlobal.h"
#include "gl.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleList.h"
#include "simpleString.h"
#include "simpleUniform.h"

namespace brogueHd::frontend
{
	using namespace simple;

	/// <summary>
	/// Represents hard coded data for a shader string source. These variables must be filled out to
	/// represent the shader as it is written. They are fed to the GL backend.
	/// </summary>
	class shaderData
	{
	public:
		shaderData()
		{
			this->source = nullptr;
			this->resource = shaderResource::backgroundColorFrag;
			this->type = 0;
		}

		shaderData(const shaderData& copy)
		{
			this->source = copy.source;
			this->resource = copy.resource;
			this->type = copy.type;

			this->attributes = copy.attributes;
			this->uniforms1 = copy.uniforms1;
			this->uniforms1i = copy.uniforms1i;
			this->uniforms2 = copy.uniforms2;
			this->uniforms2i = copy.uniforms2i;
			this->uniforms4 = copy.uniforms4;
		}

		shaderData(shaderResource aresource, GLenum atype, const simpleString& asource)
		{
			source = asource;
			type = atype;
			resource = aresource;

			// Sets up vertex array attributes for this specific shader program
			//
			switch (this->resource)
			{
			// VERTEX SHADERS
			case shaderResource::brogueLineVert:
				this->attributes.add(vertexAttributeData(0, "vertexXY_IsOdd", GL_FLOAT_VEC3));
				this->attributes.add(vertexAttributeData(1, "lineColor", GL_FLOAT_VEC4));
				break;
			case shaderResource::colorMaskVert:
				this->attributes.add(vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(1, "backgroundColor", GL_FLOAT_VEC4));
				break;
			case shaderResource::backgroundColorVert:
				this->attributes.add(vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(1, "backgroundColor", GL_FLOAT_VEC4));
				break;
			case shaderResource::brogueCellDisplayVert:
				this->attributes.add(vertexAttributeData(0, "vertexXY", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(1, "textureUV", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(2, "glyphUV", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(3, "foregroundColor", GL_FLOAT_VEC4));
				this->attributes.add(vertexAttributeData(4, "backgroundColor", GL_FLOAT_VEC4));
				this->attributes.add(vertexAttributeData(5, "outputSelector", GL_FLOAT));
				break;
			case shaderResource::diffuseColorUpwardVert:
				this->attributes.add(vertexAttributeData(0, "vertexXY", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(1, "textureUV", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(2, "glyphUV", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(3, "foregroundColor", GL_FLOAT_VEC4));
				this->attributes.add(vertexAttributeData(4, "backgroundColor", GL_FLOAT_VEC4));
				this->attributes.add(vertexAttributeData(5, "outputSelector", GL_FLOAT));
				break;
			case shaderResource::mixFrameTexturesVert:
				this->attributes.add(vertexAttributeData(0, "vertexXY", GL_FLOAT_VEC2));
				this->attributes.add(vertexAttributeData(1, "textureUV", GL_FLOAT_VEC2));
				break;

			// FRAGMENT SHADERS
			case shaderResource::brogueLineRoomGraphFrag:
				break;
			case shaderResource::colorMaskFrag:
				this->uniforms4.add(simpleUniform<vec4>("maskColor", GL_FLOAT_VEC4, vec4(0, 0, 0, 0)));
				break;
			case shaderResource::backgroundColorFrag:
				break;
			case shaderResource::brogueCellDisplayFrag:
				this->uniforms1i.add(simpleUniform<int>("frameTexture", GL_SAMPLER_2D, 0));
			// GLOBAL SETTING:  GL_TEXTURE0
				this->uniforms1i.add(simpleUniform<int>("fontTexture", GL_SAMPLER_2D, 5));
			// GLOBAL SETTING:  GL_TEXTURE5
				this->uniforms1.add(simpleUniform<float>("glyphThreshold", GL_FLOAT, 0.05f));
				break;
			case shaderResource::diffuseColorUpwardFrag:
				this->uniforms1i.add(simpleUniform<int>("flameTexture", GL_SAMPLER_2D, 1)); // Program Setting
				this->uniforms2i.add(simpleUniform<ivec2>("cellSizeUI", GL_INT_VEC2, ivec2(0, 0)));
				this->uniforms2.add(simpleUniform<vec2>("cellSizeUV", GL_FLOAT_VEC2, vec2(0, 0)));
				this->uniforms1.add(simpleUniform<float>("weight", GL_FLOAT, 1.0f));
				this->uniforms1.add(simpleUniform<float>("fadePeriodTime", GL_FLOAT, 0.0f));
			// [0,1] time ratio for the color fade
				this->uniforms1i.add(simpleUniform<int>("fadePeriodRandom1", GL_INT, 1));
			// Random integer 1 uniform for the fade period
				this->uniforms1i.add(simpleUniform<int>("fadePeriodRandom2", GL_INT, 1));
			// Random integer 1 uniform for the fade period
			//this->uniforms1i.add(simpleUniform<int>("nextColorNumber", GL_INT, 1));					// Counter for the color index of the title view

			// "Static" flame colors (could just go in the shader)
				this->uniforms4.add(simpleUniform<vec4>("flameBottomColor1", GL_FLOAT_VEC4, vec4(1.0f, 0.0f, 0.0f, 1)));
				this->uniforms4.add(simpleUniform<vec4>("flameBottomColor2", GL_FLOAT_VEC4, vec4(1.0f, 1.0f, 0.6f, 1)));
				this->uniforms4.add(simpleUniform<vec4>("flameBottomColor3", GL_FLOAT_VEC4, vec4(1.0f, 1.0f, 0.8f, 1)));
				this->uniforms4.add(simpleUniform<vec4>("flameTitleColor1", GL_FLOAT_VEC4, vec4(0.0f, 0.0f, 1.0f, 1)));
				this->uniforms4.add(simpleUniform<vec4>("flameTitleColor2", GL_FLOAT_VEC4, vec4(0.0f, 0.1f, 0.9f, 1)));
				this->uniforms4.add(simpleUniform<vec4>("flameTitleColor3", GL_FLOAT_VEC4, vec4(0.0f, 0.0f, 1.0f, 1)));

				break;
			case shaderResource::mixFrameTexturesFrag:
				this->uniforms1i.add(simpleUniform<int>("frameTexture", GL_SAMPLER_2D, 0));
			// GLOBAL SETTING:  GL_TEXTURE0
				this->uniforms1i.add(simpleUniform<int>("flameTexture", GL_SAMPLER_2D, 1));
			// GLOBAL SETTING:  GL_TEXTURE1
				this->uniforms1i.add(simpleUniform<int>("titleMaskTexture", GL_SAMPLER_2D, 2));
			// GLOBAL SETTING:  GL_TEXTURE2
				this->uniforms1i.add(simpleUniform<int>("uiTexture", GL_SAMPLER_2D, 3)); // GLOBAL SETTING:  GL_TEXTURE3
				this->uniforms1i.add(simpleUniform<int>("gameLogTexture", GL_SAMPLER_2D, 4));
			// GLOBAL SETTING:  GL_TEXTURE4
				this->uniforms1i.add(simpleUniform<int>("gameMode", GL_INT, 0)); // BrogueGameMode enum
				this->uniforms2.add(simpleUniform<vec2>("gameLogOffsetUV", GL_FLOAT_VEC2, vec2(0, 0)));
				break;
			default:
				throw simpleException("Unhandled shaderResource type:  shaderData.h");
				break;
			}
		}

		~shaderData()
		{
		}

		simpleString source;
		GLenum type;
		shaderResource resource;

		simpleList<vertexAttributeData> attributes;
		simpleList<simpleUniform<int>> uniforms1i;
		simpleList<simpleUniform<float>> uniforms1;
		simpleList<simpleUniform<vec2>> uniforms2;
		simpleList<simpleUniform<ivec2>> uniforms2i;
		simpleList<simpleUniform<vec4>> uniforms4;
	};
}

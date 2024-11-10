#pragma once

#include "brogueGlobal.h"
#include "gl.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleList.h"
#include "simpleString.h"
#include "simpleUniform.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
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
				case shaderResource::colorMaskVert:
					this->attributes.add(vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
					this->attributes.add(vertexAttributeData(1, "backgroundColor", GL_FLOAT_VEC4));
					break;
				case shaderResource::backgroundColorVert:
					this->attributes.add(vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
					this->attributes.add(vertexAttributeData(1, "backgroundColor", GL_FLOAT_VEC4));
					break;
				case shaderResource::brogueCellDisplayVert:
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
				case shaderResource::colorMaskFrag:
					this->uniforms4.add(simpleUniform<vec4>("maskColor", GL_FLOAT_VEC4, vec4(0, 0, 0, 0)));
					break;
				case shaderResource::backgroundColorFrag:
					break;
				case shaderResource::brogueCellDisplayFrag:
					this->uniforms1i.add(simpleUniform<int>("frame0Texture", GL_SAMPLER_2D, 0));        // GLOBAL SETTING:  GL_TEXTURE0
					this->uniforms1i.add(simpleUniform<int>("fontTexture", GL_SAMPLER_2D, 1));          // GLOBAL SETTING:  GL_TEXTURE1
					this->uniforms1.add(simpleUniform<float>("glyphThreshold", GL_FLOAT, 0.05f));
					break;
				case shaderResource::diffuseColorUpwardFrag:
					this->uniforms1i.add(simpleUniform<int>("frame0Texture", GL_SAMPLER_2D, 0));        // GLOBAL SETTING:  GL_TEXTURE0
					this->uniforms1i.add(simpleUniform<int>("fontTexture", GL_SAMPLER_2D, 1));          // GLOBAL SETTING:  GL_TEXTURE1
					this->uniforms2.add(simpleUniform<vec2>("cellSizeUV", GL_FLOAT_VEC2, vec2(1, 1)));
					this->uniforms1.add(simpleUniform<float>("weight", GL_FLOAT, 0.5f));
					break;
				case shaderResource::mixFrameTexturesFrag:
					this->uniforms1i.add(simpleUniform<int>("frame0Texture", GL_SAMPLER_2D, 0));        // GLOBAL SETTING:  GL_TEXTURE0
					//this->uniforms1i.add(simpleUniform<int>("frame1Texture", GL_SAMPLER_2D, 1));		// GLOBAL SETTING:  GL_TEXTURE1
					//this->uniforms1.add(simpleUniform<float>("mixWeight", GL_FLOAT, 0.5));
					break;
				default:
					simpleException::show("Unhandled shaderResource type:  shaderData.h");
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
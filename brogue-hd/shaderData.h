#pragma once

#include "brogueGlobal.h"
#include "simpleUniform.h"
#include "simpleList.h"
#include "simpleString.h"
#include "simpleGlData.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
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
            case shaderResource::diffuseColorUpwardVert:
                this->attributes.add(vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
                this->attributes.add(vertexAttributeData(1, "texcoord", GL_FLOAT_VEC2));
                this->attributes.add(vertexAttributeData(2, "backgroundColor", GL_FLOAT_VEC4));
                break;
            case shaderResource::mixFrameTexturesVert:
                this->attributes.add(vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
                this->attributes.add(vertexAttributeData(1, "texcoord", GL_FLOAT_VEC2));
                break;

            // FRAGMENT SHADERS
            case shaderResource::colorMaskFrag:
                this->uniforms4.add(simpleUniform<vec4>("maskColor", GL_FLOAT_VEC4, vec4(0, 0, 0, 0)));
                break;
            case shaderResource::backgroundColorFrag:
                break;
            case shaderResource::diffuseColorUpwardFrag:
                this->uniforms1i.add(simpleUniform<int>("frame0Texture", GL_SAMPLER_2D, 0));
                this->uniforms1.add(simpleUniform<float>("weight", GL_FLOAT, 0.5));
                break;
            case shaderResource::mixFrameTexturesFrag:
                this->uniforms1i.add(simpleUniform<int>("frame0Texture", GL_SAMPLER_2D, 0));
                this->uniforms1i.add(simpleUniform<int>("frame1Texture", GL_SAMPLER_2D, 1));
                this->uniforms1.add(simpleUniform<float>("weight", GL_FLOAT, 0.5));
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
        simpleList<simpleUniform<vec4>> uniforms4;
    };
}
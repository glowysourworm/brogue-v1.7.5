#pragma once

#include "brogueGlobal.h"
#include "simpleArray.h"
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
            this->resource = shaderResource::brogueBaseFrag;
            this->type = 0;

            this->attributes = new simpleList<vertexAttributeData>();
            this->uniforms1 = new simpleList<uniformData<float>>();
            this->uniforms1i = new simpleList<uniformData<int>>();
            this->uniforms2 = new simpleList<uniformData<vec2>>();
            this->uniforms4 = new simpleList<uniformData<vec4>>();
        }
        shaderData(const shaderData& copy)
        {
            this->source = copy.source;
            this->attributes = copy.attributes;
            this->resource = copy.resource;
            this->type = copy.type;
            this->uniforms1 = copy.uniforms1;
            this->uniforms1i = copy.uniforms1i;
            this->uniforms2 = copy.uniforms2;
            this->uniforms4 = copy.uniforms4;
        }
        shaderData(shaderResource aresource, GLenum atype, const simpleString& asource)
        {
            source = new simpleString(asource);
            type = atype;
            resource = aresource;

            this->attributes = new simpleList<vertexAttributeData>();
            this->uniforms1 = new simpleList<uniformData<float>>();
            this->uniforms1i = new simpleList<uniformData<int>>();
            this->uniforms2 = new simpleList<uniformData<vec2>>();
            this->uniforms4 = new simpleList<uniformData<vec4>>();

            // Sets up vertex array attributes for this specific shader program
            //
            switch (this->resource)
            {
                // FRAGMENT SHADERS
            case shaderResource::brogueBaseFrag:
                //this->uniforms1i->add(uniformData<int>("sceneBaseTexture", GL_SAMPLER_2D));
                break;
            case shaderResource::brogueFrameFrag:
                //this->uniforms1i->add(uniformData<int>("frameTexture", GL_SAMPLER_2D));
                break;

                // VERTEX SHADERS
            case shaderResource::brogueBaseVert:
            case shaderResource::brogueFrameVert:
                this->attributes->add(vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
                //this->attributes->add(vertexAttributeData(1, "texcoord", GL_FLOAT_VEC2));
                break;

            case shaderResource::brogueFlameMenuVert:
                this->attributes->add(vertexAttributeData(0, "valueVertex", GL_FLOAT_VEC3));
                break;
            case shaderResource::brogueFlameMenuFrag:
                break;
            default:
                simpleException::showCstr("Unhandled shaderResource type:  shaderData.h");
            }
        }
        ~shaderData()
        {
            delete this->attributes;
            delete this->uniforms1;
            delete this->uniforms1i;
            delete this->uniforms2;
            delete this->uniforms4;
        }

        simpleString* source;
        GLenum type;
        shaderResource resource;

        simpleList<vertexAttributeData>* attributes;
        simpleList<uniformData<int>>* uniforms1i;
        simpleList<uniformData<float>>* uniforms1;
        simpleList<uniformData<vec2>>* uniforms2;
        simpleList<uniformData<vec4>>* uniforms4;
    };
}
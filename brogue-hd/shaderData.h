#pragma once

#include "brogueGlobal.h"
#include "simpleArray.h"
#include "simpleGlData.h"
#include "gl.h"
#include <string>

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Represents hard coded data for a shader string source. These variables must be filled out to
    /// represent the shader as it is written. They are fed to the GL backend.
    /// </summary>
    struct shaderData
    {
        shaderData()
        {
        }
        shaderData(shaderResource aresource, GLenum atype, std::string asource)
        {
            source = asource;
            type = atype;
            resource = aresource;

            this->attributes = NULL;
            this->uniforms1 = NULL;
            this->uniforms1i = NULL;
            this->uniforms2 = NULL;
            this->uniforms4 = NULL;

            // Sets up vertex array attributes for this specific shader program
            //
            switch (this->resource)
            {
                // FRAGMENT SHADERS
            case shaderResource::brogueBaseFrag:
                this->uniforms1i = new simpleArray<uniformData<int>>(1);
                this->uniforms1i->set(0, uniformData<int>("sceneBaseTexture", GL_SAMPLER_2D));
                break;
            case shaderResource::brogueFrameFrag:
                this->uniforms1i = new simpleArray<uniformData<int>>(1);
                this->uniforms1i->set(0, uniformData<int>("frameTexture", GL_SAMPLER_2D));
                break;

                // VERTEX SHADERS
            case shaderResource::brogueBaseVert:
            case shaderResource::brogueFrameVert:
                this->attributes = new simpleArray<vertexAttributeData>(2);
                this->attributes->set(0, vertexAttributeData(0, "vertex", GL_FLOAT_VEC2));
                this->attributes->set(1, vertexAttributeData(1, "texcoord", GL_FLOAT_VEC2));
                break;
            default:
                brogueException::show("Unhandled shaderResource type:  shaderData.h");
            }
        }
        ~shaderData()
        {
            if (this->attributes != NULL)
                delete this->attributes;

            if (this->uniforms1 != NULL)
                delete this->uniforms1;

            if (this->uniforms1i != NULL)
                delete this->uniforms1i;

            if (this->uniforms2 != NULL)
                delete this->uniforms2;

            if (this->uniforms4 != NULL)
                delete this->uniforms4;
        }

        std::string source;
        GLenum type;
        shaderResource resource;

        simpleArray<vertexAttributeData>* attributes;
        simpleArray<uniformData<int>>* uniforms1i;
        simpleArray<uniformData<float>>* uniforms1;
        simpleArray<uniformData<vec2>>* uniforms2;
        simpleArray<uniformData<vec4>>* uniforms4;
    };
}
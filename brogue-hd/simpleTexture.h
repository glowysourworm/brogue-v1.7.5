#pragma once

#include "simplePrimitive.h"
#include "simpleException.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    class simpleTexture : public simplePrimitive
    {
    public:

        // Corresponds to the level of the nth minimap for the texture (see texture rendering to learn about minimaps)
        //
        const GLuint TEXTURE_MINIMAP_LEVEL = 0;

    public:

        simpleTexture(){};
        simpleTexture(int pixelBuffer, GLsizei width, GLsizei height, GLuint textureIndex, GLenum textureUnit, GLenum pixelFormat, GLenum pixelType);
        ~simpleTexture(){};

        void glCreate(GLuint programHandle) override;
        void teardown() override;
        void bind(bool bind) override;

    private:

        GLenum _textureUnit;
        GLuint _textureIndex;

        int _pixelBuffer;
        GLsizei _width;
        GLsizei _height;
        GLenum _pixelFormat;
        GLenum _pixelType;
    };
      
    simpleTexture::simpleTexture(int pixelBuffer, GLsizei width, GLsizei height, GLuint textureIndex, GLenum textureUnit, GLenum pixelFormat, GLenum pixelType)
    {
        _textureUnit = textureUnit;
        _textureIndex = textureIndex;

        _pixelBuffer = pixelBuffer;
        _width = width;
        _height = height;
        _pixelFormat = pixelFormat;
        _pixelType = pixelType;
    }

    void simpleTexture::glCreate(GLuint programHandle)
    {
        if (this->isCreated)
            simpleException::showCstr("simpleTexture already created in the backend");

        // Procedure
        //
        // 0) Generate texture to create the handle and set active texture
        // 1) Bind GL to the current texture
        // 2) Apply the pixel data to the texture
        //

        // Generate texture
        GLuint textureHandle;
        glGenTextures(1, &textureHandle);
        
        this->handle = textureHandle;

        // Set active texture
        glActiveTexture(_textureUnit);

        // Bind the texture to apply changes
        glBindTexture(GL_TEXTURE_2D, this->handle);

        // Apply the pixel data to the backend
        glTexImage2D(GL_TEXTURE_2D,
                     TEXTURE_MINIMAP_LEVEL,
                     GL_RGBA,                      // I believe this is the format used in the shader (some kind of 4-vector)
                     _width, _height, 0,           // border:  "Should always be set to zero" ....?
                     _pixelFormat,                 // This should be the format used in OUR pixel data array
                     _pixelType,
                     (void*)_pixelBuffer);                // Pixel data in byte array

        // SETTING THESE TO DEFAULTS (TODO)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glGenerateTextureMipmap(this->handle);

        this->isCreated = true;
        this->isBound = true;
    }

    void simpleTexture::teardown()
    {
        if (!this->isCreated)
            simpleException::showCstr("GLTexture already deleted from the backend");

        glDeleteTextures(1, &_textureIndex);

        this->isCreated = false;
        this->isBound = false;
        this->handle = NULL;
    }

    void simpleTexture::bind(bool bind)
    {
        if (!this->isCreated)
            simpleException::showCstr("GLTexture already deleted from the backend");

        if (bind)
        {
            glActiveTexture(_textureUnit);
            glBindTexture(GL_TEXTURE_2D, this->handle);
        }
        else
            glBindTexture(GL_TEXTURE_2D, NULL);

        this->isBound = bind;
    }   
}
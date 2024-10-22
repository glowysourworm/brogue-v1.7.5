#pragma once

#include "simple.h"
#include "simplePrimitive.h"
#include "simpleException.h"
#include "gl.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    struct simpleTexture : public simplePrimitive
    {
    public:

        // Corresponds to the level of the nth mipmap for the texture (see texture rendering to learn about minimaps)
        //
        const GLuint TEXTURE_MIPMAP_LEVEL = 0;

    public:

        simpleTexture();
        simpleTexture(const simpleTexture& copy);
        simpleTexture(int pixelBuffer, GLsizei width, GLsizei height, GLuint textureIndex, GLenum textureUnit, GLenum pixelFormat, GLenum pixelType);
        ~simpleTexture(){};

        void operator=(const simpleTexture& copy);

        void glCreate(GLuint programHandle) override;
        void teardown() override;
        void bind(bool bind) override;

        GLenum getTextureUnit() const
        {
            return _textureUnit;
        }
        GLuint getTextureIndex() const
        {
            return _textureIndex;
        }

        int getPixelBuffer() const
        {
            return _pixelBuffer;
        }
        GLsizei getWidth() const
        {
            return _width;
        }
        GLsizei getHeight() const
        {
            return _height;
        }
        GLenum getPixelFormat() const
        {
            return _pixelFormat;
        }
        GLenum getPixelType() const
        {
            return _pixelType;
        }

    public:

        size_t getHash() const override
        {
            return hashGenerator::generateHash(_textureUnit, _textureIndex, _pixelBuffer, _width, _height, _pixelFormat, _pixelType);
        }

    private:

        void copyImpl(const simpleTexture& copy);

    private:

        GLenum _textureUnit;
        GLuint _textureIndex;

        int _pixelBuffer;
        GLsizei _width;
        GLsizei _height;
        GLenum _pixelFormat;
        GLenum _pixelType;
    };
      
    simpleTexture::simpleTexture()
    {
        _textureUnit = NULL;
        _textureIndex = NULL;

        _pixelBuffer = NULL;
        _width = 0;
        _height = 0;
        _pixelFormat = NULL;
        _pixelType = NULL;
    }
    simpleTexture::simpleTexture(const simpleTexture& copy)
    {
        copyImpl(copy);
    }
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

    void simpleTexture::operator=(const simpleTexture& copy)
    {
        copyImpl(copy);
    }

    void simpleTexture::copyImpl(const simpleTexture& copy)
    {
        _textureUnit = copy.getTextureUnit();
        _textureIndex = copy.getTextureIndex();

        _pixelBuffer = copy.getPixelBuffer();
        _width = copy.getWidth();
        _height = copy.getHeight();
        _pixelFormat = copy.getPixelFormat();
        _pixelType = copy.getPixelType();
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
                     TEXTURE_MIPMAP_LEVEL,
                     GL_RGBA,                      // I believe this is the format used in the shader (some kind of 4-vector)
                     _width, _height, 0,           // border:  "Should always be set to zero" ....?
                     _pixelFormat,                 // This should be the format used in OUR pixel data array
                     _pixelType,
                     (void*)_pixelBuffer);                // Pixel data in byte array

        // SETTING THESE TO DEFAULTS (TODO)
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
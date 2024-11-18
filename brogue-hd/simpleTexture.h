#pragma once

#include "gl.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleGlObject.h"

#include "color.h"
#include "openglHelper.h"
#include "simpleDataStream.h"
#include "simpleGlData.h"
#include "simpleLogger.h"
#include <SDL_surface.h>


using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	struct simpleTexture : public simpleGlObject
	{
	public:

		// Corresponds to the level of the nth mipmap for the texture (see texture rendering to learn about minimaps)
		//
		const GLuint TEXTURE_MIPMAP_LEVEL = 0;

	public:

		simpleTexture();
		simpleTexture(const simpleTexture& copy);
		simpleTexture(SDL_Surface* pixelBuffer,
					  GLsizei width,
					  GLsizei height,
					  GLuint textureIndex,
					  GLenum textureUnit,
					  GLenum pixelFormat,
					  GLenum pixelInternalFormat,
					  GLuint pixelAlignment,
					  GLenum pixelType);
		~simpleTexture() {};

		void operator=(const simpleTexture& copy);

		void glCreate(GLuint programHandle) override;
		void clearColor(const color& color);
		void teardown() override;
		void bind() override;

		bool isCreated() const override
		{
			return this->handle != simpleGlObject::HandleNull && openglHelper::getTextureCreated(this->handle);
		}
		bool isBound() const override
		{
			return openglHelper::getTextureBinding(this->handle);
		}

		GLenum getTextureUnit() const
		{
			return _textureUnit;
		}
		GLuint getTextureIndex() const
		{
			return _textureIndex;
		}

		SDL_Surface* getPixelBuffer() const
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
		GLenum getPixelInternalFormat() const
		{
			return _pixelInternalFormat;
		}
		GLuint getPixelAlignment() const
		{
			return _pixelAlignment;
		}
		GLenum getPixelType() const
		{
			return _pixelType;
		}

	public:

		size_t getHash() const override
		{
			return hashGenerator::generateHash(_textureUnit, _textureIndex, _pixelBuffer, _width, _height, _pixelFormat, _pixelInternalFormat, _pixelAlignment, _pixelType);
		}

	private:

		void copyImpl(const simpleTexture& copy);

	private:

		GLenum _textureUnit;
		GLuint _textureIndex;

		SDL_Surface* _pixelBuffer;
		GLsizei _width;
		GLsizei _height;
		GLenum _pixelFormat;
		GLenum _pixelInternalFormat;
		GLuint _pixelAlignment;
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
		_pixelAlignment = NULL;
		_pixelInternalFormat = NULL;
	}
	simpleTexture::simpleTexture(const simpleTexture& copy)
	{
		copyImpl(copy);
	}
	simpleTexture::simpleTexture(SDL_Surface* pixelBuffer,
								 GLsizei width,
								 GLsizei height,
								 GLuint textureIndex,
								 GLenum textureUnit,
								 GLenum pixelFormat,
								 GLenum pixelInternalFormat,
								 GLuint pixelAlignment,
								 GLenum pixelType)
	{
		_textureUnit = textureUnit;
		_textureIndex = textureIndex;

		_pixelBuffer = pixelBuffer;
		_width = width;
		_height = height;
		_pixelFormat = pixelFormat;
		_pixelInternalFormat = pixelInternalFormat;
		_pixelAlignment = pixelAlignment;
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
		_pixelInternalFormat = copy.getPixelInternalFormat();
		_pixelAlignment = copy.getPixelAlignment();
		_pixelType = copy.getPixelType();
	}

	void simpleTexture::glCreate(GLuint programHandle)
	{
		if (this->isCreated())
			throw simpleException("simpleTexture already created in the backend");

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

		// Sets the pixel alignment (related to stride) for the texture bitmap. This needs to be set according to the 
		// bitmap format. (1 = byte, 2, 4, and 8) are accepted values. 4 would be the typical word alignment.
		//glPixelStorei(GL_UNPACK_ALIGNMENT, _pixelAlignment);

		// Apply the pixel data to the backend
		glTextureImage2DEXT(this->handle,
							GL_TEXTURE_2D,
							TEXTURE_MIPMAP_LEVEL,
							_pixelInternalFormat,          // specific pixel format
							_width, _height, 0,            // width, height, border (set = 0)
							_pixelFormat,                  // symbolic (base) pixel format
							_pixelType,
							_pixelBuffer != nullptr ? _pixelBuffer->pixels : NULL);                // Pixel data in byte array

		//if (_pixelBuffer != nullptr)
		//    glTextureSubImage2DEXT(this->handle, GL_TEXTURE_2D, TEXTURE_MIPMAP_LEVEL, 0, 0, _width, _height, _pixelFormat, _pixelType, _pixelBuffer->getBuffer());

		// SETTING THESE TO DEFAULTS (TODO)
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glGenerateTextureMipmap(this->handle);

		if (!this->isCreated())
			throw simpleException("simpleTexture problem creating on the backend");
	}

	void simpleTexture::clearColor(const color& color)
	{
		if (!this->isCreated())
			throw simpleException("simpleTexture already deleted from the backend");

		if (!this->isBound())
			throw simpleException("simpleTexture not active before trying to clear color");

		vec4 clearColor(color.red, color.green, color.blue, color.alpha);
		simpleDataStream clearStream(1, clearColor.getElementVertexSize(GL_POINTS), clearColor.getStreamSize(GL_POINTS));

		// GL substitutes data using the clear color stream
		glClearTexImage(this->handle, 0, _pixelFormat, _pixelType, clearStream.getData());
	}

	void simpleTexture::teardown()
	{
		if (!this->isCreated())
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleTexture already deleted from the backend");

		glDeleteTextures(1, &_textureIndex);

		if (this->isCreated())
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleTexture still registered after deletion glIsTexture(..)");

		this->handle = simpleGlObject::HandleNull;
	}

	void simpleTexture::bind()
	{
		if (!this->isCreated())
			throw simpleException("GLTexture not yet created before calling bind()");

		glActiveTexture(_textureUnit);
		glBindTexture(GL_TEXTURE_2D, this->handle);
	}
}
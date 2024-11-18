#pragma once

#include "gl.h"
#include "openglHelper.h"
#include "simpleException.h"
#include "simpleGlObject.h"
#include "simpleLogger.h"

namespace brogueHd::frontend
{
    class simpleFrameBuffer : public simpleGlObject
    {
    public:

        simpleFrameBuffer(GLsizei sceneWidth, GLsizei sceneHeight);

        void glCreate(GLuint programHandle) override;
        void bind() override;
        void unBind() override;
        void teardown() override;

        bool isReady();
        void attachTexture(GLuint textureHandle, GLenum attachment);
        void attachRenderBuffer();

        bool isCreated() const override
        {
            return openglHelper::getFrameBufferCreated(this->handle);
        }
        bool isBound() const override
        {
            return openglHelper::getFrameBufferBinding(this->handle);
        }

    private:

        // Treating this as part of the framebuffer
        GLuint _renderBufferHandle;
        GLsizei _sceneWidth;
        GLsizei _sceneHeight;
    };

    simpleFrameBuffer::simpleFrameBuffer(GLsizei sceneWidth, GLsizei sceneHeight)
    {
        _renderBufferHandle = simpleGlObject::HandleNull;
        _sceneWidth = sceneWidth;
        _sceneHeight = sceneHeight;
    }

    void simpleFrameBuffer::glCreate(GLuint programHandle)
    {
        // Procedure:  This object should utilize a GL frame buffer to target the program's rendering
        //             to a 2D texture
        //
        // 1) Create, bind, and initialize the frame buffer
        // 2) Generate the 2D texture
        // 3) Bind the texture to the render buffer
        // 4) Bind the frame buffer as the ACTIVE frame buffer
        //

        // https://learnopengl.com/Advanced-OpenGL/Framebuffers

        glGenFramebuffers(1, &this->handle);

        // *This option sets up the frame buffer for READ (AND) WRITE
        glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
    }

    bool simpleFrameBuffer::isReady()
    {
        if (!this->isCreated())
            throw simpleException("simpleFrameBuffer has not yet been created on the backend");

        // Have to check for the status of the buffer before drawing
        //
        // // https://learnopengl.com/Advanced-OpenGL/Framebuffers
        //
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    void simpleFrameBuffer::bind()
    {
        if (!this->isCreated())
            throw simpleException("simpleFrameBuffer has not yet been created on the backend");

        glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
        // glBindRenderbuffer(GL_RENDERBUFFER, this->handle);
    }

    void simpleFrameBuffer::unBind()
    {
        if (!this->isCreated())
            throw simpleException("simpleFrameBuffer has not yet been created on the backend");

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        //glBindRenderbuffer(GL_RENDERBUFFER, 0);
    }

    void simpleFrameBuffer::attachTexture(GLuint textureHandle, GLenum attachment)
    {
        if (!this->isCreated())
            throw simpleException("simpleFrameBuffer has not yet been created on the backend");

        if (!this->isBound())
            throw simpleException("simpleFrameBuffer must be bound before use");

        // Using NAMED* for anything I can find so it doesn't assume a program structure...
        //
        glNamedFramebufferTexture2DEXT(this->handle, attachment, GL_TEXTURE_2D, textureHandle, (GLint)0);

        // Bind the texture to our frame buffer (NOTE:  Scene texture level = 0 because of no minimaps)            
        //glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureHandle, (GLint)0);
    }

    void simpleFrameBuffer::attachRenderBuffer()
    {
        // Create a render buffer for the frame buffer
        glGenRenderbuffers(1, &_renderBufferHandle);
        
        // Bind the render buffer
        glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferHandle);

        // Create the buffer storage
        glNamedRenderbufferStorage(_renderBufferHandle, GL_RGBA, _sceneWidth, _sceneHeight);

        // Attach the render buffer
        //glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBufferHandle);

        // Need to learn about this:
        if (this->handle != _renderBufferHandle)
            throw simpleException("Handle mismatch for the simpleFrameBuffer / Render Buffer Handle");
    }

    void simpleFrameBuffer::teardown()
    {
        if (!this->isCreated())
            simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleFrameBuffer already removed by backend -> continuing to try and delete rest of GPU resources");

        if (_renderBufferHandle != simpleGlObject::HandleNull)
        {
            // Delete the render buffer
            glDeleteRenderbuffers(1, &_renderBufferHandle);
        }

        // Delete THIS frame buffer
        glDeleteFramebuffers(1, &this->handle);

        this->handle = simpleGlObject::HandleNull;
        _renderBufferHandle = simpleGlObject::HandleNull;
    }
}
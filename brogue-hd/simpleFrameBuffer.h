#pragma once

#include "brogueGlobal.h"
#include "simplePrimitive.h"
#include "simpleVertexBuffer.h"
#include "simpleList.h"
#include "gl.h"

namespace brogueHd::frontend::opengl
{
    class simpleFrameBuffer : public simplePrimitive
    {
    public:

        simpleFrameBuffer(GLsizei sceneWidth, GLsizei sceneHeight);

        void glCreate(GLuint programHandle) override;
        void bind(bool bind) override;
        void teardown() override;

        bool isReady();
        void attachTexture(GLuint textureHandle, GLenum attachment);
        void attachRenderBuffer();

    private:

        // Treating this as part of the framebuffer
        GLuint _renderBufferHandle;
        GLsizei _sceneWidth;
        GLsizei _sceneHeight;

        simpleList<GLenum>* _textureAttachments;
    };

    simpleFrameBuffer::simpleFrameBuffer(GLsizei sceneWidth, GLsizei sceneHeight)
    {
        _renderBufferHandle = NULL;
        _sceneWidth = sceneWidth;
        _sceneHeight = sceneHeight;

        _textureAttachments = new simpleList<GLenum>();

        this->isCreated = false;
        this->isBound = false;
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

        this->isCreated = true;
        this->isBound = true;
    }

    bool simpleFrameBuffer::isReady()
    {
        if (!this->isCreated)
            simpleException::showCstr("simpleFrameBuffer has not yet been created on the backend");

        // Have to check for the status of the buffer before drawing
        //
        // // https://learnopengl.com/Advanced-OpenGL/Framebuffers
        //
        return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
    }

    void simpleFrameBuffer::bind(bool bind)
    {
        if (!this->isCreated)
            simpleException::showCstr("simpleFrameBuffer has not yet been created on the backend");

        if (bind)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, this->handle);
        }

        else
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        this->isBound = bind;
    }

    void simpleFrameBuffer::attachTexture(GLuint textureHandle, GLenum attachment)
    {
        if (!this->isCreated)
            simpleException::showCstr("simpleFrameBuffer has not yet been created on the backend");

        if (!this->isBound)
            simpleException::showCstr("simpleFrameBuffer must be bound before use");

        // Bind the texture to our frame buffer (NOTE:  Scene texture level = 0 because of no minimaps)            
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, textureHandle, (GLint)0);

        // Store list of attachments
        _textureAttachments->add(attachment);
    }

    void simpleFrameBuffer::attachRenderBuffer()
    {
        // Create a render buffer for the frame buffer
        glGenRenderbuffers(1, &_renderBufferHandle);
        
        // Bind the render buffer
        glBindRenderbuffer(GL_RENDERBUFFER, _renderBufferHandle);

        // Create the buffer storage
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _sceneWidth, _sceneHeight);

        // Attach the render buffer
        glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _renderBufferHandle);
    }

    void simpleFrameBuffer::teardown()
    {
        if (!this->isCreated)
            simpleException::showCstr("simpleFrameBuffer has not yet been created on the backend");

        // Unbind the framebuffer target
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        if (_renderBufferHandle != 0)
        {
            // Unbind the renderbuffer target
            glBindRenderbuffer(GL_RENDERBUFFER, 0);

            // Delete the render buffer
            glDeleteRenderbuffers(1, &_renderBufferHandle);
        }

        // Detach frame buffer textures
        _textureAttachments->forEach([](GLenum attachment)
        {
            glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, 0, 0);

            return iterationCallback::iterate;    
        });
        
        // Delete THIS frame buffer
        glDeleteFramebuffers(1, &this->handle);

        this->handle = NULL;
        this->isCreated = false;
        this->isBound = false;
    }
}
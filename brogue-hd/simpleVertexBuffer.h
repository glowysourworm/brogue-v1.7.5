#pragma once

#include "simpleArray.h"
#include "simplePrimitive.h"
#include "simpleDataStream.h"
#include "simpleVertexAttribute.h"
#include "simpleException.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Corresponds to the VBO vertex buffer object on the GL backend
    /// </summary>
    template<typename T>
    class simpleVertexBuffer : public simplePrimitive
    {
    public:
        simpleVertexBuffer(){}
        simpleVertexBuffer(int bufferIndex, simpleDataStream<T>* dataStream, const simpleList<simpleVertexAttribute>& vertexAttributes);
        ~simpleVertexBuffer();

        void bind(bool bind) override;
        void glCreate(GLuint handle) override;
        void teardown() override;

        int getBufferLength();
        int calculateAttributeStride();

        size_t getHash() const override
        {
            return hashGenerator::generateHash(_bufferIndex, _stream->getHash(), _vertexAttributes->getHash());
        }

    private:

        int _bufferIndex;

        // Primary vertex data for the buffer
        simpleDataStream<T>* _stream;
        simpleList<simpleVertexAttribute>* _vertexAttributes;
    };

    template<typename T>
    simpleVertexBuffer<T>::simpleVertexBuffer(int bufferIndex, 
                                              simpleDataStream<T>* dataStream, 
                                              const simpleList<simpleVertexAttribute>& vertexAttributes)
    {
        _stream = dataStream;
        _vertexAttributes = new simpleList<simpleVertexAttribute>(vertexAttributes);

        _bufferIndex = bufferIndex;
    }

    template<typename T>
    simpleVertexBuffer<T>::~simpleVertexBuffer()
    {
        // THESE COME FROM THE SCENE BUILDER
        delete _stream;
        delete _vertexAttributes;
    }

    template<typename T>
    void simpleVertexBuffer<T>::glCreate(GLuint programHandle)
    {
        if (this->isCreated)
            simpleException::showCstr("simpleVertexBuffer already created in the backend");

        // Procedure
        //
        // 1) Create the buffer and store the handle
        // 2) Bind the buffer on the backend to modify
        // 3) Copy the vertex data to the backend
        // 4) *Set up the attribute pointer based on the
        //     associated shader (see the GLSL resource files)
        //

        // Create buffer for vertices in the GPU memory space
        glGenBuffers(1, &this->handle);

        // Bind to array buffer type in OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, this->handle);
        
        // COPY DATA TO GPU BUFFER:  This is one of the ways to take application memory to the GPU. 
        //
        glBufferData(GL_ARRAY_BUFFER,
                    (GLsizeiptr)_stream->getStreamSize(),
                    _stream->getData(),
                    GL_STATIC_DRAW);

        // SETUP VERTEX ATTRIBUTE POINTERS:
        //
        // *** Each "in" variable in the vertex shader is being read from the glBufferData float[] input.
        //
        // 1) Setup attribute "pointers" to be at the appropriate offset with the appropriate stride.
        // 2) Call GL to enable the vertex attribute array for this buffer
        //

        int offsetBytes = 0;
        int strideBytes = calculateAttributeStride();

        _vertexAttributes->forEach([&programHandle, &offsetBytes, &strideBytes](simpleVertexAttribute attribute)
        {
            // Get the attribute handle for the input variable
            GLuint attributeHandle = glGetAttribLocation(programHandle, attribute.getName().c_str());

            GLint attributeSize = 0;
            GLenum glType = GL_FLOAT;
            GLboolean glNormalized = false;
            GLsizei currentOffset = 0;

            // HANDLE ATTRIBUTES BY DATA TYPE
            switch (attribute.getUniformType())
            {
            case GL_FLOAT_VEC2:
            {
                // Element size of the vector2
                attributeSize = 2;

                // Data type to read on the GL backend
                glType = GL_FLOAT;

                // Normalized coordinates (?) (probably UV coords)
                glNormalized = false;

                // Setup an offset to be added to the total offset
                currentOffset = 2 * sizeof(float);
            }
            break;
            default:
                simpleException::show("Unhandled vertex array attribute data type:  ", attribute.getUniformType());
            }

            // Enable the vertex attribute
            glEnableVertexAttribArray(attributeHandle);

            // Declare the attribute array configuration
            glVertexAttribPointer(attribute.getIndex(),
                                  attributeSize,
                                  glType, 
                                  glNormalized,
                                  strideBytes,
                                  (void*)offsetBytes);           // SEE RogueCreator for the function call. Not sure about void* (!!)

            // Increment the data offset
            offsetBytes += currentOffset;

            return iterationCallback::iterate;
        });

        this->isCreated = true;
    }

    template<typename T>
    void simpleVertexBuffer<T>::teardown()
    {
        if (!this->isCreated)
            simpleException::showCstr("simpleVertexBuffer already deleted from the backend");

        // Bind the CURRENT buffer to a null pointer to detach the buffer from the GL
        glBindBuffer(GL_ARRAY_BUFFER, NULL);

        // Apply vertex attributes
        glDisableVertexAttribArray(_bufferIndex);

        // Now, delete THIS buffer
        glDeleteBuffers(1, &this->handle);

        this->handle = NULL;
        this->isCreated = false;
    }

    template<typename T>
    int simpleVertexBuffer<T>::getBufferLength()
    {
        // Number of elements: Equivalent to number of ELEMENTS [ element1: { attrib1, attrib2, .. }, element2: {...} ] sent into
        //                     the GL backend
        //

        // THIS MUST PRODUCE A WHOLE NUMBER
        return _stream->getStreamNumberVertices();      
        //return _stream.GetStreamSize() / 4;
    }

    template<typename T>
    int simpleVertexBuffer<T>::calculateAttributeStride()
    {
        int seed = 0;

        return _vertexAttributes->aggregate<int>(seed, [](int stride, simpleVertexAttribute attribute)
        {
            // HANDLE ATTRIBUTES BY DATA TYPE
            if (attribute.getUniformType() == GL_FLOAT_VEC2)
                return stride + (2 * sizeof(float));
            else
                simpleException::show("Unhandled vertex array attribute data type:  ", attribute.getUniformType());
        });
    }

    template<typename T>
    void simpleVertexBuffer<T>::bind(bool bind)
    {
        if (!this->isCreated)
            simpleException::showCstr("GLVertexBuffer already deleted from the backend");

        // Bind VBO before using
        if (bind)
            glBindBuffer(GL_ARRAY_BUFFER, this->handle);

        else
            glBindBuffer(GL_ARRAY_BUFFER, NULL);

        this->isBound = bind;
    }
}
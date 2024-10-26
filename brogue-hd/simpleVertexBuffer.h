#pragma once

#include "openglHelper.h"
#include "simpleArray.h"
#include "simpleGlObject.h"
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
    class simpleVertexBuffer : public simpleGlObject
    {
    public:
        simpleVertexBuffer(){}
        simpleVertexBuffer(int bufferIndex, simpleDataStream* dataStream, const simpleList<simpleVertexAttribute>& vertexAttributes);
        ~simpleVertexBuffer();

        void bind() override;
        void glCreate(GLuint programHandle) override;
        void teardown() override;

        bool isCreated() const override
        {
            return this->handle != simpleGlObject::HandleNull && openglHelper::getVBOCreated(this->handle);
        }
        bool isBound() const override
        {
            return openglHelper::getActiveVBO() == this->handle;
        }

        int getBufferLength();

        /// <summary>
        /// Sets new data stream object for the buffer. Vertex array attributes must be the same as the old stream.
        /// </summary>
        /// <param name="newStream">Pointer to new data stream</param>
        /// <param name="deleteOld">Calls delete on the old simpleDataStream</param>
        void reBuffer(GLuint programHandle, simpleDataStream* newStream, bool deleteOld = true);

        size_t getHash() const override
        {
            return hashGenerator::generateHash(_bufferIndex, _stream->getHash(), _vertexAttributes->getHash());
        }

    private:

        int calculateAttributeStride();
        void createVertexAttributes(GLuint programHandle);

    private:

        int _bufferIndex;

        // Primary vertex data for the buffer
        simpleDataStream* _stream;
        simpleList<simpleVertexAttribute>* _vertexAttributes;
    };

    template<typename T>
    simpleVertexBuffer<T>::simpleVertexBuffer(int bufferIndex, 
                                              simpleDataStream* dataStream, 
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
        if (this->isCreated())
            simpleException::show("simpleVertexBuffer already created in the backend");

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
        glNamedBufferData(this->handle,
                            (GLsizeiptr)_stream->getStreamSize(),
                            _stream->getData(),
                            GL_DYNAMIC_DRAW);

        createVertexAttributes(programHandle);

        if (!this->isCreated())
            simpleException::show("simpleVertexBuffer creation error");
    }

    template<typename T>
    void simpleVertexBuffer<T>::createVertexAttributes(GLuint programHandle)
    {
        // SETUP VERTEX ATTRIBUTE POINTERS:
        //
        // *** Each "in" variable in the vertex shader is being read from the glBufferData float[] input.
        //
        // 1) Setup attribute "pointers" to be at the appropriate offset with the appropriate stride.
        // 2) Call GL to enable the vertex attribute array for this buffer
        //

        int offsetBytes = 0;
        int strideBytes = calculateAttributeStride();

        _vertexAttributes->forEach([&programHandle, &offsetBytes, &strideBytes] (simpleVertexAttribute attribute)
        {
            // Get the attribute handle for the input variable
            GLuint attributeHandle = glGetAttribLocation(programHandle, attribute.getName().c_str());

            GLint attributeSize = 0;
            GLenum glType = GL_FLOAT;
            GLboolean glNormalized = false;
            GLsizei currentOffset = 0;

            // NOTES:         Khronos Group
            //
            // attributeSize: Element size of the float
            //
            // glType:        Data type to read on the GL backend:
            //
            //                Specifies the data type of each component in the array. The symbolic 
            //                constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, 
            //                and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer. 
            //                             
            //                Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, 
            //                GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer. 
            //            
            //                GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only 
            //                token accepted by the type parameter for that function. The initial value is GL_FLOAT.
            //
            // glNormalized:  Normalized coordinates (?) (NO)
            // 
            //                For glVertexAttribPointer, specifies whether fixed-point 
            //                data values should be normalized (GL_TRUE) or converted 
            //                directly as fixed-point values (GL_FALSE) when they are accessed.
            //
            // currentOffset: Used to lay out this "struct" on the vertex buffer. Total # of bytes
            //


            // HANDLE ATTRIBUTES BY DATA TYPE
            switch (attribute.getAttributeType())
            {
            case GL_FLOAT_VEC4:
            {
                attributeSize = 4;
                glType = GL_FLOAT;
                glNormalized = false;
                currentOffset = 4 * sizeof(float);
            }
            break;
            case GL_FLOAT_VEC3:
            {
                attributeSize = 3;
                glType = GL_FLOAT;
                glNormalized = false;
                currentOffset = 3 * sizeof(float);
            }
            break;
            case GL_FLOAT_VEC2:
            {
                attributeSize = 2;
                glType = GL_FLOAT;
                glNormalized = false;
                currentOffset = 2 * sizeof(float);
            }
            break;
            case GL_FLOAT:
            {
                attributeSize = 1;
                glType = GL_FLOAT;
                glNormalized = false;
                currentOffset = sizeof(float);
            }
            break;
            case GL_INT_VEC2:
            {
                attributeSize = 2;
                glType = GL_INT;
                glNormalized = false;
                currentOffset = 2 * sizeof(int);
            }
            break;
            case GL_INT:
            {
                attributeSize = 1;
                glType = GL_INT;
                glNormalized = false;
                currentOffset = sizeof(int);
            }
            break;
            default:
                simpleException::show("Unhandled vertex array attribute data type:  ", attribute.getAttributeType());
            }

            // Enable the vertex attribute
            glEnableVertexAttribArray(attributeHandle);

            // Declare the attribute array configuration
            glVertexAttribPointer(attribute.getIndex(),
                                    attributeSize,
                                    glType,
                                    glNormalized,
                                    strideBytes,
                                    (void*)offsetBytes);           // This is the offset of the current attribute.

            // Increment the data offset
            offsetBytes += currentOffset;

            return iterationCallback::iterate;
        });
    }

    template<typename T>
    void simpleVertexBuffer<T>::teardown()
    {
        if (!this->isCreated())
            simpleException::show("simpleVertexBuffer already deleted from the backend");

        // Deleting (likely) takes care of other resources
        glDeleteBuffers(1, &this->handle);

        if (this->isCreated())
            simpleException::show("Error deleting simpleVertexBuffer");
    }

    template<typename T>
    void simpleVertexBuffer<T>::reBuffer(GLuint programHandle, simpleDataStream* newStream, bool deleteOld)
    {
        if (!this->isCreated())
            simpleException::show("simpleVertexBuffer not yet created on the GL backend. Call glCreate first.");

        if (newStream->getStreamSize() != _stream->getStreamSize())
            simpleException::show("Trying to call glBufferSubData with a different sized data stream. Please check your data stream output.");

        // (MEMORY!) These aren't yet managed. There is a "builder" pattern; but nothing to help manage them. 
        if (deleteOld)
            delete _stream;

        // Set new data stream
        _stream = newStream;

        // Need to try named buffer. Active buffer should find the right buffer index; but it was having .. trouble.
        glNamedBufferSubData(this->handle, (GLintptr)0, (GLsizeiptr)_stream->getStreamSize(), (void*)_stream->getData());

        // Khronos Group:  glBufferData will delete anything that has been allocated on this particular buffer.
        //
        //glBufferData(GL_ARRAY_BUFFER,
        //            (GLsizeiptr)_stream->getStreamSize(), 
        //            _stream->getData(),
        //            GL_DYNAMIC_DRAW);
    }

    template<typename T>
    int simpleVertexBuffer<T>::getBufferLength()
    {
        // Number of elements: Equivalent to number of ELEMENTS [ element1: { attrib1, attrib2, .. }, element2: {...} ] sent into
        //                     the GL backend
        //

        // THIS MUST PRODUCE A WHOLE NUMBER
        return _stream->getStreamSize();
    }

    template<typename T>
    int simpleVertexBuffer<T>::calculateAttributeStride()
    {
        int seed = 0;

        return _vertexAttributes->aggregate<int>(seed, [](int stride, simpleVertexAttribute attribute)
        {
            // HANDLE ATTRIBUTES BY DATA TYPE
            if (attribute.getAttributeType() == GL_FLOAT)
                return stride + sizeof(float);
            else if (attribute.getAttributeType() == GL_INT)
                return stride + sizeof(int);
            else if (attribute.getAttributeType() == GL_FLOAT_VEC2)
                return stride + (2 * sizeof(float));
            else if (attribute.getAttributeType() == GL_INT_VEC2)
                return stride + (2 * sizeof(int));
            else if (attribute.getAttributeType() == GL_FLOAT_VEC3)
                return stride + (3 * sizeof(float));
            else if (attribute.getAttributeType() == GL_FLOAT_VEC4)
                return stride + (4 * sizeof(float));
            else
                simpleException::show("Unhandled vertex array attribute data type:  ", attribute.getAttributeType());
        });
    }

    template<typename T>
    void simpleVertexBuffer<T>::bind()
    {
        if (!this->isCreated())
            simpleException::show("GLVertexBuffer already deleted from the backend");

        // Bind VBO before using
        glBindBuffer(GL_ARRAY_BUFFER, this->handle);
    }
}
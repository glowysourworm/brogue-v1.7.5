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

        /// <summary>
        /// Sets new data stream object for the buffer. Vertex array attributes must be the same as the old stream.
        /// </summary>
        /// <param name="newStream">Pointer to new data stream</param>
        /// <param name="deleteOld">Calls delete on the old simpleDataStream</param>
        void reBuffer(simpleDataStream<T>* newStream, bool deleteOld = true);

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
            switch (attribute.getAttributeType())
            {
            case GL_FLOAT_VEC3:
            {
                // Element size of the float
                attributeSize = 3;

                // Data type to read on the GL backend:
                //
                // Khronos Group:  Specifies the data type of each component in the array. The symbolic 
                //                 constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, 
                //                 and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer. 
                //                 
                //                 Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, 
                //                 GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer. 
                //
                //                 GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only 
                //                 token accepted by the type parameter for that function. The initial value is GL_FLOAT.
                glType = GL_FLOAT;

                // Normalized coordinates (?)
                // 
                // Khronos Group:  For glVertexAttribPointer, specifies whether fixed-point 
                //                 data values should be normalized (GL_TRUE) or converted 
                //                 directly as fixed-point values (GL_FALSE) when they are accessed.
                glNormalized = false;

                // Setup an offset to be added to the total offset
                currentOffset = 3 * sizeof(float);
            }
            break;
            case GL_FLOAT_VEC2:
            {
                // Element size of the vector2
                attributeSize = 2;

                // Data type to read on the GL backend:
                //
                // Khronos Group:  Specifies the data type of each component in the array. The symbolic 
                //                 constants GL_BYTE, GL_UNSIGNED_BYTE, GL_SHORT, GL_UNSIGNED_SHORT, GL_INT, 
                //                 and GL_UNSIGNED_INT are accepted by glVertexAttribPointer and glVertexAttribIPointer. 
                //                 
                //                 Additionally GL_HALF_FLOAT, GL_FLOAT, GL_DOUBLE, GL_FIXED, GL_INT_2_10_10_10_REV, 
                //                 GL_UNSIGNED_INT_2_10_10_10_REV and GL_UNSIGNED_INT_10F_11F_11F_REV are accepted by glVertexAttribPointer. 
                //
                //                 GL_DOUBLE is also accepted by glVertexAttribLPointer and is the only 
                //                 token accepted by the type parameter for that function. The initial value is GL_FLOAT.
                glType = GL_FLOAT;

                // Normalized coordinates (?)
                // 
                // Khronos Group:  For glVertexAttribPointer, specifies whether fixed-point 
                //                 data values should be normalized (GL_TRUE) or converted 
                //                 directly as fixed-point values (GL_FALSE) when they are accessed.
                glNormalized = false;

                // Setup an offset to be added to the total offset
                currentOffset = 2 * sizeof(float);
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

        this->isCreated = true;
        this->isBound = true;
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
    void simpleVertexBuffer<T>::reBuffer(simpleDataStream<T>* newStream, bool deleteOld)
    {
        if (!this->isCreated)
            simpleException::show("simpleVertexBuffer not yet created on the GL backend. Call glCreate first.");

        // Apparently, no need to un-bind. GL_INVALID_OPERATION is thrown if the buffer is not bound

        // (MEMORY!) These aren't yet managed. There is a "builder" pattern; but nothing to help manage them. 
        if (deleteOld)
            delete _stream;

        // Set new data stream
        _stream = newStream;

        // Khronos Group:  glBufferData will delete anything that has been allocated on this particular buffer.
        //
        glBufferData(GL_ARRAY_BUFFER,
                    (GLsizeiptr)_stream->getStreamSize(),
                    _stream->getData(),
                    GL_STATIC_DRAW);
    }

    template<typename T>
    int simpleVertexBuffer<T>::getBufferLength()
    {
        // Number of elements: Equivalent to number of ELEMENTS [ element1: { attrib1, attrib2, .. }, element2: {...} ] sent into
        //                     the GL backend
        //

        // THIS MUST PRODUCE A WHOLE NUMBER
        return _stream->getStreamNumberVertices();
        //return _stream->getStreamSize();
        //return _stream.GetStreamSize() / 4;
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
            else if (attribute.getAttributeType() == GL_FLOAT_VEC2)
                return stride + (2 * sizeof(float));
            else if (attribute.getAttributeType() == GL_FLOAT_VEC3)
                return stride + (3 * sizeof(float));
            else
                simpleException::show("Unhandled vertex array attribute data type:  ", attribute.getAttributeType());
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
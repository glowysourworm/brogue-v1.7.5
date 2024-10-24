#pragma once

#include "simple.h"
#include "simpleArray.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Simple class to handle streaming the GL data vector
    /// </summary>
	class simpleDataStream : public hashableObject
	{
    public:

        simpleDataStream()
        {
            _streamNumberElements = 0;
            _numberVerticesPerElement = 0;
            _array = nullptr;
            _cursor = 0;
        }
        ~simpleDataStream()
        {
            delete _array;
        }

        /// <summary>
        /// Creates a data stream with storage for N number of elements
        /// </summary>
        /// <param name="numberShaderCallsPerElement">Total number of shader calls (vertices) per element</param>
        /// <param name="elementByteSize">Total size of a single element in bytes</param>
        /// <param name="elementCount">Total count in terms of elements</param>
        simpleDataStream(int elementCount, int numberShaderCallsPerElement, int elementByteSize)
        {
            _streamNumberElements = elementCount;
            _numberVerticesPerElement = numberShaderCallsPerElement;
            _array = new simpleArray<byte>(elementCount * elementByteSize);
            _cursor = 0;
        }

        /// <summary>
        /// Returns total BYTE length of the stream
        /// </summary>
        int getStreamSize()
        {
            return _array->count();
        }

        int getStreamNumberElements()
        {
            return _streamNumberElements;
        }

        void writeFloat(float primitive)
        {
            byte floatBytes[sizeof(float)];

            std::memcpy(&floatBytes, &primitive, sizeof(float));

            for (int index = 0; index < sizeof(float); index++)
                _array->set(_cursor++, floatBytes[index]);
        }
        void writeInt(int primitive)
        {
            byte intBytes[sizeof(int)];

            std::memcpy(&intBytes, &primitive, sizeof(int));

            for (int index = 0; index < sizeof(int); index++)
                _array->set(_cursor++, intBytes[index]);
        }

        const byte* getData()
        {
            return _array->getArray();
        }
        
        size_t getHash() const override
        {
            return hashGenerator::generateHash(_array->getHash(), _cursor, _streamNumberElements);
        }

    private:

        simpleArray<byte>* _array;

        int _cursor;
        int _streamNumberElements;
        int _numberVerticesPerElement;
	};
}
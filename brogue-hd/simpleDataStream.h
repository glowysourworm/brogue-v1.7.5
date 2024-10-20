#pragma once

#include "simple.h"
#include "simpleArray.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Simple class to handle streaming the IGLVector instances more specifically
    /// </summary>
    template<isHashable T>
	class simpleDataStream : public hashableObject
	{
    public:

        simpleDataStream()
        {
            _streamNumberVertices = 0;
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
        /// <param name="vectorElementNumberVertices">Size of a single element IN # VERTICES</param>
        /// <param name="vectorStreamSize">Total size of a single stream (BYTES)</param>
        /// <param name="capacity">Total capacity IN TERMS OF element INSTANCES!</param>
        simpleDataStream(int capacity, int vectorElementNumberVertices, int vectorStreamSize)
        {
            _streamNumberVertices = capacity * vectorElementNumberVertices;
            _array = new simpleArray<T>(capacity * vectorStreamSize);
            _cursor = 0;
        }

        /// <summary>
        /// Returns total BYTE length of the stream
        /// </summary>
        int getStreamSize()
        {
            return _array->count() * sizeof(T);
        }

        int getStreamNumberVertices()
        {
            return _streamNumberVertices;
        }

        void write(T primitive)
        {
            _array->set(_cursor++, primitive);
        }

        const T* getData()
        {
            return _array->getArray();
        }
        
        size_t getHash() const override
        {
            return hashGenerator::generateHash(_array->getHash(), _cursor, _streamNumberVertices);
        }

    private:

        simpleArray<T>* _array;

        /// <summary>
        /// Stream cursor (per ELEMENT)
        /// </summary>
        int _cursor;

        /// <summary>
        /// Number of vertices per ELEMENT
        /// </summary>
        int _streamNumberVertices;
	};
}
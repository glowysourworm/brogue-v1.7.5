#pragma once

#include "simpleArray.h"

using namespace brogueHd::component;

namespace brogueHd::frontend::opengl
{
    /// <summary>
    /// Simple class to handle streaming the IGLVector instances more specifically
    /// </summary>
    template<typename T>
	class simpleDataStream
	{
        /// <summary>
        /// Creates a data stream with storage for N number of elements
        /// </summary>
        /// <param name="vectorElementNumberVertices">Size of a single element IN # VERTICES</param>
        /// <param name="vectorStreamSize">Total size of a single stream (BYTES)</param>
        /// <param name="capacity">Total capacity IN TERMS OF element INSTANCES!</param>
        public DataStream(int capacity, int vectorElementNumberVertices, int vectorStreamSize)
        {
            _streamNumberVertices = capacity * vectorElementNumberVertices;

            _array = new simpleArray<T>(capacity);

            _cursor = 0;
        }

        /// <summary>
        /// Returns total BYTE length of the stream
        /// </summary>
        int getStreamSize()
        {
            return _array.Length * sizeof(T);
        }

        void write(T primitive)
        {
            _array[_cursor++] = primitive;
        }

        public T* getData()
        {
            return _array->getArray();
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
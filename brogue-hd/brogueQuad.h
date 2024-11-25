#pragma once
#include "gl.h"
#include "gridLocator.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleGlData.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	struct brogueQuad : simpleGlData
	{
	public:

		brogueQuad()
		{
			location = default_value::value<gridLocator>();
		}
		brogueQuad(const brogueQuad& copy)
		{
			copyImpl(copy);
		}
		brogueQuad(const gridLocator& locator)
		{
			location = locator;
		}
		void operator=(const brogueQuad& copy)
		{
			copyImpl(copy);
		}

		virtual int getElementVertexSize(GLenum primitiveType) const override
		{
			throw simpleException("getElementVertexSize must be overridden in child class");
		}
		virtual int getStreamSize(GLenum primitiveType) const override
		{
			throw simpleException("getStreamSize must be overridden in child class");
		}
		virtual void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			throw simpleException("streamBuffer must be overridden in child class");
		}

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(location);
		}

	private:

		void copyImpl(const brogueQuad& copy)
		{
			location = copy.location;
		}

	public:

		// Cached location value (used for re-streaming)
		gridLocator location;
	};
}
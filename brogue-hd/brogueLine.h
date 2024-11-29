#pragma once

#include "gl.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleGlData.h"

namespace brogueHd::frontend
{
	struct brogueLine : simpleGlData
	{
		brogueLine()
		{
			_point1 = default_value::value<vec2>();
			_point2 = default_value::value<vec2>();
		}
		brogueLine(const vec2& point1, const vec2& point2)
		{
			_point1 = point1;
			_point2 = point2;
		}
		brogueLine(const brogueLine& copy)
		{
			copyImpl(copy);
		}
		void operator=(const brogueLine& copy)
		{
			copyImpl(copy);
		}

		int getElementVertexSize(GLenum primitiveType) const override
		{
			// Total # of calls to the shader
			switch (primitiveType)
			{
				case GL_LINES:
					return 2;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for brogueLine:  PrimitiveType={}", primitiveType));
			}
		}

		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_LINES:
					return (4 * sizeof(float));
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType));
					break;
			}
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			_point1.streamBuffer(primitiveType, outputStream);	// vec2
			_point2.streamBuffer(primitiveType, outputStream);	// vec2
		}

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(_point1, _point2);
		}

	private:

		vec2 getPoint1() const
		{
			return _point1;
		}

		vec2 getPoint2() const
		{
			return _point2;
		}

		void copyImpl(const brogueLine& copy)
		{
			_point1 = copy.getPoint1();
			_point2 = copy.getPoint2();
		}

	private:

		vec2 _point1;
		vec2 _point2;

	};
}
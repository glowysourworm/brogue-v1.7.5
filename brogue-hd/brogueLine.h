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
			_point_isOdd = default_value::value<vec3>();
			_lineColor = default_value::value<vec4>();
		}
		brogueLine(const vec3& pointIsOdd, const vec4& lineColor)
		{
			_point_isOdd = pointIsOdd;
			_lineColor = lineColor;
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
					return 1;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for brogueLine:  PrimitiveType={}", primitiveType));
			}
		}

		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_LINES:
					return (7 * sizeof(float));
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType));
					break;
			}
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			_point_isOdd.streamBuffer(primitiveType, outputStream);	// vec3
			_lineColor.streamBuffer(primitiveType, outputStream);	// vec4
		}

		virtual size_t getHash() const override
		{
			return hashGenerator::generateHash(_point_isOdd, _lineColor);
		}

	private:

		vec3 getPointIsOdd() const
		{
			return _point_isOdd;
		}

		vec4 getLineColor() const
		{
			return _lineColor;
		}

		void copyImpl(const brogueLine& copy)
		{
			_point_isOdd = copy.getPointIsOdd();
			_lineColor = copy.getLineColor();
		}

	private:

		// Current point in the point set for the GL line
		vec3 _point_isOdd;
		vec4 _lineColor;

	};
}
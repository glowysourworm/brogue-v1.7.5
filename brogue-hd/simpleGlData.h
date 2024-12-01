#pragma once

/*
	Had to write this because of missing opengl data types commonly used. These
	will work better with our backend:  vec2, vec4, ...
*/

#include "gl.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleString.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	struct simpleGlData : simpleStruct
	{
		simpleGlData() {}
		simpleGlData(const simpleGlData& copy) {}
		~simpleGlData() {}

		/// <summary>
		/// Returns the number of vertices (shader calls) per element
		/// </summary>
		virtual int getElementVertexSize(GLenum primitiveType) const
		{
			return 0;
		}

		/// <summary>
		/// Returns the total number of bytes for the element's stream
		/// </summary>
		virtual int getStreamSize(GLenum primitiveType) const
		{
			return 0;
		}

		/// <summary>
		/// Utilizes the stream's output facility to stream its data bytes to the memory
		/// holder.
		/// </summary>
		virtual void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const
		{

		}
		virtual size_t getHash() const override
		{
			return 0;
		}
		virtual const char* toString() const
		{
			return "";
		}
	};

	struct vec2 : simpleGlData
	{
		float x;
		float y;

		vec2()
		{
			x = 0;
			y = 0;
		}
		vec2(const vec2& copy)
		{
			x = copy.x;
			y = copy.y;
		}
		vec2(float ax, float ay)
		{
			x = ax;
			y = ay;
		}
		void operator=(const vec2& other)
		{
			x = other.x;
			y = other.y;
		}

		bool operator==(const vec2& other)
		{
			return compare(other);
		}
		bool operator!=(const vec2& other)
		{
			return !compare(other);
		}

		int getElementVertexSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 1;
					break;
				case GL_LINES:
					return 1;
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			return 2 * sizeof(float);
		}
		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					outputStream->writeFloat(x);
					outputStream->writeFloat(y);
					break;
				case GL_LINES:
					outputStream->writeFloat(x);
					outputStream->writeFloat(y);
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y);
		}

		const char* toString() const override
		{
			return simpleString(simpleExt::format("vec2({},{})", x, y)).c_str();
		}
	private:

		bool compare(const vec2& other)
		{
			return x == other.x && y == other.y;
		}
	};

	struct ivec2 : simpleGlData
	{
		int x;
		int y;

		ivec2()
		{
			x = 0;
			y = 0;
		}
		ivec2(const ivec2& copy)
		{
			x = copy.x;
			y = copy.y;
		}
		ivec2(int ax, int ay)
		{
			x = ax;
			y = ay;
		}
		void operator=(const ivec2& other)
		{
			x = other.x;
			y = other.y;
		}

		bool operator==(const ivec2& other)
		{
			return compare(other);
		}
		bool operator!=(const ivec2& other)
		{
			return !compare(other);
		}

		int getElementVertexSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 1;
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			// total # of ints
			return 2 * sizeof(int);
		}
		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					outputStream->writeInt(x);
					outputStream->writeInt(y);
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}

		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y);
		}
		const char* toString() const override
		{
			return simpleExt::format("vec2({},{})", x, y).c_str();
		}
	private:

		bool compare(const ivec2& other)
		{
			return x == other.x && y == other.y;
		}
	};

	struct vec3 : simpleGlData
	{
		float x;
		float y;
		float z;

		vec3()
		{
			x = 0;
			y = 0;
			z = 0;
		}
		vec3(const vec3& copy)
		{
			x = copy.x;
			y = copy.y;
			z = copy.z;
		}
		vec3(float ax, float ay, float az)
		{
			x = ax;
			y = ay;
			z = az;
		}
		void operator=(const vec3& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;
		}
		int getElementVertexSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 1;
					break;
				case GL_LINES:
					return 1;
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			return 3 * sizeof(float);
		}
		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
				case GL_LINES:
					outputStream->writeFloat(x);
					outputStream->writeFloat(y);
					outputStream->writeFloat(z);
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y, z);
		}
		const char* toString() const override
		{
			return simpleExt::format("vec3({},{},{})", x, y, z).c_str();
		}
	};

	struct vec4 : simpleGlData
	{
		float x;
		float y;
		float z;
		float w;

		vec4()
		{
			x = 0;
			y = 0;
			z = 0;
			w = 0;
		}
		vec4(const vec4& copy)
		{
			x = copy.x;
			y = copy.y;
			w = copy.w;
			z = copy.z;
		}
		vec4(float ax, float ay, float az, float aw)
		{
			x = ax;
			y = ay;
			z = az;
			w = aw;
		}
		void operator=(const vec4& other)
		{
			x = other.x;
			y = other.y;
			w = other.w;
			z = other.z;
		}
		bool operator==(const vec4& other) const
		{
			return compare(other);
		}
		bool operator!=(const vec4& other) const
		{
			return !compare(other);
		}


		int getElementVertexSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_POINTS:
					return 1;
				case GL_LINES:
					return 1;
				case GL_TRIANGLES:
					return 1;
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			return 4 * sizeof(float);
		}
		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			switch (primitiveType)
			{
				case GL_POINTS:
				case GL_LINES:
				case GL_TRIANGLES:
					outputStream->writeFloat(x);
					outputStream->writeFloat(y);
					outputStream->writeFloat(z);
					outputStream->writeFloat(w);
					break;
				default:
					throw simpleException("Unhandled primitive GLenum type:  vec2 in simpleGlData.h");
			}
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(x, y, z, w);
		}
		const char* toString() const override
		{
			return simpleExt::format("vec4({},{},{},{})", x, y, z, w).c_str();
		}

	private:

		bool compare(const vec4& other) const
		{
			return x == other.x &&
				y == other.y &&
				z == other.z &&
				w == other.w;
		}
	};

	struct simpleQuad : simpleGlData
	{
		vec2 topLeft;
		vec2 topRight;
		vec2 bottomLeft;
		vec2 bottomRight;

		simpleQuad()
		{
			topLeft = default_value::value<vec2>();
			topRight = default_value::value<vec2>();
			bottomLeft = default_value::value<vec2>();
			bottomRight = default_value::value<vec2>();
		}
		simpleQuad(const simpleQuad& copy)
		{
			topLeft = copy.topLeft;
			topRight = copy.topRight;
			bottomLeft = copy.bottomLeft;
			bottomRight = copy.bottomRight;
		}
		simpleQuad(float left, float top, float right, float bottom)
		{
			topLeft = vec2(left, top);
			topRight = vec2(right, top);
			bottomLeft = vec2(left, bottom);
			bottomRight = vec2(right, bottom);
		}
		void operator=(const simpleQuad& copy)
		{
			topLeft = copy.topLeft;
			topRight = copy.topRight;
			bottomLeft = copy.bottomLeft;
			bottomRight = copy.bottomRight;
		}

		float getWidth()
		{
			return bottomRight.x - topLeft.x;
		}
		float getHeight()
		{
			return  topLeft.y - bottomRight.y;
		}
		int getElementVertexSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 6;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType).c_str());
					break;
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 12 * sizeof(float);
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType).c_str());
					break;
			}
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			// These should probably be tested! :)
			switch (primitiveType)
			{
				case GL_TRIANGLES:
				{
					// (Triangle 1) topLeft, topRight, bottomRight 
					topLeft.streamBuffer(primitiveType, outputStream);		// vec2
					topRight.streamBuffer(primitiveType, outputStream);
					bottomRight.streamBuffer(primitiveType, outputStream);

					// (Triangle 2) topLeft, bottomRight, bottomLeft
					topLeft.streamBuffer(primitiveType, outputStream);
					bottomRight.streamBuffer(primitiveType, outputStream);
					bottomLeft.streamBuffer(primitiveType, outputStream);
				}
				break;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType).c_str());
					break;
			}
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(topLeft, bottomRight);
		}
		const char* toString() const override
		{
			return simpleExt::format("quad(top={},left={},right={},bottom={})", topLeft.y, topLeft.x, bottomRight.x, bottomRight.y).c_str();
		}
	};

	struct simpleiQuad : simpleGlData
	{
		ivec2 topLeft;
		ivec2 topRight;
		ivec2 bottomLeft;
		ivec2 bottomRight;

		simpleiQuad()
		{
			topLeft = default_value::value<ivec2>();
			topRight = default_value::value<ivec2>();
			bottomLeft = default_value::value<ivec2>();
			bottomRight = default_value::value<ivec2>();
		}
		simpleiQuad(const simpleiQuad& copy)
		{
			topLeft = copy.topLeft;
			topRight = copy.topRight;
			bottomLeft = copy.bottomLeft;
			bottomRight = copy.bottomRight;
		}
		simpleiQuad(int left, int top, int right, int bottom)
		{
			topLeft = ivec2(left, top);
			topRight = ivec2(right, top);
			bottomLeft = ivec2(left, bottom);
			bottomRight = ivec2(right, bottom);
		}
		void operator=(const simpleiQuad& copy)
		{
			topLeft = copy.topLeft;
			topRight = copy.topRight;
			bottomLeft = copy.bottomLeft;
			bottomRight = copy.bottomRight;
		}

		int getElementVertexSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 6;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType).c_str());
					break;
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 12 * sizeof(int);
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType).c_str());
					break;
			}
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			// These should probably be tested! :)
			switch (primitiveType)
			{
				case GL_TRIANGLES:
				{
					// (Triangle 1) topLeft, topRight, bottomRight 
					topLeft.streamBuffer(primitiveType, outputStream);
					topRight.streamBuffer(primitiveType, outputStream);
					bottomRight.streamBuffer(primitiveType, outputStream);

					// (Triangle 2) topLeft, bottomRight, bottomLeft
					topLeft.streamBuffer(primitiveType, outputStream);
					bottomRight.streamBuffer(primitiveType, outputStream);
					bottomLeft.streamBuffer(primitiveType, outputStream);
				}
				break;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType).c_str());
					break;
			}
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(topLeft, bottomRight);
		}
		const char* toString() const override
		{
			return simpleExt::format("quad(top={},left={},right={},bottom={})", topLeft.y, topLeft.x, bottomRight.x, bottomRight.y).c_str();
		}
	};

	struct simpleQuad3 : simpleGlData
	{
	public:

		vec3 topLeft;
		vec3 topRight;
		vec3 bottomLeft;
		vec3 bottomRight;

		simpleQuad3()
		{
			topLeft = default_value::value<vec3>();
			topRight = default_value::value<vec3>();
			bottomLeft = default_value::value<vec3>();
			bottomRight = default_value::value<vec3>();
		}
		simpleQuad3(const simpleQuad3& copy)
		{
			topLeft = copy.topLeft;
			topRight = copy.topRight;
			bottomLeft = copy.bottomLeft;
			bottomRight = copy.bottomRight;
		}
		simpleQuad3(const simpleQuad& quad, bool inUse)
		{
			topLeft = vec3(quad.topLeft.x, quad.topLeft.y, inUse ? 1.0f : 0.0f);
			topRight = vec3(quad.topRight.x, quad.topRight.y, inUse ? 1.0f : 0.0f);
			bottomLeft = vec3(quad.bottomLeft.x, quad.bottomLeft.y, inUse ? 1.0f : 0.0f);
			bottomRight = vec3(quad.bottomRight.x, quad.bottomRight.y, inUse ? 1.0f : 0.0f);
		}
		simpleQuad3(float left, float top, float right, float bottom, float atopLeft, float atopRight, float abottomLeft, float abottomRight)
		{
			topLeft = vec3(left, top, atopLeft);
			topRight = vec3(right, top, atopRight);
			bottomLeft = vec3(left, bottom, abottomLeft);
			bottomRight = vec3(right, bottom, abottomRight);
		}
		void operator=(const simpleQuad3& copy)
		{
			topLeft = copy.topLeft;
			topRight = copy.topRight;
			bottomLeft = copy.bottomLeft;
			bottomRight = copy.bottomRight;
		}

		int getElementVertexSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 6;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType));
					break;
			}
		}
		int getStreamSize(GLenum primitiveType) const override
		{
			switch (primitiveType)
			{
				case GL_TRIANGLES:
					return 18 * sizeof(float);
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType));
					break;
			}
		}

		void streamBuffer(GLenum primitiveType, simpleDataStream* outputStream) const override
		{
			// These should probably be tested! :)
			switch (primitiveType)
			{
				case GL_TRIANGLES:
				{
					// (Triangle 1) topLeft, topRight, bottomRight 
					topLeft.streamBuffer(primitiveType, outputStream);		// vec3
					topRight.streamBuffer(primitiveType, outputStream);
					bottomRight.streamBuffer(primitiveType, outputStream);

					// (Triangle 2) topLeft, bottomRight, bottomLeft
					topLeft.streamBuffer(primitiveType, outputStream);
					bottomRight.streamBuffer(primitiveType, outputStream);
					bottomLeft.streamBuffer(primitiveType, outputStream);
				}
				break;
				default:
					throw simpleException(simpleExt::format("Unhandled primitive type for GLQuad:  {}", primitiveType));
					break;
			}
		}
		size_t getHash() const override
		{
			return hashGenerator::generateHash(topLeft, topRight, bottomLeft, bottomRight);
		}
		const char* toString() const override
		{
			return simpleExt::format("quad(top={},left={},right={},bottom={})", topLeft.y, topLeft.x, bottomRight.x, bottomRight.y).c_str();
		}
	};

	struct vertexAttributeData : simpleStruct
	{
		vertexAttributeData()
		{
			index = -1;
			name = default_value::value<simpleString>();
			type = default_value::value<GLenum>();
		}
		vertexAttributeData(const vertexAttributeData& copy)
		{
			copyImpl(copy);
		}
		vertexAttributeData(int aindex, const simpleString& aname, GLenum atype)
		{
			index = aindex;
			name = aname;
			type = atype;
		}
		~vertexAttributeData()
		{

		}

		void operator=(const vertexAttributeData& copy)
		{
			copyImpl(copy);
		}

		int index;
		simpleString name;

		/// <summary>
		/// Type of input data for the shader - this would be some sort of GLSL supported data type.
		/// </summary>
		GLenum type;

		size_t getHash() const override
		{
			return hashGenerator::generateHash(name, type, index);
		}

	private:

		void copyImpl(const vertexAttributeData& copy)
		{
			index = copy.index;
			name = copy.name;
			type = copy.type;
		}
	};
}
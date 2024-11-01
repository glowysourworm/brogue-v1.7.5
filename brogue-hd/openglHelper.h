#pragma once

#include "simpleLogger.h"
#include "simpleString.h"
#include "simpleList.h"
#include "simpleGlObject.h"
#include "gl.h"

#include <SDL_image.h>
#include <SDL_surface.h>

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	class openglHelper
	{
	public:

		static GLint getActiveVBO()
		{
			GLint handle = -1;
			glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &handle);

			return handle;
		}

		static GLint getActiveVAO()
		{
			GLint handle = -1;
			glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &handle);

			return handle;
		}

		static GLint getActiveProgram()
		{
			GLint handle = -1;
			glGetIntegerv(GL_CURRENT_PROGRAM, &handle);

			return handle;
		}

		static bool getProgramCreated(GLuint programHandle)
		{
			return glIsProgram(programHandle) == GL_TRUE;
		}

		static bool getVAOCreated(GLuint vaoHandle)
		{
			return glIsVertexArray(vaoHandle) == GL_TRUE;
		}

		static bool getVBOCreated(GLuint vboHandle)
		{
			return glIsBuffer(vboHandle) == GL_TRUE;
		}

		static bool getShaderCreated(GLuint shaderHandle)
		{
			return glIsShader(shaderHandle) == GL_TRUE;
		}

		static bool getTextureCreated(GLuint textureHandle)
		{
			return glIsTexture(textureHandle) == GL_TRUE;
		}

		static bool getVAOBinding(GLuint vaoHandle)
		{
			return getActiveVAO() == vaoHandle;
		}

		static bool getFrameBufferBinding(GLuint framebufferHandle)
		{
			GLint activeFrameBuffer = -1;

			glGetIntegerv(GL_FRAMEBUFFER_BINDING, &activeFrameBuffer);

			return activeFrameBuffer == framebufferHandle;
		}

		static bool getFrameBufferCreated(GLuint framebufferHandle)
		{
			return glIsFramebuffer(framebufferHandle) == GL_TRUE;
		}

		static bool getTextureBinding(GLuint textureHandle)
		{
			GLint boundHandle;

			glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundHandle);

			return boundHandle == textureHandle;
		}

		static bool getShaderCompilerError(GLuint shaderHandle)
		{
			GLchar buffer[1000];
			GLsizei  length = 0;

			// Check info log for the errors
			glGetShaderInfoLog(shaderHandle, 1000, &length, buffer);

			return length > 0;
		}

		static bool getProgramError(GLuint programHandle)
		{
			GLchar buffer[10000];
			GLsizei  length = 0;

			// Check info log for the errors
			glGetProgramInfoLog(programHandle, 10000, &length, buffer);

			return length > 0;
		}

		static void outputShaderInfoLog(GLuint shaderHandle)
		{
			GLchar buffer[1000];
			GLsizei  length = 0;

			// Check info log for the errors
			glGetShaderInfoLog(shaderHandle, 1000, &length, buffer);

			// For now, just show the exception from the shader
			if (length > 0)
				simpleLogger::logColor(brogueConsoleColor::Red, buffer);
		}

		static void outputProgramInfoLog(GLuint programHandle)
		{
			GLchar buffer[10000];
			GLsizei  length = 0;

			// Check info log for the errors
			glGetProgramInfoLog(programHandle, 10000, &length, buffer);

			// For now, just show the exception from the shader
			if (length > 0)
				simpleLogger::logColor(brogueConsoleColor::Red, buffer);
		}

		static void outputActiveBufferParameters()
		{
			GLint handle = getActiveVBO();
			GLint usage = 0;
			GLint size = 0;
			GLint mapOffset = 0;

			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_USAGE, &usage);
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
			glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_MAP_OFFSET, &mapOffset);

			simpleLogger::log("Active Buffer:  Id={} Usage={} Size={} MapOffset={}", handle, usage, size, mapOffset);
		}

		static void outputProgramParameters(GLuint programHandle)
		{
			GLchar name[256];
			GLenum properties[3]{ GL_NAME_LENGTH , GL_TYPE, GL_ARRAY_SIZE };
			GLint values[3];

			GLint numActiveAttribs = 0;
			GLint numActiveUniforms = 0;

			glGetProgramInterfaceiv(programHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);
			glGetProgramInterfaceiv(programHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

			for (int attrib = 0; attrib < numActiveAttribs; ++attrib)
			{
				glGetProgramResourceiv(programHandle, GL_PROGRAM_INPUT, attrib, 3, &properties[0], 3, NULL, &values[0]);
				glGetProgramResourceName(programHandle, GL_PROGRAM_INPUT, attrib, values[0], NULL, &name[0]);

				simpleLogger::log("Active Attribute:  Program={} Type={} Attribute={}", programHandle, values[1], simpleString(name).c_str());
			}

			for (int attrib = 0; attrib < numActiveUniforms; ++attrib)
			{
				glGetProgramResourceiv(programHandle, GL_UNIFORM, attrib, 3, &properties[0], 3, NULL, &values[0]);
				glGetProgramResourceName(programHandle, GL_UNIFORM, attrib, values[0], NULL, &name[0]);

				simpleLogger::log("Active Uniform:  Program={} Type={} Uniform={}", programHandle, values[1], simpleString(name).c_str());
			}
		}

		static void outputMaxTextureUnits()
		{
			GLint maxUnits = 0;

			glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);

			simpleLogger::log("OpenGL GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS (active textures):  {}", maxUnits);
		}

		// !!! SDL NEEDS TO BE WORKED IN !!!
		static void flipSurface(SDL_Surface* surface)
		{
			SDL_LockSurface(surface);

			int pitch = surface->pitch; // row size
			char* temp = new char[pitch]; // intermediate buffer
			char* pixels = (char*)surface->pixels;

			for (int i = 0; i < surface->h / 2; ++i) {
				// get pointers to the two rows to swap
				char* row1 = pixels + i * pitch;
				char* row2 = pixels + (surface->h - i - 1) * pitch;

				// swap rows
				memcpy(temp, row1, pitch);
				memcpy(row1, row2, pitch);
				memcpy(row2, temp, pitch);
			}

			delete[] temp;

			SDL_UnlockSurface(surface);
		}
	};
}
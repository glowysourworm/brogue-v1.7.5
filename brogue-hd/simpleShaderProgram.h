#pragma once

#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleGlObject.h"
#include "simpleShader.h"
#include "simpleString.h"
#include "simpleVertexArray.h"

#include "gl.h"
#include "openglHelper.h"
#include "simpleDataStream.h"
#include "simpleLogger.h"
#include "simpleUniform.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend::opengl
{
	class simpleShaderProgram : public simpleGlObject
	{
	public:

		simpleShaderProgram(const simpleShader& vertexShader, const simpleShader& fragmentShader, simpleVertexArray<float>* programVAO);
		~simpleShaderProgram();

		void compile();
		void draw();
		void bind() override;
		void teardown() override;

		bool isCreated() const override
		{
			return openglHelper::getProgramCreated(this->handle);
		}
		bool isBound() const override
		{
			return openglHelper::getActiveProgram() == this->handle;
		}
		bool hasErrors() const override
		{
			return openglHelper::getProgramError(this->handle);
		}
		void showErrors() const override
		{
			openglHelper::outputProgramInfoLog(this->handle);
		}

		/// <summary>
		/// Call to re-buffer the data glNamedBufferSubData. Will delete the old stream.
		/// </summary>
		/// <param name="bufferIndex">Index of VAO</param>
		void reBuffer(simpleDataStream* newBuffer);

		bool bindUniform1i(const simpleString& name, int uniformValue);
		bool bindUniform1(const simpleString& name, float uniformValue);
		bool bindUniform2(const simpleString& name, const vec2& uniformValue);
		bool bindUniform2i(const simpleString& name, const ivec2& uniformValue);
		bool bindUniform4(const simpleString& name, const vec4& uniformValue);

	private:

		void bindUniforms();

	private:

		simpleVertexArray<float>* _programVAO;

		simpleShader _vertexShader;
		simpleShader _fragmentShader;
	};

	simpleShaderProgram::simpleShaderProgram(const simpleShader& vertexShader, const simpleShader& fragmentShader, simpleVertexArray<float>* programVAO)
	{
		_programVAO = programVAO;
		_vertexShader = vertexShader;
		_fragmentShader = fragmentShader;
	}
	simpleShaderProgram::~simpleShaderProgram()
	{
		delete _programVAO;
	}

	void simpleShaderProgram::compile()
	{
		if (this->isCreated())
			throw simpleException("Already called simpleShaderProgram::compile");

		// Procedure
		//
		// 0) Declare the program 
		// 1) Create, compile, attach, and link the shaders
		// 2) Call the simplePrimitive Create() methods
		//

		// Declare the GL program
		this->handle = glCreateProgram();

		// Creates shader on the backend
		_vertexShader.glCreate(this->handle);
		_fragmentShader.glCreate(this->handle);

		// Attach shaders in the order they were declared
		glAttachShader(this->handle, _vertexShader.getHandle());
		glAttachShader(this->handle, _fragmentShader.getHandle());

		// Link the program
		glLinkProgram(this->handle);

		// Declare: VAO -> VBO
		//
		_programVAO->glCreate(this->handle);

		// "Installs the program object as part of the current rendering state"
		glUseProgram(this->handle);

		// Bind Uniforms (default values)
		bindUniforms();

		// Show output for this program:  errors, actives, etc...
		openglHelper::outputShaderInfoLog(_vertexShader.getHandle());
		openglHelper::outputShaderInfoLog(_fragmentShader.getHandle());
		openglHelper::outputProgramInfoLog(this->handle);
		openglHelper::outputProgramParameters(this->handle);
	}

	void simpleShaderProgram::draw()
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		if (!this->isBound())
			throw simpleException("Must first call bind to set the program active");

		_programVAO->bind();
		_programVAO->draw();
	}

	void simpleShaderProgram::reBuffer(simpleDataStream* newBuffer)
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		if (!this->isBound())
			throw simpleException("Must first call bind to set the program active");

		_programVAO->bind();
		_programVAO->reBuffer(this->handle, newBuffer);
	}

	void simpleShaderProgram::bind()
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		glUseProgram(this->handle);
	}

	void simpleShaderProgram::bindUniforms()
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		if (!this->isBound())
			throw simpleException("Must first call bind to set the program active");

		// Uniform data is stored in each shader
		//

		// Vertex Shaders: Uniform-1i
		for (int index = 0; index < _vertexShader.getUniform1iCount(); index++)
		{
			simpleUniform<int> uniform = _vertexShader.getUniform1i(index);
			bindUniform1i(uniform.name, uniform.value);
		}

		// Uniform-1 (1f)
		for (int index = 0; index < _vertexShader.getUniform1Count(); index++)
		{
			simpleUniform<float> uniform = _vertexShader.getUniform1(index);
			bindUniform1(uniform.name, uniform.value);
		}

		// Uniform-2 (2f)
		for (int index = 0; index < _vertexShader.getUniform2Count(); index++)
		{
			simpleUniform<vec2> uniform = _vertexShader.getUniform2(index);
			bindUniform2(uniform.name, uniform.value);
		}

		// Uniform-2i (2i)
		for (int index = 0; index < _vertexShader.getUniform2iCount(); index++)
		{
			simpleUniform<ivec2> uniform = _vertexShader.getUniform2i(index);
			bindUniform2i(uniform.name, uniform.value);
		}

		// Uniform-4 (4f)
		for (int index = 0; index < _vertexShader.getUniform4Count(); index++)
		{
			simpleUniform<vec4> uniform = _vertexShader.getUniform4(index);
			bindUniform4(uniform.name, uniform.value);
		}

		// Fragment Shaders:  Uniform 1i
		for (int index = 0; index < _fragmentShader.getUniform1iCount(); index++)
		{
			simpleUniform<int> uniform = _fragmentShader.getUniform1i(index);
			bindUniform1i(uniform.name, uniform.value);
		}

		// Uniform-1 (1f)
		for (int index = 0; index < _fragmentShader.getUniform1Count(); index++)
		{
			simpleUniform<float> uniform = _fragmentShader.getUniform1(index);
			bindUniform1(uniform.name, uniform.value);
		}

		// Uniform-2 (2f)
		for (int index = 0; index < _fragmentShader.getUniform2Count(); index++)
		{
			simpleUniform<vec2> uniform = _fragmentShader.getUniform2(index);
			bindUniform2(uniform.name, uniform.value);
		}

		// Uniform-2i (2i)
		for (int index = 0; index < _fragmentShader.getUniform2iCount(); index++)
		{
			simpleUniform<ivec2> uniform = _fragmentShader.getUniform2i(index);
			bindUniform2i(uniform.name, uniform.value);
		}

		// Uniform-4 (4f)
		for (int index = 0; index < _fragmentShader.getUniform4Count(); index++)
		{
			simpleUniform<vec4> uniform = _fragmentShader.getUniform4(index);
			bindUniform4(uniform.name, uniform.value);
		}
	}

	bool simpleShaderProgram::bindUniform1i(const simpleString& name, int uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name.c_str());

		if (location == -1)
			return false;

		glUniform1i(location, uniformValue);
	}
	bool simpleShaderProgram::bindUniform1(const simpleString& name, float uniformValue)
	{

		GLint location = glGetUniformLocation(this->handle, name.c_str());

		if (location == -1)
			return false;

		glUniform1f(location, uniformValue);
	}
	bool simpleShaderProgram::bindUniform2(const simpleString& name, const vec2& uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name.c_str());

		if (location == -1)
			return false;

		glUniform2f(location, uniformValue.x, uniformValue.y);
	}
	bool simpleShaderProgram::bindUniform2i(const simpleString& name, const ivec2& uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name.c_str());

		if (location == -1)
			return false;

		glUniform2i(location, uniformValue.x, uniformValue.y);
	}
	bool simpleShaderProgram::bindUniform4(const simpleString& name, const vec4& uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name.c_str());

		if (location == -1)
			return false;

		glUniform4f(location, uniformValue.x, uniformValue.y, uniformValue.z, uniformValue.w);
	}

	void simpleShaderProgram::teardown()
	{
		if (!this->isCreated())
		{
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleShaderProgram showing deleted by the backend - not the user code");
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleShaderProgram-> continuing on to try and delete its resources");
		}
			

		// Procedure
		//
		// 0) Detach the shaders
		// 1) Call Teardown on the IGLModel entities
		// 2) Deactivate the program by setting GL.UseProgram to null
		// 3) Reset handle and flag for this instance
		//

		// Detach the shaders
		glDetachShader(this->handle, _vertexShader.getHandle());
		glDetachShader(this->handle, _fragmentShader.getHandle());

		// Teardown all shader entities
		_vertexShader.teardown();
		_fragmentShader.teardown();

		// Teardown the VAO
		_programVAO->teardown();

		// Deactivate the program on the backend before deleting
		glUseProgram(NULL);

		// Delete this prgram from the backend
		glDeleteProgram(this->handle);
	}
}

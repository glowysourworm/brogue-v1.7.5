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

namespace brogueHd::frontend
{
	class simpleShaderProgram : public simpleGlObject
	{
	public:

		simpleShaderProgram(const simpleShader& vertexShader, const simpleShader& fragmentShader, simpleVertexArray<float>* programVAO);
		~simpleShaderProgram();

		void compile();
		void draw();
		void bind() override;
		void unBind() override;
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
		void showActives();

		/// <summary>
		/// Call to re-buffer the data glNamedBufferSubData. This will use the shared simpleDataStream* pointer. Using
		/// "forceNew" will delete the existing buffer on the GPU; and re-initialize the storage.
		/// </summary>
		void reBuffer(bool forceNew);
		
		/// <summary>
		/// Returns a pointer to the VAO's data stream (currently one-per-VAO, assigned to the VBO). Use this
		/// to rebuffer the stream; but must keep same data size and format!
		/// </summary>
		simpleDataStream* getStream() const;

		bool bindUniform1i(const char* name, int uniformValue);
		bool bindUniform1(const char* name, float uniformValue);
		bool bindUniform2(const char* name, const vec2& uniformValue);
		bool bindUniform2i(const char* name, const ivec2& uniformValue);
		bool bindUniform4(const char* name, const vec4& uniformValue);

		bool bindUniforms();
		bool hasUniform(const char* name);

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
		if (!bindUniforms())
			simpleLogger::logColor(brogueConsoleColor::Blue, "Problem binding uniforms:  Program={}", this->handle);

		// Show output for this program:  errors, actives, etc...
		openglHelper::outputShaderInfoLog(_vertexShader.getHandle());
		openglHelper::outputShaderInfoLog(_fragmentShader.getHandle());
		openglHelper::outputProgramInfoLog(this->handle);
		openglHelper::outputProgramParameters(this->handle);

		if (!this->isCreated())
			simpleLogger::logColor(brogueConsoleColor::Red, "Error creating simpleShaderProgram");
	}

	void simpleShaderProgram::showActives()
	{
		openglHelper::outputShaderInfoLog(_vertexShader.getHandle());
		openglHelper::outputShaderInfoLog(_fragmentShader.getHandle());
		openglHelper::outputProgramInfoLog(this->handle);
		openglHelper::outputProgramParameters(this->handle);
		
		_programVAO->showActives();
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

	void simpleShaderProgram::reBuffer(bool forceNew)
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		if (!this->isBound())
			throw simpleException("Must first call bind to set the program active");

		_programVAO->bind();
		_programVAO->reBuffer(this->handle, forceNew);
	}
	simpleDataStream* simpleShaderProgram::getStream() const
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		return _programVAO->getStream();
	}
	void simpleShaderProgram::bind()
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		glUseProgram(this->handle);
	}

	void simpleShaderProgram::unBind()
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		glUseProgram(NULL);
	}
	bool simpleShaderProgram::hasUniform(const char* name)
	{
		return glGetUniformLocation(this->handle, name) >= 0;
	}
	bool simpleShaderProgram::bindUniforms()
	{
		if (!this->isCreated())
			throw simpleException("Must first call compile() to run shader program:  simpleShaderProgram.h");

		if (!this->isBound())
			throw simpleException("Must first call bind to set the program active");

		// Uniform data is stored in each shader. Some of these data don't bind until
		// their backend is properly initialized.
		//
		bool result = true;

		// Vertex Shaders: Uniform-1i
		for (int index = 0; index < _vertexShader.getUniform1iCount(); index++)
		{
			simpleUniform<int> uniform = _vertexShader.getUniform1i(index);
			result &= bindUniform1i(uniform.name.c_str(), uniform.value);
		}

		// Uniform-1 (1f)
		for (int index = 0; index < _vertexShader.getUniform1Count(); index++)
		{
			simpleUniform<float> uniform = _vertexShader.getUniform1(index);
			result &= bindUniform1(uniform.name.c_str(), uniform.value);
		}

		// Uniform-2 (2f)
		for (int index = 0; index < _vertexShader.getUniform2Count(); index++)
		{
			simpleUniform<vec2> uniform = _vertexShader.getUniform2(index);
			result &= bindUniform2(uniform.name.c_str(), uniform.value);
		}

		// Uniform-2i (2i)
		for (int index = 0; index < _vertexShader.getUniform2iCount(); index++)
		{
			simpleUniform<ivec2> uniform = _vertexShader.getUniform2i(index);
			result &= bindUniform2i(uniform.name.c_str(), uniform.value);
		}

		// Uniform-4 (4f)
		for (int index = 0; index < _vertexShader.getUniform4Count(); index++)
		{
			simpleUniform<vec4> uniform = _vertexShader.getUniform4(index);
			result &= bindUniform4(uniform.name.c_str(), uniform.value);
		}

		// Fragment Shaders:  Uniform 1i
		for (int index = 0; index < _fragmentShader.getUniform1iCount(); index++)
		{
			simpleUniform<int> uniform = _fragmentShader.getUniform1i(index);
			result &= bindUniform1i(uniform.name.c_str(), uniform.value);
		}

		// Uniform-1 (1f)
		for (int index = 0; index < _fragmentShader.getUniform1Count(); index++)
		{
			simpleUniform<float> uniform = _fragmentShader.getUniform1(index);
			result &= bindUniform1(uniform.name.c_str(), uniform.value);
		}

		// Uniform-2 (2f)
		for (int index = 0; index < _fragmentShader.getUniform2Count(); index++)
		{
			simpleUniform<vec2> uniform = _fragmentShader.getUniform2(index);
			result &= bindUniform2(uniform.name.c_str(), uniform.value);
		}

		// Uniform-2i (2i)
		for (int index = 0; index < _fragmentShader.getUniform2iCount(); index++)
		{
			simpleUniform<ivec2> uniform = _fragmentShader.getUniform2i(index);
			result &= bindUniform2i(uniform.name.c_str(), uniform.value);
		}

		// Uniform-4 (4f)
		for (int index = 0; index < _fragmentShader.getUniform4Count(); index++)
		{
			simpleUniform<vec4> uniform = _fragmentShader.getUniform4(index);
			result &= bindUniform4(uniform.name.c_str(), uniform.value);
		}

		return result;
	}

	bool simpleShaderProgram::bindUniform1i(const char* name, int uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name);

		if (location < 0)
		{
			simpleLogger::logColor(brogueConsoleColor::Blue, "Uniform not found:  Program={} Uniform={}", this->handle, name);
			return false;
		}

		glUniform1i(location, uniformValue);

		return true;
	}
	bool simpleShaderProgram::bindUniform1(const char* name, float uniformValue)
	{

		GLint location = glGetUniformLocation(this->handle, name);

		if (location < 0)
		{
			simpleLogger::logColor(brogueConsoleColor::Blue, "Uniform not found:  Program={} Uniform={}", this->handle, name);
			return false;
		}

		glUniform1f(location, uniformValue);

		return true;
	}
	bool simpleShaderProgram::bindUniform2(const char* name, const vec2& uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name);

		if (location < 0)
		{
			simpleLogger::logColor(brogueConsoleColor::Blue, "Uniform not found:  Program={} Uniform={}", this->handle, name);
			return false;
		}

		glUniform2f(location, uniformValue.x, uniformValue.y);

		return true;
	}
	bool simpleShaderProgram::bindUniform2i(const char* name, const ivec2& uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name);

		if (location < 0)
		{
			simpleLogger::logColor(brogueConsoleColor::Blue, "Uniform not found:  Program={} Uniform={}", this->handle, name);
			return false;
		}

		glUniform2i(location, uniformValue.x, uniformValue.y);

		return true;
	}
	bool simpleShaderProgram::bindUniform4(const char* name, const vec4& uniformValue)
	{
		GLint location = glGetUniformLocation(this->handle, name);

		if (location < 0)
		{
			simpleLogger::logColor(brogueConsoleColor::Blue, "Uniform not found:  Program={} Uniform={}", this->handle, name);
			return false;
		}

		glUniform4f(location, uniformValue.x, uniformValue.y, uniformValue.z, uniformValue.w);

		return true;
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

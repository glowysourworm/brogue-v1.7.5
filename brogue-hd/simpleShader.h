#pragma once

#include "gl.h"
#include "openglHelper.h"
#include "shaderData.h"
#include "simpleArray.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleGlObject.h"
#include "simpleList.h"
#include "simpleLogger.h"
#include "simpleString.h"
#include "simpleUniform.h"
#include "simpleVertexAttribute.h"
#include <concepts>

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	/// <summary>
	/// Representation of a shader shared by the GPU. This contains the shader handle and data needed to
	/// compile the shader at runtime.
	/// </summary>
	struct simpleShader : public simpleGlObject
	{
	public:
		simpleShader();
		simpleShader(const simpleShader& copy);
		simpleShader(shaderData* data);
		~simpleShader();

		void operator=(const simpleShader& copy);

		void glCreate(GLuint programHandle) override;
		void teardown() override;

		bool isCreated() const override
		{
			return openglHelper::getShaderCreated(this->handle);
		}

		simpleString getSource() const
		{
			return _source;
		}
		GLenum getShaderType() const
		{
			return _shaderType;
		}

		size_t getHash() const override
		{
			return _source.getHash();
		}

		bool hasErrors() const override
		{
			return openglHelper::getShaderCompilerError(this->handle);
		}

		void showErrors() const override
		{
			openglHelper::outputShaderInfoLog(this->handle);
		}

		template<isOpenGlUniform T>
		bool hasUniform(const simpleString& name)
		{
			if (std::same_as<T, int>)
				return _uniforms1i->any([&name] (simpleUniform<int> item)
			{
				item.name == name;
			});

			else if (std::same_as<T, float>)
				return _uniforms1->any([&name] (simpleUniform<float> item)
			{
				item.name == name;
			});

			else if (std::same_as<T, vec2>)
				return _uniforms2->any([&name] (simpleUniform<vec2> item)
			{
				item.name == name;
			});

			else if (std::same_as<T, ivec2>)
				return _uniforms2i->any([&name] (simpleUniform<ivec2> item)
			{
				item.name == name;
			});

			else if (std::same_as<T, vec4>)
				return _uniforms4->any([&name] (simpleUniform<vec4> item)
			{
				item.name == name;
			});

			else
				throw simpleException("Unhandled uniform type simpleShader.h");

			return false;
		}

		int getUniform1iCount() const
		{
			return _uniforms1i->count();
		}
		int getUniform1Count() const
		{
			return _uniforms1->count();
		}
		int getUniform2Count() const
		{
			return _uniforms2->count();
		}
		int getUniform2iCount() const
		{
			return _uniforms2i->count();
		}
		int getUniform4Count() const
		{
			return _uniforms4->count();
		}

		simpleUniform<int> getUniform1i(int index) const
		{
			return _uniforms1i->get(index);
		}
		simpleUniform<float> getUniform1(int index) const
		{
			return _uniforms1->get(index);
		}
		simpleUniform<vec2> getUniform2(int index) const
		{
			return _uniforms2->get(index);
		}
		simpleUniform<ivec2> getUniform2i(int index) const
		{
			return _uniforms2i->get(index);
		}
		simpleUniform<vec4> getUniform4(int index) const
		{
			return _uniforms4->get(index);
		}

		template<isOpenGlUniform T>
		simpleUniform<T> getUniform(int index) const
		{
			if (std::same_as<T, int>)
				return getUniform1i(index);

			else if (std::same_as<T, float>)
				return getUniform1(index);

			else if (std::same_as<T, vec2>)
				return getUniform2(index);

			else if (std::same_as<T, vec2>)
				return getUniform2i(index);

			else if (std::same_as<T, vec4>)
				return getUniform4(index);

			else
				throw simpleException("Unhandled uniform type:  simpleShader.h");
		}

	public:

		simpleArray<simpleUniform<int>> getUniforms1i() const
		{
			return *_uniforms1i;
		}
		simpleArray<simpleUniform<float>> getUniforms1() const
		{
			return *_uniforms1;
		}
		simpleArray<simpleUniform<vec2>> getUniforms2() const
		{
			return *_uniforms2;
		}
		simpleArray<simpleUniform<ivec2>> getUniforms2i() const
		{
			return *_uniforms2i;
		}
		simpleArray<simpleUniform<vec4>> getUniforms4() const
		{
			return *_uniforms4;
		}
		simpleArray<simpleVertexAttribute> getVertexAttributes() const
		{
			return *_vertexAttributes;
		}

	private:

		void copyImpl(const simpleShader& copy);

	private:

		GLenum _shaderType;
		simpleString _source;

		simpleArray<simpleUniform<int>>* _uniforms1i;
		simpleArray<simpleUniform<float>>* _uniforms1;
		simpleArray<simpleUniform<vec2>>* _uniforms2;
		simpleArray<simpleUniform<ivec2>>* _uniforms2i;
		simpleArray<simpleUniform<vec4>>* _uniforms4;

		simpleArray<simpleVertexAttribute>* _vertexAttributes;
	};

	simpleShader::simpleShader()
	{
		_uniforms1i = new simpleArray<simpleUniform<int>>();
		_uniforms1 = new simpleArray<simpleUniform<float>>();
		_uniforms2 = new simpleArray<simpleUniform<vec2>>();
		_uniforms2i = new simpleArray<simpleUniform<ivec2>>();
		_uniforms4 = new simpleArray<simpleUniform<vec4>>();
		_vertexAttributes = new simpleArray<simpleVertexAttribute>();

		_shaderType = NULL;
	}
	simpleShader::~simpleShader()
	{
		delete _uniforms1i;
		delete _uniforms1;
		delete _uniforms2;
		delete _uniforms2i;
		delete _uniforms4;
		delete _vertexAttributes;
	}
	simpleShader::simpleShader(const simpleShader& copy)
	{
		copyImpl(copy);
	}
	simpleShader::simpleShader(shaderData* data)
	{
		this->handle = NULL;

		_uniforms1i = new simpleArray<simpleUniform<int>>(data->uniforms1i.toArray());
		_uniforms1 = new simpleArray<simpleUniform<float>>(data->uniforms1.toArray());
		_uniforms2 = new simpleArray<simpleUniform<vec2>>(data->uniforms2.toArray());
		_uniforms2i = new simpleArray<simpleUniform<ivec2>>(data->uniforms2i.toArray());
		_uniforms4 = new simpleArray<simpleUniform<vec4>>(data->uniforms4.toArray());

		_vertexAttributes = new simpleArray<simpleVertexAttribute>(data->attributes.count());

		for (int index = 0; index < data->attributes.count(); index++)
		{
			vertexAttributeData vaData = data->attributes.get(index);

			_vertexAttributes->set(index, simpleVertexAttribute(vaData.index, vaData.name, vaData.type));
		}

		_source = data->source;
		_shaderType = data->type;
	}

	void simpleShader::copyImpl(const simpleShader& copy)
	{
		this->handle = NULL;

		_shaderType = copy.getShaderType();
		_source = copy.getSource();

		_uniforms1i = new simpleArray<simpleUniform<int>>(copy.getUniforms1i());
		_uniforms1 = new simpleArray<simpleUniform<float>>(copy.getUniforms1());
		_uniforms2 = new simpleArray<simpleUniform<vec2>>(copy.getUniforms2());
		_uniforms2i = new simpleArray<simpleUniform<ivec2>>(copy.getUniforms2i());
		_uniforms4 = new simpleArray<simpleUniform<vec4>>(copy.getUniforms4());
		_vertexAttributes = new simpleArray<simpleVertexAttribute>(copy.getVertexAttributes());
	}

	void simpleShader::operator=(const simpleShader& copy)
	{
		copyImpl(copy);
	}

	void simpleShader::teardown()
	{
		if (!this->isCreated())
		{
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleShader already deleted from the backend");
		}

		// Deletes the shader object from the GL backend
		glDeleteShader(this->handle);
	}

	void simpleShader::glCreate(GLuint programHandle)
	{
		if (this->isCreated())
			throw simpleException("simpleShader already created on the backend");

		// Declare the shader on the GL backend
		this->handle = glCreateShader(_shaderType);

		// (some funny casting)
		const GLchar* source = _source.c_str();
		const GLint size = _source.count();

		// Set the shader source on the GL backend
		glShaderSource(this->handle, 1, &source, &size);

		// Compile the shader and check for errors
		glCompileShader(this->handle);

		if (!this->isCreated())
			throw simpleException("simpleShader not created properly on the backend");
	}
}
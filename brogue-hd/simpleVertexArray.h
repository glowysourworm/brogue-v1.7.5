#pragma once

#include "gl.h"
#include "openglHelper.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleGlObject.h"
#include "simpleLogger.h"
#include "simpleVertexBuffer.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	/// <summary>
	/// Corresponds to the VAO objects in OpenGL. These are used to manage the VBO Vertex Buffer objects
	/// that are stored on the GL backend
	/// </summary>
	template<typename T>
	class simpleVertexArray : public simpleGlObject
	{
	public:
		simpleVertexArray();
		simpleVertexArray(const GLenum& primitiveType, simpleVertexBuffer<T>* vertexBuffer);
		~simpleVertexArray();

		void glCreate(GLuint programHandle) override;
		void bind() override;
		void teardown() override;

		bool isCreated() const override
		{
			return this->handle != simpleGlObject::HandleNull && openglHelper::getVAOCreated(this->handle);
		}
		bool isBound() const override
		{
			return openglHelper::getVAOBinding(this->handle);
		}

		/// <summary>
		/// Shows active parameters of the VAO
		/// </summary>
		void showActives() const;

		/// <summary>
		/// Draws the contents of the VAO's vertex buffer (currently only a single VBO per VAO)
		/// </summary>
		void draw();

		/// <summary>
		/// Rebuffers data on the specified VBO (see simpleShaderProgram). Force new will delete the
		/// existing buffer on the GPU and re-initialize the storage.
		/// </summary>
		void reBuffer(GLuint programHandle, bool forceNew);

		/// <summary>
		/// Returns the simpleDataStream* to the VAO's single VBO. Use this to rebuffer the stream - but
		/// keeping same size and format!
		/// </summary>
		simpleDataStream* getStream() const;

		size_t getHash() const override
		{
			return _vertexBuffer->getHash();
		}

	protected:

		/// <summary>
		/// Have to handle lazy initialization of the VBO for zero-data cases during compilation.
		/// </summary>
		void initializeVertexBuffer();

	private:

		// Primary VBO vertex buffer objects indexed by the OpenGL BufferIndex
		GLenum _primitiveType;
		simpleVertexBuffer<T>* _vertexBuffer;
		GLuint _programHandle;
	};

	template<typename T>
	simpleVertexArray<T>::simpleVertexArray()
	{
		_vertexBuffer = new simpleVertexBuffer<T>();
		_programHandle = simpleGlObject::HandleNull;
	}

	template<typename T>
	simpleVertexArray<T>::simpleVertexArray(const GLenum& primitiveType, simpleVertexBuffer<T>* vertexBuffer)
	{
		_primitiveType = primitiveType;
		_vertexBuffer = vertexBuffer;
		_programHandle = simpleGlObject::HandleNull;
	}

	template<typename T>
	simpleVertexArray<T>::~simpleVertexArray()
	{
		// THESE ARE (MAY BE) CREATED BY THE SCENE BUILDER!
		delete _vertexBuffer;
	}

	template<typename T>
	void simpleVertexArray<T>::glCreate(GLuint programHandle)
	{
		if (this->isCreated())
			throw simpleException("simpleVertexArray already created in the backend");

		// Procedure:  Create / Draw / Teardown several vertex buffers
		//
		// 0) Create the vertex array
		// 1) Bind the vertex array handle (starts editing)
		// 2) Calculate the total buffer draw length and store

		// Generate the vertex array for THIS instance
		glGenVertexArrays(1, &this->handle);

		// BIND THIS INSTANCE TO BE THE CURRENT VERTEX ARRAY
		glBindVertexArray(this->handle);

		_programHandle = programHandle;

		// NOTE: The buffer indices are assigned prior to passing into this instance. Order
		//       has already been indicated by the calling this constructor.
		//       
		if (_vertexBuffer->getBufferLength() > 0)
			initializeVertexBuffer();

		if (!this->isCreated())
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleVertexArray error creating the VAO");
	}

	template<typename T>
	void simpleVertexArray<T>::initializeVertexBuffer()
	{
		if (!this->isCreated())
			throw simpleException("Trying to create VBO before calling glCreate for the VAO:  simpleVertexArray.h");

		if (_vertexBuffer->getBufferLength() == 0)
			throw simpleException("Trying to create VBO with zero size:  simpleVertexArray.h");

		if (_programHandle == simpleGlObject::HandleNull)
			throw simpleException("Trying to create VBO before calling glCreate for the VAO:  simpleVertexArray.h");

		_vertexBuffer->glCreate(_programHandle);
	}

	template<typename T>
	void simpleVertexArray<T>::showActives() const
	{
		if (!_vertexBuffer->isCreated())
			simpleLogger::logColor(brogueConsoleColor::Blue, "Vertex Buffer Not Yet Created:  VAO={}", this->handle);

		_vertexBuffer->showActives();
	}

	template<typename T>
	void simpleVertexArray<T>::reBuffer(GLuint programHandle, bool forceNew)
	{
		if (!this->isCreated())
			throw simpleException("simpleVertexArray already deleted from the backend");

		if (!this->isBound())
			throw simpleException("simpleVertexArray must be bound before calling rebuffer()");

		if (_vertexBuffer->getBufferLength() == 0)
			throw simpleException("Trying to create VBO with zero size:  simpleVertexArray.h");

		if (!_vertexBuffer->isCreated())
			initializeVertexBuffer();
		else
			_vertexBuffer->reBuffer(programHandle, forceNew);
	}

	template<typename T>
	simpleDataStream* simpleVertexArray<T>::getStream() const
	{
		if (!this->isCreated())
			throw simpleException("simpleVertexArray already deleted from the backend");

		return _vertexBuffer->getStream();
	}

	template<typename T>
	void simpleVertexArray<T>::teardown()
	{
		if (!this->isCreated())
		{
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleVertexArray already deleted from the backend");
			simpleLogger::logColor(brogueConsoleColor::Yellow, "simpleVertexArray-> Continuing on to try and delete other GPU resources");
		}
			

		// Teardown vertex buffers
		if (_vertexBuffer->isCreated())
			_vertexBuffer->teardown();

		// Delete this vertex array
		glDeleteVertexArrays(1, &this->handle);

		this->handle = simpleGlObject::HandleNull;
	}

	template<typename T>
	void simpleVertexArray<T>::draw()
	{
		if (!this->isCreated())
			throw simpleException("simpleVertexArray already deleted from the backend");

		if (!this->isBound())
			throw simpleException("simpleVertexArray must be bound before calling draw()");

		// Lazy initialization of the VBO
		if (!_vertexBuffer->isCreated())
		{
			if (_vertexBuffer->getBufferLength() > 0)
				initializeVertexBuffer();
		}

		// Draw Buffer
		glDrawArrays(_primitiveType, 0, _vertexBuffer->getBufferLength());
	}

	template<typename T>
	void simpleVertexArray<T>::bind()
	{
		if (!this->isCreated())
			throw simpleException("simpleVertexArray already deleted from the backend");

		// Bind VAO before using
		glBindVertexArray(this->handle);
	}
}
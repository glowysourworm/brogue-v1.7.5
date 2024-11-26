#pragma once

#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueImageQuad.h"
#include "brogueLine.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartConfiguration.h"
#include "brogueUIProgramPartId.h"
#include "gl.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simpleArray.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleOpenGL.h"
#include "simpleShader.h"
#include "simpleShaderProgram.h"
#include "simpleVertexArray.h"
#include "simpleVertexBuffer.h"

namespace brogueHd::frontend
{
	template<isGLStream TStream>
	class brogueViewCore
	{
	public:

		brogueViewCore(resourceController* resourceController,
					   const brogueUIProgramPartId& partId,
					   const gridRect& sceneBoundary,
					   const gridRect& viewBoundary);
		~brogueViewCore();

		brogueUIProgram getProgram() const;
		brogueUIProgramPart getProgramPart() const;
		brogueUIProgramPartId getId() const;

		gridRect getSceneBoundary() const;
		gridRect getBoundary() const;

	protected:

		/// <summary>
		/// Initializes the output stream and sets its (STATIC!) size. Must subsequently call get / set methods
		/// to work with the actual data; and then call "createStream" to finish buffering on the software side.
		/// </summary>
		/// <param name="elementSize">Size of stream in terms of elements (# of shader passes)</param>
		void initializeStream(int elementSize);

		/// <summary>
		/// Completes the buffer on the software side for the output data. This stream
		/// may be modified at run time by calling "set", and subsequently, "reStream".
		/// </summary>
		void createStream();

		/// <summary>
		/// Re-creates the stream and forces GL backend to rebuffer using the simpleShaderProgram API.
		/// </summary>
		void reStream();

	protected:	// UI <---> GL Data Functions

		TStream getElement(int elementIndex);
		void setElement(const TStream& element, int elementIndex);

	protected:	// GL Functions

		// This draw type is specific to the data streams supported by isGLStream (see simpleOpenGL.h)
		static constexpr GLenum PrimitiveType = GL_TRIANGLES;
		static constexpr GLenum PolygonPrimitiveType = GL_LINES;

		bool glHasUniform(const char* name) const;
		bool glHasErrors() const;
		void glShowErrors() const;
		void glShowActives() const;

		void glInitialize() const;
		void glActivate() const;
		void glDeactivate() const;
		void glDraw() const;

		template<isGLUniform TUniform>
		void setUniform(const char* name, const TUniform& value);

		void setUniform(const char* name, const float& value);
		void setUniform(const char* name, const int& value);
		void setUniform(const char* name, const vec2& value);
		void setUniform(const char* name, const ivec2& value);
		void setUniform(const char* name, const vec4& value);

	private:

		simpleDataStream* createDataStream(int elementCount);

	private:

		// Shared pointer with the simpleShaderProgram*. This memory doesn't get utilized
		// until there is an update to the GPU memory from the software side. This happens
		// when the stream is invalidated.
		simpleDataStream* _dataStream;
		simpleShaderProgram* _program;

		// Produced and cached by the stream building process
		simpleArray<TStream>* _elements;

		brogueUIProgramPartConfiguration* _configuration;
		brogueUIProgramPartId* _partId;
		gridRect* _sceneBoundary;
		gridRect* _viewBoundary;
	};

	template<isGLStream TStream>
	brogueViewCore<TStream>::brogueViewCore(resourceController* resourceController,
											const brogueUIProgramPartId& partId,
											const gridRect& sceneBoundary,
											const gridRect& viewBoundary)
	{
		// Procedure
		//
		// 1) Build and size the element list for this stream type
		// 2) Input elements as they are sent here from view update() functions
		// 3) Invalidate the stream
		// 4) Restream from calls to reBuffer (TODO: Invalidate / re-stream portions of the data stream)
		//

		_configuration = new brogueUIProgramPartConfiguration(*resourceController->getUIPartConfig(partId.getPartName()));

		simpleShader vertexShader(resourceController->getShader(_configuration->vertexShaderName));
		simpleShader fragmentShader(resourceController->getShader(_configuration->fragmentShaderName));

		// Define the stream size (in elements)
		_elements = new simpleArray<TStream>(0);

		// Create a placeholder stream. This will be the shared pointer with the simpleShaderProgram.
		_dataStream = createDataStream(0);

		// These could eventually go inside the simpleShaderProgram; but there's still more to see about what / how to utilize
		// the GL backend (for our purposes...) (one use would be simple multiple vertex buffers, perhaps w/ multiple stream types...)
		int vertexBufferIndex = 0;	// GL VBO index is STATIC!

		// (MEMORY!) These are managed by the simple shader program
		simpleVertexBuffer<float>* programVBO = new simpleVertexBuffer<float>(vertexBufferIndex++, _dataStream, vertexShader.getVertexAttributes());
		simpleVertexArray<float>* programVAO = new simpleVertexArray<float>(this->PrimitiveType, programVBO);

		_program = new simpleShaderProgram(vertexShader, fragmentShader, programVAO);

		// Rest of our data
		_partId = new brogueUIProgramPartId(partId);
		_sceneBoundary = new gridRect(sceneBoundary);
		_viewBoundary = new gridRect(viewBoundary);
	}

	template<isGLStream TStream>
	brogueViewCore<TStream>::~brogueViewCore()
	{
		delete _elements;
		delete _partId;
		delete _configuration;
	}

	template<isGLStream TStream>
	TStream brogueViewCore<TStream>::getElement(int elementIndex)
	{
		return _elements->get(elementIndex);
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::setElement(const TStream& element, int elementIndex)
	{
		_elements->set(elementIndex, element);
	}

	template<isGLStream TStream>
	brogueUIProgram brogueViewCore<TStream>::getProgram() const
	{
		return _partId->getName();
	}

	template<isGLStream TStream>
	brogueUIProgramPart brogueViewCore<TStream>::getProgramPart() const
	{
		return _partId->getPartName();
	}

	template<isGLStream TStream>
	brogueUIProgramPartId brogueViewCore<TStream>::getId() const
	{
		return *_partId;
	}

	template<isGLStream TStream>
	gridRect brogueViewCore<TStream>::getSceneBoundary() const
	{
		return *_sceneBoundary;
	}

	template<isGLStream TStream>
	gridRect brogueViewCore<TStream>::getBoundary() const
	{
		return *_viewBoundary;
	}

	template<isGLStream TStream>
	bool brogueViewCore<TStream>::glHasUniform(const char* name) const
	{
		return _program->hasUniform(name);
	}

	template<isGLStream TStream>
	bool brogueViewCore<TStream>::glHasErrors() const
	{
		return _program->hasErrors();
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::glShowErrors() const
	{
		_program->showErrors();
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::glShowActives() const
	{
		_program->showActives();
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::glInitialize() const
	{
		_program->compile();
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::glActivate() const
	{
		_program->bind();
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::glDeactivate() const
	{
		_program->unBind();
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::glDraw() const
	{
		// Check period counter
		//if (!_programCounters->get(partId)->update(millisecondsElapsed))
		//	continue;

		if (_configuration->useAlphaBlending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		_program->bind();
		_program->draw();

		//if (program->hasUniform("fadePeriodTime") && partId.getName() == brogueUIProgram::FlameMenuProgram)
		//	program->bindUniform1("fadePeriodTime", ((brogueFlameMenuHeatView*)view)->currentFadePeriod());

		//if (program->hasUniform("fadePeriodRandom1") && partId.getName() == brogueUIProgram::FlameMenuProgram)
		//	program->bindUniform1i("fadePeriodRandom1", ((brogueFlameMenuHeatView*)view)->currentFadePeriodRandom1());

		//if (program->hasUniform("fadePeriodRandom2") && partId.getName() == brogueUIProgram::FlameMenuProgram)
		//	program->bindUniform1i("fadePeriodRandom2", ((brogueFlameMenuHeatView*)view)->currentFadePeriodRandom2());

		//if (program->hasUniform("nextColorNumber") && partId.getName() == brogueUIProgram::FlameMenuProgram)
		//	program->bindUniform1i("nextColorNumber", ((brogueFlameMenuHeatView*)view)->fadePeriodCount() % 3);

		if (_configuration->useAlphaBlending)
			glDisable(GL_BLEND);

		glFlush();
		glFinish();

		// Lookup NVIDIA bug:  glNamedCopyBufferSubData
		//
		//if (partId.getPartName() == brogueUIProgramPart::FlameDisplay)
		//{
			//// Buffer copy the elements back into the heat source buffer; and let the next draw pass
			//// render them
			////
			//brogueUIProgramPartId diffuseId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameMenuProgram_HeatDiffuseProgram, 0);
			//brogueViewBase* diffuseView = _viewContainer->getView(diffuseId);
			//simpleShaderProgram* diffuseProgram = _programs->get(diffuseId);
			//gridRect boundary = view->getBoundary();

			//gridLocator fromStart(0, boundary.bottom());
			//gridLocator fromEnd(boundary.right(), boundary.bottom());
			//gridLocator toStart(0, boundary.bottom() - 1);
			//gridLocator toEnd(boundary.right(), boundary.bottom() - 1);

			//int fromStreamStart = 0;
			//int fromStreamEnd = 0;
			//int toStreamStart = 0;
			//int toStreamEnd = 0;

			//program->showActives();

			//// These must match (anyway). This is a POC for the stream copy (on the GPU)
			//_programBuilder->calculateStreamRange(view, view, *configuration, fromStart, fromEnd, false, fromStreamStart, fromStreamEnd);
			//_programBuilder->calculateStreamRange(view, view, *configuration, toStart, toEnd, false, toStreamStart, toStreamEnd);
			//
			//glNamedCopyBufferSubDataEXT(program->getHandle(), program->getHandle(), fromStreamStart, toStreamStart, toStreamEnd - toStreamStart);
		//}
	}

	template<isGLStream TStream>
	template<isGLUniform TUniform>
	void brogueViewCore<TStream>::setUniform(const char* name, const TUniform& value)
	{
		setUniform(name, value);
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::setUniform(const char* name, const float& value)
	{
		_program->bindUniform1(name, value);
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::setUniform(const char* name, const int& value)
	{
		_program->bindUniform1i(name, value);
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::setUniform(const char* name, const vec2& value)
	{
		_program->bindUniform2(name, value);
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::setUniform(const char* name, const ivec2& value)
	{
		_program->bindUniform2i(name, value);
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::setUniform(const char* name, const vec4& value)
	{
		_program->bindUniform4(name, value);
	}

	template<isGLStream TStream>
	simpleDataStream* brogueViewCore<TStream>::createDataStream(int elementCount)
	{
		switch (_configuration->dataStreamType)
		{
			case openglDataStreamType::brogueCellQuad:
			{
				brogueCellQuad quad;
				return new simpleDataStream(elementCount, quad.getElementVertexSize(this->PrimitiveType), quad.getStreamSize(this->PrimitiveType));
			}
			case openglDataStreamType::brogueColorQuad:
			{
				brogueColorQuad quad;
				return new simpleDataStream(elementCount, quad.getElementVertexSize(this->PrimitiveType), quad.getStreamSize(this->PrimitiveType));
			}
			case openglDataStreamType::brogueImageQuad:
			{
				brogueImageQuad quad;
				return new simpleDataStream(elementCount, quad.getElementVertexSize(this->PrimitiveType), quad.getStreamSize(this->PrimitiveType));
			}
			case openglDataStreamType::broguePolygon:
			{
				brogueLine element;
				return new simpleDataStream(elementCount, element.getElementVertexSize(this->PolygonPrimitiveType), element.getStreamSize(this->PolygonPrimitiveType));
			}
			default:
				throw simpleException("Unhandled openglDataStreamType:  brogueViewCore::createDataStream");
		}
	}

	// Brogue Coordinate Converter Frame-Type Code (Graveyard)
	// 
	//brogueImageQuad brogueCoordinateConverter::createBrogueImageQuadFrame()
	//{
	//	simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
	//	simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

	//	return brogueImageQuad(gridLocator(0, 0), quadXY, quadUV);
	//}

	//brogueColorQuad brogueCoordinateConverter::createBrogueColorQuadFrame(const color& theColor)
	//{
	//	simpleQuad quadXY = _viewConverter.createQuadNormalizedXY(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());
	//	simpleQuad quadUV = _viewConverter.createQuadNormalizedUV(0, 0, _viewConverter.getViewWidth(), _viewConverter.getViewHeight());

	//	return brogueColorQuad(gridLocator(0, 0), theColor, quadXY);
	//}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::initializeStream(int elementSize)
	{
		// Re-allocate the software-side buffer
		delete _elements;

		_elements = new simpleArray<TStream>(elementSize);

		// Create the pre-allocated data stream
		switch (_configuration->dataStreamType)
		{
			case openglDataStreamType::brogueImageQuad:
			{
				brogueImageQuad quad;
				_dataStream->reCreate(elementSize, quad.getElementVertexSize(this->PrimitiveType), quad.getStreamSize(this->PrimitiveType));
			}
			break;
			case openglDataStreamType::brogueCellQuad:
			{
				brogueCellQuad quad;
				_dataStream->reCreate(elementSize, quad.getElementVertexSize(this->PrimitiveType), quad.getStreamSize(this->PrimitiveType));
			}
			break;
			case openglDataStreamType::brogueColorQuad:
			{
				brogueColorQuad quad;
				_dataStream->reCreate(elementSize, quad.getElementVertexSize(this->PrimitiveType), quad.getStreamSize(this->PrimitiveType));
			}
			break;
			case openglDataStreamType::broguePolygon:
			{
				brogueLine element;
				_dataStream->reCreate(elementSize, element.getElementVertexSize(this->PolygonPrimitiveType), element.getStreamSize(this->PolygonPrimitiveType));
			}
			break;
			default:
				throw simpleException("Unknown openglDataStreamType:  brogueViewCore::initializeStream");
		}
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::createStream()
	{
		if (_dataStream->getStreamNumberElements() != _elements->count())
			throw simpleException("Invalid data stream size (brogueViewCore.h):  # of elements should match what was set up on the GL backend");

		// Create Scene Data Streams

		// Element Size:   Total number of primitives (floats) to commit to the stream
		// Element Length: Total number of elements as seen by OpenGL - depends on the drawing type
		//

		// RESET CURSOR:  This sends the data stream cursor back to zero; and allows overwriting the existing data.
		_dataStream->resetCursor();

		for (int index = 0; index < _elements->count(); index++)
		{
			_elements->get(index).streamBuffer(this->PrimitiveType, _dataStream);
		}
	}

	template<isGLStream TStream>
	void brogueViewCore<TStream>::reStream()
	{
		_program->bind();
		_program->reBuffer();			// Copies the contents of our shared data stream pointer to the GPU
	}
}
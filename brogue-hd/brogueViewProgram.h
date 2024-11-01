#pragma once

#include "brogueProgram.h"
#include "brogueView.h"
#include "brogueProgramBuilder.h"
#include "brogueDataStream.h"

namespace brogueHd::frontend::opengl
{
	template<isBrogueView T>
	class brogueViewProgram : public brogueProgram
	{
	public:

		/// <summary>
		/// Constructor for a brogue view program
		/// </summary>
		/// <param name="resourceController">Resource controller</param>
		/// <param name="vertexShader">Resource identifier for the vertex shader</param>
		/// <param name="fragmentShader">Resource identifier for the fragment shader</param>
		/// <param name="glyphMap">Glyph map reference</param>
		/// <param name="view">Reference to the view to be rendered</param>
		/// <param name="dataStream">(MEMORY!) Data stream:  This produces a specific data structure to render to the VBO</param>
		brogueViewProgram(T* view, 
					      resourceController* resourceController, 
						  brogueGlyphMap* glyphMap,
						  shaderResource vertexShader,
						  shaderResource fragmentShader,
						  brogueDataStream<T>* dataStream)
		{
			_view = view;
			_vertexShader = resourceController->getShader(vertexShader);
			_fragmentShader = resourceController->getShader(fragmentShader);
			_programBuilder = new brogueProgramBuilder(resourceController, glyphMap);
			_program = nullptr;
			_dataStream = dataStream;
		}
		~brogueViewProgram()
		{
			delete _programBuilder;
			delete _program;
			delete _dataStream;
		}

		virtual void initialize() override;
		virtual bool shouldUpdate(const simpleMouseState& mouseState, int millisecondsLapsed) override;
		virtual void update(const simpleMouseState& mouseState, int millisecondsLapsed) override;
		virtual void run(int millisecondsElapsed) override;
		virtual void outputStatus() const override;
		virtual bool isCompiled() const override;
		virtual bool hasErrors() const override;


		virtual gridRect getSceneBoundaryUI() const override
		{
			return _view->calculateSceneBoundaryUI();
		}

		simpleQuad getCellSizeUV() const
		{
			gridRect sceneBoundaryUI = _view->calculateSceneBoundaryUI();

			brogueCoordinateConverter converter =  _programBuilder->createCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height);

			return converter.getViewConverter()
							.createQuadNormalizedUV(0, 0, brogueCellDisplay::CellWidth, brogueCellDisplay::CellHeight);
		}

		simpleShaderProgram* getProgram() const
		{
			return _program;
		}

	private:

		T* _view;
		brogueProgramBuilder* _programBuilder;
		simpleShaderProgram* _program;
		shaderData* _vertexShader;
		shaderData* _fragmentShader;
		brogueDataStream<T>* _dataStream;
	};

	template<isBrogueView T>
	void brogueViewProgram<T>::initialize()
	{
		simpleDataStream* stream = _dataStream->createStream(_view);
		
		_program = _programBuilder->createShaderProgram(stream, _vertexShader, _fragmentShader);

		_program->compile();
		_program->bind();
	}

	template<isBrogueView T>
	bool brogueViewProgram<T>::shouldUpdate(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		gridRect sceneBoundary = this->getSceneBoundaryUI();

		brogueMouseState mouseStateUI((mouseState.getX() / sceneBoundary.width) * _view->getSceneBoundary().width,
									  (mouseState.getY() / sceneBoundary.height) * _view->getSceneBoundary().height);

		return _view->shouldUpdate(mouseStateUI, millisecondsLapsed);
	}

	template<isBrogueView T>
	void brogueViewProgram<T>::update(const simpleMouseState& mouseState, int millisecondsLapsed)
	{
		gridRect sceneBoundary = this->getSceneBoundaryUI();

		brogueMouseState mouseStateUI((mouseState.getX() / sceneBoundary.width) * _view->getSceneBoundary().width,
									  (mouseState.getY() / sceneBoundary.height) * _view->getSceneBoundary().height);

		// View will present new data
		_view->update(mouseStateUI, millisecondsLapsed);

		// Must update the data stream
		simpleDataStream* stream = _dataStream->createStream(_view);

		// Put the new stream online (deletes the old stream)
		_program->bind();
		_program->reBuffer(stream);
	}

	template<isBrogueView T>
	void brogueViewProgram<T>::run(int millisecondsElapsed)
	{
		_program->bind();
		_program->draw();
	}

	template<isBrogueView T>
	void brogueViewProgram<T>::outputStatus() const
	{
		_program->showErrors();
	}

	template<isBrogueView T>
	bool brogueViewProgram<T>::isCompiled() const
	{
		return openglHelper::getProgramCreated(_program->getHandle());
	}

	template<isBrogueView T>
	bool brogueViewProgram<T>::hasErrors() const
	{
		return _program->hasErrors();
	}
}
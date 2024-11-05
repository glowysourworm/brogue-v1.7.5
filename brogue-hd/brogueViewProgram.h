#pragma once

#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueDataStream.h"
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueProgram.h"
#include "brogueProgramBuilder.h"
#include "brogueUIConstants.h"
#include "brogueUIResponseData.h"
#include "brogueView.h"
#include "brogueViewBase.h"
#include "gl.h"
#include "gridRect.h"
#include "openglHelper.h"
#include "resourceController.h"
#include "shaderData.h"
#include "simpleDataStream.h"
#include "simpleGlData.h"
#include "simpleKeyboardState.h"
#include "simpleMouseState.h"
#include "simpleShaderProgram.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;
using namespace brogueHd::frontend::ui;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::opengl
{
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
		brogueViewProgram(brogueUIProgram programName,
						  brogueViewBase* view,
						  resourceController* resourceController,
						  brogueGlyphMap* glyphMap,
						  shaderResource vertexShader,
						  shaderResource fragmentShader,
						  brogueDataStream* dataStream,
						  bool useAlphaBlending)
			: brogueProgram(programName, false)
		{
			_view = view;
			_vertexShader = resourceController->getShader(vertexShader);
			_fragmentShader = resourceController->getShader(fragmentShader);
			_programBuilder = new brogueProgramBuilder(resourceController, glyphMap);
			_program = nullptr;
			_dataStream = dataStream;
			_useAlphaBlending = useAlphaBlending;
		}
		~brogueViewProgram()
		{
			delete _programBuilder;
			delete _program;
			delete _dataStream;
		}

		virtual void initialize() override;
		virtual void checkUpdate(brogueUIResponseData& response,
								 const simpleKeyboardState& keyboardState,
								 const simpleMouseState& mouseState,
								 int millisecondsLapsed) override;

		virtual void update(const simpleKeyboardState& keyboardState,
							const simpleMouseState& mouseState,
							int millisecondsLapsed) override;

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

			brogueCoordinateConverter converter = _programBuilder->createCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height, _view->getZoomLevel());

			return converter.getViewConverter()
				.createQuadNormalizedUV(0, 0, brogueCellDisplay::CellWidth(_view->getZoomLevel()), brogueCellDisplay::CellHeight(_view->getZoomLevel()));
		}

		simpleShaderProgram* getProgram() const
		{
			return _program;
		}

	private:

		brogueViewBase* _view;
		brogueProgramBuilder* _programBuilder;
		simpleShaderProgram* _program;
		shaderData* _vertexShader;
		shaderData* _fragmentShader;
		brogueDataStream* _dataStream;
		bool _useAlphaBlending;
	};

	void brogueViewProgram::initialize()
	{
		simpleDataStream* stream = _dataStream->createStream(_view);

		_program = _programBuilder->createShaderProgram(stream, _vertexShader, _fragmentShader);

		_program->compile();
		_program->bind();
	}

	void brogueViewProgram::checkUpdate(brogueUIResponseData& response,
										const simpleKeyboardState& keyboardState,
										const simpleMouseState& mouseState,
										int millisecondsLapsed)
	{
		// CRITICAL!  Don't forget to mark the program name! Otherwise, the main controller won't know 
		//			  how to process the message
		//
		//			  Mark the reponse. (see views to make sure the response is being marked properly)
		//
		response.signature.program = this->getProgramName();

		// Translate the UI space -> the brogue / program space and pass down the pipeline
		//
		gridRect sceneBoundary = this->getSceneBoundaryUI();

		brogueMouseState mouseStateUI((mouseState.getX() / sceneBoundary.width) * _view->getSceneBoundary().width,
									  (mouseState.getY() / sceneBoundary.height) * _view->getSceneBoundary().height,
									  mouseState.getScrolldYPending() != 0 || mouseState.getScrolldXPending() != 0,
									  mouseState.getScrolldYPending() > 0, mouseState.getLeftButton() > 0);

		char hotkey = _view->getUIData()->getHotkeyChar();

		// TODO: Get translator for the key system; and implement hotkeys
		brogueKeyboardState keyboardStateUI(hotkey, hotkey);

		// Pass the parameters in to check the program's view (tree)
		_view->checkUpdate(response, keyboardStateUI, mouseStateUI, millisecondsLapsed);
	}

	void brogueViewProgram::update(const simpleKeyboardState& keyboardState,
								   const simpleMouseState& mouseState,
								   int millisecondsLapsed)
	{
		gridRect sceneBoundary = this->getSceneBoundaryUI();

		brogueMouseState mouseStateUI((mouseState.getX() / sceneBoundary.width) * _view->getSceneBoundary().width,
									  (mouseState.getY() / sceneBoundary.height) * _view->getSceneBoundary().height,
									  mouseState.getScrolldYPending() != 0 || mouseState.getScrolldXPending() != 0,
									  mouseState.getScrolldYPending() > 0, mouseState.getLeftButton() > 0);

		char hotkey = _view->getUIData()->getHotkeyChar();

		// TODO: Get translator for the key system; and implement hotkeys
		brogueKeyboardState keyboardStateUI(hotkey, hotkey);

		// View will present new data
		_view->update(keyboardStateUI, mouseStateUI, millisecondsLapsed);

		// Must update the data stream
		simpleDataStream* stream = _dataStream->createStream(_view);

		// Put the new stream online (deletes the old stream)
		_program->bind();
		_program->reBuffer(stream);
	}

	void brogueViewProgram::run(int millisecondsElapsed)
	{
		if (_useAlphaBlending)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}

		_program->bind();
		_program->draw();

		if (_useAlphaBlending)
			glDisable(GL_BLEND);
	}

	void brogueViewProgram::outputStatus() const
	{
		_program->showErrors();
	}

	bool brogueViewProgram::isCompiled() const
	{
		return openglHelper::getProgramCreated(_program->getHandle());
	}

	bool brogueViewProgram::hasErrors() const
	{
		return _program->hasErrors();
	}
}
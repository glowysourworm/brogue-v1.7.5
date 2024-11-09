#pragma once

#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueGlyphMap.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueProgramBuilder.h"
#include "brogueUIConstants.h"
#include "brogueUIProgramPartConfiguration.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
#include "brogueViewContainer.h"
#include "gl.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleDataStream.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleHash.h"
#include "simpleKeyboardState.h"
#include "simpleMouseState.h"
#include "simplePeriodCounter.h"
#include "simpleShaderProgram.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueViewProgram
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
						  brogueViewContainer* viewContainer,
						  resourceController* resourceController,
						  brogueGlyphMap* glyphMap)
		{
			_resourceController = resourceController;
			_programName = programName;
			_viewContainer = viewContainer;
			_programBuilder = new brogueProgramBuilder(resourceController, glyphMap);
			_programs = new simpleHash<brogueUIProgramPartId, simpleShaderProgram*>();
			_programCounters = new simpleHash<brogueUIProgramPartId, simplePeriodCounter*>();
			_active = false;
		}
		~brogueViewProgram()
		{
			for (int index = 0; index < _programs->count(); index++)
			{
				// OpenGL -> Delete GPU Resources
				_programs->getAt(index)->value->teardown();

				delete _programs->getAt(index)->value;
			}

			delete _programBuilder;
			delete _programs;
		}

		bool isActive();
		void initialize();
		void activate();
		void deactivate();
		void checkUpdate(const simpleKeyboardState& keyboardState,
						 const simpleMouseState& mouseState,
						 int millisecondsLapsed);

		bool needsUpdate();
		void clearUpdate();
		void clearEvents();

		void update(const simpleKeyboardState& keyboardState,
					const simpleMouseState& mouseState,
					int millisecondsLapsed);

		void run(int millisecondsElapsed);
		void outputStatus() const;
		bool hasErrors() const;

		gridRect getSceneBoundaryUI() const;
		simpleQuad getCellSizeUV() const;

		brogueKeyboardState calculateKeyboardState(const simpleKeyboardState& keyboard);
		brogueMouseState calculateMouseState(const simpleMouseState& mouse);

	private:

		resourceController* _resourceController;

		brogueUIProgram _programName;
		bool _active;
		brogueViewContainer* _viewContainer;
		brogueProgramBuilder* _programBuilder;

		simpleHash<brogueUIProgramPartId, simpleShaderProgram*>* _programs;
		simpleHash<brogueUIProgramPartId, simplePeriodCounter*>* _programCounters;
	};

	void brogueViewProgram::initialize()
	{
		// Create shader programs for each program part
		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartConfiguration* configuration = _resourceController->getUIPartConfig(view->getUIData()->getId().getPartName());
			simpleShaderProgram* program = _programBuilder->buildProgram(view, *configuration);

			// Initialize program part to "active"
			_programs->add(view->getUIData()->getId(), program);
			_programCounters->add(view->getUIData()->getId(), new simplePeriodCounter(configuration->minimumUpdatePeriodMilliseconds));

			// *** Load OpenGL Backend
			program->compile();
			program->bind();
		}

		_active = true;
	}
	void brogueViewProgram::activate()
	{
		_programs->iterate([] (const brogueUIProgramPartId& part, simpleShaderProgram* program)
		{
			program->bind();

			return iterationCallback::iterate;
		});

		_active = true;
	}
	void brogueViewProgram::deactivate()
	{
		_programs->iterate([] (const brogueUIProgramPartId& part, simpleShaderProgram* program)
		{
			program->unBind();

			return iterationCallback::iterate;
		});

		_active = false;
	}
	gridRect brogueViewProgram::getSceneBoundaryUI() const
	{
		return _viewContainer->calculateSceneBoundaryUI();
	}

	simpleQuad brogueViewProgram::getCellSizeUV() const
	{
		gridRect sceneBoundaryUI = _viewContainer->calculateSceneBoundaryUI();

		brogueCoordinateConverter converter = _programBuilder->buildCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height, _viewContainer->getZoomLevel());

		return converter.getViewConverter()
			.createQuadNormalizedUV(0, 0, brogueCellDisplay::CellWidth(_viewContainer->getZoomLevel()),
									brogueCellDisplay::CellHeight(_viewContainer->getZoomLevel()));
	}

	brogueKeyboardState brogueViewProgram::calculateKeyboardState(const simpleKeyboardState& keyboard)
	{
		// TODO: Get translator for the key system; and implement hotkeys
		return brogueKeyboardState(-1, -1);
	}
	brogueMouseState brogueViewProgram::calculateMouseState(const simpleMouseState& mouse)
	{
		// Translate the UI space -> the brogue / program space and pass down the pipeline
		//
		gridRect sceneBoundary = this->getSceneBoundaryUI();

		brogueMouseState mouseStateUI((mouse.getX() / sceneBoundary.width) * _viewContainer->getSceneBoundary().width,
									  (mouse.getY() / sceneBoundary.height) * _viewContainer->getSceneBoundary().height,
									  mouse.getScrolldXPending() != 0, mouse.getScrolldYPending() != 0,
									  mouse.getScrolldXPending() < 0, mouse.getScrolldYPending() < 0,
									  mouse.getLeftButton() > 0);

		return mouseStateUI;
	}

	void brogueViewProgram::checkUpdate(const simpleKeyboardState& keyboardState,
										const simpleMouseState& mouseState,
										int millisecondsLapsed)
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::checkUpdate");

		brogueKeyboardState keyboardUI = calculateKeyboardState(keyboardState);
		brogueMouseState mouseUI = calculateMouseState(mouseState);

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			// Retrieve program part name
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getUIData()->getId();

			// Check counter to prevent pre-mature update
			if (_programCounters->get(partId)->update(millisecondsLapsed));
			{
				_viewContainer->checkUpdate(partId, keyboardUI, mouseUI, millisecondsLapsed);
			}
		}
	}

	bool brogueViewProgram::isActive()
	{
		return _active;
	}

	bool brogueViewProgram::needsUpdate()
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::needsUpdate");

		bool result = false;

		for (int index = 0; index < _viewContainer->getViewCount() && !result; index++)
		{
			// Retrieve program part name
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getUIData()->getId();

			result |= _viewContainer->needsUpdate(partId);
		}

		return result;
	}
	void brogueViewProgram::clearUpdate()
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::clearUpdate");

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			// Retrieve program part
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getUIData()->getId();

			_viewContainer->clearUpdate(partId);
		}
	}
	void brogueViewProgram::clearEvents()
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::clearEvents");

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			// Retrieve program part
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getUIData()->getId();

			_viewContainer->clearEvents(partId);
		}
	}
	void brogueViewProgram::update(const simpleKeyboardState& keyboardState,
								   const simpleMouseState& mouseState,
								   int millisecondsLapsed)
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::update");

		gridRect sceneBoundary = this->getSceneBoundaryUI();

		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		// TODO: Get translator for the key system; and implement hotkeys
		brogueKeyboardState keyboardStateUI(-1, -1);

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getUIData()->getId();

			if (view->needsUpdate())
			{
				// View will present new data
				view->update(keyboardStateUI, mouseStateUI, millisecondsLapsed);

				// Must update the data stream
				simpleDataStream* stream = _programBuilder->buildDataStream(view, *_resourceController->getUIPartConfig(partId.getPartName()));

				// Put the new stream online (deletes the old stream)
				_programs->get(partId)->bind();
				_programs->get(partId)->reBuffer(stream);
			}
		}
	}

	void brogueViewProgram::run(int millisecondsElapsed)
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::run");

		// OpenGL:  Run each active program
		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartId partId = view->getUIData()->getId();
			brogueUIProgramPartConfiguration* configuration = _resourceController->getUIPartConfig(partId.getPartName());
			simpleShaderProgram* program = _programs->get(partId);

			if (configuration->useAlphaBlending)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			program->bind();
			program->draw();

			if (configuration->useAlphaBlending)
				glDisable(GL_BLEND);
		}
	}

	void brogueViewProgram::outputStatus() const
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::outputStatus");

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartId partId = view->getUIData()->getId();
			simpleShaderProgram* program = _programs->get(partId);
			program->showErrors();
		}
	}

	bool brogueViewProgram::hasErrors() const
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::hasErrors");

		bool result = false;

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartId partId = view->getUIData()->getId();
			simpleShaderProgram* program = _programs->get(partId);
			result |= program->hasErrors();
		}

		return result;
	}
}
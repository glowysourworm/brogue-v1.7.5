#pragma once

#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueFlameMenuHeatView.h"
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
#include "gridLocator.h"
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

			// Create a coordinate converter to use during the render loop
			gridRect sceneBoundaryUI = _viewContainer->calculateSceneBoundaryUI();
			
			_coordinateConverter = new brogueCoordinateConverter(
				_programBuilder->buildCoordinateConverter(sceneBoundaryUI.width, sceneBoundaryUI.height, _viewContainer->getZoomLevel()));
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
			delete _coordinateConverter;
		}

		bool isActive();
		void initialize();
		void activate();
		void deactivate();
		void bindUniforms();
		void outputActives();
		void checkUpdate(const simpleKeyboardState& keyboardState,
						 const simpleMouseState& mouseState,
						 int millisecondsLapsed);

		bool needsUpdate();
		void clearUpdate();
		void clearEvents();

		void update(const simpleKeyboardState& keyboardState,
					const simpleMouseState& mouseState,
					int millisecondsLapsed,
					bool forceUpdate);

		void run(int millisecondsElapsed);
		void outputStatus() const;
		bool hasErrors() const;

		gridRect getSceneBoundaryUI() const;
		simpleQuad getCellSizeUV() const;
		vec2 getScrollUV() const;
		vec4 getClipXY() const;

		brogueKeyboardState calculateKeyboardState(const simpleKeyboardState& keyboard);
		brogueMouseState calculateMouseState(const simpleMouseState& mouse);

		simpleShaderProgram* getShaderProgram(const brogueUIProgramPartId& partId);

	private:

		resourceController* _resourceController;

		brogueUIProgram _programName;
		bool _active;
		brogueViewContainer* _viewContainer;
		brogueProgramBuilder* _programBuilder;
		brogueCoordinateConverter* _coordinateConverter;

		simpleHash<brogueUIProgramPartId, simpleShaderProgram*>* _programs;
		simpleHash<brogueUIProgramPartId, simplePeriodCounter*>* _programCounters;
	};

	void brogueViewProgram::initialize()
	{
		// Create shader programs for each program part
		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartConfiguration* configuration = _resourceController->getUIPartConfig(view->getPartId().getPartName());
			simpleShaderProgram* program = _programBuilder->buildProgram(view, *configuration);

			// Initialize program part to "active"
			_programs->add(view->getPartId(), program);
			_programCounters->add(view->getPartId(), new simplePeriodCounter(configuration->minimumUpdatePeriodMilliseconds));

			// *** Load OpenGL Backend
			program->compile();
			program->bind();

			if (view->getPartId().getPartName() == brogueUIProgramPart::FlameDisplay)
			{
				int zoomLevel = view->getZoomLevel();
				ivec2 cellSizeUI(brogueCellDisplay::CellWidth(zoomLevel), brogueCellDisplay::CellHeight(zoomLevel));
				simpleQuad cellSizeUV = this->getCellSizeUV();

				program->bindUniform2i("cellSizeUI", cellSizeUI);
				program->bindUniform2("cellSizeUV", vec2(cellSizeUV.getWidth(), cellSizeUV.getHeight()));
			}
		}

		_active = true;
	}
	void brogueViewProgram::bindUniforms()
	{
		if (!_active)
			throw simpleException("Trying to bind uniforms to inactive program:  brogueViewProgram::bindUniforms");

		_programs->iterate([] (const brogueUIProgramPartId& partId, simpleShaderProgram* program)
		{
			if (program->isBound())
				program->bindUniforms();

			return iterationCallback::iterate;
		});
	}
	void brogueViewProgram::outputActives()
	{
		if (!_active)
			throw simpleException("Trying to output actives for inactive program:  brogueViewProgram::outputActives");

		_programs->iterate([] (const brogueUIProgramPartId& partId, simpleShaderProgram* program)
		{
			if (program->isBound())
				program->showActives();

			return iterationCallback::iterate;
		});
	}
	simpleShaderProgram* brogueViewProgram::getShaderProgram(const brogueUIProgramPartId& partId)
	{
		return _programs->get(partId);
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

	vec2 brogueViewProgram::getScrollUV() const
	{
		// Scroll Offset (Not quite a simple coordinate transfer. It's also backwards in the y-space)
		vec2 offsetUI = vec2(_viewContainer->getRenderOffset().column * brogueCellDisplay::CellWidth(_viewContainer->getZoomLevel()),
							 _viewContainer->getRenderOffset().row * brogueCellDisplay::CellHeight(_viewContainer->getZoomLevel()));

		// Points to an inverted y-coordinate
		vec2 offsetUV = _coordinateConverter->getViewConverter().convertToNormalizedUV(offsetUI.x, offsetUI.y);

		offsetUV.y = 1 - offsetUV.y;

		return offsetUV;
	}

	vec4 brogueViewProgram::getClipXY() const
	{
		// Clipping Boundary
		//
		gridRect clip = _viewContainer->getContainerBoundary();
		vec2 topLeft = _coordinateConverter->getViewConverter().createQuadNormalizedXY_FromLocator(clip.left(), clip.top()).topLeft;
		vec2 bottomRight = _coordinateConverter->getViewConverter().createQuadNormalizedXY_FromLocator(clip.right(), clip.bottom()).bottomRight;

		return vec4(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, topLeft.y - bottomRight.y);
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

		// Scroll Update:  The brogueViewContainer handles offsets during checkUpdate. These
		//				   render offsets are stored with the view container; and consumed by
		//				   the main program during rendering.
		//
		_viewContainer->updateScroll(keyboardUI, mouseUI, millisecondsLapsed);

		// Iterate Child Views: Each corresponds to a program part; and the composed views are 
		//						of ViewCompositor type.
		//
		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);

			// Program Part's Update
			//
			_viewContainer->checkUpdate(view->getPartId(), keyboardUI, mouseUI, millisecondsLapsed);
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
			brogueViewBase* view = _viewContainer->getViewAt(index);

			result |= _viewContainer->needsUpdate(view->getPartId());
		}

		return result;
	}
	void brogueViewProgram::clearUpdate()
	{
		//if (!_active)
		//	throw simpleException("Brogue View Program not active:  brogueViewProgram::clearUpdate");

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			// Retrieve program part
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getPartId();

			if (_viewContainer->needsUpdate(partId))
				_viewContainer->clearUpdate(partId);

			// Reset program counter (IF PENDING!)
			if (_programCounters->get(partId)->pending())
				_programCounters->get(partId)->reset();
		}
	}
	void brogueViewProgram::clearEvents()
	{
		//if (!_active)
		//	throw simpleException("Brogue View Program not active:  brogueViewProgram::clearEvents");

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			// Retrieve program part
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getPartId();

			_viewContainer->clearEvents(partId);
		}
	}
	void brogueViewProgram::update(const simpleKeyboardState& keyboardState,
								   const simpleMouseState& mouseState,
								   int millisecondsLapsed,
								   bool forceUpdate)
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::update");

		/*
			Scroll / Clip Behavior:  The clipping rectangle is set once during initialization. The 
									 scroll is handled during the render / update loop (here)
		*/

		gridRect sceneBoundary = this->getSceneBoundaryUI();

		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		// TODO: Get translator for the key system; and implement hotkeys
		brogueKeyboardState keyboardStateUI(-1, -1);

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartId partId = _viewContainer->getViewAt(index)->getPartId();

			// This gets set from this->checkUpdate (per program part)
			//
			if (view->needsUpdate() || forceUpdate)
			{
				// View will present new data
				view->update(millisecondsLapsed, forceUpdate);

				// Must update the data stream
				simpleDataStream* stream = _programs->get(partId)->getStream();

				// Rebuilds the current data stream
				_programBuilder->rebuildDataStream(view, *_resourceController->getUIPartConfig(partId.getPartName()), stream);

				// Put the new stream online (GPU memory transfer, no allocation on the heap)
				_programs->get(partId)->bind();
				_programs->get(partId)->reBuffer();
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
			brogueUIProgramPartId partId = view->getPartId();
			brogueUIProgramPartConfiguration* configuration = _resourceController->getUIPartConfig(partId.getPartName());
			simpleShaderProgram* program = _programs->get(partId);

			// Check period counter
			if (!_programCounters->get(partId)->update(millisecondsElapsed))
				continue;

			if (configuration->useAlphaBlending)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}

			program->bind();

			if (program->hasUniform("fadePeriodTime") && partId.getName() == brogueUIProgram::FlameMenuProgram)
				program->bindUniform1("fadePeriodTime", ((brogueFlameMenuHeatView*)view)->currentFadePeriod());

			if (program->hasUniform("fadePeriodRandom1") && partId.getName() == brogueUIProgram::FlameMenuProgram)
				program->bindUniform1i("fadePeriodRandom1", ((brogueFlameMenuHeatView*)view)->currentFadePeriodRandom1());

			if (program->hasUniform("fadePeriodRandom2") && partId.getName() == brogueUIProgram::FlameMenuProgram)
				program->bindUniform1i("fadePeriodRandom2", ((brogueFlameMenuHeatView*)view)->currentFadePeriodRandom2());

			if (program->hasUniform("nextColorNumber") && partId.getName() == brogueUIProgram::FlameMenuProgram)
				program->bindUniform1i("nextColorNumber", ((brogueFlameMenuHeatView*)view)->fadePeriodCount() % 3);

			program->draw();

			if (configuration->useAlphaBlending)
				glDisable(GL_BLEND);

			glFlush();
			glFinish();

			// Lookup NVIDIA bug:  glNamedCopyBufferSubData
			//
			if (partId.getPartName() == brogueUIProgramPart::FlameDisplay)
			{
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
			}
		}
	}

	void brogueViewProgram::outputStatus() const
	{
		if (!_active)
			throw simpleException("Brogue View Program not active:  brogueViewProgram::outputStatus");

		for (int index = 0; index < _viewContainer->getViewCount(); index++)
		{
			brogueViewBase* view = _viewContainer->getViewAt(index);
			brogueUIProgramPartId partId = view->getPartId();
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
			brogueUIProgramPartId partId = view->getPartId();
			simpleShaderProgram* program = _programs->get(partId);
			result |= program->hasErrors();
		}

		return result;
	}
}
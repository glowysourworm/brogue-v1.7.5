#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueImageQuad.h"
#include "brogueKeyboardState.h"
#include "brogueLine.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIMouseData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewCore.h"
#include "brogueViewGridCore.h"
#include "brogueViewPolygonCore.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "openglQuadConverter.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleHash.h"
#include "simpleKeyboardState.h"
#include "simpleLine.h"
#include "simpleList.h"
#include "simpleMouseState.h"
#include "simpleOpenGL.h"
#include <concepts>
#include <functional>
#include "brogueViewContainer.h"


namespace brogueHd::frontend
{
	using namespace simple;

	/// <summary>
	/// This container class provide a mechanism to manage views or composed views. 
	/// </summary>
	class brogueViewProgram
	{
	public:
		brogueViewProgram(brogueCoordinateConverter* coordinateConverter,
		                  brogueUIProgram programName,
		                  const gridRect& containerBoundary,
		                  const gridRect& sceneBoundary);
		~brogueViewProgram();

		template <isGLStream TStream>
		void addView(brogueViewCore<TStream>* view);

		template <isGLUniform TUniform>
		void setUniform(const char* name, const TUniform& value);

		// Design Problem:  Two different view data streams (polygons v.s. 2D-grid)
		//
		void setGridProgram(const brogueUIProgramPartId& partId, const brogueCellDisplay& cell);
		void setPolygonProgram(const brogueUIProgramPartId& partId, const simpleLine<int>& line);

	public: // GL Program Control

		virtual void initiateStateChange(brogueUIState fromState, brogueUIState toState);
		virtual void clearStateChange();
		virtual bool checkStateChange();

		virtual void checkUpdate(const simpleKeyboardState& keyboardState,
		                         const simpleMouseState& mouseState,
		                         int millisecondsLapsed);

		virtual void invalidate(const simpleKeyboardState& keyboardState,
		                        const simpleMouseState& mouseState);

		bool needsUpdate() const;
		void clearUpdate();
		void clearEvents();
		void update(int millisecondsLapsed, bool forceUpdate);

		void initialize();
		void run();
		void activate();
		void deactivate();
		bool hasErrors();
		void showErrors();

		bool isActive() const;

		// UI mouse / keyboard mappings

		brogueKeyboardState calculateKeyboardState(const simpleKeyboardState& keyboard);
		brogueMouseState calculateMouseState(const simpleMouseState& mouse);

	public: // Public Access Getters

		gridRect getSceneBoundary() const;
		gridRect getContainerBoundary() const;
		brogueUIProgram getProgramName() const;

		vec2 getOffsetUV() const
		{
			return _viewContainer->getOffsetUV();
		}

	protected:
		const char* UniformCellSizeUI = "cellSizeUI";
		const char* UniformCellSizeUV = "cellSizeUV";

		void setRenderOffsetUI(int pixelX, float pixelY);

	private:
		brogueCoordinateConverter* _coordinateConverter;
		brogueViewContainer* _viewContainer;
		brogueUIProgram _programName;
		bool _active;
	};

	brogueViewProgram::brogueViewProgram(brogueCoordinateConverter* coordinateConverter,
	                                     brogueUIProgram programName,
	                                     const gridRect& containerBoundary,
	                                     const gridRect& sceneBoundary)
	{
		_coordinateConverter = coordinateConverter;
		_programName = programName;
		_viewContainer = new brogueViewContainer(coordinateConverter, containerBoundary, sceneBoundary);
		_active = false;
	}

	brogueViewProgram::~brogueViewProgram()
	{
		delete _viewContainer;
	}

	template <isGLStream TStream>
	void brogueViewProgram::addView(brogueViewCore<TStream>* view)
	{
		_viewContainer->addView(view);
	}

	template <isGLUniform TUniform>
	void brogueViewProgram::setUniform(const char* name, const TUniform& value)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::setUniform");

		_viewContainer->setUniform(name, value);
	}

	void brogueViewProgram::setGridProgram(const brogueUIProgramPartId& partId, const brogueCellDisplay& cell)
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::setGridProgram");

		if (_viewContainer->getCount() == 0)
			throw simpleException(
				"Must first add views to the brogueViewProgram before accessing data:  brogueViewProgram::setGridProgram");

		_viewContainer->setGridProgram(partId, cell);
	}

	void brogueViewProgram::setPolygonProgram(const brogueUIProgramPartId& partId, const simpleLine<int>& line)
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::setPolygonProgram");

		if (_viewContainer->getCount() == 0)
			throw simpleException(
				"Must first add views to the brogueViewProgram before accessing data:  brogueViewProgram::setPolygonProgram");

		_viewContainer->setPolygonProgram(partId, line);
	}

	brogueUIProgram brogueViewProgram::getProgramName() const
	{
		return _programName;
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
		gridRect sceneBoundaryUI = _coordinateConverter->calculateSceneBoundaryUI();

		brogueMouseState mouseStateUI((mouse.getX() / sceneBoundaryUI.width) * _viewContainer->getSceneBoundary().width,
		                              (mouse.getY() / sceneBoundaryUI.height) * _viewContainer->getSceneBoundary().
		                              height,
		                              mouse.getScrolldXPending() != 0, mouse.getScrolldYPending() != 0,
		                              mouse.getScrolldXPending() < 0, mouse.getScrolldYPending() < 0,
		                              mouse.getLeftButton() > 0);

		return mouseStateUI;
	}

#pragma region public get-access functions

	gridRect brogueViewProgram::getContainerBoundary() const
	{
		return _viewContainer->getContainerBoundary();
	}

	void brogueViewProgram::setRenderOffsetUI(int pixelX, float pixelY)
	{
		_viewContainer->setRenderOffsetUI(pixelX, pixelY);
	}


#pragma endregion

#pragma region GL Program control functions

	bool brogueViewProgram::isActive() const
	{
		return _active;
	}

	void brogueViewProgram::initialize()
	{
		// Initialize Uniforms
		int zoomLevel = _coordinateConverter->getZoomLevel();
		simpleQuad sizeUV = _viewContainer->getCellSizeUV();
		ivec2 cellSizeUI(brogueCellDisplay::CellWidth(zoomLevel), brogueCellDisplay::CellHeight(zoomLevel));
		vec2 cellSizeUV(sizeUV.getWidth(), sizeUV.getHeight());

		_viewContainer->setUniform(this->UniformCellSizeUI, cellSizeUI);
		_viewContainer->setUniform(this->UniformCellSizeUV, cellSizeUV);
	}

	void brogueViewProgram::run()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::run");

		_viewContainer->run();
	}

	void brogueViewProgram::activate()
	{
		_viewContainer->activate();
		_active = true;
	}

	void brogueViewProgram::deactivate()
	{
		_viewContainer->deactivate();
		_active = false;
	}

	bool brogueViewProgram::hasErrors()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::hasErrors");

		return _viewContainer->hasErrors();
	}

	void brogueViewProgram::showErrors()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::showErrors");

		_viewContainer->showErrors();
	}

	void brogueViewProgram::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_viewContainer->initiateStateChange(fromState, toState);
	}

	void brogueViewProgram::clearStateChange()
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_viewContainer->clearStateChange();
	}

	bool brogueViewProgram::checkStateChange()
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		return _viewContainer->checkStateChange();
	}

	void brogueViewProgram::checkUpdate(const simpleKeyboardState& keyboardState,
	                                    const simpleMouseState& mouseState,
	                                    int millisecondsLapsed)
	{
		if (_viewContainer->getCount() == 0)
			throw simpleException(
				"Must first add views to the brogueViewProgram before accessing data:  brogueViewProgram::checkUpdate");

		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		brogueKeyboardState keyboardStateUI = calculateKeyboardState(keyboardState);
		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		// The view container must track mouse events for specific UI invalidation (and perhaps later,
		// click and drag events)
		//
		bool containerMouseOver = this->getContainerBoundary().contains(mouseStateUI.getLocation());

		// The view container must discard mouse events outside its boundary
		//
		if (!containerMouseOver)
			return;

		_viewContainer->checkUpdate(keyboardStateUI, mouseStateUI, millisecondsLapsed);
	}

	void brogueViewProgram::invalidate(const simpleKeyboardState& keyboardState,
	                                   const simpleMouseState& mouseState)
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		brogueKeyboardState keyboardStateUI = calculateKeyboardState(keyboardState);
		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		_viewContainer->invalidate(keyboardStateUI, mouseStateUI);
	}

	bool brogueViewProgram::needsUpdate() const
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::needsUpdate");

		return _viewContainer->needsUpdate();
	}

	void brogueViewProgram::clearUpdate()
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::clearUpdate");

		_viewContainer->clearUpdate();
	}

	void brogueViewProgram::clearEvents()
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::clearEvents");

		_viewContainer->clearEvents();
	}

	void brogueViewProgram::update(int millisecondsLapsed, bool forceUpdate)
	{
		if (!_active)
			throw simpleException(
				"Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_viewContainer->update(millisecondsLapsed, forceUpdate);
	}

#pragma endregion
}

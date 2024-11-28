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

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	/// <summary>
	/// This container class provide a mechanism to manage views or composed views. 
	/// </summary>
	class brogueViewProgram
	{
	public:

		brogueViewProgram(brogueCoordinateConverter* coordinateConverter,
							brogueUIProgram programName,
							int zoomLevel,
							bool hasScrollInteraction,
							bool applyClipping,
							const gridRect& containerBoundary,
							const gridRect& sceneBoundary);
		~brogueViewProgram();

		template<isGLStream TStream>
		void addView(brogueViewCore<TStream>* view);

		template<isGLUniform TUniform>
		void setUniform(const char* name, const TUniform& value);

		// Design Problem:  Two different view data streams (polygons v.s. 2D-grid)
		//
		void setGridProgram(const brogueUIProgramPartId& partId, const brogueCellDisplay& cell);
		void setPolygonProgram(const brogueUIProgramPartId& partId, const simpleLine<int>& line);

	public:		// GL Program Control

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

	public:		// Public Access Getters

		gridRect getSceneBoundary() const;
		gridRect getContainerBoundary() const;
		gridLocator getScrollOffset() const;
		vec2 getRenderOffsetUI() const;
		int getZoomLevel() const;
		bool getClipping() const;
		brogueUIProgram getProgramName() const;

		/// <summary>
		/// Returns an additional offset that may be used by the view container. This is not
		/// the scroll offset or the clip; but is applied additionally to the texture sampler.
		/// </summary>
		vec2 getOffsetUV() const;
		vec4 getClipXY() const;
		simpleQuad getCellSizeUV() const;
		vec2 getScrollUV() const;

	protected:

		const char* UniformCellSizeUI = "cellSizeUI";
		const char* UniformCellSizeUV = "cellSizeUV";

		void setScrollOffset(int column, int row);
		void setRenderOffsetUI(int pixelX, float pixelY);
		brogueMouseState getAdjustedMouse(const brogueMouseState& mouseState) const;

	private:	// Needed way to get around template specification

		/// <summary>
		/// Returns aggregate child boundary with scroll offset
		/// </summary>
		gridRect getChildOffsetBoundary() const;

		void updateScroll(const simpleKeyboardState& keyboardState,
						  const simpleMouseState& mouseState,
						  int millisecondsLapsed);

	private:

		brogueCoordinateConverter* _coordinateConverter;

		brogueUIProgram _programName;
		bool _hasScrollInteraction;
		bool _applyClipping;
		int _zoomLevel;
		brogueUIMouseData* _mouseData;									// Container mouse data (need enter / leave handling)
		gridRect* _containerBoundary;
		gridRect* _sceneBoundary;
		gridLocator* _scrollOffset;
		vec2* _renderOffset;											// Hard render offset for shader animation

		bool _active;

		brogueViewContainer* _viewContainer;
	};

	brogueViewProgram::brogueViewProgram(brogueCoordinateConverter* coordinateConverter,
											 brogueUIProgram programName,
											 int zoomLevel,
											 bool hasScrollInteraction,
											 bool applyClipping,
											 const gridRect& containerBoundary,
											 const gridRect& sceneBoundary)
	{
		_coordinateConverter = coordinateConverter;
		_programName = programName;
		_zoomLevel = zoomLevel;
		_mouseData = new brogueUIMouseData();
		_containerBoundary = new gridRect(containerBoundary);
		_sceneBoundary = new gridRect(sceneBoundary);
		_scrollOffset = new gridLocator(0, 0);
		_renderOffset = new vec2(0, 0);
		_hasScrollInteraction = hasScrollInteraction;
		_applyClipping = applyClipping;

		_viewContainer = new brogueViewContainer();

		_active = false;
	}
	brogueViewProgram::~brogueViewProgram()
	{
		delete _viewContainer;
		delete _mouseData;
		delete _sceneBoundary;
		delete _containerBoundary;
		delete _renderOffset;
		delete _scrollOffset;
	}

	template<isGLStream TStream>
	void brogueViewProgram::addView(brogueViewCore<TStream>* view)
	{
		_viewContainer->addView(view);
	}

	template<isGLUniform TUniform>
	void brogueViewProgram::setUniform(const char* name, const TUniform& value)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::setUniform");

		_viewContainer->setUniform(name, value);
	}

	void brogueViewProgram::setGridProgram(const brogueUIProgramPartId& partId, const brogueCellDisplay& cell)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::setGridProgram");

		if (_viewContainer->getCount() == 0)
			throw simpleException("Must first add views to the brogueViewProgram before accessing data:  brogueViewProgram::setGridProgram");

		_viewContainer->setGridProgram(partId, cell);
	}
	void brogueViewProgram::setPolygonProgram(const brogueUIProgramPartId& partId, const simpleLine<int>& line)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::setPolygonProgram");

		if (_viewContainer->getCount() == 0)
			throw simpleException("Must first add views to the brogueViewProgram before accessing data:  brogueViewProgram::setPolygonProgram");

		_viewContainer->setPolygonProgram(partId, line);
	}

	brogueUIProgram brogueViewProgram::getProgramName() const
	{
		return _programName;
	}

	simpleQuad brogueViewProgram::getCellSizeUV() const
	{
		openglQuadConverter viewConverter = _coordinateConverter->getViewConverter();

		return viewConverter.createQuadNormalizedUV(0, 0, brogueCellDisplay::CellWidth(_zoomLevel), brogueCellDisplay::CellHeight(_zoomLevel));
	}
	vec2 brogueViewProgram::getOffsetUV() const
	{
		vec2 offsetUI = getRenderOffsetUI();

		// Points to an inverted y-coordinate
		vec2 offsetUV = _coordinateConverter->getViewConverter().convertToNormalizedUV(offsetUI.x, offsetUI.y);

		offsetUV.y = 1 - offsetUV.y;

		return offsetUV;
	}
	vec2 brogueViewProgram::getScrollUV() const
	{
		openglQuadConverter viewConverter = _coordinateConverter->getViewConverter();

		// Scroll Offset (Not quite a simple coordinate transfer. It's also backwards in the y-space)
		vec2 offsetUI = vec2(_scrollOffset->column * brogueCellDisplay::CellWidth(_zoomLevel),
							 _scrollOffset->row * brogueCellDisplay::CellHeight(_zoomLevel));

		// Points to an inverted y-coordinate
		vec2 offsetUV = viewConverter.convertToNormalizedUV(offsetUI.x, offsetUI.y);

		offsetUV.y = 1 - offsetUV.y;

		return offsetUV;
	}
	vec4 brogueViewProgram::getClipXY() const
	{
		openglQuadConverter viewConverter = _coordinateConverter->getViewConverter();

		// Clipping Boundary
		//
		gridRect clip = *_containerBoundary;
		vec2 topLeft = viewConverter.createQuadNormalizedXY_FromLocator(clip.left(), clip.top()).topLeft;
		vec2 bottomRight = viewConverter.createQuadNormalizedXY_FromLocator(clip.right(), clip.bottom()).bottomRight;

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
		gridRect sceneBoundaryUI = _coordinateConverter->calculateSceneBoundaryUI();

		brogueMouseState mouseStateUI((mouse.getX() / sceneBoundaryUI.width) * _sceneBoundary->width,
									  (mouse.getY() / sceneBoundaryUI.height) * _sceneBoundary->height,
									   mouse.getScrolldXPending() != 0, mouse.getScrolldYPending() != 0,
									   mouse.getScrolldXPending() < 0, mouse.getScrolldYPending() < 0,
									   mouse.getLeftButton() > 0);

		return mouseStateUI;
	}

#pragma region public get-access functions

	gridRect brogueViewProgram::getSceneBoundary() const
	{
		return *_sceneBoundary;
	}
	gridRect brogueViewProgram::getChildOffsetBoundary() const
	{
		if (_viewContainer->getCount() == 0)
			throw simpleException("Must first add views to the brogueViewProgram before accessing data:  brogueViewProgram::getBoundary");

		gridRect boundary = _viewContainer->getAggregateBoundary();

		// Apply scroll offset
		boundary.translate(_scrollOffset->column, _scrollOffset->row);

		return boundary;
	}
	bool brogueViewProgram::getClipping() const
	{
		return _applyClipping;
	}
	gridRect brogueViewProgram::getContainerBoundary() const
	{
		return *_containerBoundary;
	}
	gridLocator brogueViewProgram::getScrollOffset() const
	{
		return *_scrollOffset;
	}
	vec2 brogueViewProgram::getRenderOffsetUI() const
	{
		return *_renderOffset;
	}
	void brogueViewProgram::setScrollOffset(int column, int row)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_scrollOffset->column = column;
		_scrollOffset->row = row;
	}
	void brogueViewProgram::setRenderOffsetUI(int pixelX, float pixelY)
	{
		// During initialization
		//if (!_active)
		//	throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_renderOffset->x = pixelX;
		_renderOffset->y = pixelY;
	}
	brogueMouseState brogueViewProgram::getAdjustedMouse(const brogueMouseState& mouseState) const
	{
		// Apply mouse transform to the mouse state for the child views (utilizes scrolling).
		//
		brogueMouseState adjustedMouse(mouseState.getLocation()
												 .subtract(*_scrollOffset)
												 .subtract(_renderOffset->x / brogueCellDisplay::CellWidth(_zoomLevel),
									   _renderOffset->y / brogueCellDisplay::CellHeight(_zoomLevel)),
									   mouseState.getScrollPendingX(),
									   mouseState.getScrollPendingY(),
									   mouseState.getScrollPendingX(),
									   mouseState.getScrollNegativeY(),
									   mouseState.getMouseLeft());

		return adjustedMouse;
	}
	int brogueViewProgram::getZoomLevel() const
	{
		return _zoomLevel;
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
		ivec2 cellSizeUI(brogueCellDisplay::CellWidth(_zoomLevel), brogueCellDisplay::CellHeight(_zoomLevel));
		vec2 cellSizeUV(this->getCellSizeUV().getWidth(), this->getCellSizeUV().getHeight());

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
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_viewContainer->initiateStateChange(fromState, toState);
	}
	void brogueViewProgram::clearStateChange()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_viewContainer->clearStateChange();
	}
	bool brogueViewProgram::checkStateChange()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		return _viewContainer->checkStateChange();
	}
	void brogueViewProgram::updateScroll(const simpleKeyboardState& keyboardState,
										   const simpleMouseState& mouseState,
										   int millisecondsLapsed)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		if (!_hasScrollInteraction)
			return;

		/*
			UI Behavior:    This will add the scroll behavior. (or x-y scrolling behavior). The
							getBoundary() method will allow for all child controls. The other
							getContainerBoundary() method will check the clipping boundary for
							this view container.

			Render Offset:  Must add the UI offset to the container boundary to get the proper
							mouse position.
		*/

		brogueKeyboardState keyboardStateUI = calculateKeyboardState(keyboardState);
		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		// Aggregate child boundary
		gridRect childBoundary = this->getChildOffsetBoundary();
		gridRect boundary = this->getContainerBoundary();

		// TODO: CLEAN THIS UP

		// UI Translation:  This is the ultimate position of the mouse pointer
		boundary.translate(_renderOffset->x / brogueCellDisplay::CellWidth(_zoomLevel),
						   _renderOffset->y / brogueCellDisplay::CellHeight(_zoomLevel));

		bool mouseOver = this->getContainerBoundary().contains(mouseStateUI.getLocation());
		bool mousePressed = mouseStateUI.getMouseLeft();
		bool scrollEvent = mouseStateUI.getScrollPendingX() || mouseStateUI.getScrollPendingY();

		if (!mouseOver || !scrollEvent)
			return;

		// Check scroll bounds
		int scrollX = 0;
		int scrollY = 0;

		// Parent View -> Mouse Over -> Scroll
		if (_hasScrollInteraction && mouseOver && scrollEvent)
		{
			if (mouseStateUI.getScrollPendingX())
			{
				if (!mouseStateUI.getScrollNegativeX() && childBoundary.left() < _containerBoundary->left())
					scrollX = 1;

				else if (mouseStateUI.getScrollNegativeX() && childBoundary.right() > _containerBoundary->right())
					scrollX = -1;
			}
			if (mouseStateUI.getScrollPendingY())
			{
				// Up
				if (mouseStateUI.getScrollNegativeY() && childBoundary.bottom() > _containerBoundary->bottom())
					scrollY = -1;

				// Down
				else if (!mouseStateUI.getScrollNegativeY() && childBoundary.top() < _containerBoundary->top())
					scrollY = 1;
			}
		}

		_scrollOffset->row += scrollY;
		_scrollOffset->column += scrollX;
	}
	void brogueViewProgram::checkUpdate(const simpleKeyboardState& keyboardState,
										  const simpleMouseState& mouseState,
										  int millisecondsLapsed)
	{
		if (_viewContainer->getCount() == 0)
			throw simpleException("Must first add views to the brogueViewProgram before accessing data:  brogueViewProgram::checkUpdate");

		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		brogueKeyboardState keyboardStateUI = calculateKeyboardState(keyboardState);
		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		// The view container must track mouse events for specific UI invalidation (and perhaps later,
		// click and drag events)
		//
		bool containerMouseOver = this->getContainerBoundary().contains(mouseStateUI.getLocation());

		// Update View Container:  Keeps track of its own mouse events
		_mouseData->setUpdate(mouseStateUI.getMouseLeft(), containerMouseOver);

		// The view container must apply clipping container to the part to avoid unwanted behavior.
		//
		if (!containerMouseOver && !_mouseData->getMouseEnter() && !_mouseData->getMouseLeave())
			return;

		// (TODO: SOME REDESIGN)
		if (!containerMouseOver)
		{
			// Pre-maturely clearing the view container's mouse data because it's not supposed to be
			// handled by the render loop. The view container is acting like a view.
			_mouseData->clear();

			// The child views will now process their mouse-leave event
		}

		// Scroll Behavior:  The updating of the scroll is done at the container level. So,
		//					 this function must be run once during the normal checkUpdate path
		//					 for this UI program.
		//
		updateScroll(keyboardState, mouseState, millisecondsLapsed);

		// Apply mouse transform to the mouse state for the child views (utilizes scrolling and render offsets)
		//
		brogueMouseState adjustedMouseUI = getAdjustedMouse(mouseStateUI);

		_viewContainer->checkUpdate(keyboardStateUI, adjustedMouseUI, millisecondsLapsed);
	}
	void brogueViewProgram::invalidate(const simpleKeyboardState& keyboardState,
										 const simpleMouseState& mouseState)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		brogueKeyboardState keyboardStateUI = calculateKeyboardState(keyboardState);
		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		// Apply mouse transform to the mouse state for the child views (utilizes scrolling and render offsets)
		//
		brogueMouseState adjustedMouseUI = getAdjustedMouse(mouseStateUI);

		_viewContainer->invalidate(keyboardStateUI, adjustedMouseUI);
	}
	bool brogueViewProgram::needsUpdate() const
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::needsUpdate");

		return _viewContainer->needsUpdate();
	}
	void brogueViewProgram::clearUpdate()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::clearUpdate");

		_viewContainer->clearUpdate();
	}
	void brogueViewProgram::clearEvents()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::clearEvents");

		_viewContainer->clearEvents();
	}
	void brogueViewProgram::update(int millisecondsLapsed, bool forceUpdate)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewProgram::initialStateChange");

		_viewContainer->update(millisecondsLapsed, forceUpdate);
	}

#pragma endregion
}
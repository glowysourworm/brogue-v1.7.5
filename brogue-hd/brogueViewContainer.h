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
#include "brogueUIData.h"
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

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	/// <summary>
	/// This container class provide a mechanism to manage views or composed views. 
	/// </summary>
	class brogueViewContainer
	{
	public:

		brogueViewContainer(brogueCoordinateConverter* coordinateConverter,
							brogueUIProgram programName,
							int zoomLevel,
							bool hasScrollInteraction,
							bool applyClipping,
							const gridRect& containerBoundary,
							const gridRect& sceneBoundary);
		~brogueViewContainer();

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

		void compile();
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

		void addViewImpl(brogueViewCore<brogueImageQuad>* view);
		void addViewImpl(brogueViewCore<brogueColorQuad>* view);
		void addViewImpl(brogueViewCore<brogueCellQuad>* view);
		void addViewImpl(brogueViewCore<brogueLine>* view);

		int getViewCount() const;

		template<isGLStream TStream>
		int getViewCount();

		gridRect aggregateChildBoundary() const;

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

		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueCellQuad>*>* _cellViews;
		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueColorQuad>*>* _colorViews;
		simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueImageQuad>*>* _imageViews;
		simpleHash<brogueUIProgramPartId, brogueViewPolygonCore*>* _lineViews;
	};

	brogueViewContainer::brogueViewContainer(brogueCoordinateConverter* coordinateConverter,
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

		_cellViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueCellQuad>*>();
		_colorViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueColorQuad>*>();
		_imageViews = new simpleHash<brogueUIProgramPartId, brogueViewGridCore<brogueImageQuad>*>();
		_lineViews = new simpleHash<brogueUIProgramPartId, brogueViewPolygonCore*>();

		_active = false;
	}
	brogueViewContainer::~brogueViewContainer()
	{
		_cellViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueCellQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_colorViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueColorQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_imageViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueImageQuad>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});
		_lineViews->iterate([] (const brogueUIProgramPartId& partId, brogueViewCore<brogueLine>* view)
		{
			delete view;
			return iterationCallback::iterate;
		});

		delete _cellViews;
		delete _colorViews;
		delete _imageViews;
		delete _lineViews;
		delete _mouseData;
		delete _sceneBoundary;
		delete _containerBoundary;
		delete _renderOffset;
		delete _scrollOffset;
	}

	template<isGLStream TStream>
	void brogueViewContainer::addView(brogueViewCore<TStream>* view)
	{
		addViewImpl(view);
	}

	template<isGLUniform TUniform>
	void brogueViewContainer::setUniform(const char* name, const TUniform& value)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::setUniform");

		_cellViews->forEach([&name, &value] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			if (view->hasUniform(name))
				view->setUniform(name, value);

			return iterationCallback::iterate;
		});

		_imageViews->forEach([&name, &value] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			if (view->hasUniform(name))
				view->setUniform(name, value);

			return iterationCallback::iterate;
		});

		_colorViews->forEach([&name, &value] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			if (view->hasUniform(name))
				view->setUniform(name, value);

			return iterationCallback::iterate;
		});

		_lineViews->forEach([&name, &value] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			if (view->hasUniform(name))
				view->setUniform(name, value);

			return iterationCallback::iterate;
		});
	}

	void brogueViewContainer::setGridProgram(const brogueUIProgramPartId& partId, const brogueCellDisplay& cell)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::setGridProgram");

		if (this->getViewCount() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::setGridProgram");

		if (_cellViews->contains(partId))
			_cellViews->get(partId)->set(cell);

		else if (_imageViews->contains(partId))
			_imageViews->get(partId)->set(cell);

		else if (_colorViews->contains(partId))
			_colorViews->get(partId)->set(cell);

		else
			throw simpleException("Program part not found:  brogueViewContainer::setGridProgram");
	}
	void brogueViewContainer::setPolygonProgram(const brogueUIProgramPartId& partId, const simpleLine<int>& line)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::setPolygonProgram");

		if (this->getViewCount() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::setPolygonProgram");

		if (_lineViews->contains(partId))
			_lineViews->get(partId)->addLine(line);
		else
			throw simpleException("Program part not found:  brogueViewContainer::setPolygonProgram");
	}

	brogueUIProgram brogueViewContainer::getProgramName() const
	{
		return _programName;
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueImageQuad>* view)
	{
		_imageViews->add(view->getId(), (brogueViewGridCore<brogueImageQuad>*)view);
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueColorQuad>* view)
	{
		_colorViews->add(view->getId(), (brogueViewGridCore<brogueColorQuad>*)view);
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueCellQuad>* view)
	{
		_cellViews->add(view->getId(), (brogueViewGridCore<brogueCellQuad>*)view);
	}
	void brogueViewContainer::addViewImpl(brogueViewCore<brogueLine>* view)
	{
		_lineViews->add(view->getId(), (brogueViewPolygonCore*)view);
	}
	int brogueViewContainer::getViewCount() const
	{
		return _imageViews->count() + _colorViews->count() + _cellViews->count() + _lineViews->count();
	}

	simpleQuad brogueViewContainer::getCellSizeUV() const
	{
		openglQuadConverter viewConverter = _coordinateConverter->getViewConverter();
		
		return viewConverter.createQuadNormalizedUV(0, 0, brogueCellDisplay::CellWidth(_zoomLevel), brogueCellDisplay::CellHeight(_zoomLevel));
	}
	vec2 brogueViewContainer::getOffsetUV() const
	{
		vec2 offsetUI = getRenderOffsetUI();

		// Points to an inverted y-coordinate
		vec2 offsetUV = _coordinateConverter->getViewConverter().convertToNormalizedUV(offsetUI.x, offsetUI.y);

		offsetUV.y = 1 - offsetUV.y;

		return offsetUV;
	}
	vec2 brogueViewContainer::getScrollUV() const
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
	vec4 brogueViewContainer::getClipXY() const
	{
		openglQuadConverter viewConverter = _coordinateConverter->getViewConverter();

		// Clipping Boundary
		//
		gridRect clip = *_containerBoundary;
		vec2 topLeft = viewConverter.createQuadNormalizedXY_FromLocator(clip.left(), clip.top()).topLeft;
		vec2 bottomRight = viewConverter.createQuadNormalizedXY_FromLocator(clip.right(), clip.bottom()).bottomRight;

		return vec4(topLeft.x, topLeft.y, bottomRight.x - topLeft.x, topLeft.y - bottomRight.y);
	}

	template<isGLStream TStream>
	int brogueViewContainer::getViewCount()
	{
		if (std::same_as<TStream, brogueCellQuad>())
			return _cellViews->count();

		else if (std::same_as<TStream, brogueImageQuad>())
			return _imageViews->count();

		else if (std::same_as<TStream, brogueColorQuad>())
			return _colorViews->count();

		else if (std::same_as<TStream, brogueLine>())
			return _lineViews->count();
		else
			throw simpleException("Unhandled stream type:  brogueViewContainer::getViewCount");
	}
	
	gridRect brogueViewContainer::aggregateChildBoundary() const
	{
		gridRect boundary;

		simpleListAggregator<gridRect, gridRect> callback([] (const gridRect& seed, const gridRect& viewBounds)
		{
			gridRect nextBounds = seed;
			if (nextBounds == default_value::value<gridRect>())
				nextBounds = viewBounds;
			else
				nextBounds.expand(viewBounds);
			return nextBounds;
		});

		// Iterator...
		boundary = _cellViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueCellQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _imageViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueImageQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _colorViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueColorQuad>* view)
		{
			return callback(seed, view->getBoundary());
		});
		boundary = _lineViews->getValues().aggregate<gridRect>(boundary, [&callback] (const gridRect& seed, brogueViewCore<brogueLine>* view)
		{
			return callback(seed, view->getBoundary());
		});

		return boundary;
	}
	brogueKeyboardState brogueViewContainer::calculateKeyboardState(const simpleKeyboardState& keyboard)
	{
		// TODO: Get translator for the key system; and implement hotkeys
		return brogueKeyboardState(-1, -1);
	}
	brogueMouseState brogueViewContainer::calculateMouseState(const simpleMouseState& mouse)
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

	gridRect brogueViewContainer::getSceneBoundary() const
	{
		return *_sceneBoundary;
	}
	gridRect brogueViewContainer::getChildOffsetBoundary() const
	{
		if (this->getViewCount() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::getBoundary");

		gridRect boundary = aggregateChildBoundary();

		// Apply scroll offset
		boundary.translate(_scrollOffset->column, _scrollOffset->row);

		return boundary;
	}
	bool brogueViewContainer::getClipping() const
	{
		return _applyClipping;
	}
	gridRect brogueViewContainer::getContainerBoundary() const
	{
		return *_containerBoundary;
	}
	gridLocator brogueViewContainer::getScrollOffset() const
	{
		return *_scrollOffset;
	}
	vec2 brogueViewContainer::getRenderOffsetUI() const
	{
		return *_renderOffset;
	}
	void brogueViewContainer::setScrollOffset(int column, int row)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

		_scrollOffset->column = column;
		_scrollOffset->row = row;
	}
	void brogueViewContainer::setRenderOffsetUI(int pixelX, float pixelY)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

		_renderOffset->x = pixelX;
		_renderOffset->y = pixelY;
	}
	brogueMouseState brogueViewContainer::getAdjustedMouse(const brogueMouseState& mouseState) const
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
	int brogueViewContainer::getZoomLevel() const
	{
		return _zoomLevel;
	}

#pragma endregion

#pragma region GL Program control functions

	bool brogueViewContainer::isActive() const
	{
		return _active;
	}
	void brogueViewContainer::compile()
	{
		// Initialize Uniforms
		ivec2 cellSizeUI(brogueCellDisplay::CellWidth(_zoomLevel), brogueCellDisplay::CellHeight(_zoomLevel));
		vec2 cellSizeUV(this->getCellSizeUV().getWidth(), this->getCellSizeUV().getHeight());

		const brogueViewContainer* that = this;

		_cellViews->forEach([&that, &cellSizeUI, &cellSizeUV] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->compileCore();

			if (view->hasUniform(that->UniformCellSizeUI))
				view->setUniform(that->UniformCellSizeUI, cellSizeUI);

			if (view->hasUniform(that->UniformCellSizeUV))
				view->setUniform(that->UniformCellSizeUV, cellSizeUV);

			return iterationCallback::iterate;
		});

		_imageViews->forEach([&that, &cellSizeUI, &cellSizeUV] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->compileCore();

			if (view->hasUniform(that->UniformCellSizeUI))
				view->setUniform(that->UniformCellSizeUI, cellSizeUI);

			if (view->hasUniform(that->UniformCellSizeUV))
				view->setUniform(that->UniformCellSizeUV, cellSizeUV);

			return iterationCallback::iterate;
		});

		_colorViews->forEach([&that, &cellSizeUI, &cellSizeUV] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->compileCore();

			if (view->hasUniform(that->UniformCellSizeUI))
				view->setUniform(that->UniformCellSizeUI, cellSizeUI);

			if (view->hasUniform(that->UniformCellSizeUV))
				view->setUniform(that->UniformCellSizeUV, cellSizeUV);

			return iterationCallback::iterate;
		});

		_lineViews->forEach([&that, &cellSizeUI, &cellSizeUV] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->compileCore();

			if (view->hasUniform(that->UniformCellSizeUI))
				view->setUniform(that->UniformCellSizeUI, cellSizeUI);

			if (view->hasUniform(that->UniformCellSizeUV))
				view->setUniform(that->UniformCellSizeUV, cellSizeUV);

			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::run()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::run");

		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->run();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->run();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->run();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->run();
			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::activate()
	{
		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->activate();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->activate();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->activate();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->activate();
			return iterationCallback::iterate;
		});

		_active = true;
	}
	void brogueViewContainer::deactivate()
	{
		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->deactivate();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->deactivate();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->deactivate();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->deactivate();
			return iterationCallback::iterate;
		});

		_active = false;
	}
	bool brogueViewContainer::hasErrors()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::hasErrors");

		bool result = false;

		_cellViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			result |= view->hasErrors();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			result |= view->hasErrors();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			result |= view->hasErrors();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			result |= view->hasErrors();
			return iterationCallback::iterate;
		});

		return result;
	}
	void brogueViewContainer::showErrors()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::showErrors");

		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->showErrors();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->showErrors();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->showErrors();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->showErrors();
			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

		_cellViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});

		_imageViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});

		_colorViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});

		_lineViews->forEach([&fromState, &toState] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			//view->initiateStateChange(fromState, toState);
			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::clearStateChange()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->clearStateChange();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->clearStateChange();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->clearStateChange();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			//view->clearStateChange();
			return iterationCallback::iterate;
		});
	}
	bool brogueViewContainer::checkStateChange()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

		bool result = false;

		result |= _cellViews->any([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			return view->checkStateChange();
		});

		result |= _imageViews->any([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			return view->checkStateChange();
		});

		result |= _colorViews->any([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			return view->checkStateChange();
		});

		result |= _lineViews->any([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			return false; // view->checkStateChange();
		});

		return result;
	}
	void brogueViewContainer::updateScroll(const simpleKeyboardState& keyboardState,
										   const simpleMouseState& mouseState,
										   int millisecondsLapsed)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

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
	void brogueViewContainer::checkUpdate(const simpleKeyboardState& keyboardState,
										  const simpleMouseState& mouseState,
										  int millisecondsLapsed)
	{
		if (this->getViewCount() == 0)
			throw simpleException("Must first add views to the brogueViewContainer before accessing data:  brogueViewContainer::checkUpdate");

		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

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
		brogueMouseState adjustedMouse = getAdjustedMouse(mouseStateUI);

		_cellViews->forEach([&keyboardStateUI, &adjustedMouse, &millisecondsLapsed] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->checkUpdate(keyboardStateUI, adjustedMouse, millisecondsLapsed);
			return iterationCallback::iterate;
		});

		_imageViews->forEach([&keyboardStateUI, &adjustedMouse, &millisecondsLapsed] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->checkUpdate(keyboardStateUI, adjustedMouse, millisecondsLapsed);
			return iterationCallback::iterate;
		});

		_colorViews->forEach([&keyboardStateUI, &adjustedMouse, &millisecondsLapsed] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->checkUpdate(keyboardStateUI, adjustedMouse, millisecondsLapsed);
			return iterationCallback::iterate;
		});

		_lineViews->forEach([&keyboardStateUI, &adjustedMouse, &millisecondsLapsed] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->checkUpdate(keyboardStateUI, adjustedMouse, millisecondsLapsed);
			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::invalidate(const simpleKeyboardState& keyboardState,
										 const simpleMouseState& mouseState)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

		brogueKeyboardState keyboardStateUI = calculateKeyboardState(keyboardState);
		brogueMouseState mouseStateUI = calculateMouseState(mouseState);

		// Apply mouse transform to the mouse state for the child views (utilizes scrolling and render offsets)
		//
		brogueMouseState adjustedMouse = getAdjustedMouse(mouseStateUI);

		_cellViews->forEach([&keyboardStateUI, &adjustedMouse] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->invalidate(keyboardStateUI, adjustedMouse);
			return iterationCallback::iterate;
		});

		_imageViews->forEach([&keyboardStateUI, &adjustedMouse] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->invalidate(keyboardStateUI, adjustedMouse);
			return iterationCallback::iterate;
		});

		_colorViews->forEach([&keyboardStateUI, &adjustedMouse] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->invalidate(keyboardStateUI, adjustedMouse);
			return iterationCallback::iterate;
		});

		_lineViews->forEach([&keyboardStateUI, &adjustedMouse] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->invalidate(keyboardStateUI, adjustedMouse);
			return iterationCallback::iterate;
		});
	}
	bool brogueViewContainer::needsUpdate() const
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::needsUpdate");

		bool result = false;

		_cellViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			result |= view->needsUpdate();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			result |= view->needsUpdate();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			result |= view->needsUpdate();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([&result] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			result |= view->needsUpdate();
			return iterationCallback::iterate;
		});

		return result;
	}
	void brogueViewContainer::clearUpdate()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::clearUpdate");

		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			if (view->needsUpdate())
				view->clearUpdate();

			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			if (view->needsUpdate())
				view->clearUpdate();

			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			if (view->needsUpdate())
				view->clearUpdate();

			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			if (view->needsUpdate())
				view->clearUpdate();

			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::clearEvents()
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::clearEvents");

		_cellViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->clearEvents();
			return iterationCallback::iterate;
		});

		_imageViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->clearEvents();
			return iterationCallback::iterate;
		});

		_colorViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->clearEvents();
			return iterationCallback::iterate;
		});

		_lineViews->forEach([] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->clearEvents();
			return iterationCallback::iterate;
		});
	}
	void brogueViewContainer::update(int millisecondsLapsed, bool forceUpdate)
	{
		if (!_active)
			throw simpleException("Must activate program first before calling methods:  brogueViewContainer::initialStateChange");

		_cellViews->forEach([&millisecondsLapsed, &forceUpdate] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueCellQuad>* view)
		{
			view->update(millisecondsLapsed, forceUpdate);
			return iterationCallback::iterate;
		});

		_imageViews->forEach([&millisecondsLapsed, &forceUpdate] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueImageQuad>* view)
		{
			view->update(millisecondsLapsed, forceUpdate);
			return iterationCallback::iterate;
		});

		_colorViews->forEach([&millisecondsLapsed, &forceUpdate] (const brogueUIProgramPartId& partId, brogueViewGridCore<brogueColorQuad>* view)
		{
			view->update(millisecondsLapsed, forceUpdate);
			return iterationCallback::iterate;
		});

		_lineViews->forEach([&millisecondsLapsed, &forceUpdate] (const brogueUIProgramPartId& partId, brogueViewPolygonCore* view)
		{
			view->update(millisecondsLapsed, forceUpdate);
			return iterationCallback::iterate;
		});
	}

#pragma endregion
}
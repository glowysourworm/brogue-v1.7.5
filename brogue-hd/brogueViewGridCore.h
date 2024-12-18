#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueColorQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueImageQuad.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIMouseData.h"
#include "brogueUIProgramPartConfiguration.h"
#include "brogueUIProgramPartId.h"
#include "brogueUITagAction.h"
#include "brogueViewCore.h"
#include "eventController.h"
#include "grid.h"
#include "gridDefinitions.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleGlData.h"
#include "simpleOpenGL.h"
#include <concepts>

namespace brogueHd::frontend
{
	using namespace brogueHd::backend::model;
	using namespace brogueHd::backend;

	/// <summary>
	/// The view core's responsibility is to contain the simpleShaderProgram; and handle translations
	/// back-and-forth from UI coordinates -> GL backend coordinates + stream maintenance.
	/// </summary>	
	template <isGLStream TStream>
	class brogueViewGridCore : public brogueViewCore<TStream>
	{
	public:
		brogueViewGridCore(brogueCoordinateConverter* coordinateConverter,
		                   resourceController* resourceController,
		                   eventController* eventController,
		                   const brogueUIProgramPartId& partId,
		                   const brogueUIData& data,
		                   bool hasMouseInteraction);
		~brogueViewGridCore();

		/// <summary>
		/// Sets the invalidate flag (see other overloads for UI interaction)
		/// </summary>
		void invalidate();

		/// <summary>
		/// Sets the program core active (calls GL bind)
		/// </summary>
		void activate();

		/// <summary>
		/// Sets the program inactive (calls GL bind = 0)
		/// </summary>
		void deactivate();

		/// <summary>
		/// Runs the primary brogueViewCore glDraw method
		/// </summary>
		void run();

		/// <summary>
		/// Returns true if the shader program has the specified uniform
		/// </summary>
		bool hasUniform(const char* name);

		/// <summary>
		/// Sets uniform value for the brogueViewCore shader program
		/// </summary>
		template <isGLUniform TUniform>
		void setUniform(const char* name, const TUniform& value);

		/// <summary>
		/// Returns true if the GL backend for this program has errors
		/// </summary>
		bool hasErrors();

		/// <summary>
		/// Outputs the errors for this program to std::cout
		/// </summary>
		void showErrors();

	private: // GL Stream Functions

		/// <summary>
		/// Function used to copy over view's data elements when it is invalid
		/// </summary>
		void setDataStreamElements();

		brogueImageQuad createBrogueImageQuad(const brogueCellDisplay& cell,
		                                      openglBrogueCellOutputSelector outputSelector);
		brogueCellQuad createBrogueCellQuad(const brogueCellDisplay& cell,
		                                    openglBrogueCellOutputSelector outputSelector);
		brogueColorQuad createBrogueColorQuad(const brogueCellDisplay& cell,
		                                      openglBrogueCellOutputSelector outputSelector);

	public: // UI Functions

		/// <summary>
		/// Gets the specified view cell's data
		/// </summary>
		brogueCellDisplay get(int column, int row) const;

		/// <summary>
		/// Sets the view cell's data -> invalidates the view.
		/// </summary>
		void set(const brogueCellDisplay& cell);

		/// <summary>
		/// Iterates the view's cells. This will happen in the view's relative boundary.
		/// </summary>
		void iterate(gridCallbackConst<brogueCellDisplay> callback) const;

		/// <summary>
		/// Iterates the view's cells, inside the view's relative boundary, starting
		/// from the start location -> end location. This will mirror the way the data
		/// stream for openGL is built. So, it would be useful to use for stream operations.
		/// </summary>
		void iterateFrom(const gridLocator& start, const gridLocator& end, gridCallbackConst<brogueCellDisplay> callback) const;

		/// <summary>
		/// Starts animation sequences for the UI. Normal reports are drawn from checkUpdate; but the
		/// additional report checkStateChange must be handled. The updating of data for the primary
		/// rendering is still done during the checkUpdate function - while these animations are playing.
		/// </summary>
		virtual void initiateStateChange(brogueUIState fromState, brogueUIState toState);

		/// <summary>
		/// Clears state change (data). This is run once after all state changes have completed.
		/// </summary>
		virtual void clearStateChange();

		/// <summary>
		/// Requests a UI update for state change animations. Return true to signal that there are still
		/// active animations in progress.
		/// </summary>
		virtual bool checkStateChange();

		/// <summary>
		/// Overload of the checkUpdate function behaves as though the view is a child of a parent view
		/// </summary>
		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
		                         const brogueMouseState& mouseState,
		                         int millisecondsLapsed);

		/// <summary>
		/// Invalidate function that is used to check view re-buffer conditions
		/// </summary>
		virtual void invalidate(const brogueKeyboardState& keyboardState,
		                        const brogueMouseState& mouseState);

		/// <summary>
		/// Updates the view's grid cells when there has been a change to the view
		/// </summary>
		virtual void update(int millisecondsLapsed,
		                    bool forceUpdate);

		/// <summary>
		/// Clears update flags from the UI tree
		/// </summary>
		virtual void clearUpdate();

		/// <summary>
		/// Clears mouse interaction events from the UI tree
		/// </summary>
		virtual void clearEvents();

		/// <summary>
		/// Checks the tree to see if there are views that need updating.
		/// </summary>
		virtual bool needsUpdate() const;

	public: // UI Data Functions

		brogueUIProgramPartId getPartId() const;

		bool getHasMouseInteraction() const;
		bool getMouseOver() const;
		bool getMouseEnter() const;
		bool getMouseLeave() const;
		bool getMousePressed() const;
		bool getMousePressedChanged() const;
		int getZoomLevel() const;
		int getZIndex() const;

	public: // UI Setters

		void setUIAction(const brogueUITagAction& action);

	protected:
		brogueUIData* getUIData() const;
		void raiseClickEvent(const brogueUITagAction& response);
		void raiseHoverEvent(const brogueUITagAction& response);
		virtual void setUpdate(bool mousePressed, bool mouseOver);

	private:
		bool _hasMouseInteraction;

	private:
		brogueCoordinateConverter* _coordinateConverter;
		brogueUIProgramPartConfiguration* _configuration;
		brogueUIProgramPartId* _partId;
		grid<brogueCellDisplay*>* _view;
		brogueUITagAction* _tagAction;
		brogueUIData* _uiData;
		brogueUIMouseData* _mouseData;

		bool _invalid;

	private:
		eventController* _eventController;
	};

	template <isGLStream TStream>
	brogueViewGridCore<TStream>::brogueViewGridCore(brogueCoordinateConverter* coordinateConverter,
	                                                resourceController* resourceController,
	                                                eventController* eventController,
	                                                const brogueUIProgramPartId& partId,
	                                                const brogueUIData& data,
	                                                bool hasMouseInteraction)
		: brogueViewCore<TStream>(resourceController, partId, data.getParentBoundary(), data.getBoundary())
	{
		_eventController = eventController;
		_coordinateConverter = coordinateConverter;
		_view = new grid<brogueCellDisplay*>(data.getParentBoundary(), data.getBoundary());
		_configuration = new brogueUIProgramPartConfiguration(
			*resourceController->getUIPartConfig(partId.getPartName()));
		_partId = new brogueUIProgramPartId(partId);
		_uiData = new brogueUIData(data);
		_mouseData = new brogueUIMouseData();
		_tagAction = new brogueUITagAction();
		_hasMouseInteraction = hasMouseInteraction;
		_invalid = false;
	}

	template <isGLStream TStream>
	brogueViewGridCore<TStream>::~brogueViewGridCore()
	{
		grid<brogueCellDisplay*>* grid = _view;

		_view->getRelativeBoundary().iterate([&grid](int column, int row)
		{
			if (grid->isDefined(column, row))
				delete grid->get(column, row);

			return iterationCallback::iterate;
		});

		delete _view;
		delete _uiData;
		delete _mouseData;
		delete _tagAction;
		delete _partId;
		delete _configuration;
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::setDataStreamElements()
	{
		// Procedure: Check the stream's size before setting elements into their place. This requires
		//			  an extra iteration of the stream.
		// 

		int elementSize = 0;

		// Must iterate to get the size of the data stream
		_view->iterateWhereDefined([&elementSize](int column, int row, brogueCellDisplay* cell)
		{
			elementSize++;

			return iterationCallback::iterate;
		});

		// Resize Data Stream
		if (elementSize != brogueViewCore<TStream>::getElementCount())
			brogueViewCore<TStream>::resizeElements(elementSize);

		brogueViewGridCore<TStream>* that = this;
		openglBrogueCellOutputSelector noDisplaySelector = _configuration->noDisplaySelector;
		const brogueUIProgramPartConfiguration* configuration = _configuration;

		// Marks where we are in the output stream
		int cursor = 0;

		// Now, we can send elements to the stream's buffer; and re-buffer the view's output stream to the GPU
		//
		_view->iterateWhereDefined(
			[&that, &noDisplaySelector, &cursor, &configuration](int column, int row, brogueCellDisplay* cell)
			{
				// Stream out elements as the iterator specifies -> ordered onto the stream.
				if constexpr (std::same_as<TStream, brogueImageQuad>)
				{
					brogueImageQuad quad = that->createBrogueImageQuad(*cell, noDisplaySelector);
					that->setElement(quad, cursor++);
				}
				else if constexpr (std::same_as<TStream, brogueCellQuad>)
				{
					brogueCellQuad quad = that->createBrogueCellQuad(*cell, noDisplaySelector);
					that->setElement(quad, cursor++);
				}
				else if constexpr (std::same_as<TStream, brogueColorQuad>)
				{
					brogueColorQuad quad = that->createBrogueColorQuad(*cell, noDisplaySelector);
					that->setElement(quad, cursor++);
				}
				else
					throw simpleException("Unhandled openglDataStreamType:  brogueViewGridCore.h");

				return iterationCallback::iterate;
			});

		_invalid = false;
	}

	template <isGLStream TStream>
	brogueCellDisplay brogueViewGridCore<TStream>::get(int column, int row) const
	{
		return *(_view->get(column, row));
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::set(const brogueCellDisplay& cell)
	{
		if (!_view->isInBounds(cell.column, cell.row))
			throw simpleException("Grid out of bounds:  brogueViewGridCore::set");

		if (_view->isDefined(cell.column, cell.row))
		{
			// Run update function for the cell display
			_view->get(cell.column, cell.row)->setUI(cell);
		}
		else
		{
			// Copy over the data into a new cell
			_view->set(cell.column, cell.row, new brogueCellDisplay(cell), true);
		}

		// Update the view's stream
		_invalid = true;
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::hasUniform(const char* name)
	{
		return brogueViewCore<TStream>::glHasUniform(name);
	}

	template <isGLStream TStream>
	template <isGLUniform TUniform>
	void brogueViewGridCore<TStream>::setUniform(const char* name, const TUniform& value)
	{
		brogueViewCore<TStream>::setUniform(name, value);
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::hasErrors()
	{
		return brogueViewCore<TStream>::glHasErrors();
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::showErrors()
	{
		brogueViewCore<TStream>::glShowErrors();
	}

	template <isGLStream TStream>
	brogueUIProgramPartId brogueViewGridCore<TStream>::getPartId() const
	{
		return *_partId;
	}

	template <isGLStream TStream>
	brogueUIData* brogueViewGridCore<TStream>::getUIData() const
	{
		return _uiData;
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::getHasMouseInteraction() const
	{
		return _hasMouseInteraction;
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::getMouseOver() const
	{
		return _mouseData->getMouseOver();
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::getMouseEnter() const
	{
		return _mouseData->getMouseEnter();
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::getMouseLeave() const
	{
		return _mouseData->getMouseLeave();
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::getMousePressed() const
	{
		return _mouseData->getMouseDown(); // Check that capture works like it should
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::getMousePressedChanged() const
	{
		return _mouseData->getMousePressedChanged();
	}

	template <isGLStream TStream>
	int brogueViewGridCore<TStream>::getZoomLevel() const
	{
		return _uiData->getZoomLevel();
	}

	template <isGLStream TStream>
	int brogueViewGridCore<TStream>::getZIndex() const
	{
		return _uiData->getZIndex();
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::setUIAction(const brogueUITagAction& action)
	{
		_tagAction->set(action);

		// Turn on mouse interaction
		_hasMouseInteraction = true;
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::raiseClickEvent(const brogueUITagAction& response)
	{
		_eventController->getUIClickEvent()->publish(_partId->getName(), response);
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::raiseHoverEvent(const brogueUITagAction& response)
	{
		_eventController->getUIHoverEvent()->publish(_partId->getName(), response);
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::setUpdate(bool mousePressed, bool mouseOver)
	{
		_mouseData->setUpdate(mousePressed, mouseOver);
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
		// Must inherit this function
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::clearStateChange()
	{
		// Nothing to do
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::checkStateChange()
	{
		// Finished with changes (nothing to do)
		return false;
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::clearUpdate()
	{
		_invalid = false;
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::clearEvents()
	{
		_mouseData->clear();
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::invalidate(const brogueKeyboardState& keyboardState,
	                                             const brogueMouseState& mouseState)
	{
		_invalid = true;
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::invalidate()
	{
		_invalid = true;
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::run()
	{
		brogueViewCore<TStream>::glDraw();
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::activate()
	{
		brogueViewCore<TStream>::glActivate();
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::deactivate()
	{
		brogueViewCore<TStream>::glDeactivate();
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::checkUpdate(const brogueKeyboardState& keyboardState,
	                                              const brogueMouseState& mouseState,
	                                              int millisecondsLapsed)
	{
		// Check for mouse over (calculate for THIS render-cycle)
		bool isMouseOver = _uiData->getBoundary().contains(mouseState.getLocation());

		// Sets primary real time UI data for the mouse / live updates to the UI.
		_mouseData->setUpdate(mouseState.getMouseLeft(), isMouseOver);

		if (_hasMouseInteraction)
		{
			if (_mouseData->getMouseOver() && _mouseData->getMouseClick())
			{
				// UI EVENT:  Mouse Click
				this->raiseClickEvent(*_tagAction);
			}
			else if (_mouseData->getMouseOver())
			{
				this->raiseHoverEvent(*_tagAction);
			}
		}
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::update(int millisecondsLapsed,
	                                         bool forceUpdate)
	{
		// Invalid:  Re-Buffer the output stream
		//
		if (_invalid || forceUpdate)
		{
			// Set elements from the view into the element array
			setDataStreamElements();
		}
	}

	template <isGLStream TStream>
	bool brogueViewGridCore<TStream>::needsUpdate() const
	{
		// For more granularity, there needs to be overrides in the child class. Also, 
		// call this base class function so that this invalid flag is used.
		return _invalid;
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::iterate(gridCallbackConst<brogueCellDisplay> callback) const
	{
		_view->iterateWhereDefined(callback);
	}

	template <isGLStream TStream>
	void brogueViewGridCore<TStream>::iterateFrom(const gridLocator& start, const gridLocator& end, gridCallbackConst<brogueCellDisplay> callback) const
	{
		_view->iterateFrom(start, end, callback);
	}

	template <isGLStream TStream>
	brogueImageQuad brogueViewGridCore<TStream>::createBrogueImageQuad(const brogueCellDisplay& cell,
	                                                                   openglBrogueCellOutputSelector outputSelector)
	{
		simpleQuad quadXY = _coordinateConverter->getViewConverter()->createQuadNormalizedXY_FromLocator(cell.column, cell.row);
		simpleQuad quadUV = _coordinateConverter->getViewConverter()->createQuadNormalizedUV_FromLocator(cell.column, cell.row);

		return brogueImageQuad(cell, quadXY, quadUV);
	}

	template <isGLStream TStream>
	brogueCellQuad brogueViewGridCore<TStream>::createBrogueCellQuad(const brogueCellDisplay& cell,
	                                                                 openglBrogueCellOutputSelector outputSelector)
	{
		gridLocator glyphLocation = _coordinateConverter->getGlyphMap()->getGlyphLocation(cell.character);

		simpleQuad quadXY = _coordinateConverter->getViewConverter()->createQuadNormalizedXY_FromLocator(cell.column, cell.row);
		simpleQuad quadUV = _coordinateConverter->getViewConverter()->createQuadNormalizedUV_FromLocator(cell.column, cell.row);
		simpleQuad glyphUV = _coordinateConverter->getGlyphConverter()->createQuadNormalizedUV_FromLocator(glyphLocation);

		return brogueCellQuad(cell, quadXY, quadUV, glyphUV, outputSelector);
	}

	template <isGLStream TStream>
	brogueColorQuad brogueViewGridCore<TStream>::createBrogueColorQuad(const brogueCellDisplay& cell,
	                                                                   openglBrogueCellOutputSelector outputSelector)
	{
		simpleQuad quadXY = _coordinateConverter->getViewConverter()->createQuadNormalizedXY_FromLocator(cell.column, cell.row);

		return brogueColorQuad(cell, quadXY);
	}
}

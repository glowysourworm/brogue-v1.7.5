#pragma once

#include "brogueCoordinateConverter.h"
#include "brogueKeyboardState.h"
#include "brogueLine.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewCore.h"
#include "eventController.h"
#include "resourceController.h"
#include "simpleGlData.h"
#include "simpleLine.h"
#include "simpleList.h"
#include "simpleOpenGL.h"
#include "simplePoint.h"

namespace brogueHd::frontend
{
	using namespace simple;
	using namespace brogueHd::component;

	class brogueViewPolygonCore : public brogueViewCore<brogueLine>
	{
	public:
		brogueViewPolygonCore(brogueCoordinateConverter* coordinateConverter,
		                      eventController* eventController,
		                      resourceController* resourceController,
		                      const brogueUIProgramPartId& partId,
		                      const brogueUIData& uiData);
		~brogueViewPolygonCore();

		/// <summary>
		/// Sets the program core active (calls GL bind)
		/// </summary>
		void activate();

		/// <summary>
		/// Sets the program inactive (calls GL bind = 0)
		/// </summary>
		void deactivate();

		/// <summary>
		/// Calls the primary draw method (glDraw in brogueViewCore)
		/// </summary>
		void run();

		/// <summary>
		/// Returns true if the GL backend for this program has errors
		/// </summary>
		bool hasErrors();

		/// <summary>
		/// Outputs the errors for this program to std::cout
		/// </summary>
		void showErrors();

		/// <summary>
		/// Returns true if the shader program has the specified uniform
		/// </summary>
		bool hasUniform(const char* name);

		/// <summary>
		/// Sets uniform value for the brogueViewCore shader program
		/// </summary>
		template <isGLUniform TUniform>
		void setUniform(const char* name, const TUniform& value);

	public:
		/// <summary>
		/// Adds an edge to the polygon; but will throw an exception if it cannot be placed in a graph structure (with adjacency)
		/// </summary>
		void addLine(const simpleLine<int>& line);

		/// <summary>
		/// Returns true if the polygon's graph contains the specified edge
		/// </summary>
		bool containsLine(const simpleLine<int>& line);

		virtual void initiateStateChange(brogueUIState fromState, brogueUIState toState);
		virtual void clearStateChange();
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

	private:
		/// <summary>
		/// Sets data stream elements in the core; and resets the invalid flag.
		/// </summary>
		void setDataStreamElements();

	protected:
		brogueUIData* getUIData() const;

	private:
		brogueCoordinateConverter* _coordinateConverter;
		eventController* _eventController;
		brogueUIData* _uiData;

		simpleList<simpleLine<int>>* _graphUI;

		bool _invalid;
	};

	brogueViewPolygonCore::brogueViewPolygonCore(brogueCoordinateConverter* coordinateConverter,
	                                             eventController* eventController,
	                                             resourceController* resourceController,
	                                             const brogueUIProgramPartId& partId,
	                                             const brogueUIData& uiData)
		: brogueViewCore(resourceController, partId, uiData.getParentBoundary(), uiData.getBoundary())
	{
		_coordinateConverter = coordinateConverter;
		_eventController = eventController;
		_uiData = new brogueUIData(uiData);
		_graphUI = new simpleList<simpleLine<int>>();
		_invalid = true;
	}

	brogueViewPolygonCore::~brogueViewPolygonCore()
	{
		delete _uiData;
		delete _graphUI;
	}

	brogueUIData* brogueViewPolygonCore::getUIData() const
	{
		return _uiData;
	}

	void brogueViewPolygonCore::setDataStreamElements()
	{
		// Two point outputs per line
		int elementSize = _graphUI->count() * 2;

		if (elementSize != brogueViewCore<brogueLine>::getElementCount())
			brogueViewCore<brogueLine>::resizeElements(elementSize);

		int cursor = 0;

		// Now, we can send elements to the stream's buffer; and call createStream / reStream
		for (int index = 0; index < _graphUI->count(); index++)
		{
			simpleLine<int> line = _graphUI->get(index);
			vec2 point1 = _coordinateConverter->getViewConverter()->convertToNormalizedXY(line.node1.x, line.node1.y);
			vec2 point2 = _coordinateConverter->getViewConverter()->convertToNormalizedXY(line.node2.x, line.node2.y);

			brogueLine streamElement1(vec3(point1.x, point1.y, 1), vec4(1, 1, 1, 1));
			brogueLine streamElement2(vec3(point2.x, point2.y, 1), vec4(1, 1, 1, 1));

			// Stream out elements as the iterator specifies -> ordered onto the stream.
			this->setElement(streamElement1, cursor++);
			this->setElement(streamElement2, cursor++);
		}

		_invalid = false;
	}

	void brogueViewPolygonCore::activate()
	{
		brogueViewCore<brogueLine>::glActivate();
	}

	void brogueViewPolygonCore::deactivate()
	{
		brogueViewCore<brogueLine>::glDeactivate();
	}

	bool brogueViewPolygonCore::hasErrors()
	{
		return brogueViewCore<brogueLine>::glHasErrors();
	}

	void brogueViewPolygonCore::showErrors()
	{
		brogueViewCore<brogueLine>::glShowErrors();
	}

	bool brogueViewPolygonCore::hasUniform(const char* name)
	{
		return brogueViewCore<brogueLine>::glHasUniform(name);
	}

	template <isGLUniform TUniform>
	void brogueViewPolygonCore::setUniform(const char* name, const TUniform& value)
	{
		brogueViewCore<brogueLine>::setUniform(name, value);
	}

	void brogueViewPolygonCore::run()
	{
		brogueViewCore<brogueLine>::glDraw();
	}

	void brogueViewPolygonCore::addLine(const simpleLine<int>& line)
	{
		if (!_graphUI->contains(line))
			_graphUI->add(line);

		// Update the view's stream
		_invalid = true;
	}

	bool brogueViewPolygonCore::containsLine(const simpleLine<int>& line)
	{
		return _graphUI->contains(line);
	}

	void brogueViewPolygonCore::initiateStateChange(brogueUIState fromState, brogueUIState toState)
	{
	}

	void brogueViewPolygonCore::clearStateChange()
	{
	}

	bool brogueViewPolygonCore::checkStateChange()
	{
		return false;
	}

	void brogueViewPolygonCore::checkUpdate(const brogueKeyboardState& keyboardState,
	                                        const brogueMouseState& mouseState,
	                                        int millisecondsLapsed)
	{
		// TODO
	}

	void brogueViewPolygonCore::invalidate(const brogueKeyboardState& keyboardState,
	                                       const brogueMouseState& mouseState)
	{
		_invalid = true;
	}

	void brogueViewPolygonCore::update(int millisecondsLapsed,
	                                   bool forceUpdate)
	{
		if (_invalid)
		{
			setDataStreamElements();
		}
	}

	void brogueViewPolygonCore::clearUpdate()
	{
		_invalid = false;
	}

	void brogueViewPolygonCore::clearEvents()
	{
		// TODO
	}

	bool brogueViewPolygonCore::needsUpdate() const
	{
		return _invalid;
	}
}

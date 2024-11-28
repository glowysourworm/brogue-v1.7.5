#pragma once

#include "brogueCellQuad.h"
#include "brogueUIData.h"
#include "brogueViewGridCore.h"
#include "gridRect.h"

#include "brogueCellDisplay.h"
#include "brogueCoordinateConverter.h"
#include "brogueUIProgramPartId.h"
#include "eventController.h"
#include "resourceController.h"
#include "simple.h"

using namespace brogueHd::backend;
using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueMenuBackground : public brogueViewGridCore<brogueCellQuad>
	{
	public:

		brogueMenuBackground(brogueCoordinateConverter* coordinateConverter,
						 resourceController* resourceController,
						 eventController* eventController,
						 const brogueUIProgramPartId& partId,
						 const brogueUIData& data);
		~brogueMenuBackground();

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;

	private:

		void updateImpl(int millisecondsLapsed, bool forceUpdate);
	};

	brogueMenuBackground::brogueMenuBackground(brogueCoordinateConverter* coordinateConverter,
										 resourceController* resourceController,
										 eventController* eventController,
										 const brogueUIProgramPartId& partId,
										 const brogueUIData& data)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data)
	{
	}

	brogueMenuBackground::~brogueMenuBackground()
	{
	}

	void brogueMenuBackground::updateImpl(int millisecondsLapsed, bool forceUpdate)
	{
		brogueMenuBackground* that = this;
		brogueUIData* uiData = this->getUIData();

		// Initialize to use the background color
		uiData->getBoundary().iterate([&that, &uiData] (int column, int row)
		{
			brogueCellDisplay cell(column, row);

			// Calculate background (TODO: Bring in the gradient code, somewhere..)
			cell.backColor = uiData->calculateGradient(column, row, false, false, false);

			that->set(cell);	// -> Puts this into the data stream

			return iterationCallback::iterate;
		});
	}

	void brogueMenuBackground::update(int millisecondsLapsed, bool forceUpdate)
	{
		// Set the data elements
		updateImpl(millisecondsLapsed, forceUpdate);

		// Call the base class -> restream the data
		brogueViewGridCore<brogueCellQuad>::update(millisecondsLapsed, forceUpdate);
	}
}


#pragma once

#include "brogueColorQuad.h"
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
	class brogueBackground : public brogueViewGridCore<brogueColorQuad>
	{
	public:

		brogueBackground(brogueCoordinateConverter* coordinateConverter,
						 resourceController* resourceController,
						 eventController* eventController,
						 const brogueUIProgramPartId& partId,
						 const brogueUIData& data);
		~brogueBackground();

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;
	};

	brogueBackground::brogueBackground(brogueCoordinateConverter* coordinateConverter,
										 resourceController* resourceController,
										 eventController* eventController,
										 const brogueUIProgramPartId& partId,
										 const brogueUIData& data)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data)
	{
		brogueBackground* that = this;

		// Initialize to use the background color
		data.getBoundary().iterate([&that, &data] (int column, int row)
		{
			brogueCellDisplay cell(column, row);

			// Calculate background (TODO: Bring in the gradient code, somewhere..)
			cell.backColor = data.calculateGradient(column, row, false, false, false);

			that->set(cell);	// -> Puts this into the data stream

			return iterationCallback::iterate;
		});

		// Call initializeCore() -> sets up stream for  GL backend
		brogueViewGridCore::initializeCore();
	}

	brogueBackground::~brogueBackground()
	{
	}
	void brogueBackground::update(int millisecondsLapsed, bool forceUpdate)
	{
		// Nothing to do until we add mouse interaction
	}
}


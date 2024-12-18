#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueUITagAction.h"
#include "brogueViewGridCore.h"
#include "eventController.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"

namespace brogueHd::frontend
{
	using namespace simple;

	class brogueGameView : public brogueViewGridCore<brogueCellQuad>
	{
	public:
		brogueGameView(brogueCoordinateConverter* coordinateConverter,
		               resourceController* resourceController,
		               eventController* eventController,
		               const brogueUIProgramPartId& partId,
		               const brogueUIData& data);

		~brogueGameView()
		{
		};

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;
	};

	brogueGameView::brogueGameView(brogueCoordinateConverter* coordinateConverter,
	                               resourceController* resourceController,
	                               eventController* eventController,
	                               const brogueUIProgramPartId& partId,
	                               const brogueUIData& data)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data, true)
	{
		this->setUIAction(brogueUITagAction(brogueUIState::GameNormal));
	}


	void brogueGameView::update(int millisecondsLapsed, bool forceUpdate)
	{
		// Call the base class -> restream the data
		brogueViewGridCore<brogueCellQuad>::update(millisecondsLapsed, forceUpdate);
	}
}

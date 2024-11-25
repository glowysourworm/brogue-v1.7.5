#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueUIColorText.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewGridCore.h"
#include "eventController.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueScrollView : public brogueViewGridCore<brogueCellQuad>
	{
	public:

		brogueScrollView(brogueCoordinateConverter* coordinateConverter,
						 resourceController* resourceController,
						 eventController* eventController,
						 const brogueUIProgramPartId& partId,
						 const brogueUIData& data);
		~brogueScrollView();

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;

		void add(brogueUIColorText* item);
		void addRange(const simpleList<brogueUIColorText*>& items);

	protected:

		simpleList<brogueUIColorText*>* _list;
	};

	brogueScrollView::brogueScrollView(brogueCoordinateConverter* coordinateConverter,
									   resourceController* resourceController,
									   eventController* eventController,
									   const brogueUIProgramPartId& partId,
									   const brogueUIData& data)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data)
	{
		_list = new simpleList<brogueUIColorText*>();

		update(0, true);
	}

	brogueScrollView::~brogueScrollView()
	{
		delete _list;
	}

	void brogueScrollView::add(brogueUIColorText* item)
	{
		_list->add(item);

		brogueViewGridCore::invalidate();
	}
	void brogueScrollView::addRange(const simpleList<brogueUIColorText*>& items)
	{
		_list->addRange(items);

		brogueViewGridCore::invalidate();
	}

	void brogueScrollView::update(int millisecondsLapsed, bool forceUpdate)
	{
		brogueScrollView* that = this;
		gridRect boundary = this->getUIData()->getBoundary();

		// Initialize to use the background color
		boundary.iterate([&that] (int column, int row)
		{
			brogueCellDisplay cell(column, row);

			cell.backColor = that->getUIData()->calculateGradient(column, row, false, false, false);

			that->set(cell);

			return iterationCallback::iterate;
		});
	}
}
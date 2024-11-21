#pragma once

#include "brogueCellDisplay.h"
#include "brogueUIColorText.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
#include "eventController.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueScrollView : public brogueViewBase
	{
	public:

		brogueScrollView(eventController* eventController, const brogueUIProgramPartId& partId, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueScrollView();

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;
		virtual bool needsUpdate() const override;

		void add(brogueUIColorText* item);
		void addRange(const simpleList<brogueUIColorText*>& items);

	protected:

		simpleList<brogueUIColorText*>* _list;
	};

	brogueScrollView::brogueScrollView(eventController* eventController, const brogueUIProgramPartId& partId, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, partId, data, sceneBoundary, viewBoundary)
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
	}
	void brogueScrollView::addRange(const simpleList<brogueUIColorText*>& items)
	{
		_list->addRange(items);
	}

	void brogueScrollView::update(int millisecondsLapsed, bool forceUpdate)
	{
		brogueScrollView* that = this;

		// Initialize to use the background color
		brogueViewBase::iterate([&that] (short column, short row, brogueCellDisplay* cell)
		{
			cell->backColor = that->getBackgroundColor(column, row);
			return iterationCallback::iterate;
		});
	}
	bool brogueScrollView::needsUpdate() const
	{
		return false;
	}
}
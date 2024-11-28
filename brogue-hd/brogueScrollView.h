#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIColorText.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewGridCore.h"
#include "brogueViewProgram.h"
#include "color.h"
#include "eventController.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleException.h"
#include "simpleKeyboardState.h"
#include "simpleList.h"
#include "simpleMouseState.h"

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

		virtual void checkUpdate(const brogueKeyboardState& keyboardState,
								 const brogueMouseState& mouseState,
								 int millisecondsLapsed) override;

		void add(brogueUIColorText* item);
		void addRange(const simpleList<brogueUIColorText*>& items);

	private:

		void updateImpl(int millisecondsLapsed, bool forceUpdate);

	protected:

		simpleList<brogueUIColorText*>* _list;
		int _cursor;								// Zero-th item at the top of the list
	};

	brogueScrollView::brogueScrollView(brogueCoordinateConverter* coordinateConverter,
									   resourceController* resourceController,
									   eventController* eventController,
									   const brogueUIProgramPartId& partId,
									   const brogueUIData& data)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data, true)
	{
		_list = new simpleList<brogueUIColorText*>();
		_cursor = 0;
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

	void brogueScrollView::checkUpdate(const brogueKeyboardState& keyboardState,
									   const brogueMouseState& mouseState,
										  int millisecondsLapsed)
	{
		// Update mouse data
		brogueViewGridCore<brogueCellQuad>::checkUpdate(keyboardState, mouseState, millisecondsLapsed);

		int cursorBefore = _cursor;

		// Scroll behavior
		if (this->getHasMouseInteraction() && this->getMouseOver() && mouseState.getScrollPendingY())
		{
			if (mouseState.getScrollPendingY())
			{
				// Down
				if (!mouseState.getScrollNegativeY())
				{
					// Current offset from the initial position
					if (_cursor > 0)
						_cursor--;
				}

				// Up
				else if (mouseState.getScrollNegativeY())
				{
					if (_list->count() - _cursor > this->getBoundary().height)
						_cursor++;
				}
			}
		}

		// Invalidate the data -> update()
		if (cursorBefore != _cursor)
			brogueViewGridCore<brogueCellQuad>::invalidate();
	}

	void brogueScrollView::updateImpl(int millisecondsLapsed, bool forceUpdate)
	{
		brogueScrollView* that = this;
		gridRect boundary = this->getUIData()->getBoundary();
		simpleList<brogueUIColorText*>* textItems = _list;
		int cursor = _cursor;

		// Initialize to use the background color
		boundary.iterate([&that, &textItems, &boundary, &cursor] (int column, int row)
		{
			brogueCellDisplay cell(column, row);

			int listIndex = row - boundary.top() + cursor;
			int textIndex = column - boundary.left();

			// List Item
			if (listIndex < textItems->count())
			{
				// List Item Character
				if (textIndex < textItems->get(listIndex)->getText().getCount())
				{
					cell.foreColor = textItems->get(listIndex)->getText().getColor(textIndex);
					cell.character = textItems->get(listIndex)->getText().getChar(textIndex);
				}					
			}

			// Background
			cell.backColor = that->getUIData()->calculateGradient(column, row, false, false, false);

			// Set into the data stream
			that->set(cell);

			return iterationCallback::iterate;
		});
	}

	void brogueScrollView::update(int millisecondsLapsed, bool forceUpdate)
	{
		// Update data elements in the view
		updateImpl(millisecondsLapsed, forceUpdate);

		// Call the base class -> restream the data
		brogueViewGridCore<brogueCellQuad>::update(millisecondsLapsed, forceUpdate);
	}
}
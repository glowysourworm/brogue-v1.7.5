#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueTitleGrid.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
#include "color.h"
#include "eventController.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleList.h"

using namespace brogueHd::backend;

namespace brogueHd::frontend
{
	class brogueFlameMenuTitleMask : public brogueViewBase
	{
	public:

		brogueFlameMenuTitleMask(eventController* eventController, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
			: brogueViewBase(eventController, data, sceneBoundary, viewBoundary)
		{
			_titleGrid = new brogueTitleGrid();
			_maskCell = new brogueCellDisplay();
			_defaultCell = new brogueCellDisplay();

			_maskCell->backColor = colors::black();
			_maskCell->noDisplay = false;

			_defaultCell->backColor = colors::transparent();
			_defaultCell->noDisplay = true;
		}
		~brogueFlameMenuTitleMask()
		{
			delete _titleGrid;
			delete _maskCell;
			delete _defaultCell;
		}

		void checkUpdate(const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed) override
		{
			// Prevent updating
		}

		bool needsUpdate() const override
		{
			return false;
		}
		gridRect getBoundary() const override
		{
			return _titleGrid->sceneBounds();
		}
		gridRect getSceneBoundary() const override
		{
			return _titleGrid->sceneBounds();
		}
		bool isTheText(short column, short row)
		{
			return _titleGrid->isTheText(column, row);
		}
		void iterate(gridCallback<brogueCellDisplay*> callback) const override
		{
			brogueTitleGrid* titleGrid = _titleGrid;
			brogueCellDisplay* maskCell = _maskCell;

			_titleGrid->sceneBounds().iterate([&callback, &titleGrid, &maskCell] (short column, short row)
			{
				if (titleGrid->isTheText(column, row))
					callback(column, row, maskCell);

				return iterationCallback::iterate;
			});
		}

	private:

		brogueTitleGrid* _titleGrid;
		brogueCellDisplay* _maskCell;
		brogueCellDisplay* _defaultCell;
	};
}
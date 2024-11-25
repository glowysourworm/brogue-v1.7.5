#pragma once

#include "brogueCellDisplay.h"
#include "brogueColorQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueTitleGrid.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewGridCore.h"
#include "color.h"
#include "eventController.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"

using namespace brogueHd::backend;

namespace brogueHd::frontend
{
	class brogueFlameMenuTitleMask : public brogueViewGridCore<brogueColorQuad>
	{
	public:

		brogueFlameMenuTitleMask(brogueCoordinateConverter* coordinateConverter,
								 resourceController* resourceController,
								 eventController* eventController,
								 const brogueUIProgramPartId& partId,
								 const brogueUIData& data)
			: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data)
		{
			_titleGrid = new brogueTitleGrid();
			_maskCell = new brogueCellDisplay();
			_defaultCell = new brogueCellDisplay();

			_maskCell->backColor = colors::black();
			_maskCell->noDisplay = false;

			_defaultCell->backColor = colors::transparent();
			_defaultCell->noDisplay = true;

			// Call initializeCore() -> sets up stream for  GL backend
			brogueViewGridCore::initializeCore();
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

		void update(int millisecondsLapsed, bool forceUpdate) override
		{
			brogueFlameMenuTitleMask* that = this;
			brogueTitleGrid* titleGrid = _titleGrid;
			brogueCellDisplay* maskCell = _maskCell;

			_titleGrid->sceneBounds().iterate([&that, &titleGrid, &maskCell] (int column, int row)
			{
				if (titleGrid->isTheText(column, row))
					that->set(*maskCell);

				return iterationCallback::iterate;
			});
		}

		bool needsUpdate() const override
		{
			return false;
		}
		bool isTheText(int column, int row)
		{
			return _titleGrid->isTheText(column, row);
		}

	private:

		brogueTitleGrid* _titleGrid;
		brogueCellDisplay* _maskCell;
		brogueCellDisplay* _defaultCell;

		bool _flameMenu2;
	};
}
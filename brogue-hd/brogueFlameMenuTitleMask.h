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

			// Set initial stream data
			update(0, true);
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
			// Update data elements in the view
			updateImpl(millisecondsLapsed, forceUpdate);

			// Call the base class -> restream the data
			brogueViewGridCore<brogueColorQuad>::update(millisecondsLapsed, forceUpdate);
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

		void updateImpl(int millisecondsLapsed, int forceUpdate)
		{
			brogueFlameMenuTitleMask* that = this;
			brogueTitleGrid* titleGrid = _titleGrid;
			brogueCellDisplay* maskCell = _maskCell;

			_titleGrid->sceneBounds().iterate([&that, &titleGrid, &maskCell] (int column, int row)
			{
				brogueCellDisplay cell(column, row);

				cell.setUI(*maskCell);

				if (titleGrid->isTheText(column, row))
					that->set(cell);

				return iterationCallback::iterate;
			});
		}

	private:

		brogueTitleGrid* _titleGrid;
		brogueCellDisplay* _maskCell;
		brogueCellDisplay* _defaultCell;

		bool _flameMenu2;
	};
}
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
#include "gridDefinitions.h"
#include "gridRect.h"
#include "simple.h"

namespace brogueHd::frontend
{
	class brogueFlameMenuTitleMask : public brogueViewBase
	{
	public:

		brogueFlameMenuTitleMask(int zoomLevel) : brogueViewBase()
		{
			brogueUIProgramPartId partId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameMenuProgram_TitleMaskProgram, 0);

			_titleGrid = new brogueTitleGrid();
			_maskCell = new brogueCellDisplay();
			_defaultCell = new brogueCellDisplay();

			_uiData = new brogueUIData(partId, _titleGrid->sceneBounds(), zoomLevel);

			_maskCell->backColor = colors::black();
			_maskCell->noDisplay = false;

			_defaultCell->backColor = colors::transparent();
			_defaultCell->noDisplay = true;
		}
		~brogueFlameMenuTitleMask()
		{
			delete _titleGrid;
			delete _uiData;
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
		brogueUIData* getUIData() const override
		{
			return _uiData;
		}
		gridRect getBoundary() const override
		{
			return _titleGrid->sceneBounds();
		}
		gridRect getSceneBoundary() const override
		{
			return _titleGrid->sceneBounds();
		}
		/// <summary>
		/// (TODO: MOVE THIS) Calculates the view's boundary is UI coordinates. This is not the same as the
		/// GL viewport; but the coordinate space relates to it. Zoom, and offset must be
		/// first added to the calculation.
		/// </summary>
		gridRect calculateSceneBoundaryUI() const override
		{
			gridRect sceneBoundary = getSceneBoundary();
			gridRect boundaryUI = gridRect(sceneBoundary.left() * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   sceneBoundary.top() * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()),
										   sceneBoundary.width * brogueCellDisplay::CellWidth(_uiData->getZoomLevel()),
										   sceneBoundary.height * brogueCellDisplay::CellHeight(_uiData->getZoomLevel()));

			return boundaryUI;
		}
		int getZoomLevel() const override
		{
			return _uiData->getZoomLevel();
		}

		brogueCellDisplay* get(short column, short row) const override
		{
			if (_titleGrid->isTheText(column, row))
				return _maskCell;
			else
				return _defaultCell;
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
				if (!titleGrid->isTheText(column, row))
					callback(column, row, maskCell);

				return iterationCallback::iterate;
			});
		}

	private:

		brogueUIData* _uiData;
		brogueTitleGrid* _titleGrid;
		brogueCellDisplay* _maskCell;
		brogueCellDisplay* _defaultCell;
	};
}
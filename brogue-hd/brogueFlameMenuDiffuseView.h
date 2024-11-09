#pragma once

#include "brogueCellDisplay.h"
#include "brogueKeyboardState.h"
#include "brogueMouseState.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewBase.h"
#include "color.h"
#include "gridDefinitions.h"
#include "gridRect.h"
#include "simple.h"

using namespace brogueHd::component;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueFlameMenuDiffuseView : public brogueViewBase
	{
	public:
		brogueFlameMenuDiffuseView(const gridRect& sceneBounds, int zoomLevel);
		~brogueFlameMenuDiffuseView();

		void checkUpdate(const brogueKeyboardState& keyboardState,
						 const brogueMouseState& mouseState,
						 int millisecondsLapsed) override
		{
			// Prevent updating
		}

		void clearUpdate() override {}
		void clearEvents() override {}

		bool needsUpdate() const override
		{
			return false;
		}

		brogueUIData* getUIData() const override
		{
			return _uiData;
		}
		int getZoomLevel() const override
		{
			return _uiData->getZoomLevel();
		}
		brogueCellDisplay* get(short column, short row) const override
		{
			return _defaultCell;
		}
		gridRect getBoundary() const override
		{
			return _uiData->getBounds();
		}
		gridRect getSceneBoundary() const override
		{
			return *_sceneBounds;
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

		void iterate(gridCallback<brogueCellDisplay*> callback) const override
		{
			brogueCellDisplay* defaultCell = _defaultCell;

			_sceneBounds->iterate([&callback, &defaultCell] (short column, short row)
			{
				callback(column, row, defaultCell);

				return iterationCallback::iterate;
			});
		}

	private:

		brogueUIData* _uiData;
		brogueCellDisplay* _defaultCell;
		gridRect* _sceneBounds;
	};

	brogueFlameMenuDiffuseView::brogueFlameMenuDiffuseView(const gridRect& sceneBounds, int zoomLevel) : brogueViewBase()
	{
		brogueUIProgramPartId partId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameMenuProgram_HeatDiffuseProgram, 0);

		_defaultCell = new brogueCellDisplay();
		_defaultCell->backColor = colors::transparent();
		_defaultCell->noDisplay = false;

		_uiData = new brogueUIData(partId, sceneBounds, zoomLevel, colors::transparent());
		_uiData->setUIParameters(-1, -1, "", brogueUIAction::None, true, 0, zoomLevel);

		_sceneBounds = new gridRect(sceneBounds);
	}
	brogueFlameMenuDiffuseView::~brogueFlameMenuDiffuseView()
	{
		delete _uiData;
		delete _defaultCell;
		delete _sceneBounds;
	}
}
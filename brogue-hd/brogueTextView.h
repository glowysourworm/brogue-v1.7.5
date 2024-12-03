#pragma once

#include "brogueCellDisplay.h"
#include "brogueCellQuad.h"
#include "brogueCoordinateConverter.h"
#include "brogueGlyphMap.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueUIText.h"
#include "brogueViewGridCore.h"
#include "color.h"
#include "colorString.h"
#include "eventController.h"
#include "gridRect.h"
#include "resourceController.h"
#include "simple.h"
#include "simpleString.h"

namespace brogueHd::frontend
{
	using namespace brogueHd::component;

	class brogueTextView : public brogueViewGridCore<brogueCellQuad>
	{
	public:
		brogueTextView(brogueCoordinateConverter* coordinateConverter,
		               resourceController* resourceController,
		               eventController* eventController,
		               const brogueUIProgramPartId& partId,
		               const brogueUIData& data);
		~brogueTextView();

		virtual void update(int millisecondsLapsed,
		                    bool forceUpdate) override;

		void setLine(int row, const colorString& text, brogueTextAlignment alignment);
		void setLine(int row, const simpleString& text, brogueTextAlignment alignment);
		void setLine(int row, const simpleString& text, const color& foreground, brogueTextAlignment alignment);
		void setText(const colorString& text);
		void setText(const simpleString& text);

	private:
		void updateImpl(int millisecondsLapsed, bool forceUpdate);

	private:
		brogueUIText* _uiText;
	};

	brogueTextView::brogueTextView(brogueCoordinateConverter* coordinateConverter,
	                               resourceController* resourceController,
	                               eventController* eventController,
	                               const brogueUIProgramPartId& partId,
	                               const brogueUIData& data)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data, false)
	{
		_uiText = new brogueUIText(data.getParentBoundary(), data.getBoundary(), simpleString(""));
	}

	brogueTextView::~brogueTextView()
	{
		delete _uiText;
	}

	void brogueTextView::updateImpl(int millisecondsLapsed, bool forceUpdate)
	{
		brogueUIData* uiData = this->getUIData();
		brogueTextView* that = this;
		brogueUIText* uiText = _uiText;
		brogueGlyphMap glyphMap;

		// Iterate THIS Boundary:  apply just the background
		uiData->getBoundary().iterate([&that, &uiData, &uiText, &glyphMap](int column, int row)
		{
			brogueCellDisplay cell(column, row);

			cell.foreColor = uiText->getForeground(column, row);
			cell.backColor = uiData->calculateGradient(column, row, false, false, false);
			cell.character = glyphMap.isGlyphDefined(uiText->getCharacter(column, row))
				                 ? uiText->getCharacter(column, row)
				                 : glyphMap.Empty;

			that->set(cell);

			return iterationCallback::iterate;
		});
	}

	void brogueTextView::update(int millisecondsLapsed, bool forceUpdate)
	{
		// Update data elements in the view
		updateImpl(millisecondsLapsed, forceUpdate);

		// Call the base class -> restream the data
		brogueViewGridCore<brogueCellQuad>::update(millisecondsLapsed, forceUpdate);
	}

	void brogueTextView::setLine(int row, const colorString& text, brogueTextAlignment alignment)
	{
		_uiText->setLine(row, text, alignment);

		brogueViewGridCore::invalidate();
	}

	void brogueTextView::setLine(int row, const simpleString& text, brogueTextAlignment alignment)
	{
		_uiText->setLine(row, text, alignment);

		brogueViewGridCore::invalidate();
	}

	void brogueTextView::setLine(int row, const simpleString& text, const color& foreground,
	                             brogueTextAlignment alignment)
	{
		setLine(row, colorString(text.c_str(), foreground), alignment);
	}

	void brogueTextView::setText(const colorString& text)
	{
		_uiText->setText(text);

		brogueViewGridCore::invalidate();
	}

	void brogueTextView::setText(const simpleString& text)
	{
		_uiText->setText(text);

		brogueViewGridCore::invalidate();
	}
}

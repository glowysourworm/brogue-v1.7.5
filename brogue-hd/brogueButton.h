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

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueButton : public brogueViewGridCore<brogueCellQuad>
	{
	public:

		brogueButton(brogueCoordinateConverter* coordinateConverter,
					 resourceController* resourceController,
					 eventController* eventController,
					 const brogueUIProgramPartId& partId,
					 const brogueUIData& data);
		~brogueButton();

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;
		virtual bool needsUpdate() const override;

		void setUI(const simpleString& text, int hotkeyIndex, const color& foreground, const color& hotkeyForeground, brogueTextAlignment alignment);
		void setUI(const colorString& text, brogueTextAlignment alignment);

	private:

		brogueUIText* _uiText;
	};

	brogueButton::brogueButton(brogueCoordinateConverter* coordinateConverter,
							   resourceController* resourceController,
							   eventController* eventController,
							   const brogueUIProgramPartId& partId,
							   const brogueUIData& data)
		: brogueViewGridCore(coordinateConverter, resourceController, eventController, partId, data)
	{
		_uiText = new brogueUIText(data.getParentBoundary(), data.getBoundary(), simpleString(""));

		// Initialize the view
		//
		update(0, true);

		// Call initializeCore() -> sets up stream for  GL backend
		brogueViewGridCore::initializeCore();
	}
	brogueButton::~brogueButton()
	{
		delete _uiText;
	}
	void brogueButton::setUI(const simpleString& text, int hotkeyIndex, const color& foreground, const color& hotkeyForeground, brogueTextAlignment alignment)
	{
		gridRect boundary = this->getUIData()->getBoundary();

		if (hotkeyIndex >= 0)
			_uiText->setLine(boundary.top(), text, foreground, hotkeyForeground, hotkeyIndex, alignment);

		else
			_uiText->setLine(boundary.top(), colorString(text.c_str(), foreground), alignment);

		brogueViewGridCore::invalidate();
	}
	void brogueButton::setUI(const colorString& text, brogueTextAlignment alignment)
	{
		gridRect boundary = this->getUIData()->getBoundary();

		_uiText->setLine(boundary.top(), text, alignment);

		brogueViewGridCore::invalidate();
	}
	bool brogueButton::needsUpdate() const
	{
		// Adding the mouse enter / leave events
		//
		return brogueViewGridCore::needsUpdate() ||
			this->getMouseLeave() ||
			this->getMouseEnter() ||
			this->getMousePressedChanged();
	}
	void brogueButton::update(int millisecondsLapsed, bool forceUpdate)
	{
		brogueUIData* uiData = this->getUIData();
		brogueButton* that = this;
		brogueUIText* uiText = _uiText;
		brogueGlyphMap glyphMap;

		// Iterate THIS boundary:  Apply mouse data
		//
		uiData->getBoundary().iterate([&that, &uiData, &uiText, &glyphMap] (int column, int row)
		{
			brogueCellDisplay cell(column, row);

			cell.foreColor = uiText->getForeground(column, row);
			cell.backColor = uiData->calculateGradient(column, row, that->getHasMouseInteraction(), that->getMouseOver(), that->getMousePressed());
			cell.character = glyphMap.isGlyphDefined(uiText->getCharacter(column, row)) ? uiText->getCharacter(column, row) : glyphMap.Empty;

			that->set(cell);

			return iterationCallback::iterate;
		});
	}
}
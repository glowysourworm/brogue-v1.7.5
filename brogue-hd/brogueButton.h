#pragma once

#include "brogueGlyphMap.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueUIText.h"
#include "brogueViewBase.h"
#include "color.h"
#include "colorString.h"
#include "eventController.h"
#include "gridRect.h"
#include "simple.h"
#include "simpleString.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend
{
	class brogueButton : public brogueViewBase
	{
	public:

		brogueButton(eventController* eventController, const brogueUIProgramPartId& partId, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueButton();

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;
		virtual bool needsUpdate() const override;
		virtual void clearUpdate() override;

		void setUI(const simpleString& text, int hotkeyIndex, const color& foreground, const color& hotkeyForeground, brogueTextAlignment alignment);

	private:

		brogueUIText* _uiText;
		bool _invalid;
	};

	brogueButton::brogueButton(eventController* eventController, const brogueUIProgramPartId& partId, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, partId, data, sceneBoundary, viewBoundary)
	{
		_uiText = new brogueUIText(sceneBoundary, viewBoundary, simpleString(""));
		_invalid = false;

		// Initialize the view
		//
		update(0, true);
	}
	brogueButton::~brogueButton()
	{
		delete _uiText;
	}
	void brogueButton::setUI(const simpleString& text, int hotkeyIndex, const color& foreground, const color& hotkeyForeground, brogueTextAlignment alignment)
	{
		if (hotkeyIndex >= 0)
			_uiText->setLine(this->getBoundary().top(), text, foreground, hotkeyForeground, hotkeyIndex, alignment);

		else
			_uiText->setLine(this->getBoundary().top(), colorString(text.c_str(), foreground), alignment);

		_invalid = true;
	}
	void brogueButton::clearUpdate()
	{
		brogueViewBase::clearUpdate();

		_invalid = false;
	}
	bool brogueButton::needsUpdate() const
	{
		// Adding the mouse enter / leave events
		//
		return brogueViewBase::needsUpdate() || _invalid || this->getMouseLeave() || this->getMouseEnter();
	}
	void brogueButton::update(int millisecondsLapsed, bool forceUpdate)
	{
		gridRect bounds = this->getBoundary();
		brogueButton* that = this;
		brogueUIText* uiText = _uiText;
		brogueGlyphMap glyphMap;

		// Iterate THIS boundary:  Apply mouse data from the render boundary
		this->getBoundary().iterate([&that, &bounds, &uiText, &glyphMap] (short column, short row)
		{
			that->get(column, row)->foreColor = uiText->getForeground(column, row);
			that->get(column, row)->backColor = that->getBackgroundColor(column, row);
			that->get(column, row)->character = glyphMap.isGlyphDefined(uiText->getCharacter(column, row)) ? uiText->getCharacter(column, row) : glyphMap.Empty;

			return iterationCallback::iterate;
		});
	}
}
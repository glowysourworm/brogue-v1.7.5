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

namespace brogueHd::frontend
{
	class brogueTextView : public brogueViewBase
	{
	public:

		brogueTextView(eventController* eventController, const brogueUIProgramPartId& partId, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary);
		~brogueTextView();

		virtual void update(int millisecondsLapsed,
							bool forceUpdate) override;

		virtual bool needsUpdate() const override;
		virtual void clearUpdate() override;

		void setLine(int row, const colorString& text, brogueTextAlignment alignment);
		void setLine(int row, const simpleString& text, brogueTextAlignment alignment);
		void setLine(int row, const simpleString& text, const color& foreground, brogueTextAlignment alignment);
		void setText(const colorString& text);
		void setText(const simpleString& text);

	private:

		brogueUIText* _uiText;
		bool _invalid;
	};

	brogueTextView::brogueTextView(eventController* eventController, const brogueUIProgramPartId& partId, const brogueUIData& data, const gridRect& sceneBoundary, const gridRect& viewBoundary)
		: brogueViewBase(eventController, partId, data, sceneBoundary, viewBoundary)
	{
		_uiText = new brogueUIText(sceneBoundary, viewBoundary, simpleString(""));

		update(0, true);

		// Force initial update
		_invalid = true;
	}
	brogueTextView::~brogueTextView()
	{
		delete _uiText;
	}
	bool brogueTextView::needsUpdate() const
	{
		// Adding the mouse enter / leave events
		//
		return brogueViewBase::needsUpdate() || _invalid;
	}
	void brogueTextView::clearUpdate()
	{
		_invalid = false;

		brogueViewBase::clearUpdate();
	}
	void brogueTextView::update(int millisecondsLapsed, bool forceUpdate)
	{
		gridRect bounds = this->getBoundary();
		brogueTextView* that = this;
		brogueUIText* uiText = _uiText;
		brogueGlyphMap glyphMap;

		// Iterate THIS Boundary:  apply just the background
		this->getBoundary().iterate([&that, &bounds, &uiText, &glyphMap] (short column, short row)
		{
			that->get(column, row)->foreColor = uiText->getForeground(column, row);
			that->get(column, row)->backColor = that->getBackgroundColor(column, row);
			that->get(column, row)->character = glyphMap.isGlyphDefined(uiText->getCharacter(column, row)) ? uiText->getCharacter(column, row) : glyphMap.Empty;

			return iterationCallback::iterate;
		});
	}
	void brogueTextView::setLine(int row, const colorString& text, brogueTextAlignment alignment)
	{
		_uiText->setLine(row, text, alignment);

		_invalid = true;
	}
	void brogueTextView::setLine(int row, const simpleString& text, brogueTextAlignment alignment)
	{
		_uiText->setLine(row, text, alignment);

		_invalid = true;
	}
	void brogueTextView::setLine(int row, const simpleString& text, const color& foreground, brogueTextAlignment alignment)
	{
		setLine(row, colorString(text.c_str(), foreground), alignment);
	}
	void brogueTextView::setText(const colorString& text)
	{
		_uiText->setText(text);

		_invalid = true;
	}
	void brogueTextView::setText(const simpleString& text)
	{
		_uiText->setText(text);

		_invalid = true;
	}
}
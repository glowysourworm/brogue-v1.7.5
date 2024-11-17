#pragma once

#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueUITagAction.h"
#include "brogueViewBase.h"
#include "eventController.h"
#include "gridRect.h"
#include "simple.h"

namespace brogueHd::frontend
{
	class brogueGameView : public brogueViewBase
	{
	public:

		brogueGameView(eventController* eventController,
					   const brogueUIProgramPartId& partId,
					   const brogueUIData& data,
					   const gridRect& sceneBoundary,
					   const gridRect& viewBoundary);
		~brogueGameView() {};

		virtual void update(int millisecondsLapsed, bool forceUpdate) override;
		virtual bool needsUpdate() const override;
		virtual void clearUpdate() override;

		/// <summary>
		/// Overload of invalidate to handle a simple invalidate flag
		/// </summary>
		void invalidate();

	private:

		bool _invalid;
	};

	brogueGameView::brogueGameView(eventController* eventController,
								   const brogueUIProgramPartId& partId,
								   const brogueUIData& data,
								   const gridRect& sceneBoundary,
								   const gridRect& viewBoundary)
		: brogueViewBase(eventController, partId, data, sceneBoundary, viewBoundary)
	{
		this->setUIAction(brogueUITagAction(brogueUIState::GameNormal));

		_invalid = true;
	}

	void brogueGameView::update(int millisecondsLapsed, bool forceUpdate)
	{
		gridRect bounds = this->getBoundary();
		brogueGameView* that = this;

		// Iterate THIS boundary:  Apply mouse data from the render boundary
		this->getBoundary().iterate([&that, &bounds] (short column, short row)
		{
			//that->get(column, row)->foreColor = uiText->getForeground(column, row);
			that->get(column, row)->backColor = that->getBackgroundColor(column, row);
			//that->get(column, row)->character = glyphMap.isGlyphDefined(uiText->getCharacter(column, row)) ? uiText->getCharacter(column, row) : glyphMap.Empty;

			return iterationCallback::iterate;
		});
	}

	bool brogueGameView::needsUpdate() const
	{
		return _invalid || brogueViewBase::needsUpdate();
	}
	void brogueGameView::clearUpdate()
	{
		_invalid = false;
	}
	void brogueGameView::invalidate()
	{
		_invalid = true;
	}
}
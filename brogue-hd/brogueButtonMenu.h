#pragma once

#include "brogueView.h"
#include "gridRect.h"
#include "button.h"
#include "simpleHash.h"
#include "simpleException.h"

using namespace brogueHd::component;
using namespace brogueHd::simple;

namespace brogueHd::frontend::ui
{
	class brogueButtonMenu : public brogueView
	{
	public:

		brogueButtonMenu(const simpleList<buttonData>& buttons, int padding, gridRect sceneBoundary, gridRect viewBoundary);
		~brogueButtonMenu();

		virtual void update(int millisecondsLapsed) override;

	private:

		simpleHash<buttonData, gridRect>* _buttons;
	};

	brogueButtonMenu::brogueButtonMenu(const simpleList<buttonData>& buttons, int padding, gridRect sceneBoundary, gridRect viewBoundary) 
		: brogueView(sceneBoundary, viewBoundary)
	{
		_buttons = new simpleHash<buttonData, gridRect>();

		for (int index = 0; index < buttons.count(); index++)
		{
			gridRect boundary(viewBoundary.column + padding, 
							  viewBoundary.row + padding + (index * 2), 
							  viewBoundary.width - (2 * padding), 
							  1);

			if (boundary.width < buttons.get(index).text.getCount())
				simpleException::show("Button text too large for the button boundary:  brogueButtonMenu.h");

			_buttons->add(buttons.get(index), boundary);
		}

		update(0);
	}
	brogueButtonMenu::~brogueButtonMenu()
	{
		delete _buttons;
	}
	void brogueButtonMenu::update(int millisecondsLapsed)
	{
		brogueButtonMenu* that = this;

		// Border
		this->getBoundary().iterate([&that] (short column, short row)
		{
			that->get(column, row)->backColor = color(0.1, 0.1, 0.1, 0.8);

			return iterationCallback::iterate;
		});

		_buttons->iterate([&that] (buttonData data, gridRect bounds)
		{
			bounds.iterate([&that, &data, &bounds] (short column, short row)
			{
				short menuColumn = column - bounds.column;
				short menuRow = row - bounds.row;
				int textIndex = column - bounds.column;

				int textBeginIndex = 0;

				switch (data.textAlignment)
				{
					case brogueTextAlignment::Left:
					break;
					case brogueTextAlignment::Right:
					textBeginIndex = bounds.right() - data.text.getCount() - bounds.column;
					break;
					case brogueTextAlignment::Center:
					textBeginIndex = ((bounds.right() - data.text.getCount() - bounds.column) / 2) + 1;
					break;
					default:
						simpleException::show("Unhandled brogueTextAlignment:  brogueButtonMenu.h");
				}

				color nextColor;
				
				// Create button gradient
				//
				if ((column - bounds.column) < (bounds.width / 2.0f))
					nextColor = color::interpolate(data.background1, data.background2, (bounds.width - (2 * (menuColumn))) / (float)bounds.width);
				else
					nextColor = color::interpolate(data.background1, data.background2, (2 * ((menuColumn) - (bounds.width / 2.0f))) / (float)bounds.width);

				nextColor.alpha = 1.0;

				that->get(column, row)->backColor = nextColor;

				if (textIndex < data.text.getCount())
				{
					if (data.hotkeyIndex == textIndex)
						that->get(column + textBeginIndex, row)->foreColor = colors::yellow();
					else
						that->get(column + textBeginIndex, row)->foreColor = data.text.getColor(textIndex);

					that->get(column + textBeginIndex, row)->character = data.text.getChar(textIndex);
				}

				return iterationCallback::iterate;
			});
			
			return iterationCallback::iterate;
		});
	}
}
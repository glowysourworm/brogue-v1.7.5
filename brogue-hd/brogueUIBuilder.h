#pragma once
#include "brogueButtonMenu.h"
#include "brogueGlobal.h"
#include "simpleList.h"
#include "simpleString.h"
#include "color.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::model::game;

namespace brogueHd::frontend::ui
{
	class brogueUIBuilder
	{

	public:

		/// <summary>
		/// Defines the primary scene boundary for the brogue game
		/// </summary>
		static gridRect getBrogueSceneBoundary()
		{
			return gridRect(0,0,COLS,ROWS);
		}

		static gridRect getMainMenuSelectorBoundary()
		{
			return gridRect(COLS - 26, ROWS - 12, 24, 11);
		}

		static gridRect getPaddedBoundary(const gridRect& boundary, int padding)
		{
			return gridRect(boundary.column + padding,
							boundary.row + padding,
							boundary.width - (padding * 2),
							boundary.height - (padding * 2));
		}

		static brogueButtonMenu* createMainMenuSelector()
		{
			// Pulled from Brogue v1.7.5
			color gradient1(0.22, 0.14, 0.29, 0.5);
			color gradient2(0.11, 0.06, 0.15, 0.5);

			gridRect sceneBounds = getBrogueSceneBoundary();
			gridRect menuBounds = getMainMenuSelectorBoundary();
			gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

			color menuColor1(0, 0, 0, 1.0f);
			color menuColor2(0, 0, 0, 0.2f);

			// Menu Background
			brogueUIData menuData(menuBounds, menuColor1, menuColor2, brogueGradientType::Circular, 1);

			// Header (None)
			brogueUIData headerData(menuData); // This should probably have an "EMPTY" default somewhere

			simpleList<brogueUIData> buttons;
			int buttonPadding = 1;

			for (int index = 0; index < 5; index++)
			{
				gridRect boundary(paddedBounds.column + buttonPadding,
								  paddedBounds.row + ((index * buttonPadding) + ((index - 1) * buttonPadding)) + 1,
								  paddedBounds.width - (2 * buttonPadding),
								  1);

				if (index == 0)
					buttons.add(brogueUIData(boundary, colorString("New Game"), gradient1, gradient2, brogueTextAlignment::Center, 0, 0));
				else if (index == 1)
					buttons.add(brogueUIData(boundary, colorString("Open Game"), gradient1, gradient2, brogueTextAlignment::Center, 0, 0));
				else if (index == 2)
					buttons.add(brogueUIData(boundary, colorString("View Recording"), gradient1, gradient2, brogueTextAlignment::Center, 0, 0));
				else if (index == 3)
					buttons.add(brogueUIData(boundary, colorString("High Scores"), gradient1, gradient2, brogueTextAlignment::Center, 0, 0));
				else if (index == 4)
					buttons.add(brogueUIData(boundary, colorString("Quit"), gradient1, gradient2, brogueTextAlignment::Center, 0, 0));
			}
			
			return new brogueButtonMenu(menuData, buttons, headerData, false, sceneBounds, menuBounds);
		}
	};
}
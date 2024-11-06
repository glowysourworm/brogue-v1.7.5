#pragma once
#include "brogueGlobal.h"
#include "brogueListView.h"
#include "brogueScoreEntry.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueView.h"
#include "color.h"
#include "colorGradient.h"
#include "colorString.h"
#include "eventController.h"
#include "gridRect.h"
#include "openglRenderer.h"
#include "resourceController.h"
#include "simpleDirectoryEntry.h"
#include "simpleExt.h"
#include "simpleFileEntry.h"
#include "simpleList.h"
#include "simpleString.h"

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
			return gridRect(0, 0, COLS, ROWS);
		}

		static gridRect getMainMenuSelectorBoundary()
		{
			return gridRect(COLS - 26, ROWS - 12, 24, 11);
		}

		static gridRect getOpenMenuSelectorBoundary()
		{
			int width = COLS / 3;
			int height = ROWS / 2;

			return gridRect((COLS - width) / 2, (ROWS - height) / 2, width, height);
		}

		static gridRect getPaddedBoundary(const gridRect& boundary, int padding)
		{
			return gridRect(boundary.column + padding,
							boundary.row + padding,
							boundary.width - (padding * 2),
							boundary.height - (padding * 2));
		}

		static brogueListView* createMainMenuButtons(eventController* eventController, int zoomLevel)
		{
			// Pulled from Brogue v1.7.5
			color gradient1(0.22, 0.14, 0.29, 0.5);
			color gradient2(0.11, 0.06, 0.15, 0.5);
			color active1(0.7, 0.7, 0.7, 0.4);
			color active2(0.7, 0.7, 0.7, 0.4);
			color pressed1(0.7, 0.7, 0.7, 0.8);
			color pressed2(0.7, 0.7, 0.7, 0.8);

			gridRect sceneBounds = getBrogueSceneBoundary();
			gridRect menuBounds = getMainMenuSelectorBoundary();
			gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

			color menuColor1(0, 0, 0, 1.0f);
			color menuColor2(0, 0, 0, 0.2f);

			// Menu Background
			brogueUIData* menuData = new brogueUIData(menuBounds, zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);

			menuData->setUIParameters(-1, -1, "", brogueUIAction::None, false, true, 1, zoomLevel);

			simpleList<brogueUIData*> buttons;
			int buttonPadding = 1;

			for (int index = 0; index < 5; index++)
			{
				gridRect boundary(paddedBounds.column + buttonPadding,
								  paddedBounds.row + ((index * buttonPadding) + ((index - 1) * buttonPadding)) + 1,
								  paddedBounds.width - (2 * buttonPadding),
								  1);

				if (index == 0)
				{
					buttons.add(new brogueUIData(boundary, zoomLevel, colorString("New Game"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center));
					buttons.get(index)->setUIParameters(0, -1, "", brogueUIAction::NewGame, true, true, 0, zoomLevel);
				}
				else if (index == 1)
				{
					buttons.add(new brogueUIData(boundary, zoomLevel, colorString("Open Game"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center));
					buttons.get(index)->setUIParameters(0, -1, "", brogueUIAction::ViewOpenGameMenu, true, true, 0, zoomLevel);
				}
				else if (index == 2)
				{
					buttons.add(new brogueUIData(boundary, zoomLevel, colorString("View Recording"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center));
					buttons.get(index)->setUIParameters(0, -1, "", brogueUIAction::ViewPlaybackMenu, true, true, 0, zoomLevel);
				}
				else if (index == 3)
				{
					buttons.add(new brogueUIData(boundary, zoomLevel, colorString("High Scores"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center));
					buttons.get(index)->setUIParameters(0, -1, "", brogueUIAction::ViewHighScores, true, true, 0, zoomLevel);
				}
				else if (index == 4)
				{
					buttons.add(new brogueUIData(boundary, zoomLevel, colorString("Quit"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center));
					buttons.get(index)->setUIParameters(0, -1, "", brogueUIAction::ShowQuitGameModal, true, true, 0, zoomLevel);
				}
			}

			brogueView* listBackground = new brogueView(eventController, menuData, sceneBounds, menuBounds);

			return new brogueListView(eventController, listBackground, brogueUIView::MainMenuSelector, menuData, buttons, nullptr, nullptr);
		}

		static brogueListView* createMainMenuSelector(eventController* eventController, brogueUIView viewName, const simpleDirectoryEntry& files, int zoomLevel)
		{
			gridRect sceneBounds = getBrogueSceneBoundary();
			gridRect menuBounds = getOpenMenuSelectorBoundary();
			gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

			// Pulled from Brogue v1.7.5
			color menuColor1(0.067, 0.059, 0.149, 0.8f);
			color menuColor2(0.067, 0.059, 0.149, 0.8f);
			color active1(0.7, 0.7, 0.7, 0.4);
			color active2(0.7, 0.7, 0.7, 0.4);
			color pressed1(0.7, 0.7, 0.7, 0.8);
			color pressed2(0.7, 0.7, 0.7, 0.8);

			// Menu Background
			brogueUIData* menuData = new brogueUIData(menuBounds, zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);
			menuData->setUIParameters(-1, -1, "", brogueUIAction::None, true, true, 1, zoomLevel);

			// Header (None)
			gridRect headerBounds(paddedBounds.column, paddedBounds.row, paddedBounds.width, 1);
			brogueUIData* headerData = new brogueUIData(headerBounds, zoomLevel, colorString("Open Saved Game", colors::yellow()), menuColor1, menuColor2, brogueTextAlignment::Center);

			headerData->setUIParameters(-1, -1, "", brogueUIAction::None, false, true, 0, zoomLevel);

			simpleList<brogueUIData*> buttons;

			simpleList<simpleFileEntry*> filesSorted = files.sort();

			for (int index = 0; index < filesSorted.count(); index++)
			{
				gridRect boundary(paddedBounds.column, paddedBounds.row + index + 1, paddedBounds.width, 1);

				simpleString fileName = *(filesSorted.get(index)->getFileNameWithoutExtension());
				simpleString fileNameTrimmed;

				if (fileName.count() > ((paddedBounds.width / 2.0f) - 3))
					fileNameTrimmed = fileName.subString(0, (paddedBounds.width / 2.0f) - 3);

				char offsetChar[2]{};
				simpleString result = "";

				offsetChar[0] = (char)((int)'a' + index);
				offsetChar[1] = '\0';

				// Format the result
				int column = paddedBounds.left();
				while (column < paddedBounds.right())
				{
					int textIndex = column - paddedBounds.left();

					if (textIndex == 0)
					{
						result.append(offsetChar);
						column++;
					}
					else if (textIndex == 1)
					{
						result.append(") ");
						column += 2;
					}
					else if (textIndex == 3)
					{
						if (fileNameTrimmed.count() > 0)
						{
							result.append(fileNameTrimmed);
							result.append("...");

							column += fileNameTrimmed.count() + 3;
						}
						else
						{
							result.append(fileName);

							column += fileName.count();
						}
					}

					// Date -> break;
					else
					{
						int space = paddedBounds.width - textIndex - filesSorted.get(index)->getWriteTimeShort()->count() - 1;
						result.appendPadding(' ', space);
						result.append(*(filesSorted.get(index)->getWriteTimeShort()));
						break;
					}
				}

				brogueUIData* data = new brogueUIData(boundary, zoomLevel, colorString(result.c_str(), colors::white()), menuColor1, menuColor2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Left);

				// Hotkey is the first letter a), b), etc..
				data->setUIParameters(0, -1, *(filesSorted.get(index)->getFileFullPath()), brogueUIAction::OpenGame, true, true, 0, zoomLevel);

				buttons.add(data);
			}

			brogueView* listBackground = new brogueView(eventController, menuData, sceneBounds, menuBounds);

			return new brogueListView(eventController, listBackground, viewName, menuData, buttons, headerData, nullptr);
		}

		static brogueListView* createHighScoresView(eventController* eventController, resourceController* resourceController, int zoomLevel)
		{
			gridRect sceneBounds = getBrogueSceneBoundary();

			// Pulled from Brogue v1.7.5
			color menuColor(0, 0, 0, 0.85);
			color footerColor(0.596f, 0.494f, 0.996f, 1);

			// Menu Background
			brogueUIData* menuData = new brogueUIData(sceneBounds, zoomLevel, menuColor);
			int menuPadding = 2;

			// Padding
			menuData->setUIParameters(-1, -1, "", brogueUIAction::ViewMainMenu, true, true, menuPadding, zoomLevel);

			// Header
			gridRect headerBounds(sceneBounds.column, sceneBounds.row, sceneBounds.width, 1);
			brogueUIData* headerData = new brogueUIData(headerBounds, zoomLevel, colorString("-- High Scores --", colors::yellow()), menuColor, brogueTextAlignment::Center);

			headerData->setUIParameters(-1, -1, "", brogueUIAction::None, false, true, 0, zoomLevel);

			simpleList<brogueUIData*> items;

			for (int index = 0; index < resourceController->getHighScoreCount(); index++)
			{
				brogueScoreEntry* entry = resourceController->getHighScore(index);

				gridRect boundary(sceneBounds.column, sceneBounds.row + index + menuPadding, sceneBounds.width, 1);

				simpleString preamble = simpleExt::format("{}) ", index + 1);
				simpleString score = simpleExt::toString(entry->getScore());
				simpleString dateAndDescr = simpleExt::format("{} {}", entry->getDate()->c_str(), entry->getDescription()->c_str());

				// Size to line up scores (5)
				preamble.appendPadding(' ', 5 - preamble.count());

				// Scores (15)
				score.appendPadding(' ', 15 - score.count() - preamble.count());

				simpleString formattedEntry = simpleExt::format("{}{}{}", preamble.c_str(), score.c_str(), dateAndDescr.c_str());

				color foreGround = color::interpolate(colors::white(), colors::black(), index / (float)resourceController->getHighScoreCount());

				brogueUIData* data = new brogueUIData(boundary, zoomLevel, colorString(formattedEntry.c_str(), foreGround), menuColor, brogueTextAlignment::Center);

				items.add(data);
			}

			// Footer
			gridRect footerBounds(sceneBounds.column, sceneBounds.height - 1, sceneBounds.width, 1);
			brogueUIData* footerData = new brogueUIData(footerBounds, zoomLevel, colorString("Press space to continue.", footerColor), menuColor, brogueTextAlignment::Center);

			brogueView* listBackground = new brogueView(eventController, menuData, sceneBounds, sceneBounds);

			return new brogueListView(eventController, listBackground, brogueUIView::HighScoresView, menuData, items, headerData, footerData);
		}
	};
}
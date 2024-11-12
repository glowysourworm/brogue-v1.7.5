#pragma once
#include "brogueButton.h"
#include "brogueComposedView.h"
#include "brogueFlameMenuDiffuseView.h"
#include "brogueFlameMenuHeatView.h"
#include "brogueFlameMenuTitleMask.h"
#include "brogueGlobal.h"
#include "brogueScoreEntry.h"
#include "brogueText.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueViewContainer.h"
#include "color.h"
#include "colorGradient.h"
#include "colorString.h"
#include "eventController.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "resourceController.h"
#include "simpleDirectoryEntry.h"
#include "simpleExt.h"
#include "simpleFileEntry.h"
#include "simpleList.h"
#include "simpleString.h"
#include "brogueBackground.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
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

		static gridRect getGameBoundary()
		{
			return gridRect(COLS - DCOLS, ROWS - DROWS, DCOLS, DROWS);
		}

		static gridRect getLeftSidebarBoundary()
		{
			return gridRect(0, 0, COLS - DCOLS, ROWS - DROWS);
		}

		static gridRect getInventoryBoundary()
		{
			// Double check this (The height is meant to be dynamic - leaving space at the bottom and top for the
			// other menus)
			return gridRect(COLS - 34, 3, 34, ROWS - 5);
		}

		static gridRect getMenuBoundary()
		{
			// Double check this (The height and width are static - these are just measured from the bottom right
			// to make it easier)
			return gridRect(COLS - 30 - 3, ROWS - 16, 30, 16);
		}

		static gridRect getBottomBarBoundary()
		{
			return gridRect(COLS - DCOLS, ROWS - 1, DCOLS, 1);
		}

		static gridRect getFlavorTextBoundary()
		{
			return gridRect(COLS - DCOLS, ROWS - 2, DCOLS, 1);
		}

		static gridRect getMainMenuSelectorBoundary()
		{
			return gridRect(COLS - 26, ROWS - 12, 24, 11);
		}

		static gridRect getOpenMenuSelectorBoundary()
		{
			int width = COLS / 2;
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

		static brogueViewContainer* createFlameMenu(eventController* eventController, randomGenerator* randGenerator, int zoomLevel)
		{
			gridRect sceneBounds = getBrogueSceneBoundary();
			int fadePeriodMilliseconds = 100;

			brogueUIProgramPartId titleId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameMenuProgram_TitleMaskProgram, 0);
			brogueUIProgramPartId heatId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameMenuProgram_HeatSourceProgram, 0);
			brogueUIProgramPartId diffuseId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameMenuProgram_HeatDiffuseProgram, 0);

			brogueUIData titleMaskData(titleId, sceneBounds, zoomLevel);
			brogueUIData heatData(heatId, sceneBounds, zoomLevel);
			brogueUIData diffuseData(diffuseId, sceneBounds, zoomLevel);

			// Mouse Interaction:  This will deactivate the open menu selectors
			heatData.setUIParameters(-1, -1, "", brogueUIAction::None, true, zoomLevel, 0);

			brogueFlameMenuTitleMask* titleMask = new brogueFlameMenuTitleMask(eventController, titleMaskData, sceneBounds, sceneBounds);
			brogueFlameMenuHeatView* heatView = new brogueFlameMenuHeatView(eventController, randGenerator, fadePeriodMilliseconds, heatData, sceneBounds, sceneBounds);
			brogueFlameMenuDiffuseView* heatDiffuseView = new brogueFlameMenuDiffuseView(eventController, diffuseData, sceneBounds, sceneBounds);

			brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::FlameMenuProgram, false, false, sceneBounds);

			result->addView(heatView);
			result->addView(heatDiffuseView);
			result->addView(titleMask);

			return result;
		}

		static brogueViewContainer* createMainMenuButtons(eventController* eventController, int zoomLevel)
		{
			// Pulled from Brogue v1.7.5
			color gradient1(0.22, 0.14, 0.29, 0.8);
			color gradient2(0.11, 0.06, 0.15, 0.8);
			color active1(0.7, 0.7, 0.7, 0.4);
			color active2(0.7, 0.7, 0.7, 0.4);
			color pressed1(0.7, 0.7, 0.7, 0.6);
			color pressed2(0.7, 0.7, 0.7, 0.6);
			color menuColor1(0, 0, 0, 1.0f);
			color menuColor2(0, 0, 0, 0.5f);

			gridRect sceneBounds = getBrogueSceneBoundary();
			gridRect menuBounds = getMainMenuSelectorBoundary();
			gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

			brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::MainMenuProgram, false, false, menuBounds);
			//brogueUIProgramPartId composedId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::ViewCompositor, 0);
			//brogueUIData composedViewData(composedId, sceneBounds, zoomLevel);
			//brogueComposedView* composedView = new brogueComposedView(eventController, composedViewData, sceneBounds, menuBounds);

			// Menu Background
			brogueUIProgramPartId menuId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::MenuBackground, 0);
			brogueUIData menuData(menuId, menuBounds, zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);

			menuData.setUIParameters(-1, -1, "", brogueUIAction::None, false, zoomLevel, 0);

			simpleList<brogueUIData> buttons;
			int buttonPadding = 1;

			for (int index = 0; index < 5; index++)
			{
				gridRect boundary(paddedBounds.column + buttonPadding,
								  paddedBounds.row + ((index * buttonPadding) + ((index - 1) * buttonPadding)) + 1,
								  paddedBounds.width - (2 * buttonPadding),
								  1);

				brogueUIProgramPartId buttonId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::MenuButton, index);

				if (index == 0)
				{
					brogueUIData buttonData(buttonId, boundary, zoomLevel, colorString("New Game"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
					buttonData.setUIParameters(0, -1, " ", brogueUIAction::NewGame, true, zoomLevel, 1);
					buttons.add(buttonData);
				}
				else if (index == 1)
				{
					brogueUIData buttonData(buttonId, boundary, zoomLevel, colorString("Open Game"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
					buttonData.setUIParameters(0, -1, " ", brogueUIAction::OpenGame, true, zoomLevel, 1);
					buttons.add(buttonData);
				}
				else if (index == 2)
				{
					brogueUIData buttonData(buttonId, boundary, zoomLevel, colorString("View Recording"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
					buttonData.setUIParameters(0, -1, " ", brogueUIAction::ViewPlaybackMenu, true, zoomLevel, 1);
					buttons.add(buttonData);
				}
				else if (index == 3)
				{
					brogueUIData buttonData(buttonId, boundary, zoomLevel, colorString("High Scores"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
					buttonData.setUIParameters(0, -1, " ", brogueUIAction::ViewHighScores, true, zoomLevel, 1);
					buttons.add(buttonData);
				}
				else if (index == 4)
				{
					brogueUIData buttonData(buttonId, boundary, zoomLevel, colorString("Quit"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
					buttonData.setUIParameters(0, -1, " ", brogueUIAction::ShowQuitGameModal, true, zoomLevel, 1);
					buttons.add(buttonData);
				}
			}

			// Finally, create the views
			brogueBackground* background = new brogueBackground(eventController, menuData, sceneBounds, menuBounds);

			result->addView(background);

			for (int index = 0; index < buttons.count(); index++)
			{
				brogueButton* item = new brogueButton(eventController, buttons.get(index), sceneBounds, buttons.get(index).getBounds());

				result->addView(item);
			}

			return result;
		}

		static brogueViewContainer* createMainMenuSelector(brogueUIProgram programName, eventController* eventController, const simpleDirectoryEntry& files, int zoomLevel, const char* headerText)
		{
			// Pulled from Brogue v1.7.5
			color menuColor1(0.067, 0.059, 0.149, 0.8f);
			color menuColor2(0.067, 0.059, 0.149, 0.8f);
			color active1(0.7, 0.7, 0.7, 0.4);
			color active2(0.7, 0.7, 0.7, 0.4);
			color pressed1(0.7, 0.7, 0.7, 0.8);
			color pressed2(0.7, 0.7, 0.7, 0.8);

			gridRect sceneBounds = getBrogueSceneBoundary();
			gridRect menuBounds = getOpenMenuSelectorBoundary();
			gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

			brogueViewContainer* result = new brogueViewContainer(programName, true, true, menuBounds);
			//brogueUIProgramPartId composedId(programName, brogueUIProgramPart::ViewCompositor, 0);
			//brogueUIData composedViewData(composedId, sceneBounds, zoomLevel, menuColor1);

			// Mouse Interaction
			//composedViewData.setUIParameters(-1, -1, "", brogueUIAction::None, true, zoomLevel, 0);

			//brogueComposedView* composedView = new brogueComposedView(eventController, composedViewData, sceneBounds, paddedBounds);

			// Menu Background
			brogueUIProgramPartId menuId(programName, brogueUIProgramPart::MenuBackground, 0);
			brogueUIData menuData(menuId, menuBounds, zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);
			menuData.setUIParameters(-1, -1, "", brogueUIAction::None, true, zoomLevel, 0);

			// Header 
			gridRect headerBounds(menuBounds.column, menuBounds.row, menuBounds.width, 1);
			brogueUIProgramPartId headerId(programName, brogueUIProgramPart::Text, 0);
			brogueUIData headerData(headerId, headerBounds, zoomLevel, colorString(headerText, colors::yellow()), colors::transparent(), brogueTextAlignment::Center);

			headerData.setUIParameters(-1, -1, "", brogueUIAction::None, false, zoomLevel, 1);

			//// Footer
			//gridRect footerBounds(menuBounds.column, menuBounds.bottom() - 1, menuBounds.width, 1);
			//brogueUIProgramPartId footerId(programName, brogueUIProgramPart::Text, 0);
			//brogueUIData footerData(footerId, footerBounds, zoomLevel, "Footer", colors::transparent(), brogueTextAlignment::Center);

			//footerData.setUIParameters(-1, -1, "", brogueUIAction::None, false, 0, zoomLevel, 1);

			simpleList<brogueUIData> buttons;

			simpleList<simpleFileEntry*> filesSorted = files.sort();

			for (int index = 0; index < filesSorted.count(); index++)
			{
				gridRect boundary(paddedBounds.column, paddedBounds.row + index, paddedBounds.width, 1);

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

				brogueUIProgramPartId dataPartId(programName, brogueUIProgramPart::MenuButton, index);
				brogueUIData data(dataPartId, boundary, zoomLevel, colorString(result.c_str(), colors::white()), menuColor1, menuColor2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Left);

				// Hotkey is the first letter a), b), etc..
				data.setUIParameters(0, -1, *(filesSorted.get(index)->getFileFullPath()), brogueUIAction::OpenGame, true, zoomLevel, 1);

				buttons.add(data);
			}

			// Finally, create the views. The composed view will have the buttons to create the scroll behavior. The
			// rest will be separated into the view container; and not share mouse interaction.
			//

			// Background
			result->addView(new brogueBackground(eventController, menuData, sceneBounds, menuBounds));

			// Header
			result->addView(new brogueText(eventController, headerData, sceneBounds, headerData.getBounds()));

			// Buttons
			for (int index = 0; index < buttons.count(); index++)
			{
				brogueButton* item = new brogueButton(eventController, buttons.get(index), sceneBounds, buttons.get(index).getBounds());

				result->addView(item);
			}

			// Footer
			//composedView->addView(new brogueText(eventController, footerData, sceneBounds, footerData.getBounds()));

			//result->addView(composedView);

			return result;
		}

		static brogueViewContainer* createHighScoresView(eventController* eventController, resourceController* resourceController, int zoomLevel)
		{
			gridRect sceneBounds = getBrogueSceneBoundary();

			brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::HighScoresProgram, false, false, sceneBounds);
			brogueUIProgramPartId composedId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::ViewCompositor, 0);
			brogueUIData composedViewData(composedId, sceneBounds, zoomLevel);
			brogueComposedView* composedView = new brogueComposedView(eventController, composedViewData, sceneBounds, sceneBounds);

			// Pulled from Brogue v1.7.5
			color menuColor(0, 0, 0, 0.85);
			color footerColor(0.596f, 0.494f, 0.996f, 1);
			int menuPadding = 2;

			// Menu Background
			brogueUIProgramPartId menuId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Background, 0);
			brogueUIData menuData(menuId, sceneBounds, zoomLevel, menuColor);

			// Padding
			menuData.setUIParameters(-1, -1, "", brogueUIAction::ViewMainMenu, true, zoomLevel, 0);

			// Header
			gridRect headerBounds(sceneBounds.column, sceneBounds.row, sceneBounds.width, 1);
			brogueUIProgramPartId headerId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, 0);
			brogueUIData headerData(headerId, headerBounds, zoomLevel, colorString("-- High Scores --", colors::yellow()), menuColor, brogueTextAlignment::Center);

			headerData.setUIParameters(-1, -1, "", brogueUIAction::None, false, zoomLevel, 1);

			simpleList<brogueUIData> items;

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

				brogueUIProgramPartId dataId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, index + 1);
				brogueUIData data(dataId, boundary, zoomLevel, colorString(formattedEntry.c_str(), foreGround), menuColor, brogueTextAlignment::Center);

				data.setUIParameters(-1, -1, "", brogueUIAction::None, true, zoomLevel, 1);

				items.add(data);
			}

			// Footer
			gridRect footerBounds(sceneBounds.column, sceneBounds.height - 1, sceneBounds.width, 1);
			brogueUIProgramPartId footerId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, items.count() + 2);
			brogueUIData footerData(footerId, footerBounds, zoomLevel, colorString("Press space to continue.", footerColor), menuColor, brogueTextAlignment::Center);

			footerData.setUIParameters(-1, -1, "", brogueUIAction::None, false, zoomLevel, 1);

			// Finally, create the views
			brogueBackground* background = new brogueBackground(eventController, menuData, sceneBounds, sceneBounds);
			brogueText* header = new brogueText(eventController, headerData, sceneBounds, headerBounds);
			brogueText* footer = new brogueText(eventController, footerData, sceneBounds, footerBounds);

			for (int index = 0; index < items.count(); index++)
			{
				brogueText* item = new brogueText(eventController, items.get(index), sceneBounds, items.get(index).getBounds());

				composedView->addView(item);
			}

			composedView->addView(background);
			composedView->addView(header);
			composedView->addView(footer);

			result->addView(composedView);

			return result;
		}

		static brogueViewContainer* createGameView(eventController* eventController, resourceController* resourceController, int zoomLevel)
		{
			//gridRect sceneBounds = getBrogueSceneBoundary();
			//gridRect sidebarBounds = getLeftSidebarBoundary();
			//gridRect bottomBarBounds = getBottomBarBoundary();
			//gridRect flavorTextBounds = getFlavorTextBoundary();
			//gridRect gameBounds = getGameBoundary();

			//brogueUIData* backgroundData = new brogueUIData(sceneBounds, zoomLevel, colors::black());
			//brogueUIData* sidebarData = new brogueUIData(sceneBounds, zoomLevel, colors::blue());
			//brogueUIData* bottomBarData = new brogueUIData(sceneBounds, zoomLevel, colors::green());
			//brogueUIData* flavorTextData = new brogueUIData(sceneBounds, zoomLevel, colors::white());
			//brogueUIData* gameData = new brogueUIData(sceneBounds, zoomLevel, colors::getGray(0.5));

			//brogueView* background = new brogueView(eventController, backgroundData, sceneBounds, sceneBounds);

			//brogueView* sidebar = new brogueView(eventController, sidebarData, sceneBounds, sidebarBounds);
			//brogueView* bottomBar = new brogueView(eventController, bottomBarData, sceneBounds, bottomBarBounds);
			//brogueView* flavorText = new brogueView(eventController, flavorTextData, sceneBounds, flavorTextBounds);
			//brogueView* gameView = new brogueView(eventController, gameData, sceneBounds, gameBounds);

			//brogueViewContainer* container = new brogueViewContainer(background, brogueUIView::GameView, sceneBounds, sceneBounds);

			//container->addView(sidebar);
			//container->addView(bottomBar);
			//container->addView(flavorText);
			//container->addView(gameView);

			//return container;

			return nullptr;
		}
	};
}
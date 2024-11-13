#pragma once
#include "brogueBackground.h"
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
#include "simple.h"
#include "simpleDirectoryEntry.h"
#include "simpleException.h"
#include "simpleExt.h"
#include "simpleFileEntry.h"
#include "simpleFileIO.h"
#include "simpleList.h"
#include "simpleOrderedList.h"
#include "simpleString.h"

using namespace brogueHd::simple;
using namespace brogueHd::backend::model;

namespace brogueHd::frontend
{
	class brogueUIBuilder
	{
	public:

		brogueUIBuilder(eventController* eventController, resourceController* resourceController, randomGenerator* randomGenerator, int zoomLevel);
		~brogueUIBuilder();

		/// <summary>
		/// Defines the primary scene boundary for the brogue game
		/// </summary>
		gridRect getBrogueSceneBoundary();
		gridRect getPaddedBoundary(const gridRect& boundary, int padding);

		brogueViewContainer* buildProgramView(brogueUIProgram programName);

	protected:

		const int FlamePeriodFadeMilliseconds = 150;

	private:

		gridRect getBrogueStaticBoundary(const brogueUIProgramPartId& partId);
		gridRect getBrogueDynamicBoundary(const brogueUIProgramPartId& partId, int numberOfItems);

		brogueViewContainer* createFlameMenu(brogueUIProgram programName);
		brogueViewContainer* createMainMenu();
		brogueViewContainer* createMainMenuSelector(brogueUIProgram programName, const simpleDirectoryEntry& files);
		brogueViewContainer* createHighScoresView();
		brogueViewContainer* createHeaderedBackground(brogueUIProgram programName, const colorString& headerText);
		brogueViewContainer* createGameView(eventController* eventController, resourceController* resourceController, int zoomLevel);

	private:

		eventController* _eventController;
		resourceController* _resourceController;
		randomGenerator* _randomGenerator;
		int _zoomLevel;
	};

	brogueUIBuilder::brogueUIBuilder(eventController* eventController, resourceController* resourceController, randomGenerator* randomGenerator, int zoomLevel)
	{
		_eventController = eventController;
		_resourceController = resourceController;
		_randomGenerator = randomGenerator;
		_zoomLevel = zoomLevel;
	}
	brogueUIBuilder::~brogueUIBuilder() {}

	gridRect brogueUIBuilder::getBrogueSceneBoundary()
	{
		return gridRect(0, 0, COLS, ROWS);
	}

	gridRect brogueUIBuilder::getPaddedBoundary(const gridRect& boundary, int padding)
	{
		return gridRect(boundary.column + padding,
						boundary.row + padding,
						boundary.width - (padding * 2),
						boundary.height - (padding * 2));
	}

	brogueViewContainer* brogueUIBuilder::buildProgramView(brogueUIProgram programName)
	{
		switch (programName)
		{
			case brogueUIProgram::FlameMenuProgram1:
				return createFlameMenu(programName);
			case brogueUIProgram::FlameMenuProgram2:
				return createFlameMenu(programName);
			case brogueUIProgram::MainMenuProgram:
				return createMainMenu();
			case brogueUIProgram::OpenMenuProgram:
			{
				// Read the saved games into the display
				simpleDirectoryEntry directory = simpleFileIO::readDirectory(_resourceController->getGamesDirectory()->c_str(), ".broguesave");

				// Creates scrollable view container
				return createMainMenuSelector(programName, directory);
			}
			case brogueUIProgram::OpenMenuBackgroundProgram:
				return createHeaderedBackground(programName, colorString("~ Open Saved Game ~", colors::yellow()));
			case brogueUIProgram::PlaybackMenuProgram:
			{
				// Read the saved games into the display
				simpleDirectoryEntry directory = simpleFileIO::readDirectory(_resourceController->getPlaybackDirectory()->c_str(), ".broguerec");

				// Creates scrollable view container
				return createMainMenuSelector(programName, directory);
			}
			case brogueUIProgram::PlaybackMenuBackgroundProgram:
				return createHeaderedBackground(programName, colorString("~ View Recording ~", colors::yellow()));
			case brogueUIProgram::HighScoresProgram:
				return createHighScoresView();
			case brogueUIProgram::ModalProgram:
			case brogueUIProgram::BottomBarMenuProgram:
			case brogueUIProgram::PlayerStatusPanelProgram:
			case brogueUIProgram::GameObjectListProgram:
			case brogueUIProgram::FlavorTextPanelProgram:
			case brogueUIProgram::GameProgram:
			case brogueUIProgram::GameMenuProgram:
			case brogueUIProgram::GameInventoryProgram:
			case brogueUIProgram::GameHoverTextProgram:
			case brogueUIProgram::GameLogProgram:
			case brogueUIProgram::GameHelpProgram:
			case brogueUIProgram::GameDiscoveredItems:
			case brogueUIProgram::ContainerControlledProgram:
			default:
				throw simpleException("Unhandled program type:  brogueUIBuilder::buildProgramView");
		}
	}

	gridRect brogueUIBuilder::getBrogueStaticBoundary(const brogueUIProgramPartId& partId)
	{
		switch (partId.getPartName())
		{
			case brogueUIProgramPart::MenuBackground:
			case brogueUIProgramPart::Background:
				switch (partId.getName())
				{
					case brogueUIProgram::MainMenuProgram:
						return gridRect(COLS - 26, ROWS - 12, 24, 11);
					case brogueUIProgram::OpenMenuProgram:
					case brogueUIProgram::PlaybackMenuProgram:
					{
						int width = COLS / 2;
						int height = ROWS / 2;

						gridRect result((COLS - width) / 2, (ROWS - height) / 2, width, height);

						// Padding (this should be the scrollable contents' clipping boundary - which is static)
						return getPaddedBoundary(result, 1);
					}
					case brogueUIProgram::OpenMenuBackgroundProgram:
					case brogueUIProgram::PlaybackMenuBackgroundProgram:
					{
						int width = COLS / 2;
						int height = ROWS / 2;

						return gridRect((COLS - width) / 2, (ROWS - height) / 2, width, height);
					}
					case brogueUIProgram::HighScoresProgram:
						return getBrogueSceneBoundary();
					case brogueUIProgram::BottomBarMenuProgram:
						return gridRect(COLS - DCOLS, ROWS - 1, DCOLS, 1);
					case brogueUIProgram::PlayerStatusPanelProgram:
						return gridRect(0, 0, COLS - DCOLS, ROWS - 1);
					case brogueUIProgram::GameObjectListProgram:
						return gridRect(0, 0, COLS - DCOLS, ROWS - DROWS);
					case brogueUIProgram::FlavorTextPanelProgram:
						return gridRect(COLS - DCOLS, ROWS - 2, DCOLS, 1);
					case brogueUIProgram::GameProgram:
						return gridRect(COLS - DCOLS, ROWS - DROWS, DCOLS, DROWS);
					case brogueUIProgram::GameHelpProgram:
					case brogueUIProgram::GameDiscoveredItems:
						return gridRect(COLS - DCOLS, 0, DCOLS, ROWS);
					case brogueUIProgram::GameMenuProgram:
						return gridRect(COLS - 30 - 3, ROWS - 16, 30, 16);

						// Dynamically Sized
					case brogueUIProgram::GameInventoryProgram:
					case brogueUIProgram::GameHoverTextProgram:
					case brogueUIProgram::GameLogProgram:
					case brogueUIProgram::ModalProgram:
					default:
						throw simpleException("Unhandled brogueUIProgram part boundary:  brogueUIBuilder::getBrogueBoundary");
				}
				break;
			case brogueUIProgramPart::FlameMenuProgram_HeatDiffuseProgram:
				return getBrogueSceneBoundary();
			case brogueUIProgramPart::FlameMenuProgram_HeatSourceProgram:
				return getBrogueSceneBoundary();
			case brogueUIProgramPart::FlameMenuProgram_TitleMaskProgram:
				return getBrogueSceneBoundary();
			case brogueUIProgramPart::Button:
			case brogueUIProgramPart::Text:
			case brogueUIProgramPart::ViewCompositor:
			default:
				throw simpleException("Unhandled brogueUIProgram part boundary:  brogueUIBuilder::getBrogueBoundary");
		}
	}

	gridRect brogueUIBuilder::getBrogueDynamicBoundary(const brogueUIProgramPartId& partId, int numberOfItems)
	{
		switch (partId.getPartName())
		{
			case brogueUIProgramPart::MenuBackground:
			case brogueUIProgramPart::Background:
				switch (partId.getName())
				{
					// Item boundary of the view container for the items of these menus
					case brogueUIProgram::OpenMenuProgram:
					case brogueUIProgram::PlaybackMenuProgram:
					case brogueUIProgram::OpenMenuBackgroundProgram:
					case brogueUIProgram::PlaybackMenuBackgroundProgram:
					{
						int width = COLS / 2;
						int height = ROWS / 2;

						return gridRect((COLS - width) / 2, (ROWS - height) / 2, width, numberOfItems);
					}

					// Dynamically Sized
					case brogueUIProgram::GameObjectListProgram:
					case brogueUIProgram::GameInventoryProgram:
						return gridRect(COLS - 34, 3, 34, numberOfItems);
					case brogueUIProgram::GameHoverTextProgram:
					case brogueUIProgram::GameLogProgram:
						return gridRect(COLS - DCOLS, 0, DCOLS, numberOfItems);
					case brogueUIProgram::ModalProgram:
					default:
						throw simpleException("Unhandled brogueUIProgram part dynamic boundary:  brogueUIBuilder::getBrogueBoundary");
				}
				break;
			default:
				throw simpleException("Unhandled brogueUIProgram part dynamic boundary:  brogueUIBuilder::getBrogueBoundary");
		}
	}

	brogueViewContainer* brogueUIBuilder::createFlameMenu(brogueUIProgram programName)
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId titleId(programName, brogueUIProgramPart::FlameMenuProgram_TitleMaskProgram, 0);
		brogueUIProgramPartId heatId(programName, brogueUIProgramPart::FlameMenuProgram_HeatSourceProgram, 0);
		brogueUIProgramPartId diffuseId(programName, brogueUIProgramPart::FlameMenuProgram_HeatDiffuseProgram, 0);

		brogueUIData titleMaskData(titleId, sceneBounds, _zoomLevel);
		brogueUIData heatData(heatId, sceneBounds, _zoomLevel);
		brogueUIData diffuseData(diffuseId, sceneBounds, _zoomLevel);

		// Mouse Interaction:  This will deactivate the open menu selectors
		heatData.setUIParameters(-1, -1, "", brogueUIAction::None, true, _zoomLevel, 0);

		if (programName == brogueUIProgram::FlameMenuProgram2)
		{
			brogueFlameMenuTitleMask* titleMask = new brogueFlameMenuTitleMask(programName == brogueUIProgram::FlameMenuProgram2, _eventController, titleMaskData, sceneBounds, sceneBounds);
			brogueFlameMenuHeatView* heatView = new brogueFlameMenuHeatView(programName == brogueUIProgram::FlameMenuProgram2, _eventController, _randomGenerator, this->FlamePeriodFadeMilliseconds, heatData, sceneBounds, sceneBounds);
			brogueFlameMenuDiffuseView* heatDiffuseView = new brogueFlameMenuDiffuseView(programName == brogueUIProgram::FlameMenuProgram2, _eventController, diffuseData, sceneBounds, sceneBounds);

			brogueViewContainer* result = new brogueViewContainer(programName, false, false, sceneBounds);

			result->addView(heatView);
			result->addView(heatDiffuseView);
			result->addView(titleMask);

			return result;
		}
		else
		{
			brogueFlameMenuHeatView* heatView = new brogueFlameMenuHeatView(programName == brogueUIProgram::FlameMenuProgram2, _eventController, _randomGenerator, this->FlamePeriodFadeMilliseconds, heatData, sceneBounds, sceneBounds);
			brogueFlameMenuDiffuseView* heatDiffuseView = new brogueFlameMenuDiffuseView(programName == brogueUIProgram::FlameMenuProgram2, _eventController, diffuseData, sceneBounds, sceneBounds);

			brogueViewContainer* result = new brogueViewContainer(programName, false, false, sceneBounds);

			result->addView(heatView);
			result->addView(heatDiffuseView);

			return result;
		}
	}

	brogueViewContainer* brogueUIBuilder::createMainMenu()
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

		brogueUIProgramPartId menuId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::MenuBackground, 0);

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect menuBounds = getBrogueStaticBoundary(menuId);
		gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::MainMenuProgram, false, false, menuBounds);

		// Menu Background
		brogueUIData menuData(menuId, menuBounds, _zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);

		menuData.setUIParameters(-1, -1, "", brogueUIAction::None, false, _zoomLevel, 0);

		simpleList<brogueUIData> buttons;
		int buttonPadding = 1;

		for (int index = 0; index < 5; index++)
		{
			gridRect boundary(paddedBounds.column + buttonPadding,
								paddedBounds.row + ((index * buttonPadding) + ((index - 1) * buttonPadding)) + 1,
								paddedBounds.width - (2 * buttonPadding),
								1);

			brogueUIProgramPartId buttonId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::Button, index);

			if (index == 0)
			{
				brogueUIData buttonData(buttonId, boundary, _zoomLevel, colorString("New Game"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
				buttonData.setUIParameters(0, -1, " ", brogueUIAction::NewGame, true, _zoomLevel, 1);
				buttons.add(buttonData);
			}
			else if (index == 1)
			{
				brogueUIData buttonData(buttonId, boundary, _zoomLevel, colorString("Open Game"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
				buttonData.setUIParameters(0, -1, " ", brogueUIAction::OpenGame, true, _zoomLevel, 1);
				buttons.add(buttonData);
			}
			else if (index == 2)
			{
				brogueUIData buttonData(buttonId, boundary, _zoomLevel, colorString("View Recording"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
				buttonData.setUIParameters(0, -1, " ", brogueUIAction::ViewPlaybackMenu, true, _zoomLevel, 1);
				buttons.add(buttonData);
			}
			else if (index == 3)
			{
				brogueUIData buttonData(buttonId, boundary, _zoomLevel, colorString("High Scores"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
				buttonData.setUIParameters(0, -1, " ", brogueUIAction::ViewHighScores, true, _zoomLevel, 1);
				buttons.add(buttonData);
			}
			else if (index == 4)
			{
				brogueUIData buttonData(buttonId, boundary, _zoomLevel, colorString("Quit"), gradient1, gradient2, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Center);
				buttonData.setUIParameters(0, -1, " ", brogueUIAction::ShowQuitGameModal, true, _zoomLevel, 1);
				buttons.add(buttonData);
			}
		}

		// Finally, create the views
		brogueBackground* background = new brogueBackground(_eventController, menuData, sceneBounds, menuBounds);

		result->addView(background);

		for (int index = 0; index < buttons.count(); index++)
		{
			brogueButton* item = new brogueButton(_eventController, buttons.get(index), sceneBounds, buttons.get(index).getBounds());

			result->addView(item);
		}

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createMainMenuSelector(brogueUIProgram programName, const simpleDirectoryEntry& files)
	{
		// Pulled from Brogue v1.7.5
		color menuColor1(0.067, 0.059, 0.149, 0.8f);
		color menuColor2(0.067, 0.059, 0.149, 0.8f);
		color itemColor1(0, 0, 0, 0);
		color itemColor2(0, 0, 0, 0);
		color active1(0.3, 0.3, 0.3, 0.4);
		color active2(0.3, 0.3, 0.3, 0.4);
		color pressed1(0.3, 0.3, 0.3, 0.8);
		color pressed2(0.3, 0.3, 0.3, 0.8);

		brogueUIProgramPartId backgroundId(programName, brogueUIProgramPart::MenuBackground, 0);

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect menuBounds = getBrogueStaticBoundary(backgroundId);
		gridRect itemBounds(menuBounds);

		simpleList<brogueUIData> buttons;
		simpleOrderedList<simpleFileEntry*> filesSorted = files.sort();

		for (int index = 0; index < filesSorted.count(); index++)
		{
			gridRect boundary(menuBounds.column, menuBounds.row + index, menuBounds.width, 1);

			simpleString fileName = *(filesSorted.get(index)->getFileNameWithoutExtension());
			simpleString fileNameTrimmed;

			if (fileName.count() > ((menuBounds.width / 2.0f) - 3))
				fileNameTrimmed = fileName.subString(0, (menuBounds.width / 2.0f) - 3);

			simpleString result = "";

			if (fileNameTrimmed.count() > 0)
			{
				result.append(fileNameTrimmed);
				result.append("...");
			}
			else
			{
				result.append(fileName);
			}

			int space = menuBounds.width - result.count() - filesSorted.get(index)->getWriteTimeShort()->count() - 1;
			result.appendPadding(' ', space);
			result.append(*(filesSorted.get(index)->getWriteTimeShort()));

			brogueUIProgramPartId dataPartId(programName, brogueUIProgramPart::Button, index);
			brogueUIData data(dataPartId, boundary, _zoomLevel, colorString(result.c_str(), colors::white()), itemColor1, itemColor1, active1, active2, pressed1, pressed2, brogueGradientType::Horizontal, brogueTextAlignment::Left);

			// Expand total item container boundary for the background
			itemBounds.expand(boundary);

			// Hotkey is the first letter a), b), etc..
			data.setUIParameters(-1, -1, *(filesSorted.get(index)->getFileFullPath()), brogueUIAction::OpenGame, true, _zoomLevel, 1);

			buttons.add(data);
		}

		// Finally, create the views. The view container will handle the scroll and clipping behavior.
		//
		brogueViewContainer* result = new brogueViewContainer(programName, true, true, menuBounds);
			
		// This background will take the color of the menu while the scroll is applied; and carries the item dimensions.
		//
		brogueUIData backgroundData(backgroundId, itemBounds, _zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);
		backgroundData.setUIParameters(-1, -1, "", brogueUIAction::None, false, _zoomLevel, 1);
		brogueBackground* background = new brogueBackground(_eventController, backgroundData, sceneBounds, itemBounds);

		result->addView(background);

		// Buttons
		for (int index = 0; index < buttons.count(); index++)
		{
			brogueButton* item = new brogueButton(_eventController, buttons.get(index), sceneBounds, buttons.get(index).getBounds());

			result->addView(item);
		}

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createHighScoresView()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::HighScoresProgram, false, false, sceneBounds);
		brogueUIProgramPartId composedId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::ViewCompositor, 0);
		brogueUIData composedViewData(composedId, sceneBounds, _zoomLevel);
		brogueComposedView* composedView = new brogueComposedView(_eventController, composedViewData, sceneBounds, sceneBounds);

		// Pulled from Brogue v1.7.5
		color menuColor(0, 0, 0, 0.85);
		color footerColor(0.596f, 0.494f, 0.996f, 1);
		int menuPadding = 2;

		// Menu Background
		brogueUIProgramPartId menuId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Background, 0);
		brogueUIData menuData(menuId, sceneBounds, _zoomLevel, menuColor);

		// Padding
		menuData.setUIParameters(-1, -1, "", brogueUIAction::ViewMainMenu, true, _zoomLevel, 0);

		// Header
		gridRect headerBounds(sceneBounds.column, sceneBounds.row, sceneBounds.width, 1);
		brogueUIProgramPartId headerId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, 0);
		brogueUIData headerData(headerId, headerBounds, _zoomLevel, colorString("-- High Scores --", colors::yellow()), menuColor, brogueTextAlignment::Center);

		headerData.setUIParameters(-1, -1, "", brogueUIAction::None, false, _zoomLevel, 1);

		simpleList<brogueUIData> items;

		for (int index = 0; index < _resourceController->getHighScoreCount(); index++)
		{
			brogueScoreEntry* entry = _resourceController->getHighScore(index);

			gridRect boundary(sceneBounds.column, sceneBounds.row + index + menuPadding, sceneBounds.width, 1);

			simpleString preamble = simpleExt::format("{}) ", index + 1);
			simpleString score = simpleExt::toString(entry->getScore());
			simpleString dateAndDescr = simpleExt::format("{} {}", entry->getDate()->c_str(), entry->getDescription()->c_str());

			// Size to line up scores (5)
			preamble.appendPadding(' ', 5 - preamble.count());

			// Scores (15)
			score.appendPadding(' ', 15 - score.count() - preamble.count());

			simpleString formattedEntry = simpleExt::format("{}{}{}", preamble.c_str(), score.c_str(), dateAndDescr.c_str());

			color foreGround = color::interpolate(colors::white(), colors::black(), index / (float)_resourceController->getHighScoreCount());

			brogueUIProgramPartId dataId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, index + 1);
			brogueUIData data(dataId, boundary, _zoomLevel, colorString(formattedEntry.c_str(), foreGround), menuColor, brogueTextAlignment::Center);

			data.setUIParameters(-1, -1, "", brogueUIAction::None, true, _zoomLevel, 1);

			items.add(data);
		}

		// Footer
		gridRect footerBounds(sceneBounds.column, sceneBounds.height - 1, sceneBounds.width, 1);
		brogueUIProgramPartId footerId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, items.count() + 2);
		brogueUIData footerData(footerId, footerBounds, _zoomLevel, colorString("Press space to continue.", footerColor), menuColor, brogueTextAlignment::Center);

		footerData.setUIParameters(-1, -1, "", brogueUIAction::None, false, _zoomLevel, 1);

		// Finally, create the views
		brogueBackground* background = new brogueBackground(_eventController, menuData, sceneBounds, sceneBounds);
		brogueText* header = new brogueText(_eventController, headerData, sceneBounds, headerBounds);
		brogueText* footer = new brogueText(_eventController, footerData, sceneBounds, footerBounds);

		for (int index = 0; index < items.count(); index++)
		{
			brogueText* item = new brogueText(_eventController, items.get(index), sceneBounds, items.get(index).getBounds());

			composedView->addView(item);
		}

		composedView->addView(background);
		composedView->addView(header);
		composedView->addView(footer);

		result->addView(composedView);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createHeaderedBackground(brogueUIProgram programName, const colorString& headerText)
	{
		// Pulled from Brogue v1.7.5
		color menuColor1(0.067, 0.059, 0.149, 0.8f);
		color menuColor2(0.067, 0.059, 0.149, 0.8f);

		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::MenuBackground, 0);

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(menuId);

		// No clipping, or scrolling, of the resulting composed view
		brogueViewContainer* result = new brogueViewContainer(programName, false, false, boundary);

		// Composed View:  Takes background and header and combines the view w/ granular interactions - as
		//				   a SINGLE shader program. (updates all at once; but handles mouse interactions)
		//
		brogueUIProgramPartId composedId(programName, brogueUIProgramPart::ViewCompositor, 0);
		brogueUIData composedViewData(composedId, sceneBounds, _zoomLevel, menuColor1);
		brogueComposedView* composedView = new brogueComposedView(_eventController, composedViewData, sceneBounds, boundary);

		// Menu Background
		brogueUIData menuData(menuId, boundary, _zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);
		menuData.setUIParameters(-1, -1, "", brogueUIAction::None, false, _zoomLevel, 0);

		// Header
		gridRect headerBounds(boundary.column, boundary.row, boundary.width, 1);
		brogueUIProgramPartId headerId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, 0);
		brogueUIData headerData(headerId, headerBounds, _zoomLevel, headerText, menuColor1, brogueTextAlignment::Center);
		headerData.setUIParameters(-1, -1, "", brogueUIAction::None, false, _zoomLevel, 1);

		composedView->addView(new brogueBackground(_eventController, menuData, sceneBounds, boundary));
		composedView->addView(new brogueText(_eventController, headerData, sceneBounds, headerBounds));

		// View Container:  Just composed view
		result->addView(composedView);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGameView(eventController* eventController, resourceController* resourceController, int zoomLevel)
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
}
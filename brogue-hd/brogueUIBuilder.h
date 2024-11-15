#pragma once
#include "brogueBackground.h"
#include "brogueButton.h"
#include "brogueComposedView.h"
#include "brogueFlameMenuHeatView.h"
#include "brogueFlameMenuTitleMask.h"
#include "brogueGameLogContainer.h"
#include "brogueGlobal.h"
#include "brogueScoreEntry.h"
#include "brogueText.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueUITagAction.h"
#include "brogueViewContainer.h"
#include "color.h"
#include "colorGradient.h"
#include "colorString.h"
#include "eventController.h"
#include "gridLocator.h"
#include "gridRect.h"
#include "randomGenerator.h"
#include "resourceController.h"
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

		const int FlamePeriodFadeMilliseconds = 1500;

	private:

		gridRect getBrogueStaticBoundary(const brogueUIProgramPartId& partId);
		gridRect getBrogueDynamicBoundary(const brogueUIProgramPartId& partId, int numberOfItems);

		brogueViewContainer* createFlameMenu();
		brogueViewContainer* createFlameMenuTitleMask();
		brogueViewContainer* createMainMenu();
		brogueViewContainer* createMainMenuSelector(brogueUIProgram programName, const simpleDirectoryEntry& files);
		brogueViewContainer* createBottomMenuBar();
		brogueViewContainer* createHighScoresView();
		brogueViewContainer* createFlavorText();
		brogueViewContainer* createGameObjectList();
		brogueViewContainer* createGameLog();
		brogueViewContainer* createHeaderedBackground(brogueUIProgram programName, const colorString& headerText);
		brogueViewContainer* createPlaceholder(brogueUIProgram programName, const gridRect& boundary);

	private:

		brogueBackground* createMenuBackground(brogueUIProgram programName, const gridRect& boundary);
		brogueBackground* createBackground(brogueUIProgram programName, const gridRect& boundary);
		brogueButton* createMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, brogueUIAction action);
		brogueButton* createMenuButton(brogueUIProgram programName, const gridRect& boundary, const brogueUITagAction& action, const simpleString& text, int hotkeyIndex);
		brogueButton* createButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, brogueUIAction action, const simpleString& tag);
		brogueText* createText(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, const color& foreground, brogueTextAlignment alignment);

	private:	// These counters will make it a little easier to keep things separate w/o yet another constructor variable

		int _menuBackgroundCounter;
		int _backgroundCounter;
		int _buttonCounter;
		int _textCounter;

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

		_menuBackgroundCounter = 0;
		_backgroundCounter = 0;
		_buttonCounter = 0;
		_textCounter = 0;
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
			case brogueUIProgram::FlameMenuProgram:
				return createFlameMenu();
			case brogueUIProgram::FlameMenuTitleMaskProgram:
				return createFlameMenuTitleMask();
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
			case brogueUIProgram::BottomBarMenuProgram:
				return createBottomMenuBar();
			case brogueUIProgram::FlavorTextPanelProgram:
				return createFlavorText();
			case brogueUIProgram::GameObjectListProgram:
				return createGameObjectList();
			case brogueUIProgram::GameLogProgram:
				return createGameLog();
			case brogueUIProgram::ModalProgram:
			case brogueUIProgram::GameProgram:
			case brogueUIProgram::GameHelpProgram:
			case brogueUIProgram::GameDiscoveredItems:
			case brogueUIProgram::GameMenuProgram:
			{
				brogueUIProgramPartId partId(programName, brogueUIProgramPart::Background, 0);
				gridRect boundary = getBrogueStaticBoundary(partId);
				return createPlaceholder(programName, boundary);
			}
			case brogueUIProgram::GameInventoryProgram:
			case brogueUIProgram::GameHoverTextProgram:
			{
				brogueUIProgramPartId partId(programName, brogueUIProgramPart::Background, 0);
				gridRect boundary = getBrogueDynamicBoundary(partId, 3);
				return createPlaceholder(programName, boundary);
			}
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
						return gridRect(0, ROWS - 1, COLS, 1);
					case brogueUIProgram::GameObjectListProgram:
						return gridRect(0, 0, COLS - DCOLS, ROWS - 1);
					case brogueUIProgram::FlavorTextPanelProgram:
						return gridRect(COLS - DCOLS, ROWS - 2, DCOLS, 1);
					case brogueUIProgram::GameProgram:
						return gridRect(COLS - DCOLS, ROWS - DROWS, DCOLS, DROWS - 2);
					case brogueUIProgram::GameHelpProgram:
					case brogueUIProgram::GameDiscoveredItems:
						return gridRect(COLS - DCOLS, 0, DCOLS, ROWS);
					case brogueUIProgram::GameMenuProgram:
						return gridRect(COLS - 30 - 3, ROWS - 16, 30, 16);
					case brogueUIProgram::GameLogProgram:
						return gridRect(COLS - DCOLS, 0, DCOLS, ROWS);

						// Dynamically Sized
					case brogueUIProgram::GameInventoryProgram:
					case brogueUIProgram::GameHoverTextProgram:
					case brogueUIProgram::ModalProgram:
					default:
						throw simpleException("Unhandled brogueUIProgram part boundary:  brogueUIBuilder::getBrogueBoundary");
				}
				break;
			case brogueUIProgramPart::FlameDisplay:
				return getBrogueSceneBoundary();
			case brogueUIProgramPart::ColorMask:
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

	brogueViewContainer* brogueUIBuilder::createFlameMenu()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId heatId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameDisplay, 0);
		brogueUIData heatData(heatId, sceneBounds, _zoomLevel);
		brogueUITagAction action(brogueUIState::MainMenu);

		// Mouse Interaction:  This will deactivate the open menu selectors
		heatData.setUIParameters(action, -1, true, _zoomLevel, 0);

		brogueFlameMenuHeatView* heatView = new brogueFlameMenuHeatView(_eventController, _randomGenerator, this->FlamePeriodFadeMilliseconds, heatData, sceneBounds, sceneBounds);
		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::FlameMenuProgram, false, false, sceneBounds);

		result->addView(heatView);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createFlameMenuTitleMask()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId titleId(brogueUIProgram::FlameMenuTitleMaskProgram, brogueUIProgramPart::ColorMask, 0);
		brogueUIData titleMaskData(titleId, sceneBounds, _zoomLevel);
		brogueFlameMenuTitleMask* titleMask = new brogueFlameMenuTitleMask(_eventController, titleMaskData, sceneBounds, sceneBounds);

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::FlameMenuTitleMaskProgram, false, false, sceneBounds);

		result->addView(titleMask);
		return result;
	}

	brogueViewContainer* brogueUIBuilder::createMainMenu()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect menuBounds = getBrogueStaticBoundary(brogueUIProgramPartId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::MenuBackground, 0));
		gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::MainMenuProgram, false, false, menuBounds);

		int buttonPadding = 1;

		result->addView(createMenuBackground(brogueUIProgram::MainMenuProgram, menuBounds));

		for (int index = 0; index < 5; index++)
		{
			gridRect boundary(paddedBounds.column + buttonPadding,
							  paddedBounds.row + ((index * buttonPadding) + ((index - 1) * buttonPadding)) + 1,
							  paddedBounds.width - (2 * buttonPadding), 1);

			simpleString text = index == 0 ? "New Game" : index == 1 ? "Open Game" : index == 2 ? "View Recording" : index == 3 ? "High Scores" : "Quit";

			brogueUIState state = index == 1 ? brogueUIState::OpenMenu :
								  index == 2 ? brogueUIState::PlaybackMenu :
								  index == 3 ? brogueUIState::HighScores : brogueUIState::Modal;

			brogueUITagAction tagAction = index == 0 ? brogueUITagAction(brogueUIAction::NewGame, "") : brogueUITagAction(state);

			brogueButton* button = createMenuButton(brogueUIProgram::MainMenuProgram, boundary, tagAction, text, 0);

			result->addView(button);
		}

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createMainMenuSelector(brogueUIProgram programName, const simpleDirectoryEntry& files)
	{
		brogueUIProgramPartId backgroundId(programName, brogueUIProgramPart::MenuBackground, 0);

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect menuBounds = getBrogueStaticBoundary(backgroundId);
		gridRect itemBounds(menuBounds);

		simpleList<brogueButton*> buttons;
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

			// Expand total item container boundary for the background
			itemBounds.expand(boundary);

			buttons.add(createButton(programName, boundary, result, -1, brogueUIAction::OpenGame, *(filesSorted.get(index)->getFileFullPath())));
		}

		// Finally, create the views. The view container will handle the scroll and clipping behavior.
		//
		brogueViewContainer* result = new brogueViewContainer(programName, true, true, menuBounds);

		// This background will take the color of the menu while the scroll is applied; and carries the item dimensions.
		//
		result->addView(createBackground(programName, itemBounds));

		// Buttons
		for (int index = 0; index < buttons.count(); index++)
		{
			result->addView(buttons.get(index));
		}

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createBottomMenuBar()
	{
		int buttonWidth = DCOLS / 5;
		int buttonSpacing = 2;
		int column = COLS - DCOLS;

		gridRect depthBounds(0, ROWS - 1, COLS - DCOLS, 1);

		gridRect button1(column + 1, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button2(column + 1, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button3(column + 1, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button4(column + 1, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button5(column + 1, ROWS - 1, buttonWidth - 2, 1);

		brogueText* depthText = createText(brogueUIProgram::BottomBarMenuProgram, depthBounds, "-- Depth 1 --", colors::white(), brogueTextAlignment::Center);
		brogueButton* exploreButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button1, "Explore", 1, brogueUIAction::GameCommand_Explore);
		brogueButton* restButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button2, "Rest (z)", 6, brogueUIAction::GameCommand_Rest);
		brogueButton* searchButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button3, "Search (s)", 8, brogueUIAction::GameCommand_Search);
		brogueButton* menuButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button4, "Menu", -1, brogueUIAction::GameCommand_Menu);
		brogueButton* inventoryButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button5, "Inventory", 0, brogueUIAction::GameCommand_Inventory);

		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(brogueUIProgram::BottomBarMenuProgram, brogueUIProgramPart::Background, 0));

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::BottomBarMenuProgram, false, false, boundary);

		result->addView(depthText);
		result->addView(exploreButton);
		result->addView(restButton);
		result->addView(searchButton);
		result->addView(menuButton);
		result->addView(inventoryButton);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createFlavorText()
	{
		brogueUIProgram program = brogueUIProgram::FlavorTextPanelProgram;

		// Brogue v1.7.5
		color foreground(0.464, 0.396, 0.894, 1.0);
		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(program, brogueUIProgramPart::Background, 0));
		brogueText* flavorText = createText(program, boundary, "You see a large Troll...", foreground, brogueTextAlignment::Left);
		brogueViewContainer* result = new brogueViewContainer(program, false, false, boundary);

		result->addView(flavorText);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGameObjectList()
	{
		brogueUIProgram programName = brogueUIProgram::GameObjectListProgram;

		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(programName, brogueUIProgramPart::Background, 0));
		gridRect itemBounds(0, 0, COLS - DCOLS, 1);

		brogueViewContainer* result = new brogueViewContainer(programName, true, false, boundary);

		// Background
		result->addView(createBackground(programName, boundary));

		// Place holders
		result->addView(createText(programName, itemBounds, "@: You (lit)", colors::white(), brogueTextAlignment::Left));
		itemBounds.translate(0, 1);
		result->addView(createText(programName, itemBounds, "Health", colors::red(), brogueTextAlignment::Center));
		itemBounds.translate(0, 1);
		result->addView(createText(programName, itemBounds, "Nutrition", colors::orange(), brogueTextAlignment::Center));
		itemBounds.translate(0, 1);
		result->addView(createText(programName, itemBounds, "Str: 12 Armor: 3", colors::white(), brogueTextAlignment::Center));
		itemBounds.translate(0, 1);
		result->addView(createText(programName, itemBounds, "Stealth Range: 14", colors::white(), brogueTextAlignment::Center));

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGameLog()
	{
		brogueUIProgram program = brogueUIProgram::GameLogProgram;

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(program, brogueUIProgramPart::Background, 0));
		gridRect itemBounds(COLS - DCOLS, 0, DCOLS, 1);

		brogueGameLogContainer* result = new brogueGameLogContainer(program, false, false, boundary);

		brogueUIProgramPartId composedId(program, brogueUIProgramPart::ViewCompositor, 0);
		brogueUIData composedData(composedId, boundary, _zoomLevel, colors::black());
		brogueComposedView* composedView = new brogueComposedView(_eventController, composedData, sceneBounds, boundary);

		for (int index = 0; index < ROWS; index++)
		{
			composedView->addView(createText(program, itemBounds, "The rough stone wall is firm and unyielding", colors::white(), brogueTextAlignment::Left));
			itemBounds.translate(0, 1);
		}

		result->addView(composedView);

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

		simpleList<brogueText*> items;

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

			items.add(createText(brogueUIProgram::HighScoresProgram, boundary, formattedEntry, foreGround, brogueTextAlignment::Center));
		}

		// Header
		gridRect headerBounds(sceneBounds.column, sceneBounds.row, sceneBounds.width, 1);

		// Footer
		gridRect footerBounds(sceneBounds.column, sceneBounds.height - 1, sceneBounds.width, 1);

		// Finally, create the views
		brogueBackground* background = createBackground(brogueUIProgram::HighScoresProgram, sceneBounds);
		brogueText* header = createText(brogueUIProgram::HighScoresProgram, headerBounds, "-- High Scores --", colors::yellow(), brogueTextAlignment::Center);
		brogueText* footer = createText(brogueUIProgram::HighScoresProgram, footerBounds, "Press space to continue", footerColor, brogueTextAlignment::Center);

		composedView->addView(background);

		for (int index = 0; index < items.count(); index++)
		{
			composedView->addView(items.get(index));
		}

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
		menuData.setUIParameters(false, _zoomLevel, 0);

		// Header
		gridRect headerBounds(boundary.column, boundary.row, boundary.width, 1);
		brogueUIProgramPartId headerId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, 0);
		brogueUIData headerData(headerId, headerBounds, _zoomLevel, headerText, menuColor1, brogueTextAlignment::Center);
		headerData.setUIParameters(false, _zoomLevel, 1);

		composedView->addView(new brogueBackground(_eventController, menuData, sceneBounds, boundary));
		composedView->addView(new brogueText(_eventController, headerData, sceneBounds, headerBounds));

		// View Container:  Just composed view
		result->addView(composedView);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createPlaceholder(brogueUIProgram programName, const gridRect& boundary)
	{
		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Background, 0);
		brogueUIData uiData(partId, boundary, _zoomLevel, _randomGenerator->nextColor(colors::blue(), colors::red()));
		brogueBackground* background = new brogueBackground(_eventController, uiData, getBrogueSceneBoundary(), boundary);
		brogueViewContainer* container = new brogueViewContainer(programName, false, false, boundary);

		container->addView(background);

		return container;
	}

	brogueButton* brogueUIBuilder::createMenuButton(brogueUIProgram programName,
													const gridRect& boundary,
													const simpleString& text,
													int hotkeyIndex,
													brogueUIAction action)
	{
		return createMenuButton(programName, boundary, brogueUITagAction(action, ""), text, hotkeyIndex);
	}

	brogueButton* brogueUIBuilder::createMenuButton(brogueUIProgram programName,
													const gridRect& boundary,
													const brogueUITagAction& action,
													const simpleString& text,
													int hotkeyIndex)
	{
		// Pulled from Brogue v1.7.5
		color gradient1(0.22, 0.14, 0.29, 0.8);
		color gradient2(0.11, 0.06, 0.15, 0.8);
		color active1(0.7, 0.7, 0.7, 0.4);
		color active2(0.7, 0.7, 0.7, 0.4);
		color pressed1(0.7, 0.7, 0.7, 0.6);
		color pressed2(0.7, 0.7, 0.7, 0.6);

		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Button, _buttonCounter++);

		brogueUIData buttonData(partId, boundary, _zoomLevel,
								colorString(text.c_str(), colors::white()),
								gradient1, gradient2,
								active1, active2,
								pressed1, pressed2,
								brogueGradientType::Horizontal,
								brogueTextAlignment::Center);

		buttonData.setUIParameters(action, hotkeyIndex, true, _zoomLevel, 1);

		return new brogueButton(_eventController, buttonData, getBrogueSceneBoundary(), boundary);
	}
	brogueButton* brogueUIBuilder::createButton(brogueUIProgram programName,
												const gridRect& boundary,
												const simpleString& text,
												int hotkeyIndex,
												brogueUIAction action,
												const simpleString& tag)
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

		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Button, _buttonCounter++);

		brogueUIData uiData(partId, boundary, _zoomLevel,
							colorString(text.c_str(), colors::white()),
							itemColor1, itemColor1,
							active1, active2,
							pressed1, pressed2,
							brogueGradientType::Horizontal,
							brogueTextAlignment::Left);

		brogueUITagAction tagAction(action, tag);

		uiData.setUIParameters(tagAction, hotkeyIndex, true, _zoomLevel, 1);

		return new brogueButton(_eventController, uiData, getBrogueSceneBoundary(), boundary);
	}
	brogueText* brogueUIBuilder::createText(brogueUIProgram programName, 
											const gridRect& boundary, 
											const simpleString& text, 
											const color& foreground, 
											brogueTextAlignment alignment)
	{
		color menuColor(0, 0, 0, 0);

		brogueUIProgramPartId dataId(programName, brogueUIProgramPart::Text, _textCounter++);

		brogueUIData data(dataId, boundary, _zoomLevel, colorString(text.c_str(), foreground), menuColor, alignment);

		data.setUIParameters(true, _zoomLevel, 1);

		return new brogueText(_eventController, data, getBrogueSceneBoundary(), boundary);
	}

	brogueBackground* brogueUIBuilder::createBackground(brogueUIProgram programName, const gridRect& boundary)
	{
		// Pulled from Brogue v1.7.5
		color menuColor1(0, 0, 0, 1.0f);
		color menuColor2(0, 0, 0, 0.5f);

		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::Background, _menuBackgroundCounter++);

		brogueUIData menuData(menuId, boundary, _zoomLevel, menuColor1);

		menuData.setUIParameters(false, _zoomLevel, 0);

		return new brogueBackground(_eventController, menuData, sceneBounds, boundary);
	}

	brogueBackground* brogueUIBuilder::createMenuBackground(brogueUIProgram programName, const gridRect& boundary)
	{
		// Pulled from Brogue v1.7.5
		color menuColor1(0, 0, 0, 1.0f);
		color menuColor2(0, 0, 0, 0.5f);

		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::MenuBackground, _menuBackgroundCounter++);

		brogueUIData menuData(menuId, boundary, _zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);

		menuData.setUIParameters(false, _zoomLevel, 0);

		return new brogueBackground(_eventController, menuData, sceneBounds, boundary);
	}
}
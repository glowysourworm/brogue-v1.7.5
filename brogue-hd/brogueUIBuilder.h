#pragma once
#include "brogueBackground.h"
#include "brogueButton.h"
#include "brogueFlameMenuHeatView.h"
#include "brogueFlameMenuTitleMask.h"
#include "brogueGameLogContainer.h"
#include "brogueGameView.h"
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueScoreEntry.h"
#include "brogueTextView.h"
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
using namespace brogueHd::component;
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

		const int FlamePeriodFadeMilliseconds = 5000;

		// Brogue v1.7.5
		static const color HeaderForegroundColor() { return colors::yellow(); }
		static const color FooterForegroundColor() { return color(0.464, 0.396, 0.894, 1.0); }
		static const color FontDefaultColor() { return colors::white(); }
		static const color FontDefaultHotkeyColor() { return colors::yellow(); }
		static const color FontDefaultBackgroundColor() { return color(0, 0, 0, 0); }

		static const color FlatMenuButtonColor() { return color(0, 0, 0, 0.85f); }
		static const color FlatMenuButtonActiveColor1() { return color(0.3, 0.3, 0.3, 0.85f); }
		static const color FlatMenuButtonActiveColor2() { return color(0.1, 0.1, 0.1, 0.85f); }
		static const color FlatMenuButtonPressedColor1() { return color(0.4, 0.4, 0.4, 0.85f); }
		static const color FlatMenuButtonPressedColor2() { return color(0.1, 0.1, 0.1, 0.85f); }

		static const color GameFontSubduedColor() { return colors::getGray(0.5); }

		static const color GameMenuBackgroundColor1() { return MenuBackgroundColor1() * 0.75f; }
		static const color GameMenuBackgroundColor2() { return MenuBackgroundColor2() * 0.75f; }

		static const color GameMenuButtonColor1() { return MenuButtonColor1() * 0.75f; }
		static const color GameMenuButtonColor2() { return MenuButtonColor2() * 0.75f; }
		static const color GameMenuButtonActiveColor1() { return MenuButtonActiveColor1() * 0.75f; }
		static const color GameMenuButtonActiveColor2() { return MenuButtonActiveColor2() * 0.75f; }
		static const color GameMenuButtonPressedColor1() { return MenuButtonPressedColor1() * 0.75f; }
		static const color GameMenuButtonPressedColor2() { return MenuButtonPressedColor2() * 0.75f; }

		static const color MenuBackgroundColor1() { return color(0.156, 0.137, 0.329, 1); }
		static const color MenuBackgroundColor2() { return color(0.078, 0.058, 0.156, 1); }
		static const color MenuDefaultBackground() { return color(0, 0, 0, 0.85); }

		static const color MenuButtonColor1() { return color(0.156, 0.137, 0.329, 1); }
		static const color MenuButtonColor2() { return color(0.078, 0.058, 0.156, 1); }
		static const color MenuButtonActiveColor1() { return color(0.312, 0.274, 0.658, 1); }
		static const color MenuButtonActiveColor2() { return color(0.078, 0.058, 0.156, 1); }
		static const color MenuButtonPressedColor1() { return color(0.234, 0.205, 0.493, 1); }
		static const color MenuButtonPressedColor2() { return color(0.078, 0.058, 0.156, 1); }

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
		brogueViewContainer* createGameMenu();
		brogueViewContainer* createGameInventory();
		brogueViewContainer* createGame();
		brogueViewContainer* createHeaderedBackground(brogueUIProgram programName, const colorString& headerText);
		brogueViewContainer* createPlaceholder(brogueUIProgram programName, const gridRect& boundary);

	private:

		brogueTextView* createTextViewSingleLine(brogueUIProgram programName,
																const gridRect& boundary,
																const simpleString& text,
																const color& foreground,
																const color& background1,
																const color& background2,
																brogueTextAlignment alignment);
		brogueTextView* createTextView(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, const color& foreground);
		brogueBackground* createMenuBackground(brogueUIProgram programName, const gridRect& boundary);
		brogueBackground* createBackground(brogueUIProgram programName, const gridRect& boundary);

		brogueButton* createFlatMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, brogueUIAction action);
		brogueButton* createFlatMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, const brogueUITagAction& action);
		brogueButton* createFlatMenuButton(brogueUIProgram programName, const gridRect& boundary, const colorString& text, const brogueUITagAction& action);

		brogueButton* createMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, const brogueUITagAction& tagAction);
		brogueButton* createMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, brogueUIAction action);
		brogueButton* createMenuButton(brogueUIProgram programName, const gridRect& boundary, const colorString& text, const brogueUITagAction& action);

		brogueButton* createGameMenuButton(brogueUIProgram programName, const gridRect& boundary, const brogueUITagAction& action, const simpleString& text, brogueTextAlignment alignment, int hotkeyIndex);
		brogueButton* createGameMenuButton(brogueUIProgram programName, const gridRect& boundary, const brogueUITagAction& action, const colorString& text, brogueTextAlignment alignment);

		brogueButton* createButton(brogueUIProgram programName, 
								   const gridRect& boundary, 
								   const colorString& text,
								   const brogueUITagAction& tagAction,
								   const color& background1,
								   const color& background2,
								   const color& hover1,
								   const color& hover2,
								   const color& pressed1,
								   const color& pressed2,
								   brogueTextAlignment alignment);

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
			case brogueUIProgram::GameInventoryProgram:
				return createGameInventory();
			case brogueUIProgram::GameMenuProgram:
				return createGameMenu();
			case brogueUIProgram::GameProgram:
				return createGame();
			case brogueUIProgram::ModalProgram:
			case brogueUIProgram::GameHelpProgram:
			case brogueUIProgram::GameDiscoveredItems:
			{
				brogueUIProgramPartId partId(programName, brogueUIProgramPart::Background, 0);
				gridRect boundary = getBrogueStaticBoundary(partId);
				return createPlaceholder(programName, boundary);
			}
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
						return gridRect(COLS - 30 - 1, ROWS - 13, 30, 16);
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
						return gridRect(COLS - 40, 3, 40, numberOfItems);
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
		brogueUIData heatData(_zoomLevel, colors::transparent());
		brogueUITagAction action(brogueUIState::MainMenu);

		brogueFlameMenuHeatView* heatView = new brogueFlameMenuHeatView(_eventController, _randomGenerator, this->FlamePeriodFadeMilliseconds, heatData, sceneBounds, sceneBounds);
		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::FlameMenuProgram, _zoomLevel, false, false, sceneBounds);

		// Set state change action
		heatView->setUIAction(action);

		result->addView(heatView);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createFlameMenuTitleMask()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId titleId(brogueUIProgram::FlameMenuTitleMaskProgram, brogueUIProgramPart::ColorMask, 0);
		brogueUIData titleMaskData(_zoomLevel, colors::transparent());
		brogueFlameMenuTitleMask* titleMask = new brogueFlameMenuTitleMask(_eventController, titleMaskData, sceneBounds, sceneBounds);

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::FlameMenuTitleMaskProgram, _zoomLevel, false, false, sceneBounds);

		result->addView(titleMask);
		return result;
	}

	brogueViewContainer* brogueUIBuilder::createMainMenu()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect menuBounds = getBrogueStaticBoundary(brogueUIProgramPartId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::MenuBackground, 0));
		gridRect paddedBounds = getPaddedBoundary(menuBounds, 1);

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::MainMenuProgram, _zoomLevel, false, false, menuBounds);

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

			brogueButton* button = createMenuButton(brogueUIProgram::MainMenuProgram, boundary, text, 0, tagAction);

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

			brogueUITagAction tagAction(brogueUIAction::OpenGame, *(filesSorted.get(index)->getFileFullPath()));

			buttons.add(createFlatMenuButton(programName, boundary, result, -1, tagAction));
		}

		// Finally, create the views. The view container will handle the scroll and clipping behavior.
		//
		brogueViewContainer* result = new brogueViewContainer(programName, _zoomLevel, true, true, menuBounds);

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
		int buttonWidth = (DCOLS / 5) + 1;
		int buttonSpacing = 2;
		int column = COLS - DCOLS;

		gridRect depthBounds(0, ROWS - 1, COLS - DCOLS, 1);

		gridRect button1(column, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button2(column, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button3(column, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button4(column, ROWS - 1, buttonWidth - 2, 1);
		column += buttonWidth;
		gridRect button5(column, ROWS - 1, buttonWidth - 2, 1);

		brogueTextView* depthText = createTextViewSingleLine(brogueUIProgram::BottomBarMenuProgram, depthBounds, "-- Depth 1 --", colors::white(), colors::black(), colors::black(), brogueTextAlignment::Center);
		brogueButton* exploreButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button1, "Explore", 1, brogueUIAction::GameCommand_Explore);
		brogueButton* restButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button2, "Rest (z)", 6, brogueUIAction::GameCommand_Rest);
		brogueButton* searchButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button3, "Search (s)", 8, brogueUIAction::GameCommand_Search);
		brogueButton* menuButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button4, "Menu", -1, brogueUIAction::GameCommand_Menu);
		brogueButton* inventoryButton = createMenuButton(brogueUIProgram::BottomBarMenuProgram, button5, "Inventory", 0, brogueUIAction::GameCommand_Inventory);

		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(brogueUIProgram::BottomBarMenuProgram, brogueUIProgramPart::Background, 0));

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::BottomBarMenuProgram, _zoomLevel, false, false, boundary);

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
		brogueTextView* flavorText = createTextViewSingleLine(program, boundary, "You see a large Troll...", foreground, colors::black(), colors::black(), brogueTextAlignment::Left);
		brogueViewContainer* result = new brogueViewContainer(program, _zoomLevel, false, false, boundary);

		result->addView(flavorText);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGameObjectList()
	{
		brogueUIProgram programName = brogueUIProgram::GameObjectListProgram;

		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(programName, brogueUIProgramPart::Background, 0));
		gridRect itemBounds(0, 0, COLS - DCOLS, 1);

		brogueViewContainer* result = new brogueViewContainer(programName, _zoomLevel, true, false, boundary);

		// Background
		result->addView(createBackground(programName, boundary));

		// Place holders
		result->addView(createTextViewSingleLine(programName, itemBounds, "@: You (lit)", colors::white(), colors::black(), colors::black(), brogueTextAlignment::Left));
		itemBounds.translate(0, 1);
		result->addView(createTextViewSingleLine(programName, itemBounds, "Health", colors::red(), colors::black(), colors::black(), brogueTextAlignment::Center));
		itemBounds.translate(0, 1);
		result->addView(createTextViewSingleLine(programName, itemBounds, "Nutrition", colors::orange(), colors::black(), colors::black(), brogueTextAlignment::Center));
		itemBounds.translate(0, 1);
		result->addView(createTextViewSingleLine(programName, itemBounds, "Str: 12 Armor: 3", colors::white(), colors::black(), colors::black(), brogueTextAlignment::Center));
		itemBounds.translate(0, 1);
		result->addView(createTextViewSingleLine(programName, itemBounds, "Stealth Range: 14", colors::white(), colors::black(), colors::black(), brogueTextAlignment::Center));

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGameLog()
	{
		color menuColor(0, 0, 0, 0.85);

		brogueUIProgram program = brogueUIProgram::GameLogProgram;

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(program, brogueUIProgramPart::Background, 0));

		brogueGameLogContainer* result = new brogueGameLogContainer(program, _zoomLevel, false, false, boundary);

		brogueUIProgramPartId partId(program, brogueUIProgramPart::Text, 0);
		brogueUIData uiData(_zoomLevel, menuColor);
		brogueTextView* view = new brogueTextView(_eventController, partId, uiData, sceneBounds, boundary);

		view->setUIAction(brogueUITagAction(brogueUIAction::GameCommand_ToggleLog, ""));

		for (int index = 0; index < ROWS; index++)
		{
			view->setLine(index, simpleString("The rough stone wall is firm and unyielding"), brogueTextAlignment::Left);
		}

		result->addView(view);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGameMenu()
	{
		brogueUIProgram program = brogueUIProgram::GameMenuProgram;
		brogueUIProgramPartId backgroundId(program, brogueUIProgramPart::MenuBackground, 0);

		gridRect boundary = getBrogueStaticBoundary(backgroundId);
		gridRect itemBoundary(boundary.left(), boundary.top(), boundary.width, 1);

		brogueButton* button1 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Rest), "Z: Rest Until Better", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button2 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Autopilot), "A: Autopilot", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button3 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_EasyMode), "&: Easy Mode", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueTextView* spacer1 = createTextViewSingleLine(program, itemBoundary, "  ---", colors::white(), GameMenuBackgroundColor1(), GameMenuBackgroundColor2(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueButton* button4 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Rest), "]: Display Stealth Range", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueTextView* spacer2 = createTextViewSingleLine(program, itemBoundary, "  ---", colors::white(), GameMenuBackgroundColor1(), GameMenuBackgroundColor2(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueButton* button5 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIState::GameDiscoveredItemsOpen), "D: Discovered Items", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button6 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_ViewDungeonSeed), "~: View Dungeon Seed", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button7 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIState::GameHelpOpen), "?: Help", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueTextView* spacer3 = createTextViewSingleLine(program, itemBoundary, "  ---", colors::white(), GameMenuBackgroundColor1(), GameMenuBackgroundColor2(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueButton* button8 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Save), "S: Save Game", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueButton* button9 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Rest), "Q: Quit", brogueTextAlignment::Left, 0);

		brogueViewContainer* result = new brogueViewContainer(program, _zoomLevel, false, false, boundary);

		result->addView(button1);
		result->addView(button2);
		result->addView(button3);

		result->addView(spacer1);

		result->addView(button4);
		result->addView(spacer2);

		result->addView(button5);
		result->addView(button6);
		result->addView(button7);

		result->addView(spacer3);

		result->addView(button8);
		result->addView(button9);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGameInventory()
	{
		brogueUIProgram program = brogueUIProgram::GameInventoryProgram;
		brogueUIProgramPartId backgroundId(program, brogueUIProgramPart::MenuBackground, 0);
		gridRect boundary = getBrogueDynamicBoundary(backgroundId, 8);
		gridRect itemBoundary(boundary.left(), boundary.top(), boundary.width, 1);

		brogueTextView* header = createTextViewSingleLine(program, itemBoundary, "~ Inventory ~", HeaderForegroundColor(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Center);
		itemBoundary.translate(0, 1);

		// Hotkey, Glyph, Description, Modifiers (class, in hand, worn)
		simpleString itemFormat = "{}) {} {} {}";
		simpleString weapon = simpleExt::format(itemFormat.c_str(), "b", brogueGlyphMap::WeaponUnicode, "A +0 dagger", "<12> (in hand)");
		simpleString armor =  simpleExt::format(itemFormat.c_str(), "d", "[", "Leather Armor", "[3]<10> (worn)");
		simpleString food = simpleExt::format(itemFormat.c_str(), "a", ";", "Some food", "");
		simpleString darts = simpleExt::format(itemFormat.c_str(), "c", brogueGlyphMap::WeaponUnicode, "15 +0 darts", "<10>");

		colorString weaponColor(weapon.c_str(), colors::white());
		colorString armorColor(armor.c_str(), colors::white());
		colorString foodColor(food.c_str(), colors::white());
		colorString dartsColor(darts.c_str(), colors::white());

		weaponColor.setColor(3, colors::yellow());
		armorColor.setColor(3, colors::yellow());
		foodColor.setColor(3, colors::yellow());
		dartsColor.setColor(3, colors::yellow());

		weaponColor.setColorRange(16, weapon.count() - 16, GameFontSubduedColor());
		armorColor.setColorRange(17, armor.count() - 17, GameFontSubduedColor());
		foodColor.setColorRange(13, food.count() - 13, GameFontSubduedColor());
		dartsColor.setColorRange(15, darts.count() - 15, GameFontSubduedColor());

		brogueButton* button1 = createFlatMenuButton(program, itemBoundary, weaponColor, brogueUITagAction(brogueUIAction::GameCommand_InventorySelect));
		itemBoundary.translate(0, 1);
		brogueButton* button2 = createFlatMenuButton(program, itemBoundary, armorColor, brogueUITagAction(brogueUIAction::GameCommand_InventorySelect));
		itemBoundary.translate(0, 1);

		brogueTextView* spacer1 = createTextViewSingleLine(program, itemBoundary, "   ---", GameFontSubduedColor(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueButton* button3 = createFlatMenuButton(program, itemBoundary, foodColor, brogueUITagAction(brogueUIAction::GameCommand_InventorySelect));
		itemBoundary.translate(0, 1);
		brogueButton* button4 = createFlatMenuButton(program, itemBoundary, dartsColor, brogueUITagAction(brogueUIAction::GameCommand_InventorySelect));
		itemBoundary.translate(0, 1);

		brogueTextView* spacer2 = createTextViewSingleLine(program, itemBoundary, "   You have room for 22 more items", GameFontSubduedColor(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);
		brogueTextView* spacer3 = createTextViewSingleLine(program, itemBoundary, "   -- press (a-z) for more info --", GameFontSubduedColor(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueViewContainer* result = new brogueViewContainer(program, _zoomLevel, false, false, boundary);

		result->addView(header);
		result->addView(button1);
		result->addView(button2);

		result->addView(spacer1);

		result->addView(button3);
		result->addView(button4);

		result->addView(spacer2);
		result->addView(spacer3);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createGame()
	{
		brogueUIProgramPartId partId(brogueUIProgram::GameProgram, brogueUIProgramPart::Background, 0);
		brogueUIData uiData(_zoomLevel, colors::black());
		gridRect boundary = getBrogueSceneBoundary();
		brogueGameView* gameView = new brogueGameView(_eventController, partId, uiData, boundary, getBrogueStaticBoundary(partId));

		brogueViewContainer* container = new brogueViewContainer(brogueUIProgram::GameProgram, _zoomLevel, false, false, boundary);

		container->addView(gameView);

		return container;
	}

	brogueViewContainer* brogueUIBuilder::createHighScoresView()
	{
		// Pulled from Brogue v1.7.5
		color menuColor(0, 0, 0, 0.85);
		color footerColor(0.596f, 0.494f, 0.996f, 1);
		int menuPadding = 2;

		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueViewContainer* result = new brogueViewContainer(brogueUIProgram::HighScoresProgram, _zoomLevel, false, false, sceneBounds);

		brogueUIProgramPartId partId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, 0);
		brogueUIData uiData(_zoomLevel, menuColor);
		brogueTextView* textView = new brogueTextView(_eventController, partId, uiData, sceneBounds, sceneBounds);

		textView->setUIAction(brogueUITagAction(brogueUIState::MainMenu));

		// Header and Footer
		textView->setLine(0, "-- High Scores --", colors::yellow(), brogueTextAlignment::Center);
		textView->setLine(sceneBounds.height - 1, "Press space to continue", footerColor, brogueTextAlignment::Center);

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

			colorString coloredEntry(formattedEntry.c_str(), foreGround);

			// Add header offset
			textView->setLine(index + 2, coloredEntry, brogueTextAlignment::Center);
		}

		result->addView(textView);

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

		brogueViewContainer* result = new brogueViewContainer(programName, _zoomLevel, false, false, boundary);

		// Text View
		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Text, 0);

		// Menu Background
		brogueUIData uiData(_zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);

		// Handles lines of text / a paragraph
		brogueTextView* textView = new brogueTextView(_eventController, partId, uiData, sceneBounds, boundary);

		// Header
		textView->setLine(boundary.top(), headerText, brogueTextAlignment::Center);

		result->addView(textView);

		return result;
	}

	brogueViewContainer* brogueUIBuilder::createPlaceholder(brogueUIProgram programName, const gridRect& boundary)
	{
		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Background, 0);
		brogueUIData uiData(_zoomLevel, _randomGenerator->nextColor(colors::getGray(0.2), colors::getGray(0.5)));
		brogueBackground* background = new brogueBackground(_eventController, partId, uiData, getBrogueSceneBoundary(), boundary);
		brogueViewContainer* container = new brogueViewContainer(programName, _zoomLevel, false, false, boundary);

		container->addView(background);

		return container;
	}

	brogueButton* brogueUIBuilder::createFlatMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, const brogueUITagAction& action)
	{
		colorString colorText(text.c_str(), FontDefaultColor());

		if (hotkeyIndex >= 0)
			colorText.setColor(hotkeyIndex, FontDefaultHotkeyColor());

		return createFlatMenuButton(programName, boundary, colorText, action);
	}

	brogueButton* brogueUIBuilder::createFlatMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, brogueUIAction action)
	{
		return createFlatMenuButton(programName, boundary, text, hotkeyIndex, brogueUITagAction(action));
	}

	brogueButton* brogueUIBuilder::createFlatMenuButton(brogueUIProgram programName, const gridRect& boundary, const colorString& text, const brogueUITagAction& action)
	{
		return createButton(programName, boundary, text, action,
							FlatMenuButtonColor(),
							FlatMenuButtonColor(),
							FlatMenuButtonActiveColor1(),
							FlatMenuButtonActiveColor2(),
							FlatMenuButtonPressedColor1(),
							FlatMenuButtonPressedColor2(),
							brogueTextAlignment::Left);
	}

	brogueButton* brogueUIBuilder::createMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, brogueUIAction action)
	{
		colorString colorText(text.c_str(), FontDefaultColor());

		if (hotkeyIndex >= 0)
			colorText.setColor(hotkeyIndex, FontDefaultHotkeyColor());

		return createMenuButton(programName, boundary, colorText, action);
	}
	brogueButton* brogueUIBuilder::createMenuButton(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, int hotkeyIndex, const brogueUITagAction& tagAction)
	{
		colorString colorText(text.c_str(), FontDefaultColor());

		if (hotkeyIndex >= 0)
			colorText.setColor(hotkeyIndex, FontDefaultHotkeyColor());

		return createMenuButton(programName, boundary, colorText, tagAction);
	}
	brogueButton* brogueUIBuilder::createMenuButton(brogueUIProgram programName, const gridRect& boundary, const colorString& text, const brogueUITagAction& action)
	{
		return createButton(programName, boundary, text, 
							brogueUITagAction(action),
							MenuBackgroundColor1(),
							MenuBackgroundColor2(),
							MenuButtonActiveColor1(),
							MenuButtonActiveColor2(),
							MenuButtonPressedColor1(),
							MenuButtonPressedColor2(),
							brogueTextAlignment::Center);
	}
	brogueButton* brogueUIBuilder::createGameMenuButton(brogueUIProgram programName, const gridRect& boundary, const brogueUITagAction& action, const simpleString& text, brogueTextAlignment alignment, int hotkeyIndex)
	{
		colorString colorText(text.c_str(), FontDefaultColor());

		if (hotkeyIndex >= 0)
			colorText.setColor(hotkeyIndex, FontDefaultHotkeyColor());

		return createGameMenuButton(programName, boundary, action, colorText, alignment);
	}
	brogueButton* brogueUIBuilder::createGameMenuButton(brogueUIProgram programName, const gridRect& boundary, const brogueUITagAction& action, const colorString& text, brogueTextAlignment alignment)
	{
		return createButton(programName, boundary, text, action, 
							GameMenuBackgroundColor1(), 
							GameMenuBackgroundColor2(), 
							GameMenuButtonActiveColor1(), 
							GameMenuButtonActiveColor2(), 
							GameMenuButtonPressedColor1(), 
							GameMenuButtonPressedColor2(), 
							alignment);
	}

	brogueButton* brogueUIBuilder::createButton(brogueUIProgram programName,
											    const gridRect& boundary,
											    const colorString& text,
											    const brogueUITagAction& tagAction,
												const color& background1,
												const color& background2,
												const color& hover1,
												const color& hover2,
												const color& pressed1,
												const color& pressed2,
											    brogueTextAlignment alignment)
	{
		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Button, _buttonCounter++);

		brogueUIData buttonData(_zoomLevel,
								background1, background2,
								hover1, hover2,
								pressed1, pressed2,
								brogueGradientType::Horizontal);

		brogueButton* button = new brogueButton(_eventController, partId, buttonData, getBrogueSceneBoundary(), boundary);

		button->setUI(text, alignment);
		button->setUIAction(tagAction);

		return button;
	}
	brogueTextView* brogueUIBuilder::createTextView(brogueUIProgram programName, 
													const gridRect& boundary, 
													const simpleString& text, 
													const color& foreground)
	{
		brogueUIProgramPartId dataId(programName, brogueUIProgramPart::Text, _textCounter++);
		brogueUIData data(_zoomLevel, FontDefaultBackgroundColor());

		brogueTextView* textView = new brogueTextView(_eventController, dataId, data, getBrogueSceneBoundary(), boundary);

		textView->setText(colorString(text.c_str(), foreground));

		return textView;
	}

	brogueTextView* brogueUIBuilder::createTextViewSingleLine(brogueUIProgram programName,
																const gridRect& boundary,
																const simpleString& text,
																const color& foreground,
																const color& background1,
																const color& background2,
																brogueTextAlignment alignment)
	{
		brogueUIProgramPartId dataId(programName, brogueUIProgramPart::Text, _textCounter++);
		brogueUIData data(_zoomLevel, background1, background2, brogueGradientType::Horizontal);

		brogueTextView* textView = new brogueTextView(_eventController, dataId, data, getBrogueSceneBoundary(), boundary);

		textView->setLine(boundary.top(), colorString(text.c_str(), foreground), alignment);

		return textView;
	}

	brogueBackground* brogueUIBuilder::createBackground(brogueUIProgram programName, const gridRect& boundary)
	{
		// Pulled from Brogue v1.7.5
		color menuColor1(0, 0, 0, 1.0f);
		color menuColor2(0, 0, 0, 0.5f);

		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::Background, _menuBackgroundCounter++);

		brogueUIData menuData(_zoomLevel, menuColor1);

		return new brogueBackground(_eventController, menuId, menuData, sceneBounds, boundary);
	}

	brogueBackground* brogueUIBuilder::createMenuBackground(brogueUIProgram programName, const gridRect& boundary)
	{
		// Pulled from Brogue v1.7.5
		color menuColor1(0, 0, 0, 1.0f);
		color menuColor2(0, 0, 0, 0.5f);

		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::MenuBackground, _menuBackgroundCounter++);

		brogueUIData menuData(_zoomLevel, menuColor1, menuColor2, brogueGradientType::Circular);

		return new brogueBackground(_eventController, menuId, menuData, sceneBounds, boundary);
	}
}
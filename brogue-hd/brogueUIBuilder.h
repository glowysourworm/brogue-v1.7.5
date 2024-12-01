#pragma once
#include "brogueBackground.h"
#include "brogueButton.h"
#include "brogueCoordinateConverter.h"
#include "brogueFlameMenuHeatView.h"
#include "brogueFlameMenuTitleMask.h"
#include "brogueGameLogProgram.h"
#include "brogueGameView.h"
#include "brogueGlobal.h"
#include "brogueGlyphMap.h"
#include "brogueGraphView.h"
#include "brogueMenuBackground.h"
#include "brogueScoreEntry.h"
#include "brogueScrollView.h"
#include "brogueTextView.h"
#include "brogueUIColorText.h"
#include "brogueUIConstants.h"
#include "brogueUIData.h"
#include "brogueUIProgramPartId.h"
#include "brogueUITagAction.h"
#include "brogueViewProgram.h"
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
		gridRect getBrogueSceneBoundaryUI();
		gridRect getBrogueGameBoundary();

		brogueViewProgram* buildProgramView(brogueUIProgram programName);

		brogueCoordinateConverter* getCoordinateConverter() const;

	protected:

		const int FlamePeriodFadeMilliseconds = 5000;

		// Brogue v1.7.5
		static const color HeaderForegroundColor() { return colors::yellow(); }
		static const color FooterForegroundColor() { return color(0.464f, 0.396f, 0.894f, 1.0f); }
		static const color FontDefaultColor() { return colors::white(); }
		static const color FontFlavorTextColor() { return color(0.464f, 0.396f, 0.894f, 1.0f); }
		static const color FontHighScoresColor() { return color(0.596f, 0.494f, 0.996f, 1); }
		static const color FontDefaultHotkeyColor() { return colors::yellow(); }
		static const color FontDefaultBackgroundColor() { return color(0, 0, 0, 0); }
		
		static const color FlatMenuButtonColor() { return color(0, 0, 0, 0.85f); }
		static const color FlatMenuButtonActiveColor1() { return color(0.3f, 0.3f, 0.3f, 0.85f); }
		static const color FlatMenuButtonActiveColor2() { return color(0.1f, 0.1f, 0.1f, 0.85f); }
		static const color FlatMenuButtonPressedColor1() { return color(0.4f, 0.4f, 0.4f, 0.85f); }
		static const color FlatMenuButtonPressedColor2() { return color(0.1f, 0.1f, 0.1f, 0.85f); }

		static const color GameFontSubduedColor() { return colors::getGray(0.5); }

		static const color GameMenuBackgroundColor1() { return MenuBackgroundColor1() * 0.9f; }
		static const color GameMenuBackgroundColor2() { return MenuBackgroundColor2() * 0.9f; }

		static const color GameMenuButtonColor1() { return MenuButtonColor1() * 0.9f; }
		static const color GameMenuButtonColor2() { return MenuButtonColor2() * 0.9f; }
		static const color GameMenuButtonActiveColor1() { return MenuButtonActiveColor1() * 0.9f; }
		static const color GameMenuButtonActiveColor2() { return MenuButtonActiveColor2() * 0.9f; }
		static const color GameMenuButtonPressedColor1() { return MenuButtonPressedColor1() * 0.9f; }
		static const color GameMenuButtonPressedColor2() { return MenuButtonPressedColor2() * 0.9f; }

		static const color MenuBackgroundColor1() { return color(0.156f, 0.137f, 0.329f, 1.0f); }
		static const color MenuBackgroundColor2() { return color(0.078f, 0.058f, 0.156f, 0.0f); }
		static const color MenuDefaultBackground() { return color(0, 0, 0, 0.85f); }

		static const color MenuButtonColor1() { return color(0.156f, 0.137f, 0.329f, 1.0f); }
		static const color MenuButtonColor2() { return color(0.078f, 0.058f, 0.156f, 1.0f); }
		static const color MenuButtonActiveColor1() { return color(0.312f, 0.274f, 0.658f, 1.0f); }
		static const color MenuButtonActiveColor2() { return color(0.078f, 0.058f, 0.156f, 1.0f); }
		static const color MenuButtonPressedColor1() { return color(0.234f, 0.205f, 0.493f, 1.0f); }
		static const color MenuButtonPressedColor2() { return color(0.078f, 0.058f, 0.156f, 1.0f); }

	private:

		gridRect getBrogueStaticBoundary(const brogueUIProgramPartId& partId);
		gridRect getBrogueDynamicBoundary(const brogueUIProgramPartId& partId, int numberOfItems);

		brogueViewProgram* createFlameMenu();
		brogueViewProgram* createFlameMenuTitleMask();
		brogueViewProgram* createMainMenu();
		brogueViewProgram* createMainMenuSelector(brogueUIProgram programName, const simpleDirectoryEntry& files);
		brogueViewProgram* createBottomMenuBar();
		brogueViewProgram* createHighScoresView();
		brogueViewProgram* createFlavorText();
		brogueViewProgram* createGameObjectList();
		brogueViewProgram* createGameLog();
		brogueViewProgram* createGameMenu();
		brogueViewProgram* createGameInventory();
		brogueViewProgram* createGame();
		brogueViewProgram* createHeaderedBackground(brogueUIProgram programName, const colorString& headerText);
		brogueViewProgram* createPlaceholder(brogueUIProgram programName, const gridRect& boundary);

	private:

		brogueTextView* createTextViewSingleLine(brogueUIProgram programName,
																const gridRect& boundary,
																const simpleString& text,
																const color& foreground,
																const color& background1,
																const color& background2,
																brogueTextAlignment alignment);
		brogueTextView* createTextView(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, const color& foreground);
		brogueTextView* createTextViewHeader(brogueUIProgram programName, const gridRect& boundary, const simpleString& text, const color& foreground);
		brogueMenuBackground* createMainMenuBackground(brogueUIProgram programName, const gridRect& boundary);
		brogueMenuBackground* createMenuBackground(brogueUIProgram programName,
													const gridRect& boundary,
													const color& background1,
													const color& background2,
													brogueGradientType gradient);
		brogueBackground* createBackground(brogueUIProgram programName, const gridRect& boundary);
		brogueScrollView* createScrollView(brogueUIProgram programName, const gridRect& boundary);

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

		brogueCoordinateConverter* _coordinateConverter;
		eventController* _eventController;
		resourceController* _resourceController;
		randomGenerator* _randomGenerator;
		int _zoomLevel;
	};

	brogueUIBuilder::brogueUIBuilder(eventController* eventController, resourceController* resourceController, randomGenerator* randomGenerator, int zoomLevel)
	{
		// Set up coordinate converter once -> pass it down the UI tree.
		gridRect sceneBoundary = getBrogueStaticBoundary(brogueUIProgramPartId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameDisplay, 0));

		_coordinateConverter = new brogueCoordinateConverter(resourceController, sceneBoundary.width, sceneBoundary.height, zoomLevel);
		_eventController = eventController;
		_resourceController = resourceController;
		_randomGenerator = randomGenerator;
		_zoomLevel = zoomLevel;

		// These numbers are important for hashing pieces of the data. So, it's best to keep them off
		// of zero.
		_menuBackgroundCounter = 1;
		_backgroundCounter = 1;
		_buttonCounter = 1;
		_textCounter = 1;
	}
	brogueUIBuilder::~brogueUIBuilder() 
	{
		delete _coordinateConverter;
	}

	gridRect brogueUIBuilder::getBrogueSceneBoundary()
	{
		return gridRect(0, 0, COLS, ROWS);
	}
	gridRect brogueUIBuilder::getBrogueSceneBoundaryUI()
	{
		return _coordinateConverter->calculateSceneBoundaryUI();
	}
	gridRect brogueUIBuilder::getBrogueGameBoundary()
	{
		return getBrogueStaticBoundary(brogueUIProgramPartId(brogueUIProgram::GameProgram, brogueUIProgramPart::Background, 0));
	}

	brogueCoordinateConverter* brogueUIBuilder::getCoordinateConverter() const
	{
		return _coordinateConverter;
	}

	brogueViewProgram* brogueUIBuilder::buildProgramView(brogueUIProgram programName)
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
			case brogueUIProgram::PlaybackMenuProgram:
			{
				// Read the saved games into the display
				simpleDirectoryEntry directory = simpleFileIO::readDirectory(_resourceController->getPlaybackDirectory()->c_str(), ".broguerec");

				// Creates scrollable view container
				return createMainMenuSelector(programName, directory);
			}
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
			case brogueUIProgramPart::GameSurface:
				switch (partId.getName())
				{
					case brogueUIProgram::MainMenuProgram:
						return gridRect(COLS - 26, ROWS - 12, 24, 11);
					case brogueUIProgram::OpenMenuProgram:
					case brogueUIProgram::PlaybackMenuProgram:
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
						return gridRect(COLS - DCOLS, ROWS - DROWS, DCOLS, DROWS - 1);
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

	brogueViewProgram* brogueUIBuilder::createFlameMenu()
	{
		gridRect sceneBoundary = getBrogueSceneBoundary();

		brogueUIProgramPartId heatId(brogueUIProgram::FlameMenuProgram, brogueUIProgramPart::FlameDisplay, 0);
		brogueUIData heatData(sceneBoundary, sceneBoundary, _zoomLevel, colors::transparent());
		brogueUITagAction action(brogueUIState::MainMenu);

		brogueFlameMenuHeatView* heatView = new brogueFlameMenuHeatView(_coordinateConverter, _resourceController, _eventController, _randomGenerator, heatId, heatData, this->FlamePeriodFadeMilliseconds);
		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, brogueUIProgram::FlameMenuProgram, sceneBoundary, sceneBoundary);

		// Set state change action
		heatView->setUIAction(action);

		result->addView(heatView);

		return result;
	}

	brogueViewProgram* brogueUIBuilder::createFlameMenuTitleMask()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId titleId(brogueUIProgram::FlameMenuTitleMaskProgram, brogueUIProgramPart::ColorMask, 0);
		brogueUIData titleMaskData(sceneBounds, sceneBounds, _zoomLevel, colors::transparent());
		brogueFlameMenuTitleMask* titleMask = new brogueFlameMenuTitleMask(_coordinateConverter, _resourceController, _eventController, titleId, titleMaskData);

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, brogueUIProgram::FlameMenuTitleMaskProgram, sceneBounds, sceneBounds);

		result->addView(titleMask);
		return result;
	}

	brogueViewProgram* brogueUIBuilder::createMainMenu()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect menuBounds = getBrogueStaticBoundary(brogueUIProgramPartId(brogueUIProgram::MainMenuProgram, brogueUIProgramPart::MenuBackground, 0));
		gridRect paddedBounds = menuBounds.createPadded(1);

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, brogueUIProgram::MainMenuProgram, menuBounds, sceneBounds);

		int buttonPadding = 1;

		result->addView(createMainMenuBackground(brogueUIProgram::MainMenuProgram, menuBounds));

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

	brogueViewProgram* brogueUIBuilder::createMainMenuSelector(brogueUIProgram programName, const simpleDirectoryEntry& files)
	{
		brogueUIProgramPartId backgroundId(programName, brogueUIProgramPart::MenuBackground, 0);

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect menuBounds = getBrogueStaticBoundary(backgroundId);
		gridRect backgroundBounds = menuBounds.createExpanded(1);

		simpleString headerText = programName == brogueUIProgram::OpenMenuProgram ? "~ Open Saved Game ~" :
								  programName == brogueUIProgram::PlaybackMenuProgram ? "~ View Game Recording ~" :
								  "~ Header ~";

		simpleList<brogueUIColorText*> items;
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

			brogueUITagAction tagAction(brogueUIAction::OpenGame, *(filesSorted.get(index)->getFileFullPath()));
			colorString colorText(result.c_str(), colors::white());

			// (MEMORY!)
			items.add(new brogueUIColorText(colorText, tagAction));
		}

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, programName, backgroundBounds, sceneBounds);

		// Header will be a text view with no mouse interaction
		//
		brogueTextView* header = createTextViewHeader(programName, backgroundBounds, headerText, colors::yellow());

		// This container will handle the (virtual) scrolling of the items.
		//
		brogueScrollView* scrollView = createScrollView(programName, menuBounds);

		scrollView->addRange(items);

		result->addView(header);
		result->addView(scrollView);

		return result;
	}

	brogueViewProgram* brogueUIBuilder::createBottomMenuBar()
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

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

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, brogueUIProgram::BottomBarMenuProgram, boundary, sceneBounds);

		result->addView(depthText);
		result->addView(exploreButton);
		result->addView(restButton);
		result->addView(searchButton);
		result->addView(menuButton);
		result->addView(inventoryButton);

		return result;
	}

	brogueViewProgram* brogueUIBuilder::createFlavorText()
	{
		brogueUIProgram program = brogueUIProgram::FlavorTextPanelProgram;

		// Brogue v1.7.5
		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(program, brogueUIProgramPart::Background, 0));
		brogueTextView* flavorText = createTextViewSingleLine(program, boundary, "You see a large Troll...", FontFlavorTextColor(), colors::black(), colors::black(), brogueTextAlignment::Left);
		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, program, boundary, sceneBounds);

		result->addView(flavorText);

		return result;
	}

	brogueViewProgram* brogueUIBuilder::createGameObjectList()
	{
		brogueUIProgram programName = brogueUIProgram::GameObjectListProgram;

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(programName, brogueUIProgramPart::Background, 0));
		gridRect itemBounds(0, 0, COLS - DCOLS, 1);

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, programName, boundary, sceneBounds);

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

	brogueViewProgram* brogueUIBuilder::createGameLog()
	{
		brogueUIProgram program = brogueUIProgram::GameLogProgram;

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(brogueUIProgramPartId(program, brogueUIProgramPart::Background, 0));

		brogueGameLogProgram* result = new brogueGameLogProgram(_coordinateConverter, program, boundary, sceneBounds);

		brogueUIProgramPartId partId(program, brogueUIProgramPart::Text, 0);
		brogueUIData uiData(boundary, sceneBounds, _zoomLevel, MenuDefaultBackground());
		brogueTextView* view = new brogueTextView(_coordinateConverter, _resourceController, _eventController, partId, uiData);

		view->setUIAction(brogueUITagAction(brogueUIAction::GameCommand_ToggleLog, ""));

		for (int index = 0; index < boundary.height; index++)
		{
			view->setLine(index, simpleString("The rough stone wall is firm and unyielding"), brogueTextAlignment::Left);
		}

		result->addView(view);

		return result;
	}

	brogueViewProgram* brogueUIBuilder::createGameMenu()
	{
		brogueUIProgram program = brogueUIProgram::GameMenuProgram;
		brogueUIProgramPartId backgroundId(program, brogueUIProgramPart::MenuBackground, 0);

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(backgroundId);
		gridRect itemBoundary(boundary.left(), boundary.top(), boundary.width, 1);

		brogueButton* button1 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Rest), "Z: Rest Until Better", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button2 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Autopilot), "A: Autopilot", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button3 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_EasyMode), "&: Easy Mode", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueTextView* spacer1 = createTextViewSingleLine(program, itemBoundary, "  ---", colors::white(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueButton* button4 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Rest), "]: Display Stealth Range", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueTextView* spacer2 = createTextViewSingleLine(program, itemBoundary, "  ---", colors::white(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueButton* button5 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIState::GameDiscoveredItemsOpen), "D: Discovered Items", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button6 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_ViewDungeonSeed), "~: View Dungeon Seed", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);
		brogueButton* button7 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIState::GameHelpOpen), "?: Help", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueTextView* spacer3 = createTextViewSingleLine(program, itemBoundary, "  ---", colors::white(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Left);
		itemBoundary.translate(0, 1);

		brogueButton* button8 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Save), "S: Save Game", brogueTextAlignment::Left, 0);
		itemBoundary.translate(0, 1);

		brogueButton* button9 = createGameMenuButton(program, itemBoundary, brogueUITagAction(brogueUIAction::GameCommand_Rest), "Q: Quit", brogueTextAlignment::Left, 0);

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, program, boundary, sceneBounds);

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

	brogueViewProgram* brogueUIBuilder::createGameInventory()
	{
		brogueUIProgram program = brogueUIProgram::GameInventoryProgram;
		brogueUIProgramPartId backgroundId(program, brogueUIProgramPart::MenuBackground, 0);
		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueDynamicBoundary(backgroundId, 8);
		gridRect itemBoundary(boundary.left(), boundary.top(), boundary.width, 1);

		brogueTextView* header = createTextViewSingleLine(program, itemBoundary, "~ Inventory ~", HeaderForegroundColor(), FlatMenuButtonColor(), FlatMenuButtonColor(), brogueTextAlignment::Center);
		itemBoundary.translate(0, 1);

		// Hotkey, Glyph, Description, Modifiers (class, in hand, worn)
		simpleString itemFormat = "{}) {} {} {}";
		simpleString weapon = simpleExt::format(itemFormat.c_str(), "b", brogueGlyphMap::WeaponUnicode, "A +0 dagger", "<12> (in hand)").c_str();
		simpleString armor =  simpleExt::format(itemFormat.c_str(), "d", "[", "Leather Armor", "[3]<10> (worn)").c_str();
		simpleString food = simpleExt::format(itemFormat.c_str(), "a", ";", "Some food", "").c_str();
		simpleString darts = simpleExt::format(itemFormat.c_str(), "c", brogueGlyphMap::WeaponUnicode, "15 +0 darts", "<10>").c_str();

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

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, program, boundary, sceneBounds);

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

	brogueViewProgram* brogueUIBuilder::createGame()
	{
		brogueUIProgramPartId partId(brogueUIProgram::GameProgram, brogueUIProgramPart::GameSurface, 0);
		gridRect boundary = getBrogueStaticBoundary(partId);
		gridRect sceneBounds = getBrogueSceneBoundary();
		brogueUIData uiData(boundary, sceneBounds, _zoomLevel, colors::black());
		brogueGameView* gameView = new brogueGameView(_coordinateConverter, _resourceController, _eventController, partId, uiData);

		brogueUIProgramPartId debugPartId(brogueUIProgram::GameProgram, brogueUIProgramPart::Polygon, 0);
		brogueUIData debugUIData(boundary, sceneBounds, _zoomLevel, colors::transparent());
		brogueGraphView* graphView = new brogueGraphView(_coordinateConverter, _eventController, _resourceController, debugPartId, debugUIData);

		brogueViewProgram* container = new brogueViewProgram(_coordinateConverter, brogueUIProgram::GameProgram, boundary, sceneBounds);

		container->addView(gameView);
		container->addView(graphView);

		return container;
	}

	brogueViewProgram* brogueUIBuilder::createHighScoresView()
	{
		int menuPadding = 2;

		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, brogueUIProgram::HighScoresProgram, sceneBounds, sceneBounds);

		brogueUIProgramPartId partId(brogueUIProgram::HighScoresProgram, brogueUIProgramPart::Text, 0);
		brogueUIData uiData(sceneBounds, sceneBounds, _zoomLevel, MenuDefaultBackground());
		brogueTextView* textView = new brogueTextView(_coordinateConverter, _resourceController, _eventController, partId, uiData);

		textView->setUIAction(brogueUITagAction(brogueUIState::MainMenu));

		// Header and Footer
		textView->setLine(0, "-- High Scores --", colors::yellow(), brogueTextAlignment::Center);
		textView->setLine(sceneBounds.height - 1, "Press space to continue", FontHighScoresColor(), brogueTextAlignment::Center);

		for (int index = 0; index < _resourceController->getHighScoreCount(); index++)
		{
			brogueScoreEntry* entry = _resourceController->getHighScore(index);

			gridRect boundary(sceneBounds.column, sceneBounds.row + index + menuPadding, sceneBounds.width, 1);

			simpleString preamble = simpleExt::format("{}) ", index + 1).c_str();
			simpleString score = simpleExt::toString(entry->getScore()).c_str();
			simpleString dateAndDescr = simpleExt::format("{} {}", entry->getDate()->c_str(), entry->getDescription()->c_str()).c_str();

			// Size to line up scores (5)
			preamble.appendPadding(' ', 5 - preamble.count());

			// Scores (15)
			score.appendPadding(' ', 15 - score.count() - preamble.count());

			simpleString formattedEntry = simpleExt::format("{}{}{}", preamble.c_str(), score.c_str(), dateAndDescr.c_str()).c_str();

			color foreGround = color::interpolate(colors::white(), colors::black(), index / (float)_resourceController->getHighScoreCount());

			colorString coloredEntry(formattedEntry.c_str(), foreGround);

			// Add header offset
			textView->setLine(index + 2, coloredEntry, brogueTextAlignment::Center);
		}

		result->addView(textView);

		return result;
	}

	brogueViewProgram* brogueUIBuilder::createHeaderedBackground(brogueUIProgram programName, const colorString& headerText)
	{
		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::MenuBackground, 0);

		gridRect sceneBounds = getBrogueSceneBoundary();
		gridRect boundary = getBrogueStaticBoundary(menuId);

		brogueViewProgram* result = new brogueViewProgram(_coordinateConverter, programName, boundary, sceneBounds);

		// Text View
		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Text, 0);

		// Menu Background
		brogueUIData uiData(boundary, sceneBounds, _zoomLevel, MenuBackgroundColor1(), MenuBackgroundColor2(), brogueGradientType::Circular);

		// Handles lines of text / a paragraph
		brogueTextView* textView = new brogueTextView(_coordinateConverter, _resourceController, _eventController, partId, uiData);

		// Header
		textView->setLine(boundary.top(), headerText, brogueTextAlignment::Center);

		result->addView(textView);

		return result;
	}

	brogueViewProgram* brogueUIBuilder::createPlaceholder(brogueUIProgram programName, const gridRect& boundary)
	{
		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Background, 0);
		gridRect sceneBounds = getBrogueSceneBoundary();
		brogueUIData uiData(boundary, sceneBounds, _zoomLevel, _randomGenerator->nextColor(colors::getGray(0.2f), colors::getGray(0.5f)));
		brogueBackground* background = new brogueBackground(_coordinateConverter, _resourceController, _eventController, partId, uiData);
		brogueViewProgram* container = new brogueViewProgram(_coordinateConverter, programName, boundary, sceneBounds);

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
							MenuButtonColor1(),
							MenuButtonColor2(),
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
							FlatMenuButtonColor(), 
							FlatMenuButtonColor(),
							FlatMenuButtonActiveColor1(), 
							FlatMenuButtonActiveColor2(), 
							FlatMenuButtonPressedColor1(), 
							FlatMenuButtonPressedColor2(), 
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
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId partId(programName, brogueUIProgramPart::Button, _buttonCounter++);

		brogueUIData buttonData(boundary,
								sceneBounds,
								_zoomLevel,
								background1, background2,
								hover1, hover2,
								pressed1, pressed2,
								brogueGradientType::Horizontal);

		brogueButton* button = new brogueButton(_coordinateConverter, _resourceController, _eventController, partId, buttonData);

		button->setUI(text, alignment);
		button->setUIAction(tagAction);

		return button;
	}
	brogueTextView* brogueUIBuilder::createTextView(brogueUIProgram programName, 
													const gridRect& boundary, 
													const simpleString& text, 
													const color& foreground)
	{
		gridRect sceneBounds = getBrogueSceneBoundary();
		brogueUIProgramPartId dataId(programName, brogueUIProgramPart::Text, _textCounter++);
		brogueUIData data(boundary, sceneBounds, _zoomLevel, FontDefaultBackgroundColor());

		brogueTextView* textView = new brogueTextView(_coordinateConverter, _resourceController, _eventController, dataId, data);

		textView->setText(colorString(text.c_str(), foreground));

		return textView;
	}

	brogueTextView* brogueUIBuilder::createTextViewHeader(brogueUIProgram programName,
															const gridRect& boundary,
															const simpleString& text,
															const color& foreground)
	{
		gridRect sceneBounds = getBrogueSceneBoundary();
		brogueUIProgramPartId dataId(programName, brogueUIProgramPart::Text, _textCounter++);
		brogueUIData data(boundary, sceneBounds, _zoomLevel, MenuDefaultBackground());

		brogueTextView* textView = new brogueTextView(_coordinateConverter, _resourceController, _eventController, dataId, data);

		textView->setLine(boundary.top(), text, brogueTextAlignment::Center);

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
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId dataId(programName, brogueUIProgramPart::Text, _textCounter++);
		brogueUIData data(boundary, sceneBounds, _zoomLevel, background1, background2, brogueGradientType::Horizontal);

		brogueTextView* textView = new brogueTextView(_coordinateConverter, _resourceController, _eventController, dataId, data);

		textView->setLine(boundary.top(), colorString(text.c_str(), foreground), alignment);

		return textView;
	}

	brogueBackground* brogueUIBuilder::createBackground(brogueUIProgram programName, const gridRect& boundary)
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::Background, _menuBackgroundCounter++);

		brogueUIData menuData(boundary, sceneBounds, _zoomLevel, MenuBackgroundColor1());

		return new brogueBackground(_coordinateConverter, _resourceController, _eventController, menuId, menuData);
	}

	brogueScrollView* brogueUIBuilder::createScrollView(brogueUIProgram programName, const gridRect& boundary)
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId scrollId(programName, brogueUIProgramPart::MenuBackground, 0);

		brogueUIData scrollData(boundary, sceneBounds, _zoomLevel, MenuDefaultBackground(), MenuDefaultBackground(), brogueGradientType::Circular);

		return new brogueScrollView(_coordinateConverter, _resourceController, _eventController, scrollId, scrollData);
	}

	brogueMenuBackground* brogueUIBuilder::createMainMenuBackground(brogueUIProgram programName, const gridRect& boundary)
	{
		return createMenuBackground(programName, boundary, MenuDefaultBackground(), colors::transparent(), brogueGradientType::Circular);
	}
	brogueMenuBackground* brogueUIBuilder::createMenuBackground(brogueUIProgram programName, 
																const gridRect& boundary, 
																const color& background1, 
																const color& background2, 
																brogueGradientType gradient)
	{
		gridRect sceneBounds = getBrogueSceneBoundary();

		brogueUIProgramPartId menuId(programName, brogueUIProgramPart::MenuBackground, _menuBackgroundCounter++);

		brogueUIData menuData(boundary, sceneBounds, _zoomLevel, background1, background2, brogueGradientType::Circular);

		return new brogueMenuBackground(_coordinateConverter, _resourceController, _eventController, menuId, menuData);
	}
}
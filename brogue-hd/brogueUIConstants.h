#pragma once

namespace brogueHd::frontend
{
	enum class brogueUIProgram : int
	{
		/// <summary>
		/// This type of program is managed by a parent. So, the response should never
		/// indicate that it needs to be managed. An exception is thrown if this name is
		/// allowed to bubble up to the opengl controller.
		/// </summary>
		ContainerControlledProgram = 0,

		/// <summary>
		/// The primary brogue flame menu title program
		/// </summary>
		FlameMenuProgram = 1,

		/// <summary>
		/// The primary brogue flame menu title mask program
		/// </summary>
		FlameMenuTitleMaskProgram = 2,

		/// <summary>
		/// The main menu button menu program
		/// </summary>
		MainMenuProgram = 3,

		/// <summary>
		/// The title screen's open menu program (scrollable area)
		/// </summary>
		OpenMenuProgram = 4,

		/// <summary>
		/// The title screen's open menu header'd background program
		/// </summary>
		OpenMenuBackgroundProgram = 5,

		/// <summary>
		/// The title screen's view recording program (scrollable area)
		/// </summary>
		PlaybackMenuProgram = 6,

		/// <summary>
		/// The title screen's view recording menu header'd background program
		/// </summary>
		PlaybackMenuBackgroundProgram = 7,

		/// <summary>
		/// The title screen's high scores program
		/// </summary>
		HighScoresProgram = 8,

		/// <summary>
		/// The generic modal program
		/// </summary>
		ModalProgram = 9,

		/// <summary>
		/// The bottom menu bar in the game
		/// </summary>
		BottomBarMenuProgram = 10,

		/// <summary>
		/// Upper left hand corner status panel (permanent, static). Search bar is part of this panel.
		/// </summary>
		PlayerStatusPanelProgram = 11,

		/// <summary>
		/// Scrollable object list program on the left hand side.
		/// </summary>
		GameObjectListProgram = 12,

		/// <summary>
		/// Flavor text panel program across the bottom above the menu bar
		/// </summary>
		FlavorTextPanelProgram = 13,

		/// <summary>
		/// Primary Brogue game display program
		/// </summary>
		GameProgram = 15,

		/// <summary>
		/// Game menu program, bottom right
		/// </summary>
		GameMenuProgram = 16,

		/// <summary>
		/// Dynamically sized inventory program - right side - non-scrollable
		/// </summary>
		GameInventoryProgram = 17,

		/// <summary>
		/// Program to show text to the user in a briefly "modal" fashion (darkening the background). This
		/// must be positioned in one of the 4 quadrants of the screen to avoid covering up the object of
		/// the text description. 
		/// </summary>
		GameHoverTextProgram = 18,

		/// <summary>
		/// Program to show the game log - minimized to the top 3 rows; and expanded to show 34 total rows
		/// of text. This will require dynamic sizing. 
		/// </summary>
		GameLogProgram = 19,

		/// <summary>
		/// Program to show the help display
		/// </summary>
		GameHelpProgram = 20,

		/// <summary>
		/// Program to show the discovered items display
		/// </summary>
		GameDiscoveredItems = 21
	};

	enum class brogueUIProgramPart : int
	{
		ViewCompositor = 0,

		Background = 1,
		Text = 2,
		Button = 3,
		MenuBackground = 4,

		FlameDisplay = 5,
		ColorMask = 6
	};

	enum class brogueProgramPurpose : int
	{
		/// <summary>
		/// No action required by the program. This implies that no data is gathered when the user
		/// interacts with the program which will be needed for further processing.
		/// </summary>
		None = 0,
		Game_XYZ = 2, 
		MenuItem = 3,
		OpenFileItem = 4,
		ModalSelector = 5,
	};

	enum class brogueProgramExitCondition : int
	{
		OnMouseButton = 1,
		OnMouseClickOff = 2,
		OnKeyboardAny = 4,
		OnKeyboardEsc = 8,
		OnKeyboardHotkey = 16,
		Any = 32
	};

	enum class brogueUIAction
	{
		None = 0,
		ViewMainMenu = 1,
		ViewOpenGameMenu = 2,
		ViewPlaybackMenu = 3,
		ViewHighScores = 4,
		NewGame = 5,
		OpenGame = 6,
		ShowQuitGameModal = 7,
		QuitGame = 8
	};

	enum class brogueTextAlignment
	{
		Left = 0,
		Center = 1,
		Right = 2
	};

	enum class openglDataStreamType
	{
		brogueImageQuad = 0,
		brogueCellQuad = 1,
		brogueColorQuad = 2,
		brogueFlameQuad = 3
	};

	enum class openglBrogueCellOutputSelector
	{
		/// <summary>
		/// Outputs a default color for the cell display
		/// </summary>
		NoDisplay = 0,

		/// <summary>
		/// Outputs the contents of the current cell
		/// </summary>
		Display = 1,

		/// <summary>
		/// Outputs the currently drawn contents of the current cell.
		/// </summary>
		DisplayCurrentFrame = 2
	};
}
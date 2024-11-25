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
		/// Scrollable object list program on the left hand side. (This includes the player status panel)
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
		GameDiscoveredItems = 21,

		/// <summary>
		/// Overlay for debugging various pieces of the game
		/// </summary>
		DebugOverlay = 22
	};

	enum class brogueUIProgramPart : int
	{
		/// <summary>
		/// Frame-type data stream for a single color output to the GPU. Can carry mouse interaction.
		/// </summary>
		Background = 0,

		/// <summary>
		/// A single (or) multi-line non-updating view of color string characters. Does not utilize the mouse
		/// for individual cell update; but can forward click, and hover events.
		/// </summary>
		Text = 1,

		/// <summary>
		/// A cell-type data stream with mouse interaction, click events, and hotkey events.
		/// </summary>
		Button = 2,

		/// <summary>
		/// Cell-type data stream for a more detailed color output to the GPU. Can carry mouse interaction.
		/// </summary>
		MenuBackground = 3,

		/// <summary>
		/// A specific data stream type for "flame quads". This has, at the moment, only a single use - the
		/// title display.
		/// </summary>
		FlameDisplay = 4,

		/// <summary>
		/// A color mask cell-type data stream with reduced data output; and non-mouse interacting handling.
		/// </summary>
		ColorMask = 5,

		/// <summary>
		/// A polygon (view) data stream with optional mouse interaction
		/// </summary>
		Polygon = 6
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

		/// <summary>
		/// Generic game command to change the state of the game or UI
		/// </summary>
		StateChange = 1,
		NewGame = 2,
		OpenGame = 3,
		QuitGame = 4,

		GameCommand_Autopilot = 5,
		GameCommand_EasyMode = 6,
		GameCommand_Explore = 7,
		GameCommand_HoverText = 8,
		GameCommand_Inventory = 9,
		GameCommand_InventorySelect = 10,
		GameCommand_Menu = 11,
		GameCommand_Rest = 12,
		GameCommand_Save = 13,
		GameCommand_Search = 14,
		GameCommand_ToggleLog = 15,
		GameCommand_ToggleStealthRange = 16,
		GameCommand_ViewDungeonSeed = 17
	};

	enum class brogueUIState : int
	{
		MainMenu = 0,
		OpenMenu = 1,
		PlaybackMenu = 2,
		HighScores = 3,
		Modal = 4,
		GameNormal = 5,
		GameLogOpen = 6,
		GameMenuOpen = 7,
		GameInventoryOpen = 8,
		GameDiscoveredItemsOpen = 9,
		GameHelpOpen = 10
	};

	enum class brogueUIAnimation
	{
		None = 0,
		GameLogScrollOpen = 1,
		GameLogScrollClose = 2
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
		broguePolygon = 3
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
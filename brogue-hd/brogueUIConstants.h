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
		/// The main menu button menu program
		/// </summary>
		MainMenuProgram = 2,

		/// <summary>
		/// The title screen's open menu program
		/// </summary>
		OpenMenuProgram = 3,

		/// <summary>
		/// The title screen's open menu header'd background program
		/// </summary>
		OpenMenuProgram_HeaderProgram = 4,

		/// <summary>
		/// The title screen's view recording program
		/// </summary>
		PlaybackMenuProgram = 5,

		/// <summary>
		/// The title screen's view recording menu header'd background program
		/// </summary>
		PlaybackMenuProgram_HeaderProgram = 6,

		/// <summary>
		/// The title screen's high scores program
		/// </summary>
		HighScoresProgram = 7,

		/// <summary>
		/// The quit modal program
		/// </summary>
		ModalProgram = 8
	};

	enum class brogueUIProgramPart : int
	{
		ViewCompositor = 0,

		Background = 1,
		Text = 2,
		MenuButton = 3,
		MenuBackground = 4,

		FlameMenuProgram_HeatSourceProgram = 5,
		FlameMenuProgram_TitleMaskProgram = 6,
		FlameMenuProgram_HeatDiffuseProgram = 7,
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
		brogueAdjacencyColorQuad = 3
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
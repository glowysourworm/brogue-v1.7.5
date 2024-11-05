#pragma once

namespace brogueHd::frontend
{
	enum brogueUIProgram : int
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
		/// The title screen's view recording program
		/// </summary>
		PlaybackMenuProgram = 4,

		/// <summary>
		/// The title screen's high scores program
		/// </summary>
		HighScoresProgram = 5,

		/// <summary>
		/// The quit modal program
		/// </summary>
		ModalProgram = 6
	};

	enum brogueProgramPurpose : int
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

	enum brogueProgramExitCondition : int
	{
		OnMouseButton = 1,
		OnMouseClickOff = 2,
		OnKeyboardAny = 4,
		OnKeyboardEsc = 8,
		OnKeyboardHotkey = 16,
		Any = 32
	};

	enum brogueOpenglDataStream
	{
		/// <summary>
		/// brogueDataStream renders the default brogueView* data - either frame or entire view, with 
		/// a user inclusion predicate.
		/// </summary>
		BrogueView = 0,

		/// <summary>
		/// Heat source stream for the title menu
		/// </summary>
		HeatSourceStream = 1,

		/// <summary>
		/// Title mask stream for  the title menu
		/// </summary>
		TitleMaskStream = 2
	};

	enum brogueUIContainer
	{
		TitleContainer = 0,
		GameContainer = 1
	};

	/// <summary>
	/// Static name list for UI views responsible for sending detailed interaction messages
	/// back to the opengl primary controller.
	/// </summary>
	enum brogueUIView
	{
		Unnamed = 0,
		MainMenuSelector = 1,
		OpenGameSelector = 2,
		PlaybackSelector = 3,
		HighScoresView = 4,
		QuitModal = 5,
		GameView = 6,
	};

	enum class brogueUIAction
	{
		None = 0,
		Close = 1,
		ViewMainMenu = 2,
		ViewOpenGameMenu = 3,
		ViewPlaybackMenu = 4,
		ViewHighScores = 5,
		NewGame = 6,
		OpenGame = 7,
		ShowQuitGameModal = 8,
		QuitGame = 9
	};

	enum brogueTextAlignment
	{
		Left = 0,
		Center = 1,
		Right = 2
	};

	enum openglDataStreamType
	{
		brogueImageQuad = 0,
		brogueCellQuad = 1,
		brogueColorQuad = 2
	};

	enum openglBrogueCellOutputSelector
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
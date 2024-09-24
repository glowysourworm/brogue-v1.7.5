#pragma once

namespace brogueHd
{
	enum brogueConsoleReturn
	{
		Continue = 0,
		Completed = 1,
		Completed_SetMode_Game = 2,
		Completed_SetMode_Dev = 3,
		Completed_SetMode_Resource = 4,
		CompletedWithError = 5,
		Exit = 6
	};

	enum NGCommands {
		NG_NOTHING = 0,
		NG_NEW_GAME,
		NG_NEW_GAME_WITH_SEED,
		NG_OPEN_GAME,
		NG_VIEW_RECORDING,
		NG_HIGH_SCORES,
		NG_SCUM,
		NG_QUIT,
	};

	/// <summary>
	/// Specific modes that relate to game controller settings. Changing modes means that the controller
	/// will reset for the next mode. This has an impact on loaded variables.
	/// </summary>
	enum BrogueGameMode {
		Menu = 0,
		MenuHighScores = 1,
		MenuOpenGame = 2,
		MenuSetSeed = 3,
		Game = 4,
		Playback = 5,
		Scum = 6,
		Quit = 7
	};
}
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "gridRect.h"

using namespace brogueHd::component;

namespace brogueHd
{
	/*	
		Embedded Resources: These are compiled into the executable; and must be read from
							there using the resourceController		
	*/

	enum shaderResource
	{
		colorMaskVert = 0,
		colorMaskFrag = 1,
		backgroundColorVert = 2,
		backgroundColorFrag = 3,
		brogueCellDisplayVert = 4,
		brogueCellDisplayFrag = 5,
		diffuseColorUpwardVert = 6,
		diffuseColorUpwardFrag = 7,
		mixFrameTexturesVert = 8,
		mixFrameTexturesFrag = 9
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

	// {} -> savedGamesDirectory, resourceConfig { ... }, ...

	static const char* ConfigResourceConfig = "resourceConfig";

	static const char* ConfigColorMaskVertShader = "colorMaskVertexShader";
	static const char* ConfigColorMaskFragShader = "colorMaskFragmentShader";
	static const char* ConfigBackgroundColorVertexShader = "backgroundColorVertexShader";
	static const char* ConfigBackgroundColorFragmentShader = "backgroundColorFragmentShader";
	static const char* ConfigBrogueCellDisplayVertShader = "brogueCellDisplayVertexShader";
	static const char* ConfigBrogueCellDisplayFragShader = "brogueCellDisplayFragmentShader";
	static const char* ConfigDiffuseColorUpwardVertexShader = "diffuseColorUpwardVertexShader";
	static const char* ConfigDiffuseColorUpwardFragmentShader = "diffuseColorUpwardFragmentShader";
	static const char* ConfigMixFrameTexturesVertexShader = "mixFrameTexturesVertexShader";
	static const char* ConfigMixFrameTexturesFragmentShader = "mixFrameTexturesFragmentShader";

	static const char* ConfigSavedGameDirectory = "savedGameDirectory";
	static const char* ConfigFontGlyphDirectory = "fontGlyphDirectory";

	static const char* FontGlyphFileNameFormat = "font-{}.bmp";
	static const char* FontGlyphFileNameFormat_RGBA24_UNSIGNED = "font-{}-RGBA24-unsigned.bmp";
	static const char* FontGlyphFileNameFormat_RGBA32F = "font-{}-RGBA32F.bmp";
	
	static const char* BrogueHighScoresFile = "BrogueHighScores.txt";


	/*
	
		Global Enums (console level)
	
	*/

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

	/*

		Global Macros:  These are preprocessed #define statements for all Rogue macros

	*/

	#define MIN_ZOOM	1
	#define MAX_ZOOM	13

	#define GLYPH_SHEET_COLUMNS		16
	#define GLYPH_SHEET_ROWS		14

	// <windows.h> has a conflict with std::byte. That was rather frustrating. Please use char* or
	//			   std unless you have to deal with windows libraries. 
	//				


	// unicode: comment this line to revert to ASCII

	//#define USE_UNICODE

	// version string -- no more than 16 bytes:
	#define BROGUE_VERSION_STRING "1.7.5"

	//// debug macros -- define DEBUGGING as 1 to enable wizard mode.
	//#define DEBUGGING						0

	//#define DEBUG							if (DEBUGGING)
	//#define MONSTERS_ENABLED				(!DEBUGGING || 1)	// Quest room monsters can be generated regardless.
	//#define ITEMS_ENABLED					(!DEBUGGING || 1)

	//#define D_BULLET_TIME					(DEBUGGING && 0)
	//#define D_WORMHOLING					(DEBUGGING && 1)
	//#define D_IMMORTAL						(DEBUGGING && 1)

	//#define D_SAFETY_VISION					(DEBUGGING && 0)
	//#define D_SCENT_VISION                  (DEBUGGING && 0)
	//#define D_DISABLE_BACKGROUND_COLORS		(DEBUGGING && 0)

	//#define D_INSPECT_LEVELGEN				(DEBUGGING && 0)
	//#define D_INSPECT_MACHINES				(DEBUGGING && 0)

	//#define D_MESSAGE_ITEM_GENERATION       (DEBUGGING && 0)
	//#define D_MESSAGE_MACHINE_GENERATION    (DEBUGGING && 0)

	//// set to false to allow multiple loads from the same saved file:
	//#define DELETE_SAVE_FILE_AFTER_LOADING	true

	//// set to false to disable references to keystrokes (e.g. for a tablet port)
	//#define KEYBOARD_LABELS true

	//#define BROGUE_ASSERTS		// introduces several assert()s -- useful to find certain array overruns and other bugs
	//#define AUDIT_RNG             // VERY slow, but sometimes necessary to debug out-of-sync recording errors
	//#define GENERATE_FONT_FILES	// Displays font in grid upon startup, which can be screen-captured into font files for PC.

	//#ifdef BROGUE_ASSERTS
	//#include <assert.h>
	//#define brogueAssert(x)         assert(x)
	//#else
	//#define brogueAssert(x)
	//#endif

	////#define boolean					char

	//#define false					0
	//#define true					1

	#define Fl(N)					(1 << (N))
	//#define FP_BASE 16 // Don't change this without recalculating all of the power tables throughout the code!
	//#define FP_FACTOR (1LL << FP_BASE)

	// recording and save filenames
	#define LAST_GAME_PATH			"LastGame.broguesave"
	#define LAST_GAME_NAME          "LastGame"
	#define LAST_RECORDING_NAME     "LastRecording"
	#define RECORDING_SUFFIX		".broguerec"
	#define GAME_SUFFIX				".broguesave"
	#define ANNOTATION_SUFFIX		".txt"
	#define RNG_LOG					"RNGLog.txt"

	// Random Generator Instances:		Separates instances for substantiative / non-substantiative generation (affects game linearity)
	#define RANDOM_GENERATOR_MAIN		1
	#define RANDOM_GENERATOR_COSMETIC	2

	// recording constants
	#define MAX_RECORDING_PLAYBACK  200000000 // Approx. 200MB of playback (~ 1.49 million brogue events)

	//#define BROGUE_FILENAME_MAX		(min(1024*4, FILENAME_MAX))

	// Allows unicode characters:
	//#define uchar					unsigned short

	#define MESSAGE_LINES			3
	#define MESSAGE_ARCHIVE_LINES	ROWS

	// Size of the entire terminal window. These need to be hard-coded here and in Viewport.h
	#define COLS					100
	#define ROWS					(31 + MESSAGE_LINES)

	// Size of the portion of the terminal window devoted to displaying the dungeon:
	#define DCOLS					(COLS - STAT_BAR_WIDTH - 1) // n columns on the left for the sidebar;
															// one column to separate the sidebar from the map.
	#define DROWS					(ROWS - MESSAGE_LINES - 2)	// n lines at the top for messages;
															// one line at the bottom for flavor text;
															// another line at the bottom for the menu bar.

	#define CELLULAR_AUTOMATA_MAX	10			// max number of smoothing iterations for CA algorithm

	#define STAT_BAR_WIDTH			20			// number of characters in the stats bar to the left of the map

	#define LOS_SLOPE_GRANULARITY	32768		// how finely we divide up the squares when calculating slope;
												// higher numbers mean fewer artifacts but more memory and processing
	#define INTERFACE_OPACITY		95

	#define LIGHT_SMOOTHING_THRESHOLD 150       // light components higher than this magnitude will be toned down a little

	#define MAX_BOLT_LENGTH			DCOLS*10

	#define VISIBILITY_THRESHOLD	50			// how bright cumulative light has to be before the cell is marked visible

	#define AMULET_LEVEL			26			// how deep before the amulet appears
	#define DEEPEST_LEVEL           40          // how deep the universe goes

	// Flame Menu
	#define MENU_TITLE_WIDTH	75
	#define MENU_TITLE_HEIGHT	19

	//#define MACHINES_FACTOR         (1 << FP_BASE)         // use this to adjust machine frequency

	//#define MACHINES_BUFFER_LENGTH  200

	//#define WEAPON_KILLS_TO_AUTO_ID	20
	//#define ARMOR_DELAY_TO_AUTO_ID	1000
	//#define RING_DELAY_TO_AUTO_ID	1500

	//#define FALL_DAMAGE_MIN         8
	//#define FALL_DAMAGE_MAX         10

	//#define INPUT_RECORD_BUFFER		1000		// how many bytes of input data to keep in memory before saving it to disk
	//#define DEFAULT_PLAYBACK_DELAY	50

	//#define HIGH_SCORES_COUNT		30

	//// color escapes
	//#define COLOR_ESCAPE			25
	//#define COLOR_VALUE_INTERCEPT	25

	// display characters:

	//#ifdef USE_UNICODE

	#define FLOOR_CHAR		0x00b7
	#define LIQUID_CHAR		'~'
	#define CHASM_CHAR		0x2237
	#define TRAP_CHAR		0x25c7
	#define FIRE_CHAR		0x22CF
	#define GRASS_CHAR		'"'
	#define BRIDGE_CHAR		'='
	#define DESCEND_CHAR	'>'
	#define ASCEND_CHAR		'<'
	#define WALL_CHAR		'#'
	#define DOOR_CHAR		'+'
	#define OPEN_DOOR_CHAR	'\''
	#define ASH_CHAR		'\''
	#define BONES_CHAR		','
	#define MUD_CHAR		','
	#define WEB_CHAR		':'
	//#define FOLIAGE_CHAR	0x03A8 // lower-case psi
	#define FOLIAGE_CHAR	0x2648 // Aries symbol
	#define VINE_CHAR       ':'
	#define ALTAR_CHAR		'|'
	#define LEVER_CHAR      '/'
	#define LEVER_PULLED_CHAR '\\'
	#define STATUE_CHAR		0x00df
	#define VENT_CHAR		'='
	#define DEWAR_CHAR      '&'

	//#define TRAMPLED_FOLIAGE_CHAR	'"'		// 0x2034 // 0x2037

	//#define PLAYER_CHAR		'@'

	//#define AMULET_CHAR		0x2640
	//#define FOOD_CHAR		';'
	//#define SCROLL_CHAR		0x266A//'?'		// 0x039E
	//#define RING_CHAR		0x26AA
	////#define RING_CHAR		0xffee
	//#define CHARM_CHAR      0x03DF
	//#define POTION_CHAR		'!'
	//#define ARMOR_CHAR		'['
	//#define WEAPON_CHAR		0x2191
	//#define STAFF_CHAR		'\\'
	//#define WAND_CHAR		'~'
	//#define GOLD_CHAR		'*'
	//#define GEM_CHAR		0x25cf
	//#define TOTEM_CHAR		0x26b2
	//#define TURRET_CHAR		0x25cf
	//#define UNICORN_CHAR    0x00da
	//#define KEY_CHAR		'-'
	//#define ELECTRIC_CRYSTAL_CHAR 0x00A4

	//#define UP_ARROW_CHAR		0x2191
	//#define DOWN_ARROW_CHAR		0x2193
	//#define LEFT_ARROW_CHAR		0x2190
	//#define RIGHT_ARROW_CHAR	0x2192
	//#define UP_TRIANGLE_CHAR	0x2206
	//#define DOWN_TRIANGLE_CHAR	0x2207
	//#define OMEGA_CHAR			0x03A9
	//#define THETA_CHAR			0x03B8
	//#define LAMDA_CHAR			0x03BB
	//#define KOPPA_CHAR			0x03DF//0x03DE
	//#define LOZENGE_CHAR		0x29EB
	//#define CROSS_PRODUCT_CHAR	0x2A2F

	//#define CHAIN_TOP_LEFT		'\\'
	//#define CHAIN_BOTTOM_RIGHT	'\\'
	//#define CHAIN_TOP_RIGHT		'/'
	//#define CHAIN_BOTTOM_LEFT	'/'
	//#define CHAIN_TOP			'|'
	//#define CHAIN_BOTTOM		'|'
	//#define CHAIN_LEFT			'-'
	//#define CHAIN_RIGHT			'-'

	//#define BAD_MAGIC_CHAR		0x29F2
	//#define GOOD_MAGIC_CHAR		0x29F3

	//#else

	//#define FLOOR_CHAR		'.'
	//#define LIQUID_CHAR		'~'
	//#define CHASM_CHAR		':'
	//#define TRAP_CHAR		'%'
	//#define FIRE_CHAR		'^'
	//#define GRASS_CHAR		'"'
	//#define BRIDGE_CHAR		'='
	//#define DESCEND_CHAR	'>'
	//#define ASCEND_CHAR		'<'
	//#define WALL_CHAR		'#'
	//#define DOOR_CHAR		'+'
	//#define OPEN_DOOR_CHAR	'\''
	//#define ASH_CHAR		'\''
	//#define BONES_CHAR		','
	//#define MUD_CHAR		','
	//#define WEB_CHAR		':'
	//#define FOLIAGE_CHAR	'&'
	//#define VINE_CHAR       ':'
	//#define ALTAR_CHAR		'|'
	//#define LEVER_CHAR      '/'
	//#define LEVER_PULLED_CHAR '\\'
	//#define STATUE_CHAR		'&'
	//#define VENT_CHAR		'='
	//#define DEWAR_CHAR      '&'

	//#define TRAMPLED_FOLIAGE_CHAR	'"'

	//#define PLAYER_CHAR		'@'

	//#define AMULET_CHAR		','
	//#define FOOD_CHAR		';'
	//#define SCROLL_CHAR		'?'
	//#define RING_CHAR		'='
	//#define CHARM_CHAR      '+'
	//#define POTION_CHAR		'!'
	//#define ARMOR_CHAR		'['
	//#define WEAPON_CHAR		'('
	//#define STAFF_CHAR		'\\'
	//#define WAND_CHAR		'~'
	//#define GOLD_CHAR		'*'
	//#define GEM_CHAR		'+'
	//#define TOTEM_CHAR		'0'
	//#define TURRET_CHAR		'*'
	//#define UNICORN_CHAR    'U'
	//#define KEY_CHAR		'-'
	//#define ELECTRIC_CRYSTAL_CHAR '$'

	//#define UP_ARROW_CHAR		'^'
	//#define DOWN_ARROW_CHAR		'v'
	//#define LEFT_ARROW_CHAR		'<'
	//#define RIGHT_ARROW_CHAR	'>'
	//#define UP_TRIANGLE_CHAR	'^'
	//#define DOWN_TRIANGLE_CHAR	'v'
	//#define OMEGA_CHAR			'^'
	//#define THETA_CHAR			'0'
	//#define LAMDA_CHAR			'\\'
	//#define KOPPA_CHAR			'k'
	//#define LOZENGE_CHAR		'+'
	//#define CROSS_PRODUCT_CHAR	'x'

	//#define CHAIN_TOP_LEFT		'\\'
	//#define CHAIN_BOTTOM_RIGHT	'\\'
	//#define CHAIN_TOP_RIGHT		'/'
	//#define CHAIN_BOTTOM_LEFT	'/'
	//#define CHAIN_TOP			'|'
	//#define CHAIN_BOTTOM		'|'
	//#define CHAIN_LEFT			'-'
	//#define CHAIN_RIGHT			'-'

	//#define BAD_MAGIC_CHAR		'+'
	//#define GOOD_MAGIC_CHAR		'$'

	//#endif






	//#define NUMBER_DYNAMIC_COLORS	6

	//#define MAX_PACK_ITEMS				26
	//#define NUMBER_MUTATORS             8
	//#define	NUMBER_HORDES				177
	//#define MONSTER_CLASS_COUNT         15

	//// flavors

	//#define NUMBER_ITEM_COLORS			21
	//#define NUMBER_TITLE_PHONEMES		21
	//#define NUMBER_ITEM_WOODS			21
	//#define NUMBER_POTION_DESCRIPTIONS	18
	//#define NUMBER_ITEM_METALS			12
	//#define NUMBER_ITEM_GEMS			18

	//#define TURNS_FOR_FULL_REGEN				300
	//#define STOMACH_SIZE						2150
	//#define HUNGER_THRESHOLD					(STOMACH_SIZE - 1800)
	//#define WEAK_THRESHOLD						150
	//#define FAINT_THRESHOLD						50
	//#define MAX_EXP_LEVEL						20
	//#define MAX_EXP								100000000L

	//#define XPXP_NEEDED_FOR_TELEPATHIC_BOND     1400 // XPXP required to enable telepathic awareness with the ally

	//#define ROOM_MIN_WIDTH						4
	//#define ROOM_MAX_WIDTH						20
	//#define ROOM_MIN_HEIGHT						3
	//#define ROOM_MAX_HEIGHT						7
	//#define HORIZONTAL_CORRIDOR_MIN_LENGTH		5
	//#define HORIZONTAL_CORRIDOR_MAX_LENGTH		15
	//#define VERTICAL_CORRIDOR_MIN_LENGTH		2
	//#define VERTICAL_CORRIDOR_MAX_LENGTH		9
	//#define CROSS_ROOM_MIN_WIDTH				3
	//#define CROSS_ROOM_MAX_WIDTH				12
	//#define CROSS_ROOM_MIN_HEIGHT				2
	//#define CROSS_ROOM_MAX_HEIGHT				5
	//#define MIN_SCALED_ROOM_DIMENSION			2

	//#define ROOM_TYPE_COUNT                     8

	//#define CORRIDOR_WIDTH						1

	//#define WAYPOINT_SIGHT_RADIUS				10
	//#define MAX_WAYPOINT_COUNT                  40

	//#define MAX_ITEMS_IN_MONSTER_ITEMS_HOPPER   100

	// Making these larger means cave generation will take more trials; set them too high and the program will hang.
	#define CAVE_MIN_WIDTH						50
	#define CAVE_MIN_HEIGHT						20

	//// Keyboard commands:
	//#define UP_KEY				'k'
	//#define DOWN_KEY			'j'
	//#define LEFT_KEY			'h'
	//#define RIGHT_KEY			'l'
	//#define UP_ARROW			63232
	//#define LEFT_ARROW			63234
	//#define DOWN_ARROW			63233
	//#define RIGHT_ARROW			63235
	//#define UPLEFT_KEY			'y'
	//#define UPRIGHT_KEY			'u'
	//#define DOWNLEFT_KEY		'b'
	//#define DOWNRIGHT_KEY		'n'
	//#define DESCEND_KEY			'>'
	//#define ASCEND_KEY			'<'
	//#define REST_KEY			'z'
	//#define AUTO_REST_KEY		'Z'
	//#define SEARCH_KEY			's'
	//#define INVENTORY_KEY		'i'
	//#define ACKNOWLEDGE_KEY		' '
	//#define EQUIP_KEY			'e'
	//#define UNEQUIP_KEY			'r'
	//#define APPLY_KEY			'a'
	//#define THROW_KEY			't'
	//#define RELABEL_KEY         'R'
	//#define TRUE_COLORS_KEY		'\\'
	//#define AGGRO_DISPLAY_KEY   ']'
	//#define DROP_KEY			'd'
	//#define CALL_KEY			'c'
	//#define QUIT_KEY			'Q'
	//#define MESSAGE_ARCHIVE_KEY	'M'
	//#define HELP_KEY			'?'
	//#define DISCOVERIES_KEY		'D'
	//#define EXPLORE_KEY			'x'
	//#define AUTOPLAY_KEY		'A'
	//#define SEED_KEY			'~'
	//#define EASY_MODE_KEY		'&'
	//#define ESCAPE_KEY			'\033'
	//#define RETURN_KEY			'\015'
	//#define ENTER_KEY			'\012'
	//#define DELETE_KEY			'\177'
	//#define TAB_KEY				'\t'
	//#define SHIFT_TAB_KEY       25 // Cocoa reports shift-tab this way for some reason.
	//#define PERIOD_KEY			'.'
	//#define VIEW_RECORDING_KEY	'V'
	//#define LOAD_SAVED_GAME_KEY	'O'
	//#define SAVE_GAME_KEY		'S'
	//#define NEW_GAME_KEY		'N'
	//#define NUMPAD_0			48
	//#define NUMPAD_1			49
	//#define NUMPAD_2			50
	//#define NUMPAD_3			51
	//#define NUMPAD_4			52
	//#define NUMPAD_5			53
	//#define NUMPAD_6			54
	//#define NUMPAD_7			55
	//#define NUMPAD_8			56
	//#define NUMPAD_9			57
	//#define PAGE_UP_KEY			63276
	//#define PAGE_DOWN_KEY		63277

	//#define UNKNOWN_KEY			(128+19)

	//#define min(x, y)		(((x) < (y)) ? (x) : (y))
	//#define max(x, y)		(((x) > (y)) ? (x) : (y))
	//#define clamp(x, low, hi)	(min(hi, max(x, low))) // pins x to the [y, z] interval

	//#define terrainFlags(x, y)					(tileCatalog[pmap[x][y].layers[DUNGEON]].flags \
	//										| tileCatalog[pmap[x][y].layers[LIQUID]].flags \
	//										| tileCatalog[pmap[x][y].layers[SURFACE]].flags \
	//										| tileCatalog[pmap[x][y].layers[GAS]].flags)

	//#define terrainMechFlags(x, y)				(tileCatalog[pmap[x][y].layers[DUNGEON]].mechFlags \
	//										| tileCatalog[pmap[x][y].layers[LIQUID]].mechFlags \
	//										| tileCatalog[pmap[x][y].layers[SURFACE]].mechFlags \
	//										| tileCatalog[pmap[x][y].layers[GAS]].mechFlags)

	//#ifdef BROGUE_ASSERTS
	//boolean cellHasTerrainFlag(short x, short y, unsigned long flagMask);
	//#else
	//#define cellHasTerrainFlag(x, y, flagMask)	((flagMask) & terrainFlags((x), (y)) ? true : false)
	//#endif

	//#define cellHasTMFlag(x, y, flagMask)       ((flagMask) & terrainMechFlags((x), (y)) ? true : false)

	//#define cellHasTerrainType(x, y, terrain)	((pmap[x][y].layers[DUNGEON] == (terrain) \
	//										|| pmap[x][y].layers[LIQUID] == (terrain) \
	//										|| pmap[x][y].layers[SURFACE] == (terrain) \
	//										|| pmap[x][y].layers[GAS] == (terrain)) ? true : false)

	//#define cellHasKnownTerrainFlag(x, y, flagMask)	((flagMask) & pmap[(x)][(y)].rememberedTerrainFlags ? true : false)

	//#define cellIsPassableOrDoor(x, y)			(!cellHasTerrainFlag((x), (y), T_PATHING_BLOCKER) \
	//										|| (cellHasTMFlag((x), (y), (TM_IS_SECRET | TM_PROMOTES_WITH_KEY | TM_CONNECTS_LEVEL)) \
	//								    		&& cellHasTerrainFlag((x), (y), T_OBSTRUCTS_PASSABILITY)))

	//#define coordinatesAreInMap(x, y)			((x) >= 0 && (x) < DCOLS	&& (y) >= 0 && (y) < DROWS)
	//#define coordinatesAreInWindow(x, y)		((x) >= 0 && (x) < COLS		&& (y) >= 0 && (y) < ROWS)
	//#define mapToWindowX(x)						((x) + STAT_BAR_WIDTH + 1)
	//#define mapToWindowY(y)						((y) + MESSAGE_LINES)
	//#define windowToMapX(x)						((x) - STAT_BAR_WIDTH - 1)
	//#define windowToMapY(y)						((y) - MESSAGE_LINES)

	//#define playerCanDirectlySee(x, y)			(pmap[x][y].flags & VISIBLE)
	//#define playerCanSee(x, y)					(pmap[x][y].flags & ANY_KIND_OF_VISIBLE)
	//#define playerCanSeeOrSense(x, y)			((pmap[x][y].flags & ANY_KIND_OF_VISIBLE) \
	//										|| (rogue.playbackOmniscience \
	//											&& (pmap[x][y].layers[DUNGEON] != GRANITE || (pmap[x][y].flags & DISCOVERED))))

	//#define CYCLE_MONSTERS_AND_PLAYERS(x)		for ((x) = &player; (x) != NULL; (x) = ((x) == &player ? monsters->nextCreature : (x)->nextCreature))

	//#define assureCosmeticRNG					short oldRNG = rogue.RNG; rogue.RNG = RNG_COSMETIC;
	//#define restoreRNG							rogue.RNG = oldRNG;

	//#define MIN_COLOR_DIFF			600

	//// weighted sum of the squares of the component differences. Weights are according to color perception.
	//#define COLOR_DIFF(f, b)		 (((f).red - (b).red) * ((f).red - (b).red) * 0.2126 \
	//				    		+ ((f).green - (b).green) * ((f).green - (b).green) * 0.7152 \
	//							+ ((f).blue - (b).blue) * ((f).blue - (b).blue) * 0.0722)


	//#define KEY_ID_MAXIMUM	20

	//#define NUMBER_AUTOGENERATORS 49

	//#define PDS_FORBIDDEN   -1
	//#define PDS_OBSTRUCTION -2
	//#define PDS_CELL(map, x, y) ((map)->links + ((x) + DCOLS * (y)))

	//typedef struct pdsLink pdsLink;
	//typedef struct pdsMap pdsMap;			

}


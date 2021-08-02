#pragma once

#define XRES 1920
#define YRES 1080
#define STAGE_CLEAR_SCORE 3
#define MAX_STAGE 10
#define TITLE_BG_FRAME 150
#define MAIN_BG_FRAME 50
#define BAMM_FRAME 70
#define FLASH_SIZE 260
#define PIECE_BLOCK_X 32
#define PIECE_BLOCK_Y 32
#define MENU_SIZE_X 162
#define MENU_SIZE_Y 60
#define BMENU_SIZE_X 200
#define BMENU_SIZE_Y 72

#define BOARD_X 516
#define BOARD_Y 657
#define SMALL_BOARD_X 260
#define SMALL_BOARD_Y 331
#define SMALL_BOARD_INTERVAL 70
#define SCORE_SIZE_X 14
#define SCORE_SIZE_Y 25
#define BORDER_LINE_FRAME 100
#define SPARK_EFFECT_X 140
#define SPARK_EFFECT_Y 99
#define SPARK_SIZE 140

#define BLANK 0
#define MAPBLOCK 1
#define GREEN 2
#define RED 3
#define BLUE 4
#define ORANGE 5
#define PURPLE 6
#define YELLOW 7
#define SKY 8
#define PENALTYBLOCK 9

#define AI_DOWN_SPEED 200
#define AI_SIDEMOVE_SPEED 500
#define AI_ROTATE_TIME 1000

struct HANDLES
{
	HBITMAP hbmp, hOldbmp;
	HDC dc;
	BITMAP bit;
	POINT pt;
	BLENDFUNCTION bf;
};

struct UPDATE
{
	// move
	DWORD mStartTick;
	DWORD mEndTick;
	// animate
	DWORD aStartTick;
	DWORD aEndTick;

	int MaxFrame;
	int Frame;
	int dir;
	int size;
	int tricnt;
	BOOL bTrigger;
};

struct FRAME
{
	DWORD sTick;
	DWORD eTick;
	int StartFrameCount;
	int FrameCount;
	int FrameRate;
	BOOL bTrigger;
};

struct BlockRail
{
	class Block* data;
	BlockRail* pNode;
};

struct TopScore
{
	int data;
	TopScore* pNode;
};

enum GAME_STATE
{
	GAME_TITLE,
	GAME_MODE
};

enum MODES
{
	PLAYER_COM = 1,
	PLAYER_PLAYER = 2,
	PLAYER_MANY = 3,
	GAME_PLAY = 4
};

enum IN_MODE
{
	GAME_READY,
	GAME_START,
	GAME_MAIN,
	GAME_CLEAR
};

enum IN_GAME
{
	READY,
	START,
	PLAY,
	CLEAR,
	EXIT
};

enum SCORE
{
	ONE = 1,
	TWO = 2,
	THREE = 3,
	FOUR = 4,
	FIVE =5,
	SIX = 6,
	SEVEN = 7,
	EIGHT = 8,
	NINE = 9,
	ZERO = 0
};



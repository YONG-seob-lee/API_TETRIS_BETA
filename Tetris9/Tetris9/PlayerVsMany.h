#pragma once
#include "GameMode.h"
#include "framework.h"

class PlayerVsMany :
	public GameMode
{
private:
	Tetris* tetris[9];
	int state;
	int StageNum;

	int tScore[9][6];		// 배열형 스코어
	int tscore[9];			// 인트형 스코어
	int TplusScore[9];			// 추가된 점수 스코어
	int HitScore;

	// interFace //
	int TX;			// Player Point
	int TY;
	int tx[8];		// Computers Point
	int ty[8];

	HANDLES h_Main[MAIN_BG_FRAME];
	HANDLES h_TetrisBoard;
	HANDLES h_Score;
	HANDLES h_Ready;
	HANDLES h_Start;
	HANDLES h_Clear;
	HANDLES h_MenuBG;
	HANDLES h_BMenu;
	HANDLES h_Menu;
	HANDLES h_GameOver;

	UPDATE u_Main;
	UPDATE u_Scorecnt;
	UPDATE u_Ready;
	UPDATE u_Start;
	UPDATE u_BMenu;
	UPDATE u_Menu;
	UPDATE u_GotoPlay;
	UPDATE u_GotoTitle;
	UPDATE u_GotoExit;
	UPDATE u_Clear;
	UPDATE u_BackMenu;
	UPDATE u_GameOver;

public:
	PlayerVsMany();
	~PlayerVsMany();

	void LoadAll(HANDLES h_Base);
	void InitTetris();
	void InitData();
	void ResourceMainBG(HANDLES h_Base);


	///////////////		Draw	//////////////
	void DrawAll(HANDLES h_Base, RECT m_rtClient);
	void DrawMain(HANDLES h_Base);
	void DrawTetrisBoard(HANDLES h_Base);
	void DrawScore(HANDLES h_Base);
	void DrawMenu(HANDLES h_Base, RECT m_rtClient);
	void DrawMenuBoard(HANDLES h_Base, RECT m_rtClient);
	void DrawBButton(HANDLES h_Base, RECT m_rtClient);
	void DrawGotoPlayButton(HANDLES h_Base, RECT m_rtClient);
	void DrawGotoTitleButton(HANDLES h_Base, RECT m_rtClient);
	void DrawGotoExitButton(HANDLES h_Base, RECT m_rtClient);
	void DrawReady(HANDLES h_Base, RECT m_rtClient);
	void DrawStart(HANDLES h_Base, RECT m_rtClient);
	void DrawClear(HANDLES h_Base, RECT m_rtClient);
	void DrawGameOver(HANDLES h_Base);

	///////////////		Update	///////////////
	void UpdateAll();
	void UpdateMain();
	void UpdateReady();
	void UpdateStart();
	void UpdateMenu();
	void UpdateUPButton();
	void UpdateDOWNButton();
	void UpdateGameOver();
	void SelectMenu();
	BOOL BackMenu();
	void UpdateClear();
	void SetScore();
	void AddRandomLine();
	void ResetaddLine();
	void Exit();

	void keyhandle(WPARAM wParam);
	void InitTick(UPDATE& up, int mode = 0);
};


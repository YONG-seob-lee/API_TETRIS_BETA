#pragma once
#include "GameMode.h"
#include "framework.h"

class PlayerVsCom :
	public GameMode
{
private:
	Tetris* t1;
	Tetris* t2;
	int state;
	int StageNum;

	int t1Score[6];	// Maximum : 999999
	int t2Score[6];	// 스코어(배열 형)
	int t1score;	// 스코어(int 형)
	int t2score;
	int t1plusScore;
	int t2plusScore;

	// interface //
	int t1x;	// 보드 시작 좌표
	int t1y;
	int t2x;
	int t2y;

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
	PlayerVsCom();
	~PlayerVsCom();

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
	void ResetaddLine();
	void Exit();

	void keyhandle(WPARAM wParam);
	void InitTick(UPDATE& up, int mode = 0);
};


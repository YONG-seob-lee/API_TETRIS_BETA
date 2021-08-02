#pragma once
#include "Tetris.h"
#include "framework.h"

class GameMode
{
private:

public:
	GameMode();
	~GameMode();

	virtual void LoadAll(HANDLES h_Base);
	virtual void InitTetris();
	virtual void InitData();
	virtual void ResourceMainBG(HANDLES h_Base);


	///////////////		Draw	//////////////
	virtual void DrawAll(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawMain(HANDLES h_Base);
	virtual void DrawTetrisBoard(HANDLES h_Base);
	virtual void DrawScore(HANDLES h_Base);
	virtual void DrawMenu(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawMenuBoard(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawBButton(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawGotoPlayButton(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawGotoTitleButton(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawGotoExitButton(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawReady(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawStart(HANDLES h_Base, RECT m_rtClient);
	virtual void DrawGameOver(HANDLES h_Base);


	///////////////		Update	///////////////
	virtual void UpdateAll();
	virtual void UpdateMain();
	virtual void UpdateReady();
	virtual void UpdateStart();
	virtual void UpdateMenu();
	virtual void UpdateUPButton();
	virtual void UpdateDOWNButton();
	virtual void UpdateGameOver(HANDLES h_Base);
	virtual void SelectMenu(HANDLES h_Base);
	virtual BOOL BackMenu();
	virtual void SetScore();
	virtual void ResetaddLine();
	virtual void Exit();

	virtual void InitTick(UPDATE& up, int mode = 0);
	virtual void keyhandle(WPARAM wParam);
	virtual void Load(HANDLES& h, LPCWSTR name, HANDLES h_Base);
	virtual BOOL UpdateCheck(UPDATE& up, DWORD tick);
};

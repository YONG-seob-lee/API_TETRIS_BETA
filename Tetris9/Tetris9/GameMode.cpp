#include "GameMode.h"

GameMode::GameMode()
{
}

GameMode::~GameMode()
{
}


void GameMode::LoadAll(HANDLES h_Base)
{
}

void GameMode::InitTetris()
{
}

void GameMode::InitData()
{
}

void GameMode::ResourceMainBG(HANDLES h_Base)
{
}

void GameMode::DrawAll(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawMain(HANDLES h_Base)
{
}

void GameMode::DrawTetrisBoard(HANDLES h_Base)
{
}

void GameMode::DrawScore(HANDLES h_Base)
{
}

void GameMode::DrawMenu(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawMenuBoard(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawBButton(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawGotoPlayButton(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawGotoTitleButton(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawGotoExitButton(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawReady(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawStart(HANDLES h_Base, RECT m_rtClient)
{
}

void GameMode::DrawGameOver(HANDLES h_Base)
{
}

void GameMode::UpdateAll()
{
}

void GameMode::UpdateMain()
{
}

void GameMode::UpdateReady()
{
}

void GameMode::UpdateStart()
{
}

void GameMode::UpdateMenu()
{
}

void GameMode::UpdateUPButton()
{
}

void GameMode::UpdateDOWNButton()
{
}

void GameMode::UpdateGameOver(HANDLES h_Base)
{
}

void GameMode::SelectMenu(HANDLES h_Base)
{
}

BOOL GameMode::BackMenu()
{
	return 0;
}

void GameMode::SetScore()
{
}

void GameMode::ResetaddLine()
{
}

void GameMode::Exit()
{
}


void GameMode::InitTick(UPDATE & up, int mode)
{
}

void GameMode::keyhandle(WPARAM wParam)
{
}

void GameMode::Load(HANDLES & h, LPCWSTR name, HANDLES h_Base)
{
	h.hbmp = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (h.hbmp == NULL)
	{
		MessageBox(NULL, name, L"비트맵 로딩 에러", MB_OK);
	}
	h.dc = CreateCompatibleDC(h_Base.dc);//베이스에 호환되는 DC 얻기 
	h.hOldbmp = (HBITMAP)SelectObject(h.dc, h.hbmp);//dc 랑 비트맵이랑 연결

	GetObject(h.hbmp, sizeof(BITMAP), &h.bit);
}

BOOL GameMode::UpdateCheck(UPDATE & up, DWORD tick)
{
	up.mEndTick = GetTickCount();
	if (up.mEndTick - up.mStartTick > tick)
	{
		up.mStartTick = up.mEndTick;
		return TRUE;
	}
	return FALSE;
}

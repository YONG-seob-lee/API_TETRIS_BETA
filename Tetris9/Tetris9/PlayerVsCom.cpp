#include "PlayerVsCom.h"
#include "framework.h"

PlayerVsCom::PlayerVsCom() : GameMode()
{
	t1 = new Tetris;
	t2 = new Tetris;

	state = READY;

	t1Score[6] = { 0, };
	t2Score[6] = { 0, };
	t1score = 0;
	t2score = 0;
	t1plusScore = 0;
	t2plusScore = 0;
	u_GotoPlay.bTrigger = TRUE;
	u_GotoTitle.bTrigger = FALSE;
	u_GotoExit.bTrigger = FALSE;
	u_BackMenu.bTrigger = FALSE;
}

PlayerVsCom::~PlayerVsCom()
{
	delete t1;
	delete t2;
}


void PlayerVsCom::LoadAll(HANDLES h_Base)
{
	ResourceMainBG(h_Base);
	Load(h_TetrisBoard, L"image/tetrisboard.bmp", h_Base);
	Load(h_Score, L"image/number.bmp", h_Base);
	Load(h_Ready, L"image/ready.bmp", h_Base);
	Load(h_Start, L"image/start.bmp", h_Base);
	Load(h_MenuBG, L"image/menubackground.bmp", h_Base);
	Load(h_Menu, L"image/menu.bmp", h_Base);
	Load(h_BMenu, L"image/bmenu.bmp", h_Base);
	Load(h_GameOver, L"image/gameover.bmp", h_Base);

	t1->LoadTetris(h_Base);
	t2->LoadTetris(h_Base);
}

void PlayerVsCom::InitTetris()
{
	t1x = 200;
	t1y = 130;
	t2x = 1100;
	t2y = 130;
	t1->InitAll(t1x, t1y);
	t2->InitAll(t2x, t2y);
}

void PlayerVsCom::InitData()
{
	u_Main.Frame = 0;
	u_Main.MaxFrame = MAIN_BG_FRAME;
	InitTetris();
	u_Menu.bTrigger = FALSE;
	InitTick(u_GameOver);
	InitTick(u_Ready);
}

void PlayerVsCom::ResourceMainBG(HANDLES h_Base)
{
	WCHAR name[40];
	for (int i = 0; i < MAIN_BG_FRAME; i++)
	{
		wsprintf(name, L"image/main/main%d.bmp", i);
		h_Main[i].hbmp = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		h_Main[i].dc = CreateCompatibleDC(h_Base.dc);
		GetObject(h_Main[i].hbmp, sizeof(BITMAP), &h_Main[i].bit);

		h_Main[i].hOldbmp = (HBITMAP)SelectObject(h_Main[i].dc, h_Main[i].hbmp);
	}
}

void PlayerVsCom::DrawAll(HANDLES h_Base, RECT m_rtClient)
{
	switch (state)
	{
	case READY:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		t1->DrawPersonal(h_Base);
		t2->DrawPersonal(h_Base);
		DrawReady(h_Base, m_rtClient);
		break;

	case START:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		t1->DrawPersonal(h_Base);
		t2->DrawPersonal(h_Base);
		DrawStart(h_Base, m_rtClient);
		break;

	case PLAY:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		t1->DrawPersonal(h_Base);
		t2->DrawPersonal(h_Base);
		DrawScore(h_Base);
		DrawMenu(h_Base, m_rtClient);
		DrawBButton(h_Base, m_rtClient);
		break;

	case CLEAR:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		t1->DrawPersonal(h_Base);
		t2->DrawPersonal(h_Base);
		DrawScore(h_Base);
		DrawClear(h_Base, m_rtClient);
		break;

	case EXIT:
		DrawGameOver(h_Base);
		break;

	default:
		break;
	}
}

void PlayerVsCom::DrawMain(HANDLES h_Base)
{
	StretchBlt(h_Base.dc, 0, 0, XRES, YRES, h_Main[u_Main.Frame].dc, 0, 0, XRES, YRES, SRCCOPY);
}

void PlayerVsCom::DrawTetrisBoard(HANDLES h_Base)
{
	TransparentBlt(h_Base.dc, t1x-67, t1y + 91, BOARD_X, BOARD_Y, h_TetrisBoard.dc, 0, 0, BOARD_X, BOARD_Y, RGB(255, 0, 255));
	TransparentBlt(h_Base.dc, t2x-67, t2y + 91, BOARD_X, BOARD_Y, h_TetrisBoard.dc, 0, 0, BOARD_X, BOARD_Y, RGB(255, 0, 255));
}

void PlayerVsCom::DrawScore(HANDLES h_Base)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (t1Score[i] == j)
			{
				TransparentBlt(h_Base.dc, t1x + 359 + SCORE_SIZE_X * i, t1y + 543 , SCORE_SIZE_X, SCORE_SIZE_Y, h_Score.dc, j * 60, 0, 60, 70, RGB(255, 0, 255));
				// ???????? i ???? ???????? j ?????? ????????.
			}
			if (t2Score[i] == j)
			{
				TransparentBlt(h_Base.dc, t2x + 359 + SCORE_SIZE_X * i, t2y + 543, SCORE_SIZE_X, SCORE_SIZE_Y, h_Score.dc, j * 60, 0, 60, 70, RGB(255, 0, 255));
				// ???????? i ???? ???????? j ?????? ????????.
			}
		}
	}
}

void PlayerVsCom::DrawMenu(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE) return;

	DrawMenuBoard(h_Base, m_rtClient);
}

void PlayerVsCom::DrawMenuBoard(HANDLES h_Base, RECT m_rtClient)
{
	BitBlt(h_Base.dc, 0, 0, XRES, YRES, h_MenuBG.dc, 0, 0, SRCCOPY);
	TransparentBlt(h_Base.dc, 800, 30, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, 0, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
	TransparentBlt(h_Base.dc, 800, 350, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, MENU_SIZE_Y * 2, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
	TransparentBlt(h_Base.dc, 800, 500, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, MENU_SIZE_Y * 1, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
	TransparentBlt(h_Base.dc, 800, 650, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, MENU_SIZE_Y * 3, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsCom::DrawBButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_GotoPlay.bTrigger == TRUE)	DrawGotoPlayButton(h_Base, m_rtClient);
	if (u_GotoTitle.bTrigger == TRUE)	DrawGotoTitleButton(h_Base, m_rtClient);
	if (u_GotoExit.bTrigger == TRUE)	DrawGotoExitButton(h_Base, m_rtClient);
}

void PlayerVsCom::DrawGotoPlayButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE)	return;
	TransparentBlt(h_Base.dc, 760, 344, BMENU_SIZE_X * 2, BMENU_SIZE_Y * 2, h_BMenu.dc, 0, BMENU_SIZE_Y * 1, BMENU_SIZE_X, BMENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsCom::DrawGotoTitleButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE)	return;
	TransparentBlt(h_Base.dc, 760, 490, BMENU_SIZE_X * 2, BMENU_SIZE_Y * 2, h_BMenu.dc, 0, 0, BMENU_SIZE_X, BMENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsCom::DrawGotoExitButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE)	return;
	TransparentBlt(h_Base.dc, 760, 635, BMENU_SIZE_X * 2, BMENU_SIZE_Y * 2, h_BMenu.dc, 0, BMENU_SIZE_Y * 2, BMENU_SIZE_X, BMENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsCom::DrawReady(HANDLES h_Base, RECT m_rtClient)
{
	int x = m_rtClient.right / 2 - h_Ready.bit.bmWidth / 2;
	int y = m_rtClient.bottom / 2 - h_Ready.bit.bmHeight / 2;
	TransparentBlt(h_Base.dc,//dest
		x,
		y,
		h_Ready.bit.bmWidth,
		h_Ready.bit.bmHeight,
		h_Ready.dc,//src
		0, 0,
		h_Ready.bit.bmWidth,
		h_Ready.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void PlayerVsCom::DrawStart(HANDLES h_Base, RECT m_rtClient)
{
	int x = m_rtClient.right / 2 - h_Start.bit.bmWidth / 2;
	int y = m_rtClient.bottom / 2 - h_Start.bit.bmHeight / 2;
	TransparentBlt(h_Base.dc,//dest
		x - 100,
		y - 50,
		h_Start.bit.bmWidth + 200,
		h_Start.bit.bmHeight + 100,
		h_Start.dc,//src
		0, 0,
		h_Start.bit.bmWidth,
		h_Start.bit.bmHeight,
		RGB(255, 0, 255)
	);
}

void PlayerVsCom::DrawClear(HANDLES h_Base, RECT m_rtClient)
{
	int x = m_rtClient.right / 2 - h_Clear.bit.bmWidth / 2;
	int y = m_rtClient.bottom / 2 - h_Clear.bit.bmHeight / 2;


	TransparentBlt(h_Base.dc,
		x,
		y,
		h_Clear.bit.bmWidth,
		h_Clear.bit.bmHeight,
		h_Clear.dc,
		0, 0,
		h_Clear.bit.bmWidth,
		h_Clear.bit.bmHeight,
		RGB(255, 0, 255));//????
}

void PlayerVsCom::DrawGameOver(HANDLES h_Base)
{
	if (u_GameOver.bTrigger == FALSE)	return;

	BitBlt(h_Base.dc, 0, 0, XRES, YRES, h_GameOver.dc, 0, 0, SRCCOPY);
}

void PlayerVsCom::UpdateAll()
{
	switch (state)
	{
	case READY:
		UpdateReady();
		break;

	case START:
		UpdateStart();
		break;

	case PLAY:
		if (u_Menu.bTrigger == FALSE)
		{
			UpdateMain();
			t1->UpdateAutoDown(1000);
			t1->UpdateMap();
			t2->UpdateAutoDown(400);
			t2->UpdateMap();
			SetScore();
			t1->AddLine(t2plusScore);
			t2->AddLine(t1plusScore);
			ResetaddLine();
			t2->Update_AI();
			BackMenu();
		}
		break;

	case CLEAR:
		UpdateClear();
		break;

	default:
		break;
	}
}

void PlayerVsCom::UpdateMain()
{
	if (UpdateCheck(u_Main, 40) == TRUE)
	{
		u_Main.Frame++;
		if (u_Main.Frame >= u_Main.MaxFrame)
			u_Main.Frame = 1;
	}
}

void PlayerVsCom::UpdateReady()
{
	if (UpdateCheck(u_Ready, 3000) == TRUE)//5???? ???? ???? ??????
	{
		InitTick(u_Start);//?????? ???????? ?????? ?????? ?????? 
		state = START;
	}
}

void PlayerVsCom::UpdateStart()
{
	if (UpdateCheck(u_Start, 2000) == TRUE) //2???? ?????? ????.
	{
		state = PLAY;
	}
}

void PlayerVsCom::UpdateMenu()
{
	if (state == PLAY)
	{
		u_Menu.bTrigger = !u_Menu.bTrigger;
	}
}

void PlayerVsCom::UpdateUPButton()
{
	if (u_GotoPlay.bTrigger == TRUE)
	{
		u_GotoPlay.bTrigger = FALSE;
		u_GotoTitle.bTrigger = FALSE;
		u_GotoExit.bTrigger = TRUE;
	}
	else if (u_GotoTitle.bTrigger == TRUE)
	{
		u_GotoPlay.bTrigger = TRUE;
		u_GotoTitle.bTrigger = FALSE;
		u_GotoExit.bTrigger = FALSE;
	}
	else if (u_GotoExit.bTrigger == TRUE)
	{
		u_GotoPlay.bTrigger = FALSE;
		u_GotoTitle.bTrigger = TRUE;
		u_GotoExit.bTrigger = FALSE;
	}
	else	return;
}

void PlayerVsCom::UpdateDOWNButton()
{
	if (u_GotoPlay.bTrigger == TRUE)
	{
		u_GotoPlay.bTrigger = FALSE;
		u_GotoTitle.bTrigger = TRUE;
		u_GotoExit.bTrigger = FALSE;
	}
	else if (u_GotoTitle.bTrigger == TRUE)
	{
		u_GotoPlay.bTrigger = FALSE;
		u_GotoTitle.bTrigger = FALSE;
		u_GotoExit.bTrigger = TRUE;
	}
	else if (u_GotoExit.bTrigger == TRUE)
	{
		u_GotoPlay.bTrigger = TRUE;
		u_GotoTitle.bTrigger = FALSE;
		u_GotoExit.bTrigger = FALSE;
	}
	else	return;
}

void PlayerVsCom::UpdateGameOver()
{
	u_GameOver.bTrigger = TRUE;

	state = EXIT;		// ?????? ??????
}

void PlayerVsCom::SelectMenu()
{
	if (u_GotoPlay.bTrigger == TRUE)	u_Menu.bTrigger = !u_Menu.bTrigger;
	else if (u_GotoTitle.bTrigger == TRUE) u_BackMenu.bTrigger = !u_BackMenu.bTrigger;		// TetrisManager ?? GameState ?? Game_Title?? ????????.
	else if (u_GotoExit.bTrigger == TRUE)
	{
		UpdateGameOver();
	}
	else return;
}

BOOL PlayerVsCom::BackMenu()
{
	if (u_BackMenu.bTrigger == TRUE)
	{
		state = READY;
		InitTetris();
		InitData();
		return TRUE;
	}
	else	return FALSE;
}

void PlayerVsCom::UpdateClear()
{
	if (UpdateCheck(u_Clear, 3000) == TRUE)
	{
		StageNum++;//?????????????? ?????? 
		state = READY; //???????????? ???? 

		InitTetris();//?????? ??????
		InitData();

		if (StageNum > MAX_STAGE)//?????? ???????? ????????
			StageNum = 0;//???????????? 
	}
}

void PlayerVsCom::SetScore()
{
	t1plusScore += t1->GetScore(); // ?????? ????
	t1->SetScore();		// ???? ???????? ?????? ?????? 0???? ??????
	t2plusScore += t2->GetScore();
	t2->SetScore();

	t1score += t1plusScore * 1000;	// ?????? ???? ????
	t2score += t2plusScore * 1000;

	t1Score[0] = t1score / 100000;
	t1Score[1] = (t1score - t1Score[0] * 100000) / 10000;
	t1Score[2] = (t1score - t1Score[0] * 100000 - t1Score[1] * 10000) / 1000;
	t1Score[3] = (t1score - t1Score[0] * 100000 - t1Score[1] * 10000 - t1Score[2] * 1000) / 100;
	t1Score[4] = (t1score - t1Score[0] * 100000 - t1Score[1] * 10000 - t1Score[2] * 1000 - t1Score[3] * 100) / 10;
	t1Score[5] = (t1score - t1Score[0] * 100000 - t1Score[1] * 10000 - t1Score[2] * 1000 - t1Score[3] * 100 - t1Score[4] * 10) / 1;

	t2Score[0] = t2score / 100000;
	t2Score[1] = (t2score - t2Score[0] * 100000) / 10000;
	t2Score[2] = (t2score - t2Score[0] * 100000 - t2Score[1] * 10000) / 1000;
	t2Score[3] = (t2score - t2Score[0] * 100000 - t2Score[1] * 10000 - t2Score[2] * 1000) / 100;
	t2Score[4] = (t2score - t2Score[0] * 100000 - t2Score[1] * 10000 - t2Score[2] * 1000 - t2Score[3] * 100) / 10;
	t2Score[5] = (t2score - t2Score[0] * 100000 - t2Score[1] * 10000 - t2Score[2] * 1000 - t2Score[3] * 100 - t2Score[4] * 10) / 1;
}

void PlayerVsCom::ResetaddLine()
{
	t1plusScore = 0;
	t2plusScore = 0;
}

void PlayerVsCom::Exit()
{
	DestroyWindow(hWnd);//???? ???????? ????????????
}

void PlayerVsCom::keyhandle(WPARAM wParam)
{
	switch (state)
	{
	case READY:
		break;

	case START:
		break;

	case PLAY:
		switch (wParam)
		{
		case VK_UP:
			t1->Rotate();
			UpdateUPButton();
			break;

		case VK_DOWN:
			t1->goDown();
			UpdateDOWNButton();
			break;

		case VK_LEFT:
			t1->goLeft();
			break;

		case VK_RIGHT:
			t1->goRight();
			break;

		case VK_SPACE:
			t1->goStraightDown();
			break;

		case 'R':
			t1->Rotate();
			break;

		case VK_CONTROL:
			t1->SwapBlock();
			break;

		case 'E':
			t1->holdBlock();
			break;

		case VK_RETURN:
			SelectMenu();
			break;

		case VK_ESCAPE:
			UpdateMenu();
			break;

		default:
			break;
		}
		break;

	case CLEAR:
		UpdateClear();
		break;

	case EXIT:
		switch (wParam)
		{
		case VK_RETURN:
			Exit();
			break;

		case VK_ESCAPE:
			Exit();
			break;

		case VK_SPACE:
			Exit();
			break;

		default:
			break;
		}
		break;

	default:
		break;
	}
}

void PlayerVsCom::InitTick(UPDATE & up, int mode)
{
	switch (mode)
	{
	case 0://all
		up.mStartTick = up.mEndTick = up.aStartTick
			= up.aEndTick = GetTickCount();
		break;
	default:
		break;
	}
}

#include "PlayerVsMany.h"

PlayerVsMany::PlayerVsMany() : GameMode()
{
	for (int i = 0; i < 9; i++)
	{
		tetris[i] = new Tetris;
		tScore[i][6] = { 0, };
		tscore[i] = { 0, };
		TplusScore[i] = 0;
	}
	state = READY;

	HitScore = 0;
	u_GotoPlay.bTrigger = TRUE;
	u_GotoTitle.bTrigger = FALSE;
	u_GotoExit.bTrigger = FALSE;
	u_BackMenu.bTrigger = FALSE;
}

PlayerVsMany::~PlayerVsMany()
{
	for (int i = 0; i < 9; i++)
	{
		delete tetris[i];
	}
}

void PlayerVsMany::LoadAll(HANDLES h_Base)
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

	for (int i = 0; i < 9; i++)
	{
		tetris[i]->LoadTetris(h_Base);
	}
}

void PlayerVsMany::InitTetris()
{
	// UI part
	int part1_x = 60;
	int part1_y = 100;
	int part2_x = 60;
	int part2_y = YRES / 2 + 60;
	int part3_x = XRES / 2 + 330;
	int part3_y = 100;
	int part4_x = XRES / 2 + 330;
	int part4_y = YRES / 2 + 60;
	TX = XRES / 2 - BOARD_X / 2 + 60;
	TY = YRES / 2 - BOARD_Y / 2 - 40;


	// BLOCK part
	for (int i = 1; i < 3; i++)
	{
		tx[i] = part1_x + (i - 1) *(SMALL_BOARD_INTERVAL + SMALL_BOARD_X);
		ty[i] = part1_y;
	}
	for (int i = 3; i < 5; i++)
	{
		tx[i] = part2_x + (i - 3) *(SMALL_BOARD_INTERVAL + SMALL_BOARD_X);
		ty[i] = part2_y ;
	}
	for (int i = 5; i < 7; i++)
	{
		tx[i] = part3_x + (i - 5) *(SMALL_BOARD_INTERVAL + SMALL_BOARD_X);
		ty[i] = part3_y;
	}
	for (int i = 7; i < 9; i++)
	{
		tx[i] = part4_x + (i - 7) *(SMALL_BOARD_INTERVAL + SMALL_BOARD_X);
		ty[i] = part4_y;
	}
	tetris[0]->InitAll(TX, TY);
	for (int i = 1; i < 9; i++)
	{
		tetris[i]->InitAll(tx[i], ty[i]);
	}
}

void PlayerVsMany::InitData()
{
	u_Main.Frame = 0;
	u_Main.MaxFrame = MAIN_BG_FRAME;
	InitTetris();
	u_Menu.bTrigger = FALSE;
	InitTick(u_GameOver);
	InitTick(u_Ready);
}

void PlayerVsMany::ResourceMainBG(HANDLES h_Base)
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

void PlayerVsMany::DrawAll(HANDLES h_Base, RECT m_rtClient)
{
	switch (state)
	{
	case READY:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		tetris[0]->DrawPersonal(h_Base);
		for (int i = 1; i < 9; i++)
		{
			tetris[i]->DrawSmall(h_Base);
		}
		DrawReady(h_Base, m_rtClient);
		break;

	case START:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		tetris[0]->DrawPersonal(h_Base);
		for (int i = 1; i < 9; i++)
		{
			tetris[i]->DrawSmall(h_Base);
		}
		DrawStart(h_Base, m_rtClient);
		break;

	case PLAY:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		tetris[0]->DrawPersonal(h_Base);
		for (int i = 1; i < 9; i++)
		{
			tetris[i]->DrawSmall(h_Base);
		}
		DrawScore(h_Base);
		DrawMenu(h_Base, m_rtClient);
		DrawBButton(h_Base, m_rtClient);
		break;

	case CLEAR:
		DrawMain(h_Base);
		DrawTetrisBoard(h_Base);
		tetris[0]->DrawPersonal(h_Base);
		for (int i = 1; i < 9; i++)
		{
			tetris[i]->DrawSmall(h_Base);
		}
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

void PlayerVsMany::DrawMain(HANDLES h_Base)
{
	StretchBlt(h_Base.dc, 0, 0, XRES, YRES, h_Main[u_Main.Frame].dc, 0, 0, XRES, YRES, SRCCOPY);

}

void PlayerVsMany::DrawTetrisBoard(HANDLES h_Base)
{
	TransparentBlt(h_Base.dc, TX - 66, TY + 90, BOARD_X, BOARD_Y, h_TetrisBoard.dc, 0, 0, BOARD_X, BOARD_Y, RGB(255, 0, 255));
	for (int i = 1; i < 9; i++)
	{
		TransparentBlt(h_Base.dc, tx[i] - 35, ty[i] + 43, SMALL_BOARD_X, SMALL_BOARD_Y, h_TetrisBoard.dc, 0, 0, BOARD_X, BOARD_Y, RGB(255, 0, 255));
	}
}

void PlayerVsMany::DrawScore(HANDLES h_Base)
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (tScore[0][i] == j)
			{
				TransparentBlt(h_Base.dc, TX + 359 + SCORE_SIZE_X * i, TY + 543, SCORE_SIZE_X, SCORE_SIZE_Y, h_Score.dc, j * 60, 0, 60, 70, RGB(255, 0, 255));
				// 스코어의 i 번째 위치에서 j 번호를 출력한다.
			}
		}
	}
	for (int k = 1; k < 9; k++)
	{
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (tScore[k][i] == j)
				{
					TransparentBlt(h_Base.dc, tx[k] + 211 + (SCORE_SIZE_X-20) * i, ty[k] + 270, SCORE_SIZE_X/2, SCORE_SIZE_Y/2, h_Score.dc, j * 60, 0, 60, 70, RGB(255, 0, 255));
					// 스코어의 i 번째 위치에서 j 번호를 출력한다.
				}
			}
		}
	}
}

void PlayerVsMany::DrawMenu(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE) return;

	DrawMenuBoard(h_Base, m_rtClient);
}

void PlayerVsMany::DrawMenuBoard(HANDLES h_Base, RECT m_rtClient)
{
	BitBlt(h_Base.dc, 0, 0, XRES, YRES, h_MenuBG.dc, 0, 0, SRCCOPY);
	TransparentBlt(h_Base.dc, 800, 30, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, 0, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
	TransparentBlt(h_Base.dc, 800, 350, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, MENU_SIZE_Y * 2, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
	TransparentBlt(h_Base.dc, 800, 500, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, MENU_SIZE_Y * 1, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
	TransparentBlt(h_Base.dc, 800, 650, MENU_SIZE_X * 2, MENU_SIZE_Y * 2, h_Menu.dc, 0, MENU_SIZE_Y * 3, MENU_SIZE_X, MENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsMany::DrawBButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_GotoPlay.bTrigger == TRUE)	DrawGotoPlayButton(h_Base, m_rtClient);
	if (u_GotoTitle.bTrigger == TRUE)	DrawGotoTitleButton(h_Base, m_rtClient);
	if (u_GotoExit.bTrigger == TRUE)	DrawGotoExitButton(h_Base, m_rtClient);
}

void PlayerVsMany::DrawGotoPlayButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE)	return;
	TransparentBlt(h_Base.dc, 760, 344, BMENU_SIZE_X * 2, BMENU_SIZE_Y * 2, h_BMenu.dc, 0, BMENU_SIZE_Y * 1, BMENU_SIZE_X, BMENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsMany::DrawGotoTitleButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE)	return;
	TransparentBlt(h_Base.dc, 760, 490, BMENU_SIZE_X * 2, BMENU_SIZE_Y * 2, h_BMenu.dc, 0, 0, BMENU_SIZE_X, BMENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsMany::DrawGotoExitButton(HANDLES h_Base, RECT m_rtClient)
{
	if (u_Menu.bTrigger == FALSE)	return;
	TransparentBlt(h_Base.dc, 760, 635, BMENU_SIZE_X * 2, BMENU_SIZE_Y * 2, h_BMenu.dc, 0, BMENU_SIZE_Y * 2, BMENU_SIZE_X, BMENU_SIZE_Y, RGB(255, 0, 255));
}

void PlayerVsMany::DrawReady(HANDLES h_Base, RECT m_rtClient)
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

void PlayerVsMany::DrawStart(HANDLES h_Base, RECT m_rtClient)
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

void PlayerVsMany::DrawClear(HANDLES h_Base, RECT m_rtClient)
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
		RGB(255, 0, 255));//흰색
}

void PlayerVsMany::DrawGameOver(HANDLES h_Base)
{
	if (u_GameOver.bTrigger == FALSE)	return;

	BitBlt(h_Base.dc, 0, 0, XRES, YRES, h_GameOver.dc, 0, 0, SRCCOPY);
}

void PlayerVsMany::UpdateAll()
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
			tetris[0]->UpdateAutoDown(1000);
			tetris[0]->UpdateMap();

			for (int i = 1; i < 9; i++)
			{
				tetris[i]->UpdateAutoDown(400);
				tetris[i]->UpdateMap();
			}
			this->SetScore();		// 점수를 추가함
			AddRandomLine();		// 랜덤한 객체에 페널티라인을 부여함
			ResetaddLine();

			for (int i = 1; i < 9; i++)
			{
				tetris[i]->Update_AI();
			}
		}
		break;

	case CLEAR:
		UpdateClear();
		break;

	default:
		break;
	}
}

void PlayerVsMany::UpdateMain()
{
	if (UpdateCheck(u_Main, 40) == TRUE)
	{
		u_Main.Frame++;
		if (u_Main.Frame >= u_Main.MaxFrame)
			u_Main.Frame = 1;
	}
}

void PlayerVsMany::UpdateReady()
{
	if (UpdateCheck(u_Ready, 3000) == TRUE)//5초간 레디 문자 띄우기
	{
		InitTick(u_Start);//스타트 업데이트 변수를 여기서 초기화 
		state = START;
	}
}

void PlayerVsMany::UpdateStart()
{
	if (UpdateCheck(u_Start, 2000) == TRUE) //2초간 스타트 찍기.
	{
		state = PLAY;
	}
}

void PlayerVsMany::UpdateMenu()
{
	if (state == PLAY)
	{
		u_Menu.bTrigger = !u_Menu.bTrigger;
	}
}

void PlayerVsMany::UpdateUPButton()
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

void PlayerVsMany::UpdateDOWNButton()
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

void PlayerVsMany::UpdateGameOver()
{
	u_GameOver.bTrigger = TRUE;

	state = EXIT;		// 탈출로 넘어감
}

void PlayerVsMany::SelectMenu()
{
	if (u_GotoPlay.bTrigger == TRUE)	u_Menu.bTrigger = !u_Menu.bTrigger;
	else if (u_GotoTitle.bTrigger == TRUE) u_BackMenu.bTrigger = !u_BackMenu.bTrigger;		// TetrisManager 의 GameState 를 Game_Title로 변경한다.
	else if (u_GotoExit.bTrigger == TRUE)
	{
		UpdateGameOver();
	}
	else return;
}

BOOL PlayerVsMany::BackMenu()
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

void PlayerVsMany::UpdateClear()
{
	if (UpdateCheck(u_Clear, 3000) == TRUE)
	{
		StageNum++;//다음스테이지로 넘어감 
		state = READY; //다음스테이지 레디 

		InitTetris();//데이터 초기화
		InitData();

		if (StageNum > MAX_STAGE)//마지막 스테이지 넘어가면
			StageNum = 0;//첫스테이지로 
	}
}

void PlayerVsMany::SetScore()
{
	for (int i = 0; i < 9; i++)
	{
		TplusScore[i] += tetris[i]->GetScore();
		tetris[i]->SetScore();
	}
	for (int i = 0; i < 9; i++)
	{
		tscore[i] += TplusScore[i] * 1000;
	}
	for (int i = 0; i < 9; i++)
	{
		if (TplusScore[i] > 0)	HitScore = i;	// 추가점수를 받은 곳을 저장하기 위함
	}
	for (int i = 0; i < 9; i++)
	{
		tScore[i][0] = tscore[i] / 100000;
		tScore[i][1] = (tscore[i] - tScore[i][0] * 100000) / 10000;;
		tScore[i][2] = (tscore[i] - tScore[i][0] * 100000 - tScore[i][1] * 10000) / 1000;;
		tScore[i][3] = (tscore[i] - tScore[i][0] * 100000 - tScore[i][1] * 10000 - tScore[i][2] * 1000) / 100;;
		tScore[i][4] = (tscore[i] - tScore[i][0] * 100000 - tScore[i][1] * 10000 - tScore[i][2] * 1000 - tScore[i][3] * 100) / 10;;
		tScore[i][5] = (tscore[i] - tScore[i][0] * 100000 - tScore[i][1] * 10000 - tScore[i][2] * 1000 - tScore[i][3] * 100 - tScore[i][4] * 10) / 1;;
	}
}

void PlayerVsMany::AddRandomLine()
{
	srand((unsigned int)time(NULL));
	int randomselect = rand() % 9;;
	int r = rand() % 9;
	if (randomselect == HitScore)	randomselect = (randomselect + r) % 9;

	tetris[randomselect]->AddLine(TplusScore[HitScore]);	// 랜덤한곳에 페널티라인 추가
}

void PlayerVsMany::ResetaddLine()
{
	HitScore = 0;
	for (int i = 0; i < 9; i++)
	{
		TplusScore[i] = 0;
	}
}

void PlayerVsMany::Exit()
{
	DestroyWindow(hWnd);//종료 메시지를 발동하는함수
}

void PlayerVsMany::keyhandle(WPARAM wParam)
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
			tetris[0]->Rotate();
			UpdateUPButton();
			break;

		case VK_DOWN:
			tetris[0]->goDown();
			UpdateDOWNButton();
			break;

		case VK_LEFT:
			tetris[0]->goLeft();
			break;

		case VK_RIGHT:
			tetris[0]->goRight();
			break;

		case VK_SPACE:
			tetris[0]->goStraightDown();
			break;

		case 'R':
			tetris[0]->Rotate();
			break;

		case VK_CONTROL:
			tetris[0]->SwapBlock();
			break;

		case 'E':
			tetris[0]->holdBlock();
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

void PlayerVsMany::InitTick(UPDATE & up, int mode)
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

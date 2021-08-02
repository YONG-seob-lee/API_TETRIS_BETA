#pragma once
#include "Tetris.h"
#include "framework.h"
#include "PlayerVsCom.h"
#include "PlayerVsPlayer.h"
#include "PlayerVsMany.h"

class TetrisHandler
{
private:
	PlayerVsCom* pvc;
	PlayerVsPlayer* pvp;
	PlayerVsMany* pvm;
	HANDLES MapBlock;
	int mode;				// 게임 모드 선택을 위한 인트자

	HANDLES h_MenuBG;		// 메뉴 배경을 위한 핸들
	HANDLES h_VS;			// vs
	HANDLES h_Player;		// 왼쪽 player
	HANDLES h_Pvc;			// com
	HANDLES h_Pvp;			// vsplayer
	HANDLES h_Pvm;			// many(COM)
	HANDLES h_BPvc;			// big com
	HANDLES h_BPvp;			// big vsplayer
	HANDLES h_BPvm;			// big many
	HANDLES h_Border[BORDER_LINE_FRAME];		// 테두리 라인 핸들 구조체

	UPDATE u_MenuBG;		// 횡스크롤을 위한 업데이트
	UPDATE u_PVC;
	UPDATE u_PVP;
	UPDATE u_PVM;
	UPDATE u_BPvc;			// 큰 이미지를 보여주기 위함
	UPDATE u_BPvp;			// 큰 이미지를 보여주기 위함
	UPDATE u_BPvm;			// 큰 이미지를 보여주기 위함
	UPDATE u_Border;

public:
	TetrisHandler();
	~TetrisHandler();

	void ResourceBorderline(HANDLES h_Base);

	// draw
	void DrawAll(HDC hdc, HANDLES h_Base, RECT m_rtClient);
	void DrawMenuBG(HANDLES h_Base);
	void DrawVS(HANDLES h_Base, RECT m_rtClient);
	void DrawPlayer(HANDLES h_Base, RECT m_rtClient);
	void DrawPVC(HANDLES h_Base, RECT m_rtClient);
	void DrawPVP(HANDLES h_Base, RECT m_rtClient);
	void DrawPVM(HANDLES h_Base, RECT m_rtClient);
	void DrawBPvc(HANDLES h_Base, RECT m_rtClient);
	void DrawBPvp(HANDLES h_Base, RECT m_rtClient);
	void DrawBPvm(HANDLES h_Base, RECT m_rtClient);
	void DrawPlay(HANDLES h_Base, RECT m_rtClient);
	void DrawBorder(HANDLES h_Base, RECT m_rtClient, int x, int y);

	// update
	void UpdateAll(HANDLES h_Base);
	void UpdateMenuBG();
	void UpdateBmode();
	void UpdatePVC();
	void UpdatePVP();
	void UpdatePVM();
	void UpdatePlay();
	void UpdateBorder();
	void Setmode(HANDLES h_Base);
	
	// util
	void InitMode();
	void InitTetrisResource(HANDLES h_Base);
	void Load(HANDLES& h, LPCWSTR name, HANDLES h_Base);
	BOOL UpdateCheck(UPDATE& up, DWORD tick);
	void setModeUP();
	void setModeDown();
	void keyhandle(WPARAM wParam, HANDLES h_Base);
};


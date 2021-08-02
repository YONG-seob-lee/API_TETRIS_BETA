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
	int mode;				// ���� ��� ������ ���� ��Ʈ��

	HANDLES h_MenuBG;		// �޴� ����� ���� �ڵ�
	HANDLES h_VS;			// vs
	HANDLES h_Player;		// ���� player
	HANDLES h_Pvc;			// com
	HANDLES h_Pvp;			// vsplayer
	HANDLES h_Pvm;			// many(COM)
	HANDLES h_BPvc;			// big com
	HANDLES h_BPvp;			// big vsplayer
	HANDLES h_BPvm;			// big many
	HANDLES h_Border[BORDER_LINE_FRAME];		// �׵θ� ���� �ڵ� ����ü

	UPDATE u_MenuBG;		// Ⱦ��ũ���� ���� ������Ʈ
	UPDATE u_PVC;
	UPDATE u_PVP;
	UPDATE u_PVM;
	UPDATE u_BPvc;			// ū �̹����� �����ֱ� ����
	UPDATE u_BPvp;			// ū �̹����� �����ֱ� ����
	UPDATE u_BPvm;			// ū �̹����� �����ֱ� ����
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


#include "tetris.h"

Tetris::Tetris() : posX(5), posY(0)
{
	BLinked = new BlockLinked;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			BlockData[i][j] = 0;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			HBlock[i][j] = 0;
		}
	}
	CheckScoreMap[5][6] = { 0, };
	Scorecount = 0;
	InitMap();
}

Tetris::~Tetris()
{
	delete BLinked;
}


void Tetris::LoadTetris(HANDLES h_Base)
{
	Load(h_Block, L"image/pblock.bmp", h_Base);
	Load(h_TransBlock, L"image/transblock.bmp", h_Base);
	Load(h_Spark, L"image/spark.bmp", h_Base);
}

void Tetris::InitAll(int x, int y)
{
	posX = 5;
	posY = 0;
	u_Block.mEndTick = u_Block.mStartTick = GetTickCount();
	InitPosition(x, y);

	HoldFirst = 0;
	HowManyRotate = 0;
	u_Spark.aEndTick = u_Spark.aStartTick = GetTickCount();
	u_Spark.bTrigger = FALSE;
	u_Spark.Frame = 0;
	u_Spark.MaxFrame = 7;
	u_Spark.size = SPARK_SIZE;
	u_Spark.tricnt = 0;

	u_Block.bTrigger = FALSE;		// 현재 화면에 블록이 없는 상태
	u_MAP.bTrigger = FALSE;			// 맵과 블록간의 충돌이 없는 상태
	u_CheckOver.bTrigger = TRUE;	// 게임 징행중인 상태
	u_AI.bTrigger = FALSE;
	u_TransBlock.bTrigger = FALSE;	// 홀드 사용 안한 상태
	InitBlock();
}

void Tetris::InitPosition(int x, int y)
{
	PositionX = x;
	PositionY = y;
}

void Tetris::InitMap()
{
	for (int h = 0; h < 24; h++)			// 벽 초기화를 위한 2중 for 문
	{													// 블록의 1값(블록) 과 벽의 1의 값이 같을때 그만 움직이도록 할 예정
		for (int w = 0; w < 12; w++)
		{
			if (w == 0 || h == 23 || w == 11)
				MapData[h][w] = 1;
			else
				MapData[h][w] = 0;
		}
	}
}

void Tetris::InitBlock()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			BlockData[i][j] = BLinked->getBpart(i, j);
		}
	}
	u_Block.bTrigger = TRUE;		// 블록 생성
}

void Tetris::InitCM()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			checkMap[i][j] = MapData[posY + i][posX + j];
		}
	}
}

void Tetris::InitAIcm()
{
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			CheckScoreMap[i][j] = MapData[posY + i][posX + j - 1];
		}
	}
}

void Tetris::SetBlock()
{
	u_MAP.bTrigger = FALSE;
	u_Rotate.bTrigger = FALSE;
	u_Penalty.bTrigger = FALSE;
	BLinked->SetLinked();		// 첫번째 블록 지우고 뒤에 새로운 블록 보충하기
	posX = 5;					// 블록 위치 조정
	posY = 0;
	InitCM();					// 맵데이터 초기화
	InitBlock();				// 블록 초기화
	AI_posX = CheckAllLine();
}

void Tetris::DrawPersonal(HANDLES h_Base)
{
	DrawMap(h_Base, 1);
	DrawHoldBlock(h_Base, 1);
	DrawNextBlock(h_Base, 1);
	DrawBlock(h_Base, 1);
	DrawBottomBlock(h_Base, 1);
	//DrawSpark(h_Base);
}

void Tetris::DrawSmall(HANDLES h_Base)
{
	DrawMap(h_Base, 2);
	DrawSmallHoldBlock(h_Base);
	DrawSmallNextBlock(h_Base);
	DrawBlock(h_Base, 2);
	DrawBottomBlock(h_Base, 2);
	//DrawSpark(h_Base);
}

void Tetris::DrawMap(HANDLES h_Base, int size)
{
	for (int h = 3; h < 24; h++)
	{
		for (int w = 0; w < 12; w++)
		{
			//if (MapData[h][w] == MAPBLOCK)		// UI 가 대체하므로 필요없음 ( 개발자 버전 )
			//{
			//	StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
			//		, h_Block.dc, PIECE_BLOCK_X * MAPBLOCK, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			//}
			if (MapData[h][w] == GREEN)
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * GREEN, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (MapData[h][w] == RED)
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)),PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * RED, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (MapData[h][w] == BLUE)
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)),PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * BLUE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (MapData[h][w] == ORANGE)
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * ORANGE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (MapData[h][w] == PURPLE)
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * PURPLE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (MapData[h][w] == YELLOW)
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * YELLOW, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (MapData[h][w] == SKY)
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * SKY, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (MapData[h][w] == PENALTYBLOCK)	
			{
				StretchBlt(h_Base.dc, (PositionX + (w*PIECE_BLOCK_X / size)), (PositionY + (h*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * PENALTYBLOCK, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
		}
	}
}

void Tetris::DrawBlock(HANDLES h_Base, int size)
{
	for (int h = 0; h < 4; h++)
	{
		for (int w = 0; w < 4; w++)
		{
			if (BlockData[h][w] == GREEN)
			{
				StretchBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * GREEN, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (BlockData[h][w] == RED)
			{
				StretchBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * RED, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (BlockData[h][w] == BLUE)
			{
				StretchBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * BLUE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (BlockData[h][w] == ORANGE)
			{
				StretchBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * ORANGE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (BlockData[h][w] == PURPLE)
			{
				StretchBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * PURPLE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (BlockData[h][w] == YELLOW)
			{
				StretchBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * YELLOW, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (BlockData[h][w] == SKY)
			{
				StretchBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_Block.dc, PIECE_BLOCK_X * SKY, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
		}
	}
}

void Tetris::DrawBottomBlock(HANDLES h_Base, int size)
{
	int y = posY;	// 현재 위치값 저장

	while (1)
	{
		posY++;		// 바닥에 닿을때까지 내리기
		if (CheckWall() == FALSE)		// 바닥에 다 닿으면
		{
			posY--;						// 한칸 올리고
			break;
		}
	}
	for (int h = 0; h < 4; h++)
	{
		for (int w = 0; w < 4; w++)
		{
			if (BlockData[h][w] == GREEN)
			{
				TransparentBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_TransBlock.dc, PIECE_BLOCK_X * GREEN, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, RGB(255, 0, 255));
			}
			if (BlockData[h][w] == RED)
			{
				TransparentBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_TransBlock.dc, PIECE_BLOCK_X * RED, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, RGB(255, 0, 255));
			}
			if (BlockData[h][w] == BLUE)
			{
				TransparentBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_TransBlock.dc, PIECE_BLOCK_X * BLUE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, RGB(255, 0, 255));
			}
			if (BlockData[h][w] == ORANGE)
			{
				TransparentBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_TransBlock.dc, PIECE_BLOCK_X * ORANGE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, RGB(255, 0, 255));
			}
			if (BlockData[h][w] == PURPLE)
			{
				TransparentBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_TransBlock.dc, PIECE_BLOCK_X * PURPLE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, RGB(255, 0, 255));
			}
			if (BlockData[h][w] == YELLOW)
			{
				TransparentBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_TransBlock.dc, PIECE_BLOCK_X * YELLOW, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, RGB(255, 0, 255));
			}
			if (BlockData[h][w] == SKY)
			{
				TransparentBlt(h_Base.dc, (PositionX + ((w + posX)*PIECE_BLOCK_X / size)), (PositionY + ((h + posY)*PIECE_BLOCK_Y / size)), PIECE_BLOCK_X / size, PIECE_BLOCK_Y / size
					, h_TransBlock.dc, PIECE_BLOCK_X * SKY, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, RGB(255, 0, 255));
			}
		}
	}

	posY = y;		// 위치값 돌려놓기
}

void Tetris::DrawHoldBlock(HANDLES h_Base, int size)
{
	int Holdx = PositionX - 58;
	int Holdy = PositionY + 120;

	for (int h = 0; h < 4; h++)
	{
		for (int w = 0; w < 4; w++)
		{
			if (HBlock[h][w] == GREEN)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12)) / size, Holdy + (h * (PIECE_BLOCK_Y - 12)) / size, (PIECE_BLOCK_X- 12) / size, (PIECE_BLOCK_Y- 12) / size
					, h_Block.dc, PIECE_BLOCK_X * GREEN, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == RED)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12)) / size, Holdy + (h * (PIECE_BLOCK_Y - 12)) / size, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size
					, h_Block.dc, PIECE_BLOCK_X * RED, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == BLUE)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12)) / size, Holdy + (h * (PIECE_BLOCK_Y - 12)) / size, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size
					, h_Block.dc, PIECE_BLOCK_X * BLUE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == ORANGE)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12)) / size, Holdy + (h * (PIECE_BLOCK_Y - 12)) / size, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size
					, h_Block.dc, PIECE_BLOCK_X * ORANGE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == PURPLE)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12)) / size, Holdy + (h * (PIECE_BLOCK_Y - 12)) / size, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size
					, h_Block.dc, PIECE_BLOCK_X * PURPLE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == YELLOW)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12)) / size, Holdy + (h * (PIECE_BLOCK_Y - 12)) / size, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size
					, h_Block.dc, PIECE_BLOCK_X * YELLOW, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == SKY)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12)) / size, Holdy + (h * (PIECE_BLOCK_Y - 12)) / size, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size
					, h_Block.dc, PIECE_BLOCK_X * SKY, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
		}
	}
}

void Tetris::DrawNextBlock(HANDLES h_Base, int size)
{
	int NextBx;
	int NextBy;
	int Interval = 0;
	for (int count = 1; count < 5; count++)		// count == 1 두번째 블록 , count == 2 세번째 블록
	{
		NextBx = PositionX + 356;
		NextBy = PositionY + 120 + count*3;
		for (int h = 0; h < 4; h++)
		{
			for (int w = 0; w < 4; w++)
			{
				if (BLinked->getNextB(count)->getBpart(h, w) == GREEN)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12)) / size, (NextBy + h * (PIECE_BLOCK_Y - 12) + Interval) / size
						, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size, h_Block.dc, PIECE_BLOCK_X * GREEN, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == RED)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12)) / size, (NextBy + h * (PIECE_BLOCK_Y - 12) + Interval) / size
						, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size, h_Block.dc, PIECE_BLOCK_X * RED, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == BLUE)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12)) / size, (NextBy + h * (PIECE_BLOCK_Y - 12) + Interval) / size
						, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size, h_Block.dc, PIECE_BLOCK_X * BLUE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == ORANGE)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12)) / size, (NextBy + h * (PIECE_BLOCK_Y - 12) + Interval) / size
						, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size, h_Block.dc, PIECE_BLOCK_X * ORANGE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == PURPLE)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12)) / size, (NextBy + h * (PIECE_BLOCK_Y - 12) + Interval) / size
						, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size, h_Block.dc, PIECE_BLOCK_X * PURPLE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == YELLOW)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12)) / size, (NextBy + h * (PIECE_BLOCK_Y - 12) + Interval) / size
						, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size, h_Block.dc, PIECE_BLOCK_X * YELLOW, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == SKY)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12)) / size, (NextBy + h * (PIECE_BLOCK_Y - 12) + Interval) / size
						, (PIECE_BLOCK_X - 12) / size, (PIECE_BLOCK_Y - 12) / size, h_Block.dc, PIECE_BLOCK_X * SKY, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
			}
		}
		Interval += 90;
	}
}

void Tetris::DrawSmallHoldBlock(HANDLES h_Base)
{
	int Holdx = PositionX - 31;
	int Holdy = PositionY + 58;

	for (int h = 0; h < 4; h++)
	{
		for (int w = 0; w < 4; w++)
		{
			if (HBlock[h][w] == GREEN)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12) / 2), Holdy + (h * (PIECE_BLOCK_Y - 12) / 2), (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2
					, h_Block.dc, PIECE_BLOCK_X * GREEN, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == RED)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12) / 2), Holdy + (h * (PIECE_BLOCK_Y - 12) / 2), (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2
					, h_Block.dc, PIECE_BLOCK_X * RED, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == BLUE)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12) / 2), Holdy + (h * (PIECE_BLOCK_Y - 12) / 2), (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2
					, h_Block.dc, PIECE_BLOCK_X * BLUE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == ORANGE)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12) / 2), Holdy + (h * (PIECE_BLOCK_Y - 12) / 2), (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2
					, h_Block.dc, PIECE_BLOCK_X * ORANGE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == PURPLE)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12) / 2), Holdy + (h * (PIECE_BLOCK_Y - 12) / 2), (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2
					, h_Block.dc, PIECE_BLOCK_X * PURPLE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == YELLOW)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12) / 2), Holdy + (h * (PIECE_BLOCK_Y - 12) / 2), (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2
					, h_Block.dc, PIECE_BLOCK_X * YELLOW, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
			if (HBlock[h][w] == SKY)
			{
				StretchBlt(h_Base.dc, Holdx + (w * (PIECE_BLOCK_X - 12) / 2), Holdy + (h * (PIECE_BLOCK_Y - 12) / 2), (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2
					, h_Block.dc, PIECE_BLOCK_X * SKY, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
			}
		}
	}
}

void Tetris::DrawSmallNextBlock(HANDLES h_Base)
{
	int NextBx;
	int NextBy;
	int Interval = 0;
	for (int count = 1; count < 5; count++)		// count == 1 두번째 블록 , count == 2 세번째 블록
	{
		NextBx = PositionX + 180;
		NextBy = PositionY + 53 + count * 3;
		for (int h = 0; h < 4; h++)
		{
			for (int w = 0; w < 4; w++)
			{
				if (BLinked->getNextB(count)->getBpart(h, w) == GREEN)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12) / 2), (NextBy + h * (PIECE_BLOCK_Y - 12) / 2 + Interval / 2)
						, (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2, h_Block.dc, PIECE_BLOCK_X * GREEN, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == RED)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12) / 2), (NextBy + h * (PIECE_BLOCK_Y - 12) / 2 + Interval / 2)
						, (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2, h_Block.dc, PIECE_BLOCK_X * RED, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == BLUE)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12) / 2), (NextBy + h * (PIECE_BLOCK_Y - 12) / 2 + Interval / 2)
						, (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2, h_Block.dc, PIECE_BLOCK_X * BLUE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == ORANGE)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12) / 2), (NextBy + h * (PIECE_BLOCK_Y - 12) / 2 + Interval / 2)
						, (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2, h_Block.dc, PIECE_BLOCK_X * ORANGE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == PURPLE)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12) / 2), (NextBy + h * (PIECE_BLOCK_Y - 12) / 2 + Interval / 2)
						, (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2, h_Block.dc, PIECE_BLOCK_X * PURPLE, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == YELLOW)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12) / 2), (NextBy + h * (PIECE_BLOCK_Y - 12) / 2 + Interval / 2)
						, (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2, h_Block.dc, PIECE_BLOCK_X * YELLOW, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
				if (BLinked->getNextB(count)->getBpart(h, w) == SKY)
				{
					StretchBlt(h_Base.dc, (NextBx + w * (PIECE_BLOCK_X - 12) / 2), (NextBy + h * (PIECE_BLOCK_Y - 12) / 2 + Interval / 2)
						, (PIECE_BLOCK_X - 12) / 2, (PIECE_BLOCK_Y - 12) / 2, h_Block.dc, PIECE_BLOCK_X * SKY, 0, PIECE_BLOCK_X, PIECE_BLOCK_Y, SRCCOPY);
				}
			}
		}
		Interval += 90;
	}
}

void Tetris::DrawSpark(HANDLES h_Base)
{
	if (u_Spark.bTrigger == FALSE)	return;

	TransparentBlt(h_Base.dc, PositionX + posX*PIECE_BLOCK_X + 50 , PositionY + posY * PIECE_BLOCK_Y + 600,
		30, 50, h_Spark.dc, u_Spark.Frame * u_Spark.size, 0, u_Spark.size, h_Spark.bit.bmHeight, RGB(255, 0, 255));
}

bool Tetris::CheckWall()		// 오로지 블록과 맵간의 충돌 확인 함수
{
	InitCM();								// 블럭이 움직이려는 방향에서 1:1매칭되는 4x4 맵데이터 초기화

	for (int i = 0; i < 4; i++)							// 블럭데이터값 가져오기
	{
		for (int j = 0; j < 4; j++)
		{
			if (checkMap[i][j] != 0 && BlockData[i][j] != 0)	// 체크맵에서 i,j 좌표의 값이 0이 아닌데 블록데이터 i,j 좌표의 값도 0이 아니면 
			{
				return FALSE;		// 4x4 맵데이터와 4x4 블럭데이터간의 충돌이 생기면	FALSE 반환
			}
		}
	}
	return TRUE;
}

void Tetris::CheckLine()
{
	int line = 0;		// 지우려고하는 줄의 최초 시작라인을 확인하기 위한 변수
	int count = 0;
	for (int h = 1; h < 23; h++)
	{
		if (MapData[h][1] != BLANK && MapData[h][2] != BLANK && MapData[h][3] != BLANK && MapData[h][4] != BLANK && MapData[h][5] != BLANK
			&& MapData[h][6] != BLANK && MapData[h][7] != BLANK && MapData[h][8] != BLANK && MapData[h][9] != BLANK && MapData[h][10] != BLANK)
		{
			line = h;			// 모든 데이터가 1인 최초라인 저장
			count++;			// h라인을 기준으로 몇줄까지 모든 데이터가 1인지 확인해서 저장
			for (int i = 1; i < 11; i++)		// h라인 데이터 0으로 초기화
				MapData[h][i] = 0;
		}
	}

	for (int h = line - count; h > 0; h--)	// h 라인에서 카운트된 라인부터 맨 위 라인까지
	{
		for (int w = 1; w < 11; w++)
		{
			MapData[h + count][w] = MapData[h][w];	// h라인에서 카운트된 라인부터 줄줄이 복사하기
		}
	}
	Scorecount += count;
}

void Tetris::CheckDeadLine()
{
	for (int w = 1; w < 11; w++)
	{
		if (MapData[4][w] != BLANK)			// 맵 UI 맨 윗줄에 '0' 이외의 데이터가 저장되면
			u_CheckOver.bTrigger = FALSE;			// 실패 트리거 발동
	}
}

void Tetris::AddLine(int sidescore)
{
	if (sidescore == 0)	return;		// 점수가 추가 안돼면 라인추가 없음

	for (int h = 5; h < 23; h++)
	{
		for (int w = 1; w < 11; w++)
		{
			MapData[h - sidescore][w] = MapData[h][w];		// 추가된 점수의 개수만큼 맵 데이터를 위로 올림
		}

	}
	srand((unsigned int)time(NULL));
	int blank = rand() % 10 + 1;		// 아래서부터 추가될 라인에서 빈 공간의 위치를 확인하기 위한 랜덤 변수

	for (int h = 22; h > 22 - sidescore; h--)
	{
		for (int w = 1; w < 11; w++)
		{
			if (w == blank)	MapData[h][w] = BLANK;	// 랜덤으로 생겨난 곳의 공간은 Blank 로 둠
			else	MapData[h][w] = PENALTYBLOCK;
		}
	}
}

int Tetris::GetScore()
{
	return Scorecount;
}

void Tetris::SetScore()
{
	Scorecount = 0;
}


void Tetris::goLeft()
{
	posX--;
	if (CheckWall() == FALSE)	posX++;			// 블럭이 움직였더니만 맵과 충돌이 생겨버려서 이전위치로 회귀
}

void Tetris::goRight()
{
	posX++;
	if (CheckWall() == FALSE)	posX--;
}

void Tetris::goDown()
{
	posY++;
	if (CheckWall() == FALSE)
	{
		u_MAP.bTrigger = TRUE;	// 맵데이터의 바닥과 블록이 충돌했으므로 트리거 발동
		u_Block.bTrigger = FALSE;	// 충돌하자마자 블럭 정지
		posY--;
	}
}

void Tetris::goStraightDown()
{
	while (1)
	{
		posY++;
		if (CheckWall() == FALSE)
		{
			u_MAP.bTrigger = TRUE;	// 맵데이터의 바닥과 블록이 충돌했으므로 트리거 발동
			u_Block.bTrigger = FALSE;	// 충돌하자마자 블럭 정지
			posY--;
			break;
		}
	}
}

void Tetris::Rotate()
{
	BLinked->RotateH();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			BlockData[i][j] = BLinked->getBpart(i, j);
		}
	}
	if (CheckWall() == FALSE)		// 벽체크를 했는데 걸리면 (기본값은 왼쪽에 걸리는것으로)
	{
		posX++;						// 오른쪽으로 한칸 이동
		if (CheckWall() == FALSE)	// 오른쪽으로 이동했는데도 불구하고 걸렸다면
		{
			posX++;					// 오른쪽으로 한칸 더 이동
			if (CheckWall() == FALSE)
			{
				posX -= 3;			// 왼쪽으로 한칸 이동(오른쪽에 걸리는것으로 판단)

				if (CheckWall() == FALSE)	// 왼쪽으로 이동했는데도 불구하고 걸렸다면
				{
					posX--;			// 왼쪽으로 한칸 더 이동
				}
			}
		}
	}
}
 
void Tetris::CRotate()
{
	BLinked->CRotateH();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			BlockData[i][j] = BLinked->getBpart(i, j);
		}
	}
	if (CheckWall() == FALSE)		// 벽체크를 했는데 걸리면 (기본값은 왼쪽에 걸리는것으로)
	{
		posX++;						// 오른쪽으로 한칸 이동
		if (CheckWall() == FALSE)	// 오른쪽으로 이동했는데도 불구하고 걸렸다면
		{
			posX++;					// 오른쪽으로 한칸 더 이동
			if (CheckWall() == FALSE)
			{
				posX -= 3;			// 왼쪽으로 한칸 이동(오른쪽에 걸리는것으로 판단)

				if (CheckWall() == FALSE)	// 왼쪽으로 이동했는데도 불구하고 걸렸다면
				{
					posX--;			// 왼쪽으로 한칸 더 이동
				}
			}
		}
	}
}

void Tetris::SwapBlock()
{
	BLinked->Swap();
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			BlockData[i][j] = BLinked->getBpart(i, j);
		}
	}
}

void Tetris::holdBlock()
{
	if (u_TransBlock.bTrigger == TRUE)	return;	// 홀드 한번 사용했으면 바로 퇴출

	if (HoldFirst < 1)			// 홀드를 한번도 사용하지 않았으면
	{
		HoldBlock = BLinked->getB();		// 현재 블록 가져오기
		for (int i = 0; i < 4; i++)			// 교체 후 데이터 저장
		{
			for (int j = 0; j < 4; j++)
			{
				HBlock[i][j] = HoldBlock->getBpart(i, j);	// 홀드된 블록 데이터 저장하기
			}
		}
		BLinked->SetLinked();				// 리스트 한칸 밀어내기
		for (int i = 0; i < 4; i++)			// 교체 후 데이터 저장
		{
			for (int j = 0; j < 4; j++)
			{
				BlockData[i][j] = BLinked->getBpart(i, j);	// 새로 생성된 블록 데이터 저장하기
			}
		}
		u_TransBlock.bTrigger = TRUE;		// 홀드 사용함
		HoldFirst++;
		return;
	}
	else
	{

		Block* temp;
		temp = HoldBlock;
		HoldBlock = BLinked->getB();		// 홀드블럭에 현재 진행중인 블럭 넣음

		for (int i = 0; i < 4; i++)			// 교체 전 홀드 데이터에 데이터 저장
		{
			for (int j = 0; j < 4; j++)
			{
				HBlock[i][j] = BlockData[i][j];	// hblock 는 더미에 들어갔으니 건드려도 됨
			}
		}
		BLinked->SetData(temp);		// 현재 진행중인 블럭에 홀드블럭을 넣음

		for (int i = 0; i < 4; i++)			// 교체 전 홀드블럭 데이터를 블럭 데이터에 저장
		{
			for (int j = 0; j < 4; j++)
			{
				BlockData[i][j] = BLinked->getBpart(i, j);	// 교체한 블록 데이터 저장하기
			}
		}
		posX = 5;
		posY = 0;
		u_TransBlock.bTrigger = TRUE;		// 홀드 사용함
	}
}

void Tetris::UpdateAutoDown(int DownSpeed)
{
	u_Block.mEndTick = GetTickCount();
	if (u_Block.mEndTick - u_Block.mStartTick > DownSpeed)	// 일정 시간 지나면
	{
		goDown();				// 한칸 떨구기
		u_Block.mStartTick = u_Block.mEndTick;	// startTick 초기화
	}
}

void Tetris::UpdateMap()
  {
	if (u_MAP.bTrigger == FALSE)	return;	// 벽간의 충돌이 안생기면 반환

	if (u_MAP.bTrigger == TRUE)	// 벽간의 충돌이 생기고 더이상 갈 곳이 없어져부리면
	{
		InitCM();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (checkMap[i][j] == MAPBLOCK)		MapData[posY + i][posX + j] = MAPBLOCK;	// 맵체크 배열에 벽부분 저장
				if (BlockData[i][j] == GREEN)		MapData[posY + i][posX + j] = GREEN;
				if (BlockData[i][j] == RED)			MapData[posY + i][posX + j] = RED;
				if (BlockData[i][j] == BLUE)		MapData[posY + i][posX + j] = BLUE;
				if (BlockData[i][j] == ORANGE)		MapData[posY + i][posX + j] = ORANGE;
				if (BlockData[i][j] == PURPLE)		MapData[posY + i][posX + j] = PURPLE;
				if (BlockData[i][j] == YELLOW)		MapData[posY + i][posX + j] = YELLOW;
				if (BlockData[i][j] == SKY)			MapData[posY + i][posX + j] = SKY;	// 맵 데이터와 블럭에서 1~8인 부분이 있으면 맵 데이터에 저장
			}
		}

		///////////////////	delete part	////////////////
		delete TopMember;		// 클래스 객체 지우기

		u_Penalty.bTrigger = FALSE;
		u_TransBlock.bTrigger = FALSE;	// 블록 다시 만들어지는 순간 홀드 한번 사용 가능
		CheckDeadLine();
 		CheckLine();				// 줄이 완성되었는지 확인할 수 있는 함수
		SetBlock();
	}
}

void Tetris::UpdateSpark()
{
	if (u_Spark.bTrigger == FALSE)	return;

	u_Spark.aEndTick = GetTickCount();
	if (u_Spark.aEndTick - u_Spark.aStartTick > 100)
	{
		u_Spark.aStartTick = u_Spark.aEndTick;
		u_Spark.Frame++;//
		if (u_Spark.Frame > u_Spark.MaxFrame)// 애니가 끝나면
		{
			u_Spark.bTrigger = FALSE;//폭발이 끝났음을 알림 
		}
	}
}

BOOL Tetris::UpdateGetOver()
{
	if (u_CheckOver.bTrigger == TRUE)	return TRUE;	// 게임이 아직 진행상태라면 반환

	return FALSE;		// 게임이 종료되었으면 FALSE 반환
}

void Tetris::Update_AI()
{
	while (1)
	{
		if (u_Rotate.bTrigger == TRUE)	break;
		
		for (int i = 0; i < HowManyRotate; i++)
		{
			Rotate();
		}

		u_Rotate.bTrigger = TRUE;		// for문 다 끝나고 탈출
	}
	
	if (UpdateCheck(u_AI, 500) == TRUE)		// 0.5 초 마다
	{
		if (posX > AI_posX)	// 현재 위치가 인덱스위치보다 클때
		{
			goLeft();
		}
		if (posX < AI_posX)
		{
			goRight();
		}
	}
	// x축 위치 조정 후 바닥으로 떨어뜨리는 part
	if (posX == AI_posX)		// 움직인 이후 인덱스위치랑 일치하게되면
	{
		if (UpdateCheck(u_AI, 300) == TRUE)	// 0.3 초 마다
		{
			goDown();		// 떨어 뜨리기
		}
	}
}

int Tetris::GetScoreToCheckBottom()
{
	int score = 0;		// 점수를 체크하려는 함수
	int y = posY;
	int count[5] = { 0, };	// 맨 아래부터 쌓여진 개수를 확인하기 위한 배열
	int ScoreMap[5][4];
	int down = 0;

	while (1)				// 전부 떨어뜨리기 위한 while 문
	{
		posY++;
		if (CheckWall() == FALSE)
		{
			for (int w = 0; w < 4; w++)
			{
				checkMap[4][w] = ScoreMap[5][w];
			}
			posY --;
			break;
		}
	}
	InitCM();				// 스코어 맵 체크
	for (int h = 0; h < 4; h++)
	{
		for (int w = 0; w < 4; w++)
		{
			if (BlockData[h][w] != BLANK)	ScoreMap[h][w] = BlockData[h][w];		// 빈 공간이 아닐 때 만 저장
			if (checkMap[h][w] != BLANK)	ScoreMap[h][w] = checkMap[h][w];		// 빈 공간이 아닐 때 만 저장
		}
	}

	for (int h = 4; h >= 0; h--)		// 열 라인에 채워진 블록 개수 확인
	{
		for (int w = 0; w < 4; w++)
		{
			if (ScoreMap[h][w] != BLANK)	count[h]++;		// 각 층마다 개수 확인
		}
		if (count[5] == 6)
		{
			score += 6;	// 맨 아랫줄부터 꽉 찼는지 확인하기 위한 for문 (아래에 더 많이 있을수록 추가점수)
			if (count[4] == 6)
			{
				score += 6;
				if (count[3] == 6)
				{
					score += 12;
					if (count[2] == 6)
					{
						score += 18;
						if (count[1] == 6)
							score += 30;
					}
				}
			}
		}
	}

	for (int i = 4; i >= 0; i--)	// 맨 아래층부터
	{
		if (count[i] - down <= 1)		// 만약 윗층보다 채워진 개수가 한 칸적으면
		{
			if (i == 4)	score += 6;	// 보정
			score -= 6;				// 엄청난 페널티
			down = count[i];		// 윗층 카운트 저장
		}
		if (count[i] - down <= 2)		// 만약 윗층보다 채워진 개수가 두 칸적으면
		{
			if (i == 4)	score += 6;	// 보정
			score -= 6;				// 엄청난 페널티
			down = count[i];		// 윗층 카운트 저장
		}
		if (count[i] - down <= 3)		// 만약 윗층보다 채워진 개수가 세 칸적으면
		{
			if (i == 4)	score += 6;	// 보정
			score -= 6;				// 엄청난 페널티
			down = count[i];		// 윗층 카운트 저장
		}
	}
	score += 100 * posY;		// 낮은 위치일수록 추가점수 부여
	posY = y;
	return score;
}

int Tetris::CheckAllLine()
{
	int x = posX;
	int Score[48] = { 0, };
	int MaxScore = 0;
	int Randomindex = 0;
	int index = 0;
	int num = 0;
	TopMember = new ScoreLinked;
		
	for (int r = 0; r < 4; r++)			// 총 네번 회전 (원 위치로 돌아올 때 까지) r = 3 일 때 제자리
	{
		for (int w = -2; w < 10; w++)
		{
			posX = w;								//	맨 왼쪽좌표부터 체크 시작
			Score[num] = GetScoreToCheckBottom();		//	스코어 체크
			num++;
			// Score[0, 1, 2, 3] : 왼쪽첫번째에서 회전해서 나온 결과, Score[4, 5, 6, 7] : 왼쪽부터 두번째에서 회전해서 나온 결과
		}
		Rotate();							// 한번 회전
	}

	for (int i = 0; i < 48; i++)
	{
		if (MaxScore < Score[i])		MaxScore = Score[i]; // 가장 높은 점수를 확인
	}

	for (int i = 0; i <= 48; i++)
	{
		if (Score[i] == MaxScore)			// 가장 높은 점수를 받은 개체들은 
		{
			TopMember->InsertAddTail(i);	// 차례대로 넣기
			index++;
		}
	}
	//if (MaxScore <= 3)		// 점수 미달인 경우가 발생하면
	//{
	//	holdBlock();		// 홀드 블럭 바꿔 끼기
	//	CheckAllLine();		// 재귀함수
	//}

	srand((unsigned int)time(NULL));
	Randomindex = rand() % index;		// 인덱스 크기 내에서 랜덤한 상수 생성
	HowManyRotate = (TopMember->GetScoreData(Randomindex)) / 12;			// 1 이면 한번 돌린거, 0 이면 원위치
	posX = x;		// x 축 원위치
	return (TopMember->GetScoreData(Randomindex)) % 12 - 2;		// 40 가지의 경우의 수 에서 한가지를 뽑아서 보내기
}

	

void Tetris::Load(HANDLES & h, LPCWSTR name, HANDLES h_Base)
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

BOOL Tetris::UpdateCheck(UPDATE & up, DWORD tick)
{
	up.mEndTick = GetTickCount();
	if (up.mEndTick - up.mStartTick > tick)
	{
		up.mStartTick = up.mEndTick;
		return TRUE;
	}
	return FALSE;
}

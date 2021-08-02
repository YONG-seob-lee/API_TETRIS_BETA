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

	u_Block.bTrigger = FALSE;		// ���� ȭ�鿡 ����� ���� ����
	u_MAP.bTrigger = FALSE;			// �ʰ� ��ϰ��� �浹�� ���� ����
	u_CheckOver.bTrigger = TRUE;	// ���� ¡������ ����
	u_AI.bTrigger = FALSE;
	u_TransBlock.bTrigger = FALSE;	// Ȧ�� ��� ���� ����
	InitBlock();
}

void Tetris::InitPosition(int x, int y)
{
	PositionX = x;
	PositionY = y;
}

void Tetris::InitMap()
{
	for (int h = 0; h < 24; h++)			// �� �ʱ�ȭ�� ���� 2�� for ��
	{													// ����� 1��(���) �� ���� 1�� ���� ������ �׸� �����̵��� �� ����
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
	u_Block.bTrigger = TRUE;		// ��� ����
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
	BLinked->SetLinked();		// ù��° ��� ����� �ڿ� ���ο� ��� �����ϱ�
	posX = 5;					// ��� ��ġ ����
	posY = 0;
	InitCM();					// �ʵ����� �ʱ�ȭ
	InitBlock();				// ��� �ʱ�ȭ
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
			//if (MapData[h][w] == MAPBLOCK)		// UI �� ��ü�ϹǷ� �ʿ���� ( ������ ���� )
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
	int y = posY;	// ���� ��ġ�� ����

	while (1)
	{
		posY++;		// �ٴڿ� ���������� ������
		if (CheckWall() == FALSE)		// �ٴڿ� �� ������
		{
			posY--;						// ��ĭ �ø���
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

	posY = y;		// ��ġ�� ��������
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
	for (int count = 1; count < 5; count++)		// count == 1 �ι�° ��� , count == 2 ����° ���
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
	for (int count = 1; count < 5; count++)		// count == 1 �ι�° ��� , count == 2 ����° ���
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

bool Tetris::CheckWall()		// ������ ��ϰ� �ʰ��� �浹 Ȯ�� �Լ�
{
	InitCM();								// ���� �����̷��� ���⿡�� 1:1��Ī�Ǵ� 4x4 �ʵ����� �ʱ�ȭ

	for (int i = 0; i < 4; i++)							// �������Ͱ� ��������
	{
		for (int j = 0; j < 4; j++)
		{
			if (checkMap[i][j] != 0 && BlockData[i][j] != 0)	// üũ�ʿ��� i,j ��ǥ�� ���� 0�� �ƴѵ� ��ϵ����� i,j ��ǥ�� ���� 0�� �ƴϸ� 
			{
				return FALSE;		// 4x4 �ʵ����Ϳ� 4x4 �������Ͱ��� �浹�� �����	FALSE ��ȯ
			}
		}
	}
	return TRUE;
}

void Tetris::CheckLine()
{
	int line = 0;		// ��������ϴ� ���� ���� ���۶����� Ȯ���ϱ� ���� ����
	int count = 0;
	for (int h = 1; h < 23; h++)
	{
		if (MapData[h][1] != BLANK && MapData[h][2] != BLANK && MapData[h][3] != BLANK && MapData[h][4] != BLANK && MapData[h][5] != BLANK
			&& MapData[h][6] != BLANK && MapData[h][7] != BLANK && MapData[h][8] != BLANK && MapData[h][9] != BLANK && MapData[h][10] != BLANK)
		{
			line = h;			// ��� �����Ͱ� 1�� ���ʶ��� ����
			count++;			// h������ �������� ���ٱ��� ��� �����Ͱ� 1���� Ȯ���ؼ� ����
			for (int i = 1; i < 11; i++)		// h���� ������ 0���� �ʱ�ȭ
				MapData[h][i] = 0;
		}
	}

	for (int h = line - count; h > 0; h--)	// h ���ο��� ī��Ʈ�� ���κ��� �� �� ���α���
	{
		for (int w = 1; w < 11; w++)
		{
			MapData[h + count][w] = MapData[h][w];	// h���ο��� ī��Ʈ�� ���κ��� ������ �����ϱ�
		}
	}
	Scorecount += count;
}

void Tetris::CheckDeadLine()
{
	for (int w = 1; w < 11; w++)
	{
		if (MapData[4][w] != BLANK)			// �� UI �� ���ٿ� '0' �̿��� �����Ͱ� ����Ǹ�
			u_CheckOver.bTrigger = FALSE;			// ���� Ʈ���� �ߵ�
	}
}

void Tetris::AddLine(int sidescore)
{
	if (sidescore == 0)	return;		// ������ �߰� �ȵŸ� �����߰� ����

	for (int h = 5; h < 23; h++)
	{
		for (int w = 1; w < 11; w++)
		{
			MapData[h - sidescore][w] = MapData[h][w];		// �߰��� ������ ������ŭ �� �����͸� ���� �ø�
		}

	}
	srand((unsigned int)time(NULL));
	int blank = rand() % 10 + 1;		// �Ʒ������� �߰��� ���ο��� �� ������ ��ġ�� Ȯ���ϱ� ���� ���� ����

	for (int h = 22; h > 22 - sidescore; h--)
	{
		for (int w = 1; w < 11; w++)
		{
			if (w == blank)	MapData[h][w] = BLANK;	// �������� ���ܳ� ���� ������ Blank �� ��
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
	if (CheckWall() == FALSE)	posX++;			// ���� ���������ϸ� �ʰ� �浹�� ���ܹ����� ������ġ�� ȸ��
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
		u_MAP.bTrigger = TRUE;	// �ʵ������� �ٴڰ� ����� �浹�����Ƿ� Ʈ���� �ߵ�
		u_Block.bTrigger = FALSE;	// �浹���ڸ��� �� ����
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
			u_MAP.bTrigger = TRUE;	// �ʵ������� �ٴڰ� ����� �浹�����Ƿ� Ʈ���� �ߵ�
			u_Block.bTrigger = FALSE;	// �浹���ڸ��� �� ����
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
	if (CheckWall() == FALSE)		// ��üũ�� �ߴµ� �ɸ��� (�⺻���� ���ʿ� �ɸ��°�����)
	{
		posX++;						// ���������� ��ĭ �̵�
		if (CheckWall() == FALSE)	// ���������� �̵��ߴµ��� �ұ��ϰ� �ɷȴٸ�
		{
			posX++;					// ���������� ��ĭ �� �̵�
			if (CheckWall() == FALSE)
			{
				posX -= 3;			// �������� ��ĭ �̵�(�����ʿ� �ɸ��°����� �Ǵ�)

				if (CheckWall() == FALSE)	// �������� �̵��ߴµ��� �ұ��ϰ� �ɷȴٸ�
				{
					posX--;			// �������� ��ĭ �� �̵�
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
	if (CheckWall() == FALSE)		// ��üũ�� �ߴµ� �ɸ��� (�⺻���� ���ʿ� �ɸ��°�����)
	{
		posX++;						// ���������� ��ĭ �̵�
		if (CheckWall() == FALSE)	// ���������� �̵��ߴµ��� �ұ��ϰ� �ɷȴٸ�
		{
			posX++;					// ���������� ��ĭ �� �̵�
			if (CheckWall() == FALSE)
			{
				posX -= 3;			// �������� ��ĭ �̵�(�����ʿ� �ɸ��°����� �Ǵ�)

				if (CheckWall() == FALSE)	// �������� �̵��ߴµ��� �ұ��ϰ� �ɷȴٸ�
				{
					posX--;			// �������� ��ĭ �� �̵�
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
	if (u_TransBlock.bTrigger == TRUE)	return;	// Ȧ�� �ѹ� ��������� �ٷ� ����

	if (HoldFirst < 1)			// Ȧ�带 �ѹ��� ������� �ʾ�����
	{
		HoldBlock = BLinked->getB();		// ���� ��� ��������
		for (int i = 0; i < 4; i++)			// ��ü �� ������ ����
		{
			for (int j = 0; j < 4; j++)
			{
				HBlock[i][j] = HoldBlock->getBpart(i, j);	// Ȧ��� ��� ������ �����ϱ�
			}
		}
		BLinked->SetLinked();				// ����Ʈ ��ĭ �о��
		for (int i = 0; i < 4; i++)			// ��ü �� ������ ����
		{
			for (int j = 0; j < 4; j++)
			{
				BlockData[i][j] = BLinked->getBpart(i, j);	// ���� ������ ��� ������ �����ϱ�
			}
		}
		u_TransBlock.bTrigger = TRUE;		// Ȧ�� �����
		HoldFirst++;
		return;
	}
	else
	{

		Block* temp;
		temp = HoldBlock;
		HoldBlock = BLinked->getB();		// Ȧ����� ���� �������� �� ����

		for (int i = 0; i < 4; i++)			// ��ü �� Ȧ�� �����Ϳ� ������ ����
		{
			for (int j = 0; j < 4; j++)
			{
				HBlock[i][j] = BlockData[i][j];	// hblock �� ���̿� ������ �ǵ���� ��
			}
		}
		BLinked->SetData(temp);		// ���� �������� ���� Ȧ����� ����

		for (int i = 0; i < 4; i++)			// ��ü �� Ȧ��� �����͸� �� �����Ϳ� ����
		{
			for (int j = 0; j < 4; j++)
			{
				BlockData[i][j] = BLinked->getBpart(i, j);	// ��ü�� ��� ������ �����ϱ�
			}
		}
		posX = 5;
		posY = 0;
		u_TransBlock.bTrigger = TRUE;		// Ȧ�� �����
	}
}

void Tetris::UpdateAutoDown(int DownSpeed)
{
	u_Block.mEndTick = GetTickCount();
	if (u_Block.mEndTick - u_Block.mStartTick > DownSpeed)	// ���� �ð� ������
	{
		goDown();				// ��ĭ ������
		u_Block.mStartTick = u_Block.mEndTick;	// startTick �ʱ�ȭ
	}
}

void Tetris::UpdateMap()
  {
	if (u_MAP.bTrigger == FALSE)	return;	// ������ �浹�� �Ȼ���� ��ȯ

	if (u_MAP.bTrigger == TRUE)	// ������ �浹�� ����� ���̻� �� ���� �������θ���
	{
		InitCM();
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				if (checkMap[i][j] == MAPBLOCK)		MapData[posY + i][posX + j] = MAPBLOCK;	// ��üũ �迭�� ���κ� ����
				if (BlockData[i][j] == GREEN)		MapData[posY + i][posX + j] = GREEN;
				if (BlockData[i][j] == RED)			MapData[posY + i][posX + j] = RED;
				if (BlockData[i][j] == BLUE)		MapData[posY + i][posX + j] = BLUE;
				if (BlockData[i][j] == ORANGE)		MapData[posY + i][posX + j] = ORANGE;
				if (BlockData[i][j] == PURPLE)		MapData[posY + i][posX + j] = PURPLE;
				if (BlockData[i][j] == YELLOW)		MapData[posY + i][posX + j] = YELLOW;
				if (BlockData[i][j] == SKY)			MapData[posY + i][posX + j] = SKY;	// �� �����Ϳ� ������ 1~8�� �κ��� ������ �� �����Ϳ� ����
			}
		}

		///////////////////	delete part	////////////////
		delete TopMember;		// Ŭ���� ��ü �����

		u_Penalty.bTrigger = FALSE;
		u_TransBlock.bTrigger = FALSE;	// ��� �ٽ� ��������� ���� Ȧ�� �ѹ� ��� ����
		CheckDeadLine();
 		CheckLine();				// ���� �ϼ��Ǿ����� Ȯ���� �� �ִ� �Լ�
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
		if (u_Spark.Frame > u_Spark.MaxFrame)// �ִϰ� ������
		{
			u_Spark.bTrigger = FALSE;//������ �������� �˸� 
		}
	}
}

BOOL Tetris::UpdateGetOver()
{
	if (u_CheckOver.bTrigger == TRUE)	return TRUE;	// ������ ���� ������¶�� ��ȯ

	return FALSE;		// ������ ����Ǿ����� FALSE ��ȯ
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

		u_Rotate.bTrigger = TRUE;		// for�� �� ������ Ż��
	}
	
	if (UpdateCheck(u_AI, 500) == TRUE)		// 0.5 �� ����
	{
		if (posX > AI_posX)	// ���� ��ġ�� �ε�����ġ���� Ŭ��
		{
			goLeft();
		}
		if (posX < AI_posX)
		{
			goRight();
		}
	}
	// x�� ��ġ ���� �� �ٴ����� ����߸��� part
	if (posX == AI_posX)		// ������ ���� �ε�����ġ�� ��ġ�ϰԵǸ�
	{
		if (UpdateCheck(u_AI, 300) == TRUE)	// 0.3 �� ����
		{
			goDown();		// ���� �߸���
		}
	}
}

int Tetris::GetScoreToCheckBottom()
{
	int score = 0;		// ������ üũ�Ϸ��� �Լ�
	int y = posY;
	int count[5] = { 0, };	// �� �Ʒ����� �׿��� ������ Ȯ���ϱ� ���� �迭
	int ScoreMap[5][4];
	int down = 0;

	while (1)				// ���� ����߸��� ���� while ��
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
	InitCM();				// ���ھ� �� üũ
	for (int h = 0; h < 4; h++)
	{
		for (int w = 0; w < 4; w++)
		{
			if (BlockData[h][w] != BLANK)	ScoreMap[h][w] = BlockData[h][w];		// �� ������ �ƴ� �� �� ����
			if (checkMap[h][w] != BLANK)	ScoreMap[h][w] = checkMap[h][w];		// �� ������ �ƴ� �� �� ����
		}
	}

	for (int h = 4; h >= 0; h--)		// �� ���ο� ä���� ��� ���� Ȯ��
	{
		for (int w = 0; w < 4; w++)
		{
			if (ScoreMap[h][w] != BLANK)	count[h]++;		// �� ������ ���� Ȯ��
		}
		if (count[5] == 6)
		{
			score += 6;	// �� �Ʒ��ٺ��� �� á���� Ȯ���ϱ� ���� for�� (�Ʒ��� �� ���� �������� �߰�����)
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

	for (int i = 4; i >= 0; i--)	// �� �Ʒ�������
	{
		if (count[i] - down <= 1)		// ���� �������� ä���� ������ �� ĭ������
		{
			if (i == 4)	score += 6;	// ����
			score -= 6;				// ��û�� ���Ƽ
			down = count[i];		// ���� ī��Ʈ ����
		}
		if (count[i] - down <= 2)		// ���� �������� ä���� ������ �� ĭ������
		{
			if (i == 4)	score += 6;	// ����
			score -= 6;				// ��û�� ���Ƽ
			down = count[i];		// ���� ī��Ʈ ����
		}
		if (count[i] - down <= 3)		// ���� �������� ä���� ������ �� ĭ������
		{
			if (i == 4)	score += 6;	// ����
			score -= 6;				// ��û�� ���Ƽ
			down = count[i];		// ���� ī��Ʈ ����
		}
	}
	score += 100 * posY;		// ���� ��ġ�ϼ��� �߰����� �ο�
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
		
	for (int r = 0; r < 4; r++)			// �� �׹� ȸ�� (�� ��ġ�� ���ƿ� �� ����) r = 3 �� �� ���ڸ�
	{
		for (int w = -2; w < 10; w++)
		{
			posX = w;								//	�� ������ǥ���� üũ ����
			Score[num] = GetScoreToCheckBottom();		//	���ھ� üũ
			num++;
			// Score[0, 1, 2, 3] : ����ù��°���� ȸ���ؼ� ���� ���, Score[4, 5, 6, 7] : ���ʺ��� �ι�°���� ȸ���ؼ� ���� ���
		}
		Rotate();							// �ѹ� ȸ��
	}

	for (int i = 0; i < 48; i++)
	{
		if (MaxScore < Score[i])		MaxScore = Score[i]; // ���� ���� ������ Ȯ��
	}

	for (int i = 0; i <= 48; i++)
	{
		if (Score[i] == MaxScore)			// ���� ���� ������ ���� ��ü���� 
		{
			TopMember->InsertAddTail(i);	// ���ʴ�� �ֱ�
			index++;
		}
	}
	//if (MaxScore <= 3)		// ���� �̴��� ��찡 �߻��ϸ�
	//{
	//	holdBlock();		// Ȧ�� �� �ٲ� ����
	//	CheckAllLine();		// ����Լ�
	//}

	srand((unsigned int)time(NULL));
	Randomindex = rand() % index;		// �ε��� ũ�� ������ ������ ��� ����
	HowManyRotate = (TopMember->GetScoreData(Randomindex)) / 12;			// 1 �̸� �ѹ� ������, 0 �̸� ����ġ
	posX = x;		// x �� ����ġ
	return (TopMember->GetScoreData(Randomindex)) % 12 - 2;		// 40 ������ ����� �� ���� �Ѱ����� �̾Ƽ� ������
}

	

void Tetris::Load(HANDLES & h, LPCWSTR name, HANDLES h_Base)
{
	h.hbmp = (HBITMAP)LoadImage(NULL, name, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (h.hbmp == NULL)
	{
		MessageBox(NULL, name, L"��Ʈ�� �ε� ����", MB_OK);
	} 
	h.dc = CreateCompatibleDC(h_Base.dc);//���̽��� ȣȯ�Ǵ� DC ��� 
	h.hOldbmp = (HBITMAP)SelectObject(h.dc, h.hbmp);//dc �� ��Ʈ���̶� ����

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

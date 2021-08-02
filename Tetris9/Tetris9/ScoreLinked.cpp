#include "ScoreLinked.h"

TopScore * ScoreLinked::CreateNode(int data)
{
	TopScore* ptr = new TopScore;		// ptr 포인터 생성후 힙변수 저장
	ptr->data = data;		// 새로만든 포인터의 데이터 초기화
	ptr->pNode = NULL;		// 새로만든 포인터의 pNode 초기화
	return ptr;
}

ScoreLinked::ScoreLinked()
{
	ptr = NULL;
	ptrHead = NULL;
}

ScoreLinked::~ScoreLinked()
{
	DelNode();
}

void ScoreLinked::InsertAddHead(int data)
{
	TopScore* ptr;
	if (ptrHead == NULL)	ptrHead = CreateNode(data);	// 헤드ptr 이 가리키는것이 없으면(NULL) 새로 노드 생성

	else
	{
		ptr = CreateNode(data);		// ptr 생성하고
		ptr->pNode = ptrHead;		// ptr의 노드가 ptrHead 포인터를 가리키게 하고
		ptrHead = ptr;				// ptrHead 포인터를 ptr이 가리키는곳으로 옮김
	}
}

void ScoreLinked::InsertAddTail(int data)
{
	TopScore* ptr;

	if (ptrHead == NULL)	ptrHead = CreateNode(data);	// 헤드ptr 이 가리키는것이 없으면(NULL) 새로 노드 생성

	else
	{
		ptr = ptrHead;						// 일단 ptr포인터를 Head 포인터로 옮겨두고
		while (ptr->pNode != NULL)			// ptr이 pNode를 따라 노드의 마지막값이 null 이 되는곳까지 이동
		{
			ptr = ptr->pNode;				// 다 이동한 뒤에 마지막 노드를 ptr포인터에 연결
		}
		ptr->pNode = CreateNode(data);				// 연결 후에 ptr 노드 생성
	}
}

int ScoreLinked::GetScoreData(int index)
{
	TopScore* ptr;
	ptr = ptrHead;
	for (int i = 0; i < index; i++)	 // 인덱스 만큼
	{
		ptr = ptr->pNode;		// 이동
	}
	return ptr->data;		// 인덱스만큼 이동한 곳의 데이터 전달
}

void ScoreLinked::DelNode()
{
	TopScore* ptr;
	for (ptr = ptrHead; ptrHead != NULL; ptr = ptrHead)
	{
		ptrHead = ptrHead->pNode;
		if (ptr != NULL)
			delete ptr;
	}
}

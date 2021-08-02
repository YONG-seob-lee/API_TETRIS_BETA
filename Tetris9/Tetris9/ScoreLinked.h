#pragma once
#include "framework.h"

class ScoreLinked
{
private:
	TopScore* CreateNode(int data);
	TopScore* ptr;
	TopScore* ptrHead;

public:
	ScoreLinked();
	~ScoreLinked();
	void InsertAddHead(int data);
	void InsertAddTail(int data);
	int GetScoreData(int index);
	void DelNode();

};


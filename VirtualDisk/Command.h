#pragma once
#include "DiskPath.h"
#include "FileDisk.h"
#include "TruePathDeal.h"

//����������Ļ���
class Command
{
public:
	Command();
	virtual bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) = 0;
	void getArgAndPath(list<wstring>&,list<wstring>&,list<wstring>&); //�����������
	virtual ~Command();
};

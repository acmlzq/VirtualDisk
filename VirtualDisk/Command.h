#pragma once
#include "DiskPath.h"
#include "FileDisk.h"
#include "TruePathDeal.h"

//所有命令类的基类
class Command
{
public:
	Command();
	virtual bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) = 0;
	void getArgAndPath(list<wstring>&,list<wstring>&,list<wstring>&); //将输入的命令
	virtual ~Command();
};

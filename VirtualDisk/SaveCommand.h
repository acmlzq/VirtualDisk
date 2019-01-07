#pragma once
#include <cmath>
#include <cstdio>
#include <direct.h>
#include "Command.h"
#include "TruePathDeal.h"
#include "StringDeal.h"

//保存虚拟磁盘信息到真实磁盘的命令   格式：save dstPath
class SaveCommand :	public Command
{
public:
	SaveCommand();
	bool executeCmd(DiskPath* diskpath,list<wstring>&, FileDisk*) override;
	~SaveCommand();
};


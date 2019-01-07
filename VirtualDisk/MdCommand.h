#pragma once
#include "Command.h"
//创建文件夹 格式：md path
class MdCommand :public Command
{
public:
	MdCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~MdCommand();
};


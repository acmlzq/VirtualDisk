#pragma once
#include "Command.h"

//重命名文件/文件夹 格式：ren src dst
class RenCommand :	public Command
{
public:
	RenCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~RenCommand();
};


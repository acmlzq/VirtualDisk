#pragma once
#include "Command.h"

//移动文件/文件夹  move /y src dst
class MoveCommand : public Command
{
public:
	MoveCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~MoveCommand();
};


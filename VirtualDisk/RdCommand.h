#pragma once
#include "Command.h"

//ɾ��·�������ʽ��rd [/s] path [path1] ...
class RdCommand :public Command
{
public:
	RdCommand();
	bool executeCmd(DiskPath*, list<wstring>&,FileDisk*) override;
	~RdCommand();
};


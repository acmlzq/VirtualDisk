#pragma once
#include "Command.h"

//�л�·�� ��ʽ cd [path]
class CdCommand :public Command
{
public:
	CdCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~CdCommand();
};


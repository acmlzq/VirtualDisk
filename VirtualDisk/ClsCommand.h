#pragma once
#include "Command.h"

//�����Ļ
class ClsCommand :public Command
{
public:
	ClsCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~ClsCommand();
};


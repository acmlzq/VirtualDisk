#pragma once
#include "Command.h"

//ɾ���ļ�����ʽ del [/s] path [path1] ...
class DelCommand :public Command
{
public:
	DelCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~DelCommand();
};


#pragma once
#include "Command.h"
//�����ļ��� ��ʽ��md path
class MdCommand :public Command
{
public:
	MdCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~MdCommand();
};


#pragma once
#include "Command.h"

//�������ļ�/�ļ��� ��ʽ��ren src dst
class RenCommand :	public Command
{
public:
	RenCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~RenCommand();
};


#pragma once
#include "Command.h"

//�ƶ��ļ�/�ļ���  move /y src dst
class MoveCommand : public Command
{
public:
	MoveCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~MoveCommand();
};


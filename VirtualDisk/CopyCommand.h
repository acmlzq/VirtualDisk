#pragma once
#include <Windows.h>
#include <io.h>
#include <algorithm>
#include "Command.h"
//�ļ��ĸ��ƣ���ʽcopy path1 path2
class CopyCommand : public Command
{
public:
	CopyCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~CopyCommand();
};


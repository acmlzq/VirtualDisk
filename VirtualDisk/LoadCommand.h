#pragma once
#include "Command.h"
//���ر����������������̽ṹ��Ϣ  ��ʽ��load dstPath
class LoadCommand :	public Command
{
public:
	LoadCommand();
	bool executeCmd(DiskPath* diskpath, list<wstring>&, FileDisk*) override;
	~LoadCommand();
};


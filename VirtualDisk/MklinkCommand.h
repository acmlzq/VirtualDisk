#pragma once
#include "Command.h"
//������������ ��ʽ��mklink srcPath symbolLinkPath
class MklinkCommand :public Command
{
public:
	MklinkCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~MklinkCommand();
};


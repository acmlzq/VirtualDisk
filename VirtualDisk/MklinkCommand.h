#pragma once
#include "Command.h"
//创建符号链接 格式：mklink srcPath symbolLinkPath
class MklinkCommand :public Command
{
public:
	MklinkCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~MklinkCommand();
};


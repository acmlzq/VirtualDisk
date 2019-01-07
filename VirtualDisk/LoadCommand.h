#pragma once
#include "Command.h"
//加载本地所保存的虚拟磁盘结构信息  格式：load dstPath
class LoadCommand :	public Command
{
public:
	LoadCommand();
	bool executeCmd(DiskPath* diskpath, list<wstring>&, FileDisk*) override;
	~LoadCommand();
};


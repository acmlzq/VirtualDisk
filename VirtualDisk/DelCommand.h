#pragma once
#include "Command.h"

//删除文件，格式 del [/s] path [path1] ...
class DelCommand :public Command
{
public:
	DelCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~DelCommand();
};


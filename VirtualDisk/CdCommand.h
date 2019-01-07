#pragma once
#include "Command.h"

//ÇÐ»»Â·¾¶ ¸ñÊ½ cd [path]
class CdCommand :public Command
{
public:
	CdCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~CdCommand();
};


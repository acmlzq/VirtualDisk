#pragma once
#include "Command.h"

//É¾³ıÂ·¾¶ÃüÁî£¬¸ñÊ½£ºrd [/s] path [path1] ...
class RdCommand :public Command
{
public:
	RdCommand();
	bool executeCmd(DiskPath*, list<wstring>&,FileDisk*) override;
	~RdCommand();
};


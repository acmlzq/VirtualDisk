#pragma once
#include "Command.h"

//Çå¿ÕÆÁÄ»
class ClsCommand :public Command
{
public:
	ClsCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~ClsCommand();
};


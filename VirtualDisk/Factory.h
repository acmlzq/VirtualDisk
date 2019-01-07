#pragma once
#include <string>
#include "DiskPath.h"
#include "MdCommand.h"
#include "RenCommand.h"
#include "MklinkCommand.h"
#include "RdCommand.h"
#include "DirCommand.h"
#include "DelCommand.h"
#include "CdCommand.h"
#include "ClsCommand.h"
#include "CopyCommand.h"
#include "SaveCommand.h"
#include "LoadCommand.h"
#include "MoveCommand.h"

//工厂，用于根据命令名称，返回相应的命令的实例
class Factory
{
public:
	Command* CreateCommand(const wstring &);
	Factory();
	~Factory();
};


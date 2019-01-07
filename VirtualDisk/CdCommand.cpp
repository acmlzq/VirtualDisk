#include "CdCommand.h"



CdCommand::CdCommand()
{
}

bool CdCommand::executeCmd(DiskPath*diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list,arg_list,path_list);
	if (arg_list.size()!=0||path_list.size() == 0)
	{
		wcout << diskpath->getCurPath() << endl;
		return true;
	}
	if (path_list.size() != 1)
	{
		wcout << L"系统找不到指定的路径。" << endl;
		return false;
	}
	int size, type;
	diskpath->containNode(path_list.front(), size, type);
	if (type == TYPE_NOT_HAVE)
	{
		wcout << L"系统找不到指定的路径。" << endl;
		return false;
	}
	else if (type == TYPE_FILE)
	{
		wcout << L"目录名称无效。" << endl;
		return false;
	}
	if (diskpath->updateCurPath(path_list.front()) == false)
	{
		wcout << L"系统找不到指定的文件。" << endl;
		return false;
	}
	return true;
}

CdCommand::~CdCommand()
{
}

/*
* 具体流程
0. 先判断格式
1. 判断源文件是否存在
2. 切换到目的路径
*/

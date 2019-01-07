#include "LoadCommand.h"



LoadCommand::LoadCommand()
{
}

bool LoadCommand::executeCmd(DiskPath* diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list,arg_list,path_list);
	for (auto it : arg_list)
	{
		wcout << L"命令语法不正确" << endl;
		return false;
	}
	if (path_list.size() != 1)
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	if (!TruePathDeal::isTrueDiskPath(path_list.front()))
	{
		wcout << L"请输入真实磁盘路径" << endl;
		return false;
	}
	ifstream infile(path_list.front());
	if (!infile)
	{
		wcout << L"系统找不到指定的文件。" << endl;
		return false;
	}
	else
	{
		TruePathDeal::readDataFromTrueDisk(path_list.front(), filedisk);
	}
	diskpath->ClearPathList();
	diskpath->setListHead(filedisk->getRootNode(), filedisk->getRootNode()->getName());
	return true;
}

LoadCommand::~LoadCommand()
{
}

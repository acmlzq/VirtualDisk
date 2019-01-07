#include "SaveCommand.h"



SaveCommand::SaveCommand()
{
}

bool SaveCommand::executeCmd(DiskPath* diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	if (arg_list.size()!=0||path_list.size() != 1) 
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	if (!TruePathDeal::isTrueDiskPath(path_list.front()))
	{
		wcout << L"请输入真实磁盘路径" << endl;
		return false;
	}
	DWORD ftype = GetFileAttributes(path_list.front().c_str());
	if (_access(StringDeal::dealWideCharToMultiByte(path_list.front().c_str(),path_list.size()).c_str(), 0)!=-1&&ftype&FILE_ATTRIBUTE_DIRECTORY) //如果是目录
	{
		wcout << L"系统找不到指定的文件。" << endl;
		return false;
	}
	pair<wstring, wstring> folder_file = DiskPathDeal::getPathFolderAndFile(path_list.front());
	ftype = GetFileAttributesA(StringDeal::dealWideCharToMultiByte(folder_file.first.c_str(),folder_file.first.size()).c_str());
	if (_access(StringDeal::dealWideCharToMultiByte(folder_file.first.c_str(),folder_file.first.size()).c_str(), 0) != -1 && !(ftype&FILE_ATTRIBUTE_DIRECTORY)) //如果不是目录
	{
		wcout << L"系统找不到指定的文件。" << endl;
		return false;
	}
	DeleteFile(path_list.front().c_str()); //将这个文件删了（清空文件）
	queue<Component*>comp_q;
	comp_q.push(diskpath->getRootNode());
	while (!comp_q.empty())
	{
		Component *comp = comp_q.front();
		comp_q.pop();
		if (comp->getType() == TYPE_FOLDER) //文件夹
		{
			//Folder*folder = (Folder*)comp;
			Folder* folder = static_cast<Folder*>(comp);
			for (auto it : folder->Cmap)
			{
				comp_q.push(it.second);
			}
		}
		TruePathDeal::writeDataToTrueDisk(path_list.front(), comp);
	}
	return true;
}

SaveCommand::~SaveCommand()
{

}

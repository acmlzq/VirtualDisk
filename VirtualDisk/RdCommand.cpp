#include "RdCommand.h"

RdCommand::RdCommand()
{
}

bool RdCommand::executeCmd(DiskPath* diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	for (auto it : arg_list)
	{
		if (it == L"/s")
		{
			have_arg = true;
		}
		else
		{
			wcout << L"命令语法不正确。" << endl;
			return false;
		}
	}
	if (path_list.size() == 0)
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	int size, type;
	for (auto it : path_list)
	{
		Component *comp = diskpath->containNode(it, size, type);
		if (diskpath->nodeIsInPathList(comp)) //路径状态中的节点不能删除
		{
			wcout << L"另一个程序正在使用此文件，进程无法访问。" << endl;
			continue;
		}
		if (type == 0)
		{
			wcout << L"系统找不到指定的文件。" << endl;
		}
		else if (type == TYPE_FILE || type == TYPE_LINK_FILE)
		{
			wcout << L"目录名称无效。" << endl;
		}
		else if (type == TYPE_FOLDER || type == TYPE_LINK_FOLDER)
		{
			if (have_arg == true) //有参数
			{
				wstring answer;
				do
				{
					wcout << it << L",是否确认<Y/N>? ";
					wcin >> answer;
				} while (!(answer.size() == TYPE_FOLDER && ((char)toupper(answer[0]) == 'Y' || (char)toupper(answer[0]) == 'N')));
				if ((char)toupper(answer[0]) == 'Y')
				{
					Folder* father_node = static_cast<Folder*>(comp->getFatherNode());
					father_node->Cmap.erase(comp->getName());
					delete comp;  //删除的同时，释放内存
				}

			}
			else if (have_arg == false)//无参数
			{
				if (type == TYPE_LINK_FOLDER) //符号链接文件夹
				{
					Folder * father_node = static_cast<Folder*>(comp->getFatherNode());
					father_node->Cmap.erase(comp->getName());
					delete comp;  //删除的同时，释放内存
				}
				else //文件夹
				{
					Folder * folder = (Folder*)comp;
					if (folder->Cmap.size() == TYPE_FILE)
					{
						Folder* father_node = static_cast<Folder*>(comp->getFatherNode());
						father_node->Cmap.erase(comp->getName());
						delete comp;  //删除的同时，释放内存
					}
					else
					{
						wcout << L"目录不是空的。" << endl;
					}
				}
			}
		}
	}

	return true;
}

RdCommand::~RdCommand()
{
}

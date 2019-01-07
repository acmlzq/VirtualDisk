#include "DelCommand.h"



DelCommand::DelCommand()
{
}

bool DelCommand::executeCmd(DiskPath*diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	if (path_list.size() == 0)
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
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
	if (have_arg&&path_list.size() == 0)
	{
		path_list.push_back(L".");
	}
	for (auto path_list_it : path_list)
	{
		bool have_wildcard = false;
		list<Component*> comp_list;
		diskpath->getPathNode(path_list_it, have_wildcard, comp_list);

		if (comp_list.size() == 0)
		{
			wcout << L"系统找不到指定的文件。" << endl;
			continue;
		}

		if (have_wildcard == true) //如果含有通配符
		{
			for (auto it : comp_list)
			{
				if (it->getType() == TYPE_FILE || it->getType() == TYPE_LINK_FILE) //删除
				{
					Folder* folder = static_cast<Folder*>(it->getFatherNode());
					folder->Remove(it->getName());
				}
				else if (it->getType() == TYPE_LINK_FOLDER || it->getType() == TYPE_FOLDER) //文件夹不做任何操作
				{

				}
			}
		}
		else //如果不含通配符
		{
			if (comp_list.front()->getType() == TYPE_LINK_FOLDER) //链接文件夹
			{
				Component * link_node = DiskPathDeal::getLinkNode(comp_list.front());
				comp_list.pop_front();
				comp_list.push_front(link_node);
			}
			if (comp_list.front()->getType() == TYPE_FOLDER && !have_arg) //文件夹
			{
				Folder* folder = static_cast<Folder*>(comp_list.front());
				comp_list.pop_front();
				for (auto it : folder->Cmap)
				{
					comp_list.push_back(it.second);
				}
			}
			for (auto comp_list_it : comp_list)
			{
				if (comp_list_it->getType() == TYPE_FILE || comp_list_it->getType() == TYPE_LINK_FILE) //删除
				{
					Folder* folder = static_cast<Folder*>(comp_list_it->getFatherNode());
					folder->Remove(comp_list_it->getName());
				}
				else if (comp_list_it->getType() == TYPE_LINK_FOLDER || comp_list_it->getType() == TYPE_FOLDER) //文件夹，需要判断有无参数
				{
					Folder *folder = static_cast<Folder*>(comp_list_it);
					if (comp_list_it->getType() == TYPE_LINK_FOLDER) //如果是符号链接文件
					{
						Component * p = DiskPathDeal::getLinkNode(comp_list_it);
						if (p == nullptr)
						{
							wcout << L"系统找不到指定的文件。" << endl;
							continue;
						}
						else
						{
							folder = (Folder*)p;
						}
					}
					if (have_arg) //如果有参数 这个dir写完用dir的
					{
						list<pair<wstring, Component*>> list_pair;
						DiskPathDeal::getNodeByRecur(folder, list_pair);
						if (list_pair.size() != 0)
						{
							for (auto it : list_pair)
							{
								Component * comp_node = it.second;
								wstring str;
								do
								{
									wcout << DiskPathDeal::getAbsPath(comp_node) << L"\\" << comp_node->getName() << L"\\*,是否确认<Y/N>? ";
									wcin >> str;
								} while (!(str.size() == 1 && (tolower(str[0]) == 'y' || tolower(str[0]) == 'n')));
								if (tolower(str[0]) == 'y')
								{
									Folder* folder = static_cast<Folder*>(comp_node);
									for (map<wstring, Component*>::iterator file = folder->Cmap.begin(); file != folder->Cmap.end();)
									{
										if (file->second->getType() == TYPE_FILE || file->second->getType() == TYPE_LINK_FILE)
										{
											folder->Remove((file++)->second->getName());
										}
										else
											file++;
									}
								}
							}
						}
					}
					else //没有参数
					{
						for (auto it : folder->Cmap)
						{
							if (it.second->getType() == TYPE_FILE || it.second->getType() == TYPE_LINK_FILE)
							{
								folder->Remove(it.second->getName());
							}
						}
					}

				}
			}
		}
	}
	return true;
}

DelCommand::~DelCommand()
{
}

/*
* 具体流程
0. 先判断格式
1. 判断源文件是否存在
2. 遍历每个文件路径
		如果含有通配符，对于每个文件||文件夹：
			文件：删除
			文件夹：不管
		如果不含通配符
			文件：删除
			文件夹：
				无参数：删除目录下文件
				有参数：递归删除所有目录下文件

*/

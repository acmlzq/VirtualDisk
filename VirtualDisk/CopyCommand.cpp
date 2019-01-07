#include "CopyCommand.h"



CopyCommand::CopyCommand()
{
}

bool CopyCommand::executeCmd(DiskPath*diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list,arg_list,path_list);
	bool have_arg = false;
	if (path_list.size() == 0)
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	for (auto it : arg_list)
	{
		if (it == L"/y")
		{
			have_arg = true;
		}
		else
		{
			wcout << L"命令语法不正确。" << endl;
			return false;
		}
	}
	if (path_list.size() == 1) path_list.push_back(L"");
	if (path_list.size() != 2)
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	bool src_true_disk = TruePathDeal::isTrueDiskPath(path_list.front()); //真实磁盘
	bool dst_true_disk = TruePathDeal::isTrueDiskPath(path_list.back()); //真实磁盘

	list<pair<pair<wstring, int>, wchar_t*> >file_list;//存放<名字，大小>，首地址
	wstring front_wildcards_str = L""; //存放前面的路径的通配符前的字符串
	bool front_wildcard; //标记前面的路径是否含有匹配符
	if (src_true_disk) //如果是真实磁盘
	{
		TruePathDeal::getTrueDiskFile(file_list,path_list.front());  //获取文件目录
		if (file_list.size() == 0)
		{
			wcout << L"系统找不到指定的文件。" << endl;
			return false;
		}
	}
	else //虚拟磁盘
	{
		bool wildcard;
		list<Component*>all_node;
		diskpath->getPathNode(path_list.front(), wildcard,all_node);
		front_wildcard = wildcard;
		if (wildcard == true) //有通配符
		{
			pair<wstring, wstring> folder_file = DiskPathDeal::getPathFolderAndFile(path_list.front());
			front_wildcards_str = StringDeal::getWildcardStr(folder_file.second);
		}
		else //没有通配符
		{
			all_node.clear();
			int type, size;
			Component *comp_node = diskpath->containNode(path_list.front(), size, type);
			if (type == TYPE_LINK_FOLDER)
				comp_node = DiskPathDeal::getLinkNode(comp_node), type = TYPE_FOLDER;
			else if (type == TYPE_LINK_FILE)
				comp_node = DiskPathDeal::getLinkNode(comp_node), type = TYPE_FILE;
			if (type == TYPE_FOLDER) //文件夹
			{
				Folder * folder = static_cast<Folder*>(comp_node);
				for (auto it : folder->Cmap)
				{
					if (it.second->getType() == TYPE_FILE || it.second->getType() == TYPE_LINK_FILE)
						all_node.push_back(it.second);
				}
			}
			else if (type == TYPE_FILE) //文件
			{
				all_node.push_back(comp_node);
			}
			else if (type == TYPE_NOT_HAVE)
			{
				wcout << L"系统找不到指定的文件。" << endl;
				return false;
			}
		}
		for (auto it : all_node)
		{
			File* file = static_cast<File*>(it);
			int wchar_size = 0;
			wchar_t *content_p = file->getContent(wchar_size);
			wchar_t *tmp_content_p = new wchar_t[wchar_size];
			wmemcpy(tmp_content_p, content_p, wchar_size);
			file_list.push_back(make_pair(make_pair(file->getName(), wchar_size), tmp_content_p));
		}
	}


	if (dst_true_disk) //真实磁盘，没有这个需求
	{

	}
	else //虚拟磁盘
	{
		pair<wstring, wstring>folder_file = DiskPathDeal::getPathFolderAndFile(path_list.back());
		int size, type;
		Component* comp_node = diskpath->containNode(folder_file.first, size, type);
		if (type == TYPE_NOT_HAVE)
		{
			wcout << L"系统找不到指定的路径。" << endl;
			return false;
		}
		bool have_wildcards = StringDeal::isHaveWildcard(folder_file.second);
		if (have_wildcards) //含有通配符，先改名字
		{
			if (type != TYPE_FOLDER) //不是文件夹
			{
				wcout << L"系统找不到指定的路径。" << endl;
				return false;
			}
			wstring back_wildcard_str = StringDeal::getWildcardStr(folder_file.second);
			bool all_flag = have_arg;
			for (auto& it : file_list)
			{
				StringDeal::strReplace(it.first.first, front_wildcards_str, back_wildcard_str);
				Folder* folder = static_cast<Folder*>(comp_node);
				wstring str;
				if (folder->getNodeByName(it.first.first) && !all_flag)    //存放<名字，大小>，首地址
				{
					do
					{

						wcout << L"覆盖 " << it.first.first << L" 吗？ (Yes/No/All): ";
						wcin >> str;
						transform(str.begin(), str.end(), str.begin(), ::tolower);
					} while (!(str == L"yes" || str == L"no" || str == L"all"));
					if (str == L"no") continue;
					if (str == L"all") all_flag = true;
				}
				if (str == L"yes" || str == L"all")
				{
					File* file = static_cast<File*>(folder->getNodeByName(it.first.first));
					file->setContent(it.second, it.first.second);
				}
				else
				{
					folder->Add(it.first.first, TYPE_FILE);
					File* file = static_cast<File*>(folder->getNodeByName(it.first.first));
					file->setContent(it.second, it.first.second);
				}
			}

		}
		else //不含通配符
		{
			Component * comp_node = diskpath->containNode(path_list.back(), size, type);
			if (type == TYPE_LINK_FILE) //符号链接文件
			{
				comp_node = DiskPathDeal::getLinkNode(comp_node);
				type = TYPE_FILE;
			}
			if (type == TYPE_LINK_FOLDER) //符号链接文件夹
			{
				comp_node = DiskPathDeal::getLinkNode(comp_node);
				type = TYPE_FOLDER;
			}
			if (type == TYPE_FOLDER) //文件夹 list<pair<pair<wstring, int>, char*> >file_list;//存放<名字，大小>，首地址
			{
				Folder* folder = static_cast<Folder*>(comp_node);
				bool flag = false;
				for (auto it : file_list)
				{

					if (folder->getNodeByName(it.first.first) != nullptr)
					{
						wstring str;
						if (flag == false)
						{
							do
							{
								wcout << L"覆盖 " << it.first.first << L" 吗？ (Yes/No/All): ";
								wcin >> str;
								transform(str.begin(), str.end(), str.begin(), ::tolower);
							} while (!(str == L"yes" || str == L"no" || str == L"all"));
						}
						if (str == L"all") flag = true;
						if (flag || str == L"yes" || str == L"all")
						{

							folder->Remove(it.first.first);
							folder->Add(it.first.first, TYPE_FILE);//添加文件
							File* new_file_node = static_cast<File*>(folder->getNodeByName(it.first.first));
							new_file_node->setContentAgain(it.second, it.first.second);
						}
						else if (str == L"no")
						{
							continue;
						}
					}
					else
					{
						folder->Add(it.first.first, TYPE_FILE);//添加文件
						File* new_file_node = static_cast<File*>(folder->getNodeByName(it.first.first));
						new_file_node->setContent(it.second, it.first.second);
					}
				}
			}
			else if (type == TYPE_FILE) //如果是文件 //对应comp
			{
				File* file = static_cast<File*>(comp_node);
				wstring str;
				do
				{
					wcout << L"覆盖 " << file->getName() << L" 吗？ (Yes/No/All): ";
					wcin >> str;
					transform(str.begin(), str.end(), str.begin(), ::tolower);
				} while (!(str == L"yes" || str == L"no" || str == L"all"));
				if (str == L"yes" || str == L"all")
				{
					for (auto it : file_list) //list<pair<pair<wstring, int>, char*> >file_list;//存放<名字，大小>，首地址
					{
						file->setContentAgain(it.second, it.first.second);
					}
				}
			}
			else //判断其目录是否存在，存在创建该文件
			{
				int type, size;
				Component* tmp_com_node = diskpath->containNode(folder_file.first, size, type);
				if (type != TYPE_FOLDER)
				{
					wcout << L"系统找不到指定的路径。" << endl;
					return false;
				}
				Folder* folder = static_cast<Folder*>(tmp_com_node);
				folder->Add(folder_file.second, TYPE_FILE);
				File* file = static_cast<File*>(folder->getNodeByName(folder_file.second));
				wstring str;
				for (auto it : file_list) //list<pair<pair<wstring, int>, char*> >file_list;//存放<名字，大小>，首地址
				{
					file->setContentAgain(it.second, it.first.second);
				}
			}
		}

	}
	for (auto it : file_list)
	{
		delete[]it.second;
	}
	return true;
}

CopyCommand::~CopyCommand()
{
}

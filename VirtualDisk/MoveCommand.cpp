#include "MoveCommand.h"



MoveCommand::MoveCommand()
{
}

bool MoveCommand::executeCmd(DiskPath* diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
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

	if (path_list.size() != 2)
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	bool src_wildcard;
	list<Component*> src_comp_list;
	diskpath->getPathNode(path_list.front(), src_wildcard,src_comp_list);

	if (src_wildcard == true) //如果含有通配符
	{
		if (src_comp_list.size() == 0)
		{
			wcout << L"系统找不到指定的文件。" << endl;
			return false;
		}
	}
	else //如果不含通配符
	{
		if (src_comp_list.size() == 0)
		{
			wcout << L"系统找不到指定的文件。" << endl;
			return false;
		}
	}


	int dst_size, dst_type;
	Component* dst_comp = diskpath->containNode(path_list.back(), dst_size, dst_type);
	if (dst_type == TYPE_NOT_HAVE) //不是文件夹
	{
		pair<wstring, wstring>folder_and_file = DiskPathDeal::getPathFolderAndFile(path_list.back());
		int tmp_size, tmp_type;
		Component* tmp_comp = diskpath->containNode(folder_and_file.first, tmp_size, tmp_type);
		if (tmp_type != TYPE_FILE)
		{
			wcout << L"系统找不到指定的文件。" << endl;
		}
	}
	else if (dst_type == TYPE_FOLDER) //是文件夹dst_comp
	{
		bool do_not_ask = have_arg ? true : false;
		for (auto it : src_comp_list) //对应节点
		{
			Folder* folder_dst_comp = static_cast<Folder*>(dst_comp);
			if (folder_dst_comp->getNodeByName(it->getName())) //如果名字冲突
			{
				wstring input;
				if (!do_not_ask)
				{
					do
					{
						wcout << L"覆盖 " << it->getName() << L" 吗？ (Yes/No/All):";
						wcin >> input;
					} while (!(input == L"yes" || input == L"no" || input == L"all"));
					if (input == L"all")
						do_not_ask = true;
				}
				if (input == L"no") continue;
				Component* comp = folder_dst_comp->getNodeByName(it->getName());
				if (comp->getType() == TYPE_FOLDER)//文件夹，不能将文件夹覆盖文件夹，也不能将文件覆盖文件夹
				{
					wcout << L"拒绝访问。" << endl;
					continue;
				}
			}
			Folder* father_folder = static_cast<Folder*>(it->getFatherNode());
			father_folder->RemoveNotDelete(it->getName());
			if (folder_dst_comp->getNodeByName(it->getName())!=nullptr) //如果有重名
			{
				folder_dst_comp->Remove(it->getName());
			}
			folder_dst_comp->AddNotCreat(it);

		}
	}
	else if (dst_type == TYPE_FILE) //是文件
	{
		if (src_comp_list.size() != 1)
		{
			wcout << L"拒绝访问。" << endl;
			return false;
		}
		if (src_comp_list.front()->getType() != TYPE_FILE) //必须是文件
		{
			wcout << L"拒绝访问。" << endl;
			return false;
		}
		Component* src_comp = src_comp_list.front();
		wstring input;
		bool do_not_ask = have_arg ? true : false;
		if (!do_not_ask)
		{
			do
			{
				wcout << L"覆盖 " << dst_comp->getName() << L" 吗？ (Yes/No/All):";
				wcin >> input;
			} while (!(input == L"yes" || input == L"no" || input == L"all"));
			if (input == L"all")
				do_not_ask = true;
		}
		if (input == L"no") return true;
		File* dst_file = static_cast<File*>(dst_comp);
		File* src_file = static_cast<File*>(src_comp);
		int dst_size;
		wchar_t* dst_content = src_file->getContent(dst_size);
		dst_file->setContent(dst_content, dst_size);
		Folder* father_src_file = static_cast<Folder*>(src_file->getFatherNode());
		father_src_file->Remove(src_file->getName());
	}
	return true;
}

MoveCommand::~MoveCommand()
{
}

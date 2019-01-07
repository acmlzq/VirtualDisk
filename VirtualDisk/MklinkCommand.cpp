#include "MklinkCommand.h"



MklinkCommand::MklinkCommand()
{

}

bool MklinkCommand::executeCmd(DiskPath*diskdeal, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	for (auto it : arg_list)
	{
		if (it == L"/d")
			have_arg = true;
		else
		{
			wcout << L"命令语法不正确。" << endl;
			return false;
		}
	}
	if (path_list.size() != 2) //先判断格式
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}

	int size, type, link_type;

	Component *comp_node = diskdeal->containNode(path_list.back(), size, link_type);
	if (size == -1) //判断目的链接路径是否存在(链接路径必须存在)
	{
		wcout << L"拒绝访问。" << endl;
		return false;
	}

	if (!DiskPathDeal::pathIsLegit(path_list.front())) //判断mklink链接文件名称是否合法（文件名称必须合法）
	{
		wcout << L"文件名、目录名或卷标语法不正确。" << endl;
		return false;
	}

	auto complete_path = DiskPathDeal::getPathFolderAndFile(path_list.front());

	comp_node = diskdeal->containNode(complete_path.first, size, type);
	if (size == -1) //没有找到
	{
		wcout << L"系统找不到指定的路径。" << endl;
		return false;
	}

	Folder* folder = static_cast<Folder*>(comp_node);
	if (folder->getNodeByName(complete_path.second) != nullptr) //如果重名
	{
		wcout << L"当文件已存在时，无法创建该文件。" << endl;
		return false;
	}
	if (link_type == TYPE_FOLDER || link_type == TYPE_LINK_FOLDER) type = TYPE_LINK_FOLDER;
	if (link_type == TYPE_FILE || link_type == TYPE_LINK_FILE) type = TYPE_LINK_FILE;
	folder->Add(complete_path.second, type); //创建链接文件
	SymLink* symlink_node = static_cast<SymLink*>(folder->getNodeByName(complete_path.second));
	symlink_node->setLink(path_list.back());
	return true;
}

MklinkCommand::~MklinkCommand()
{
}

/*
##mklink命令流程

* 格式：mklink [/d] srcPath symbolLinkPath

* 具体流程
0. 先判断格式
1. 判断链接路径是否存在
2. 判断链接文件名称是否合法
3. 判断链接文件名称是否会有重名情况
4. 创建链接文件
*/

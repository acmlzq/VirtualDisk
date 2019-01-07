#include "RenCommand.h"



RenCommand::RenCommand()
{

}

bool RenCommand::executeCmd(DiskPath*diskdeal, list<wstring>&arg_path_list, FileDisk* filedisk) //这里的path_list是除了命令外的其他字符串链表
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	if (arg_list.size()!=0||path_list.size() != 2) //判断格式（路径参数个数必须为2，没有命令参数）
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	int size, type;
	auto comp_node = diskdeal->containNode(path_list.front(), size, type);
	if (type == 0) //判断源文件是否存在(源文件必须存在)
	{
		wcout << L"系统找不到指定的文件。" << endl;
		return false;
	}
	if (!DiskPathDeal::pathIsLegit(path_list.back())) //判断重命名的名字是否合法（必须合法）
	{
		wcout << L"文件名、目录名或卷标语法不正确。" << endl;
		return false;
	}
	Folder* father_node = static_cast<Folder*>(comp_node->getFatherNode());//判断重命名的名字是否会有重名情况
	if (father_node->getNodeByName(path_list.back()) != nullptr)
	{
		wcout << L"存在一个重名文件，或是找不到文件。" << endl;
		return false;
	}
	wstring old_name = comp_node->getName(); //改名字，同时更新父节点对于子节点名字和节点的映射关系
	comp_node->setName(path_list.back());
	father_node->Cmap.erase(old_name);
	father_node->Cmap.insert(make_pair(path_list.back(), comp_node));
	return true;
}

RenCommand::~RenCommand()
{

}

/*

* 格式：ren src dst

* 具体流程
0. 先判断格式
1. 判断源文件是否存在
2. 再判断重命名的名字是否合法
3. 判断重命名的名字是否会有重名情况
4. 改名字，同时更新父节点的对于子节点名字和节点的映射关系

*/

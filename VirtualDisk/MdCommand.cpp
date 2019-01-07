#include "MdCommand.h"

MdCommand::MdCommand()
{

}

/*
先判断路径是否合法，
再判断路径是否存在
然后创建

尚未完成功能：
  创建存在空格的文件夹
*/
bool MdCommand::executeCmd(DiskPath* diskpath, list<wstring> &arg_path_list, FileDisk* filedisk)//传入的list参数为所输入的所有路径
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list,arg_list,path_list);
	if (arg_list.size()!=0||path_list.size() == 0)
	{
		wcout << L"命令语法不正确。" << endl;
		return false;
	}
	for (auto path : path_list)
	{
		
		if (!DiskPathDeal::pathIsLegit(path)) // 路径不合法，输出路径不合法提示
		{
			wcout << L"文件名、目录名或卷标语法不正确。" << endl;
			continue;
		}
		int type, size;
		diskpath->containNode(path, size, type); //判断路径是否存在
		if (type != TYPE_NOT_HAVE) //说明该目录已经存在，输出路径已经存在提示
		{
			wcout << L"该路径已经存在" << endl;
			continue;
		}

		Component * comp_node;//通过判断是绝对路径还是相对路径，来获取目录节点
		list<wstring>list_single_path;
		DiskPathDeal::getSplitPath(list_single_path,path);
		if (DiskPathDeal::isAbsPath(path)) //绝对路径
		{
			comp_node = diskpath->getRootNode();
			list_single_path.pop_front();
		}
		else
		{
			comp_node = diskpath->getCurNode();
		}
		for (auto it : list_single_path) //依次查找，不存在就创建
		{
			Folder* folder = static_cast<Folder*>(comp_node);
			if (it == L".")
				continue;
			if (it == L"..")
			{
				if (comp_node->getFatherNode() == nullptr)
				{
					wcout << L"文件名、目录名或卷标语法不正确。" << endl;
					break;
				}
				else
					comp_node = comp_node->getFatherNode();
				continue;
			}
			if (folder->getNodeByName(it) == nullptr) //如果没有就创建
			{
				bool flag = false;
				folder->Add(it, TYPE_FOLDER);
				comp_node = folder->getNodeByName(it);
			}
			else  //已经存在的
			{
				comp_node = folder->getNodeByName(it);
				if (comp_node->getType() == TYPE_LINK_FOLDER)
					comp_node = DiskPathDeal::getLinkNode(comp_node);
			}
		}
	}
	return true;
}


MdCommand::~MdCommand()
{

}

/*
##md命令流程

* 格式：md path

* 具体流程
1. 先判断路径名称是否合法
2. 再判断路径是否存在
3. 获取目录节点
4. 依次判断并创建
*/

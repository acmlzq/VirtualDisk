#include "DiskPath.h"

DiskPath::DiskPath()
{

}


Component* DiskPath::getRootNode() //获取根节点
{
	if (0 != path_list.size())
	{
		return path_list.front().second;
	}
	else
	{
		return nullptr;
	}
}
void DiskPath::ClearPathList() //清空（格式化磁盘的时候需要）
{
	path_list.clear();
}
Component* DiskPath::getCurNode() //获取当前节点
{
	if (0 != path_list.size())
	{
		return path_list.back().second;
	}
	else
	{
		return nullptr;
	}
}



//这里的路径为绝对路径或相对路径，给定绝对路径，返回其对应节点 //path为空，返回当前路径
Component* DiskPath::containNode(const wstring &path, int &size, int &type)
{
	//C:\b in\sg

	list<wstring> split_path;
	DiskPathDeal::getSplitPath(split_path,path);
	Component* cur_folder;
	bool contain_flag = true; //默认包含
	if (split_path.size() && DiskPathDeal::isAbsPath(split_path.front())) //如果是绝对路径，去掉第一个路径参数
	{
		split_path.pop_front();
		cur_folder = getRootNode();
	}
	else //如果是相对路径
	{
		cur_folder = getCurNode();
	}

	list<wstring>::iterator it = split_path.begin();
	for (int i = 0; it != split_path.end(); it++, i++)
	{
		if (*it == L".")
		{
			continue;
		}
		else if (*it == L"..")
		{
			if (cur_folder->getFatherNode() == nullptr)
				return nullptr;
			else
				cur_folder = cur_folder->getFatherNode();
			continue;
		}
		if (cur_folder->getType() == TYPE_FOLDER) //对应为文件夹类型
		{
			Folder* tmp_folder = static_cast<Folder*>(cur_folder);
			Component * tmp_com = tmp_folder->getNodeByName(*it);

			if (nullptr != tmp_com) //找到了，
			{
				cur_folder = tmp_com;
				if (cur_folder->getType() == TYPE_LINK_FOLDER) //如果是链接文件，获取其链接节点
				{
					if (i != split_path.size() - 1)
					{
						cur_folder = DiskPathDeal::getLinkNode(cur_folder);
						if (cur_folder == nullptr)
						{
							contain_flag = false;
							break;
						}
					}
				}
			}
			else //如果没有找到，那么说明该路径不存在，
			{
				contain_flag = false;
				break;
			}
		}
		else//对应为文件类型
		{
			contain_flag = false;
			break;
		}
	}

	if (contain_flag == true) //找到了
	{
		size = cur_folder->getSize()*2;  //宽字符，所以真实大小要乘以2
		type = cur_folder->getType();
		return cur_folder;
	}
	else //未找到
	{
		size = -1;
		type = 0;
		return nullptr;
	}

	return nullptr;
}

wstring DiskPath::getCurPath()
{
	wstring cur_path;
	bool flag = true;
	for (auto it : path_list)
	{
		cur_path += flag ? L"" : L"\\";
		cur_path += it.first;
		flag = false;
	}
	if (path_list.size() == 1)
		cur_path += L"\\";
	return cur_path;
}





/*
给定path，获取所有路径对应的节点，考虑*,?运算符匹配问题
*?运算符只有最后一级目录存在

流程：
将路径切割
back_str对应最后一个字符串
front_str对应前面的字符串

获取front_str对应的节点，然后在此目录中遍历所有文件/文件夹 和back_str进行字符匹配判断，如果匹配则返回push_back
*/
void DiskPath::getPathNode(const wstring &path, bool &have_wildcards, list<Component*>& ans_node)
{
	list<wstring>split_path;
	DiskPathDeal::getSplitPath(split_path,path);
	unsigned int i = 0;
	bool is_first = true;
	wstring front_str, back_str;
	for (list<wstring>::iterator it = split_path.begin(); i < split_path.size(); it++, i++)  //拼合front_str,back_str
	{
		if (i == split_path.size() - 1)//如果是最后一个路径，得到back_str
		{
			back_str += *it;
		}
		else //如果不是，得到front_str
		{
			if (is_first == false)
				front_str += '\\';
			front_str += *it;
			is_first = false;
		}
	}
	if (front_str == L"" && (back_str == L"C:" || back_str == L"\\" || back_str == L"/"))   //可能路径分割后只有一个，可能对应文件，或者文件夹，将可能是根的放前面
		swap(front_str,back_str);
	have_wildcards = StringDeal::isHaveWildcard(back_str);
	//可能出现的问题：front_str为空，back_str为空
	int size, type;
	Component* comp_node = containNode(front_str, size, type);
	if (type == TYPE_NOT_HAVE || type == TYPE_FILE || type == TYPE_LINK_FILE) //没有文件，或者类型为文件或者符号链接文件
	{
		return ;
	}
	while (comp_node->getType() == TYPE_LINK_FOLDER) //符号链接文件夹，获取其链接节点
	{
		Component* link_file = DiskPathDeal::getLinkNode(comp_node);
		if (link_file == nullptr || link_file->getType() == TYPE_FILE || link_file->getType() == TYPE_LINK_FILE) //链接路径不能为文件或者链接文件（必须是文件夹或者链接文件夹）
		{
			return ;
		}
		comp_node = link_file;
	}
	Folder* folder_node = static_cast<Folder*>(comp_node);
	if (back_str == L"" || back_str == L".") //可能此文件路径为空。此时对应当前路径
		ans_node.push_back(folder_node);
	for (map<wstring, Component*>::iterator it = folder_node->Cmap.begin(); it != folder_node->Cmap.end(); it++)
	{
		if (StringDeal::matches(back_str.c_str(), it->first.c_str()))
		{
			ans_node.push_back(it->second);
		}
	}
}

void DiskPath::setListHead(Component* com, const wstring& name)
{
	path_list.clear();
	path_list.push_front(make_pair(name, com));
}


//如果返回类型为false的话，说明符号链接文件夹对应路径没找到
/*
流程：
先判断是相对路径，还是绝对路径，并获取其对应初始目录节点
沿着路径一层一层向下走
*/
bool DiskPath::updateCurPath(const wstring& path) //更新当前路径，在使用此方法前先确定path存在，且为文件夹或者符号链接文件夹
{
	list<wstring> split_path;
	DiskPathDeal::getSplitPath(split_path,path);
	Component* cur_folder;
	bool contain_flag = true; //默认包含
	if (DiskPathDeal::isAbsPath(split_path.front())) //如果是绝对路径，去掉第一个路径参数
	{
		split_path.pop_front();
		cur_folder = getRootNode();
		while (path_list.size() != 1) //如果是绝对路径，那么对于路径状态将除了根节点外的其他所有节点删除掉
		{
			path_list.pop_back();
		}
	}
	else //如果是相对路径
	{
		cur_folder = getCurNode();
	}
	for (list<wstring>::iterator it = split_path.begin(); it != split_path.end(); it++)
	{
		if (*it == L".")
			continue;
		else if (*it == L"..")//如果是上一级的话
		{
			if (path_list.size() == 1)
			{
				return false;
			}
			else
			{
				path_list.pop_back();
				continue;
			}
		}
		if (cur_folder->getType() == TYPE_LINK_FOLDER)
		{
			cur_folder = DiskPathDeal::getLinkNode(cur_folder);

			if (cur_folder == nullptr)
			{
				return false;
			}
		}
		if (cur_folder->getType() == 1) //如果是文件夹
		{
			Folder* tmp_folder = static_cast<Folder*>(cur_folder);
			Component* tmp_com = tmp_folder->getNodeByName(*it);
			if (nullptr != tmp_com) //找到了，pair<名字，节点>push到尾部
			{
				if (tmp_com->getType() == TYPE_LINK_FOLDER)
				{
					Component* tmp_link_path = DiskPathDeal::getLinkNode(tmp_com);
					if (tmp_link_path != nullptr)
					{
						path_list.push_back(make_pair(tmp_com->getName(), tmp_link_path));
						cur_folder = tmp_link_path;
					}
					else
					{
						return false;
					}
				}
				else
				{
					path_list.push_back(make_pair(tmp_com->getName(), tmp_com));
					cur_folder = tmp_com;
				}
			}
			else
				return false;
		}
	}
	return true;


}

Component* DiskPath::getCurFather(Component* comp)
{
	for (list<pair<wstring, Component *> >::reverse_iterator it = path_list.rbegin(); it != path_list.rend(); it++)
	{
		if (it->second == comp)
		{
			if (it->second != getRootNode())
			{
				++it;
				return it->second;
			}
		}
	}
	return nullptr;
}

bool DiskPath::nodeIsInPathList(Component* comp_node)//判断节点是否在路径中
{
	for (auto it : path_list)
	{
		if (it.second == comp_node)
		{
			return true;
		}
	}
	return false;
}


DiskPath::~DiskPath()
{
}

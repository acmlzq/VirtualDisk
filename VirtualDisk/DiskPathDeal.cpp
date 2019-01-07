#include "DiskPathDeal.h"



DiskPathDeal::DiskPathDeal()
{

}



Component* DiskPathDeal::getRootNode(Component* node)
{
	while (node->getFatherNode() != nullptr)
	{
		node = node->getFatherNode();
	}
	return node;
}

wstring DiskPathDeal::getAbsPath(Component* comp_node) //获取该节点的绝对路径
{
	if (comp_node == nullptr)
		return L"";
	bool flag = false;
	wstring abs_path;
	stack<wstring>s;
	s.push(comp_node->getName());
	while (comp_node->getFatherNode() != nullptr)
	{
		comp_node = comp_node->getFatherNode();
		s.push(comp_node->getName());
	}
	while (!s.empty())
	{
		if (flag)
			abs_path += L"\\";
		abs_path += s.top();
		flag = true;
		s.pop();
	}
	if (abs_path == L"C:")
		abs_path += L"\\";
	return abs_path;
}
void DiskPathDeal::recurGetNode(Component* comp_node, list<pair<wstring, Component*>>& recur_node)
{
	Folder* folder = static_cast<Folder*>(comp_node);

	for (auto it : folder->Cmap)
	{
		if (it.second->getName() == L"." || it.second->getName() == L"..")
			continue;
		if (it.second->getType() == TYPE_FOLDER) //文件夹
		{
			recur_node.push_back(make_pair(getAbsPath(it.second), it.second));
			recurGetNode(static_cast<Folder*>(it.second), recur_node);
		}
		else if (it.second->getType() == TYPE_LINK_FOLDER) //如果是链接文件夹
		{
			if (getLinkNode(it.second) != nullptr)
			{
				recur_node.push_back(make_pair(getAbsPath(it.second), getLinkNode(it.second)));
			}
		}
	}
}

void DiskPathDeal::getNodeByRecur(Component* comp, list<pair<wstring, Component*>>& recur_node)
{
	recur_node.push_back(make_pair(getAbsPath(comp), comp));
	Folder* folder = static_cast<Folder*>(comp);
	recurGetNode(folder, recur_node);
}

//获取路径对应的文件和文件夹，如果path中路径分割后只有一个，那么folder为空，file为其路径（注意：有可能file也为目录）
pair<wstring, wstring> DiskPathDeal::getPathFolderAndFile(const wstring &path)
{
	wstring folder_name;
	wstring file_name;
	bool div_flag = true;
	for (int i = path.size() - 1; i >= 0; i--)
	{
		if ((path[i] == '\\' || path[i] == '/') && div_flag == true)
		{
			div_flag = false;
			continue;
		}
		if (div_flag == true)
		{
			file_name += path[i];
		}
		else
		{
			folder_name += path[i];
		}
	}
	reverse(folder_name.begin(), folder_name.end());
	reverse(file_name.begin(), file_name.end());
	return make_pair(folder_name, file_name);
}

bool DiskPathDeal::pathIsLegit(const wstring &path) //不能出现 /\ : * ? " < > | 字符，以及文件名不能为 . 及 ..
{
	list<wstring>list_str;
	getSplitPath(list_str,path);//将路径拆分之后再判断
	bool is_first = true;
	int len = list_str.size(), i;
	list<wstring>::iterator it;
	for (it = list_str.begin(), i = 0; it != list_str.end(); it++, i++)
	{
		if (is_first && (*it == L"C:" || *it == L"") && list_str.size() != TYPE_FOLDER) //根目录
			continue;
		if (*it == L"" || ((*it == L"." || *it == L"..") && i == len - 1)) //最终创建的文件名字不能叫做.或者..
		{
			return false;
		}
		for (unsigned int i = 0; i < (*it).size(); i++)
		{
			if ((*it)[i] == '/' || (*it)[i] == '\\' || (*it)[i] == '/' || (*it)[i] == ':' || (*it)[i] == '*' || (*it)[i] == '?' || (*it)[i] == '"' || (*it)[i] == '<' || (*it)[i] == '>' || (*it)[i] == '|')
			{
				return false;
			}
		}
	}
	return true;
}


void DiskPathDeal::getSplitPath(list<wstring>&ans_path,const wstring &path, char split_char)
{
	wstring single_path = L"";
	if (split_char == '\\' || split_char == '/')
	{
		for (unsigned int i = 0; i < path.size(); i++)
		{
			if (path[i] == '\\' || path[i] == '/')
			{
				if (i == 0)
				{
					ans_path.push_back(L"C:");
				}
				else
					ans_path.push_back(single_path), single_path = L"";
			}
			else
				single_path += path[i];
		}
	}
	else
	{
		for (unsigned int i = 0; i < path.size(); i++)
		{
			if (path[i] == ' ')
			{
				if (single_path.size() != 0)
				{
					ans_path.push_back(single_path), single_path = L"";
				}
			}

			else
				single_path += path[i];
		}
	}
	if (single_path != L"")
		ans_path.push_back(single_path);
}

//判断该路径是否为绝对路径，
bool DiskPathDeal::isAbsPath(const wstring& path)
{
	if (path.size() >= 1 && (path[0] == '\\' || path[0] == '/')) //1:路径第一个字符为'\'
	{
		return true;
	}
	if (path.size() == 2 && path[0] == 'C'&&path[1] == ':')
	{
		return true;
	}
	if (path.size() >= 3 && path[0] == 'C'&&path[1] == ':' && (path[2] == '\\' || path[2] == '/'))
	{
		return true;
	}
	return false;
}


Component* DiskPathDeal::getLinkNode(Component* link_node) //获得符号链接文件链接节点
{
	SymLink *tmp = static_cast<SymLink*>(link_node);
	wstring path = tmp->getLinkPath();
	list<wstring> split_path;
	getSplitPath(split_path,path);
	Component* cur_folder;
	bool contain_flag = true; //默认包含
	if (DiskPathDeal::isAbsPath(split_path.front())) //如果是绝对路径，则去掉第一个路径
	{
		split_path.pop_front();
		cur_folder = getRootNode(link_node);
	}
	else //如果是相对路径
	{
		cur_folder = link_node->getFatherNode();
	}
	for (auto  it : split_path)
	{
		if (it == L".")
			continue;
		else if (it == L"..")
		{
			cur_folder = cur_folder->getFatherNode();
			continue;
		}
		if (cur_folder == nullptr)
		{
			contain_flag = false;
			break;
		}
		if (cur_folder->getType() == TYPE_FOLDER) //对应文件夹类型
		{
			Folder* tmp_folder = static_cast<Folder*>(cur_folder);
			Component * tmp_com = tmp_folder->getNodeByName(it);
			if (tmp_com == nullptr) ////如果没有找到，那么说明该路径不存在，
			{
				contain_flag = false;
				break;
			}
			while (tmp_com->getType() == TYPE_LINK_FOLDER)
			{
				//tmp_com = getLinkNode((SymLink*)tmp_com);
				tmp_com = getLinkNode(static_cast<Folder*>(tmp_com));
				if (tmp_com == nullptr) return nullptr;
			}
			if (tmp_com->getType() == TYPE_LINK_FILE)
			{
				//tmp_com = getLinkNode((SymLink*)tmp_com);
				tmp_com = static_cast<SymLink*>(tmp_com);
			}
			if (nullptr != tmp_com) //找到了，
			{
				cur_folder = tmp_com;
			}
		}
		else if (cur_folder->getType() == TYPE_FILE) //对应文件类型
		{
			contain_flag = false;
			break;
		}
		else if (cur_folder->getType() == TYPE_LINK_FILE) //对应符号链接类型
		{
			contain_flag = false;
			break;
		}
		else if (cur_folder->getType() == TYPE_LINK_FOLDER) //对应符号链接文件夹类型
		{
			while (cur_folder->getType() == TYPE_LINK_FOLDER)
				cur_folder = getLinkNode(static_cast<SymLink*>(cur_folder));
		}
	}
	if (contain_flag == false)
		return nullptr;
	else
		return cur_folder;
}

















DiskPathDeal::~DiskPathDeal()
{

}














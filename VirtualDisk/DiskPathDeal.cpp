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

wstring DiskPathDeal::getAbsPath(Component* comp_node) //��ȡ�ýڵ�ľ���·��
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
		if (it.second->getType() == TYPE_FOLDER) //�ļ���
		{
			recur_node.push_back(make_pair(getAbsPath(it.second), it.second));
			recurGetNode(static_cast<Folder*>(it.second), recur_node);
		}
		else if (it.second->getType() == TYPE_LINK_FOLDER) //����������ļ���
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

//��ȡ·����Ӧ���ļ����ļ��У����path��·���ָ��ֻ��һ������ôfolderΪ�գ�fileΪ��·����ע�⣺�п���fileҲΪĿ¼��
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

bool DiskPathDeal::pathIsLegit(const wstring &path) //���ܳ��� /\ : * ? " < > | �ַ����Լ��ļ�������Ϊ . �� ..
{
	list<wstring>list_str;
	getSplitPath(list_str,path);//��·�����֮�����ж�
	bool is_first = true;
	int len = list_str.size(), i;
	list<wstring>::iterator it;
	for (it = list_str.begin(), i = 0; it != list_str.end(); it++, i++)
	{
		if (is_first && (*it == L"C:" || *it == L"") && list_str.size() != TYPE_FOLDER) //��Ŀ¼
			continue;
		if (*it == L"" || ((*it == L"." || *it == L"..") && i == len - 1)) //���մ������ļ����ֲ��ܽ���.����..
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

//�жϸ�·���Ƿ�Ϊ����·����
bool DiskPathDeal::isAbsPath(const wstring& path)
{
	if (path.size() >= 1 && (path[0] == '\\' || path[0] == '/')) //1:·����һ���ַ�Ϊ'\'
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


Component* DiskPathDeal::getLinkNode(Component* link_node) //��÷��������ļ����ӽڵ�
{
	SymLink *tmp = static_cast<SymLink*>(link_node);
	wstring path = tmp->getLinkPath();
	list<wstring> split_path;
	getSplitPath(split_path,path);
	Component* cur_folder;
	bool contain_flag = true; //Ĭ�ϰ���
	if (DiskPathDeal::isAbsPath(split_path.front())) //����Ǿ���·������ȥ����һ��·��
	{
		split_path.pop_front();
		cur_folder = getRootNode(link_node);
	}
	else //��������·��
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
		if (cur_folder->getType() == TYPE_FOLDER) //��Ӧ�ļ�������
		{
			Folder* tmp_folder = static_cast<Folder*>(cur_folder);
			Component * tmp_com = tmp_folder->getNodeByName(it);
			if (tmp_com == nullptr) ////���û���ҵ�����ô˵����·�������ڣ�
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
			if (nullptr != tmp_com) //�ҵ��ˣ�
			{
				cur_folder = tmp_com;
			}
		}
		else if (cur_folder->getType() == TYPE_FILE) //��Ӧ�ļ�����
		{
			contain_flag = false;
			break;
		}
		else if (cur_folder->getType() == TYPE_LINK_FILE) //��Ӧ������������
		{
			contain_flag = false;
			break;
		}
		else if (cur_folder->getType() == TYPE_LINK_FOLDER) //��Ӧ���������ļ�������
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














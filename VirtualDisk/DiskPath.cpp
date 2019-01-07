#include "DiskPath.h"

DiskPath::DiskPath()
{

}


Component* DiskPath::getRootNode() //��ȡ���ڵ�
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
void DiskPath::ClearPathList() //��գ���ʽ�����̵�ʱ����Ҫ��
{
	path_list.clear();
}
Component* DiskPath::getCurNode() //��ȡ��ǰ�ڵ�
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



//�����·��Ϊ����·�������·������������·�����������Ӧ�ڵ� //pathΪ�գ����ص�ǰ·��
Component* DiskPath::containNode(const wstring &path, int &size, int &type)
{
	//C:\b in\sg

	list<wstring> split_path;
	DiskPathDeal::getSplitPath(split_path,path);
	Component* cur_folder;
	bool contain_flag = true; //Ĭ�ϰ���
	if (split_path.size() && DiskPathDeal::isAbsPath(split_path.front())) //����Ǿ���·����ȥ����һ��·������
	{
		split_path.pop_front();
		cur_folder = getRootNode();
	}
	else //��������·��
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
		if (cur_folder->getType() == TYPE_FOLDER) //��ӦΪ�ļ�������
		{
			Folder* tmp_folder = static_cast<Folder*>(cur_folder);
			Component * tmp_com = tmp_folder->getNodeByName(*it);

			if (nullptr != tmp_com) //�ҵ��ˣ�
			{
				cur_folder = tmp_com;
				if (cur_folder->getType() == TYPE_LINK_FOLDER) //����������ļ�����ȡ�����ӽڵ�
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
			else //���û���ҵ�����ô˵����·�������ڣ�
			{
				contain_flag = false;
				break;
			}
		}
		else//��ӦΪ�ļ�����
		{
			contain_flag = false;
			break;
		}
	}

	if (contain_flag == true) //�ҵ���
	{
		size = cur_folder->getSize()*2;  //���ַ���������ʵ��СҪ����2
		type = cur_folder->getType();
		return cur_folder;
	}
	else //δ�ҵ�
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
����path����ȡ����·����Ӧ�Ľڵ㣬����*,?�����ƥ������
*?�����ֻ�����һ��Ŀ¼����

���̣�
��·���и�
back_str��Ӧ���һ���ַ���
front_str��Ӧǰ����ַ���

��ȡfront_str��Ӧ�Ľڵ㣬Ȼ���ڴ�Ŀ¼�б��������ļ�/�ļ��� ��back_str�����ַ�ƥ���жϣ����ƥ���򷵻�push_back
*/
void DiskPath::getPathNode(const wstring &path, bool &have_wildcards, list<Component*>& ans_node)
{
	list<wstring>split_path;
	DiskPathDeal::getSplitPath(split_path,path);
	unsigned int i = 0;
	bool is_first = true;
	wstring front_str, back_str;
	for (list<wstring>::iterator it = split_path.begin(); i < split_path.size(); it++, i++)  //ƴ��front_str,back_str
	{
		if (i == split_path.size() - 1)//��������һ��·�����õ�back_str
		{
			back_str += *it;
		}
		else //������ǣ��õ�front_str
		{
			if (is_first == false)
				front_str += '\\';
			front_str += *it;
			is_first = false;
		}
	}
	if (front_str == L"" && (back_str == L"C:" || back_str == L"\\" || back_str == L"/"))   //����·���ָ��ֻ��һ�������ܶ�Ӧ�ļ��������ļ��У��������Ǹ��ķ�ǰ��
		swap(front_str,back_str);
	have_wildcards = StringDeal::isHaveWildcard(back_str);
	//���ܳ��ֵ����⣺front_strΪ�գ�back_strΪ��
	int size, type;
	Component* comp_node = containNode(front_str, size, type);
	if (type == TYPE_NOT_HAVE || type == TYPE_FILE || type == TYPE_LINK_FILE) //û���ļ�����������Ϊ�ļ����߷��������ļ�
	{
		return ;
	}
	while (comp_node->getType() == TYPE_LINK_FOLDER) //���������ļ��У���ȡ�����ӽڵ�
	{
		Component* link_file = DiskPathDeal::getLinkNode(comp_node);
		if (link_file == nullptr || link_file->getType() == TYPE_FILE || link_file->getType() == TYPE_LINK_FILE) //����·������Ϊ�ļ����������ļ����������ļ��л��������ļ��У�
		{
			return ;
		}
		comp_node = link_file;
	}
	Folder* folder_node = static_cast<Folder*>(comp_node);
	if (back_str == L"" || back_str == L".") //���ܴ��ļ�·��Ϊ�ա���ʱ��Ӧ��ǰ·��
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


//�����������Ϊfalse�Ļ���˵�����������ļ��ж�Ӧ·��û�ҵ�
/*
���̣�
���ж������·�������Ǿ���·��������ȡ���Ӧ��ʼĿ¼�ڵ�
����·��һ��һ��������
*/
bool DiskPath::updateCurPath(const wstring& path) //���µ�ǰ·������ʹ�ô˷���ǰ��ȷ��path���ڣ���Ϊ�ļ��л��߷��������ļ���
{
	list<wstring> split_path;
	DiskPathDeal::getSplitPath(split_path,path);
	Component* cur_folder;
	bool contain_flag = true; //Ĭ�ϰ���
	if (DiskPathDeal::isAbsPath(split_path.front())) //����Ǿ���·����ȥ����һ��·������
	{
		split_path.pop_front();
		cur_folder = getRootNode();
		while (path_list.size() != 1) //����Ǿ���·������ô����·��״̬�����˸��ڵ�����������нڵ�ɾ����
		{
			path_list.pop_back();
		}
	}
	else //��������·��
	{
		cur_folder = getCurNode();
	}
	for (list<wstring>::iterator it = split_path.begin(); it != split_path.end(); it++)
	{
		if (*it == L".")
			continue;
		else if (*it == L"..")//�������һ���Ļ�
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
		if (cur_folder->getType() == 1) //������ļ���
		{
			Folder* tmp_folder = static_cast<Folder*>(cur_folder);
			Component* tmp_com = tmp_folder->getNodeByName(*it);
			if (nullptr != tmp_com) //�ҵ��ˣ�pair<���֣��ڵ�>push��β��
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

bool DiskPath::nodeIsInPathList(Component* comp_node)//�жϽڵ��Ƿ���·����
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

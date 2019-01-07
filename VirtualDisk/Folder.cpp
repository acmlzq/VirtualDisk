#include "Folder.h"



Folder::Folder(Component* _father_node, const wstring &_name, int _type) :Component(_father_node, _name, _type)
{

}

void Folder::Add(const wstring &_name, int _type)
{
	Component * new_node;
	if (_type == TYPE_FOLDER) //�ļ�������
	{
		Folder * new_folder = new Folder(static_cast<Component*>(this), _name, _type);
		if (_name != L"."&&_name != L"..")
		{
			new_folder->Add(L".", TYPE_FOLDER);
			new_folder->Add(L"..", TYPE_FOLDER);
		}
		new_node = new_folder;
	}
	else if (_type == TYPE_FILE) //�ļ�����
	{
		File* new_file = new File(static_cast<Component*>(this), _name, _type);
		new_node = new_file;
	}
	else if (_type == TYPE_LINK_FILE || _type == TYPE_LINK_FOLDER) //������������
	{
		SymLink* new_symlink = new SymLink(static_cast<Component*>(this),_name,_type);
		new_node = new_symlink;
	}
	Cmap.insert(make_pair(_name, new_node));

}

void Folder::displayAll(bool is_dis_file) //����Ϊ�Ƿ��ӡ�ļ���
{
	//����Ҫͳ��Ŀ¼���ļ��������ļ��и�������ǰĿ¼��ռ�ܿռ�
	if (is_dis_file == true)//��ӡ��ǰĿ¼�����ļ�������Ϣ
	{
		int sum_file = 0, sum_folder = 0, all_size = 0; //ͳ���ļ��������ļ��и������ܿռ��С
		for (auto it : Cmap)
		{
			if (it.second->getType() == TYPE_FOLDER || it.second->getType() == TYPE_LINK_FOLDER) //1��Ӧ�ļ���,TYPE_LINK_FOLDER��Ӧ���������ļ�
			{
				sum_folder++;
			}
			else if (it.second->getType() == TYPE_FILE || it.second->getType() == TYPE_LINK_FILE)//TYPE_FILE��Ӧ�ļ���TYPE_LINK_FILE��Ӧ���������ļ�
			{
				sum_file++;
				if (it.second->getType() == TYPE_FILE)
					all_size += it.second->getSize();
			}
			it.second->Display();
		}
		wcout << L"               " << sum_file << L"���ļ�    " << StringDeal::intToString(all_size) << L"�ֽ�" << endl;
		wcout << L"               " << sum_folder << L"��Ŀ¼" << endl;
	}
	else if (is_dis_file == false)//ֻ��ӡ��ǰĿ¼�ļ�����Ϣ
	{
		int sum_file = 0, sum_folder = 0, all_size = 0; //ͳ���ļ��������ļ��и������ܿռ��С
		for (auto it : Cmap)
		{
			if (it.second->getType() == TYPE_FOLDER || it.second->getType() == TYPE_LINK_FOLDER) //1��Ӧ�ļ���,TYPE_LINK_FOLDER��Ӧ���������ļ�
			{
				sum_folder++;
				it.second->Display();
			}
		}
		wcout << L"               " << sum_file << L"���ļ�    " << StringDeal::intToString(all_size) << L"�ֽ�" << endl;
		wcout << L"               " << sum_folder << L"��Ŀ¼" << endl;
	}
}
Component * Folder::getNodeByName(const wstring &to_find_name)
{
	auto it = Cmap.find(to_find_name);
	if (it == Cmap.end())
	{
		if (StringDeal::isHaveWildcard(to_find_name)==true) //�������ͨ����Ļ�
		{
			for (auto comp_it : Cmap)
			{
				if (StringDeal::matches(to_find_name.c_str(), comp_it.first.c_str()))
				{
					return comp_it.second;
				}
			}
		}
		return nullptr;
	}
	return it->second;
}
void Folder::Remove(const wstring &node_name)
{
	auto it = Cmap.find(node_name);
	if (it != Cmap.end())
	{
		Component *son_node = it->second;
		Cmap.erase(it);
		delete son_node;
	}
}
void Folder::AddNotCreat(Component* comp)
{
	Cmap.insert(make_pair(comp->getName(), comp));
}
void Folder::RemoveNotDelete(const wstring&node_name)
{
	auto it = Cmap.find(node_name);
	if (it != Cmap.end())
	{
		Component *son_node = it->second;
		Cmap.erase(it);
	}
}
Folder::~Folder()
{
	for (auto it : Cmap)
	{
		delete it.second;
	}
}

#include "DelCommand.h"



DelCommand::DelCommand()
{
}

bool DelCommand::executeCmd(DiskPath*diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	if (path_list.size() == 0)
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}
	for (auto it : arg_list)
	{
		if (it == L"/s")
		{
			have_arg = true;
		}
		else
		{
			wcout << L"�����﷨����ȷ��" << endl;
			return false;
		}
	}
	if (have_arg&&path_list.size() == 0)
	{
		path_list.push_back(L".");
	}
	for (auto path_list_it : path_list)
	{
		bool have_wildcard = false;
		list<Component*> comp_list;
		diskpath->getPathNode(path_list_it, have_wildcard, comp_list);

		if (comp_list.size() == 0)
		{
			wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
			continue;
		}

		if (have_wildcard == true) //�������ͨ���
		{
			for (auto it : comp_list)
			{
				if (it->getType() == TYPE_FILE || it->getType() == TYPE_LINK_FILE) //ɾ��
				{
					Folder* folder = static_cast<Folder*>(it->getFatherNode());
					folder->Remove(it->getName());
				}
				else if (it->getType() == TYPE_LINK_FOLDER || it->getType() == TYPE_FOLDER) //�ļ��в����κβ���
				{

				}
			}
		}
		else //�������ͨ���
		{
			if (comp_list.front()->getType() == TYPE_LINK_FOLDER) //�����ļ���
			{
				Component * link_node = DiskPathDeal::getLinkNode(comp_list.front());
				comp_list.pop_front();
				comp_list.push_front(link_node);
			}
			if (comp_list.front()->getType() == TYPE_FOLDER && !have_arg) //�ļ���
			{
				Folder* folder = static_cast<Folder*>(comp_list.front());
				comp_list.pop_front();
				for (auto it : folder->Cmap)
				{
					comp_list.push_back(it.second);
				}
			}
			for (auto comp_list_it : comp_list)
			{
				if (comp_list_it->getType() == TYPE_FILE || comp_list_it->getType() == TYPE_LINK_FILE) //ɾ��
				{
					Folder* folder = static_cast<Folder*>(comp_list_it->getFatherNode());
					folder->Remove(comp_list_it->getName());
				}
				else if (comp_list_it->getType() == TYPE_LINK_FOLDER || comp_list_it->getType() == TYPE_FOLDER) //�ļ��У���Ҫ�ж����޲���
				{
					Folder *folder = static_cast<Folder*>(comp_list_it);
					if (comp_list_it->getType() == TYPE_LINK_FOLDER) //����Ƿ��������ļ�
					{
						Component * p = DiskPathDeal::getLinkNode(comp_list_it);
						if (p == nullptr)
						{
							wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
							continue;
						}
						else
						{
							folder = (Folder*)p;
						}
					}
					if (have_arg) //����в��� ���dirд����dir��
					{
						list<pair<wstring, Component*>> list_pair;
						DiskPathDeal::getNodeByRecur(folder, list_pair);
						if (list_pair.size() != 0)
						{
							for (auto it : list_pair)
							{
								Component * comp_node = it.second;
								wstring str;
								do
								{
									wcout << DiskPathDeal::getAbsPath(comp_node) << L"\\" << comp_node->getName() << L"\\*,�Ƿ�ȷ��<Y/N>? ";
									wcin >> str;
								} while (!(str.size() == 1 && (tolower(str[0]) == 'y' || tolower(str[0]) == 'n')));
								if (tolower(str[0]) == 'y')
								{
									Folder* folder = static_cast<Folder*>(comp_node);
									for (map<wstring, Component*>::iterator file = folder->Cmap.begin(); file != folder->Cmap.end();)
									{
										if (file->second->getType() == TYPE_FILE || file->second->getType() == TYPE_LINK_FILE)
										{
											folder->Remove((file++)->second->getName());
										}
										else
											file++;
									}
								}
							}
						}
					}
					else //û�в���
					{
						for (auto it : folder->Cmap)
						{
							if (it.second->getType() == TYPE_FILE || it.second->getType() == TYPE_LINK_FILE)
							{
								folder->Remove(it.second->getName());
							}
						}
					}

				}
			}
		}
	}
	return true;
}

DelCommand::~DelCommand()
{
}

/*
* ��������
0. ���жϸ�ʽ
1. �ж�Դ�ļ��Ƿ����
2. ����ÿ���ļ�·��
		�������ͨ���������ÿ���ļ�||�ļ��У�
			�ļ���ɾ��
			�ļ��У�����
		�������ͨ���
			�ļ���ɾ��
			�ļ��У�
				�޲�����ɾ��Ŀ¼���ļ�
				�в������ݹ�ɾ������Ŀ¼���ļ�

*/

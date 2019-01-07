#include "CopyCommand.h"



CopyCommand::CopyCommand()
{
}

bool CopyCommand::executeCmd(DiskPath*diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list,arg_list,path_list);
	bool have_arg = false;
	if (path_list.size() == 0)
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}
	for (auto it : arg_list)
	{
		if (it == L"/y")
		{
			have_arg = true;
		}
		else
		{
			wcout << L"�����﷨����ȷ��" << endl;
			return false;
		}
	}
	if (path_list.size() == 1) path_list.push_back(L"");
	if (path_list.size() != 2)
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}
	bool src_true_disk = TruePathDeal::isTrueDiskPath(path_list.front()); //��ʵ����
	bool dst_true_disk = TruePathDeal::isTrueDiskPath(path_list.back()); //��ʵ����

	list<pair<pair<wstring, int>, wchar_t*> >file_list;//���<���֣���С>���׵�ַ
	wstring front_wildcards_str = L""; //���ǰ���·����ͨ���ǰ���ַ���
	bool front_wildcard; //���ǰ���·���Ƿ���ƥ���
	if (src_true_disk) //�������ʵ����
	{
		TruePathDeal::getTrueDiskFile(file_list,path_list.front());  //��ȡ�ļ�Ŀ¼
		if (file_list.size() == 0)
		{
			wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
			return false;
		}
	}
	else //�������
	{
		bool wildcard;
		list<Component*>all_node;
		diskpath->getPathNode(path_list.front(), wildcard,all_node);
		front_wildcard = wildcard;
		if (wildcard == true) //��ͨ���
		{
			pair<wstring, wstring> folder_file = DiskPathDeal::getPathFolderAndFile(path_list.front());
			front_wildcards_str = StringDeal::getWildcardStr(folder_file.second);
		}
		else //û��ͨ���
		{
			all_node.clear();
			int type, size;
			Component *comp_node = diskpath->containNode(path_list.front(), size, type);
			if (type == TYPE_LINK_FOLDER)
				comp_node = DiskPathDeal::getLinkNode(comp_node), type = TYPE_FOLDER;
			else if (type == TYPE_LINK_FILE)
				comp_node = DiskPathDeal::getLinkNode(comp_node), type = TYPE_FILE;
			if (type == TYPE_FOLDER) //�ļ���
			{
				Folder * folder = static_cast<Folder*>(comp_node);
				for (auto it : folder->Cmap)
				{
					if (it.second->getType() == TYPE_FILE || it.second->getType() == TYPE_LINK_FILE)
						all_node.push_back(it.second);
				}
			}
			else if (type == TYPE_FILE) //�ļ�
			{
				all_node.push_back(comp_node);
			}
			else if (type == TYPE_NOT_HAVE)
			{
				wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
				return false;
			}
		}
		for (auto it : all_node)
		{
			File* file = static_cast<File*>(it);
			int wchar_size = 0;
			wchar_t *content_p = file->getContent(wchar_size);
			wchar_t *tmp_content_p = new wchar_t[wchar_size];
			wmemcpy(tmp_content_p, content_p, wchar_size);
			file_list.push_back(make_pair(make_pair(file->getName(), wchar_size), tmp_content_p));
		}
	}


	if (dst_true_disk) //��ʵ���̣�û���������
	{

	}
	else //�������
	{
		pair<wstring, wstring>folder_file = DiskPathDeal::getPathFolderAndFile(path_list.back());
		int size, type;
		Component* comp_node = diskpath->containNode(folder_file.first, size, type);
		if (type == TYPE_NOT_HAVE)
		{
			wcout << L"ϵͳ�Ҳ���ָ����·����" << endl;
			return false;
		}
		bool have_wildcards = StringDeal::isHaveWildcard(folder_file.second);
		if (have_wildcards) //����ͨ������ȸ�����
		{
			if (type != TYPE_FOLDER) //�����ļ���
			{
				wcout << L"ϵͳ�Ҳ���ָ����·����" << endl;
				return false;
			}
			wstring back_wildcard_str = StringDeal::getWildcardStr(folder_file.second);
			bool all_flag = have_arg;
			for (auto& it : file_list)
			{
				StringDeal::strReplace(it.first.first, front_wildcards_str, back_wildcard_str);
				Folder* folder = static_cast<Folder*>(comp_node);
				wstring str;
				if (folder->getNodeByName(it.first.first) && !all_flag)    //���<���֣���С>���׵�ַ
				{
					do
					{

						wcout << L"���� " << it.first.first << L" �� (Yes/No/All): ";
						wcin >> str;
						transform(str.begin(), str.end(), str.begin(), ::tolower);
					} while (!(str == L"yes" || str == L"no" || str == L"all"));
					if (str == L"no") continue;
					if (str == L"all") all_flag = true;
				}
				if (str == L"yes" || str == L"all")
				{
					File* file = static_cast<File*>(folder->getNodeByName(it.first.first));
					file->setContent(it.second, it.first.second);
				}
				else
				{
					folder->Add(it.first.first, TYPE_FILE);
					File* file = static_cast<File*>(folder->getNodeByName(it.first.first));
					file->setContent(it.second, it.first.second);
				}
			}

		}
		else //����ͨ���
		{
			Component * comp_node = diskpath->containNode(path_list.back(), size, type);
			if (type == TYPE_LINK_FILE) //���������ļ�
			{
				comp_node = DiskPathDeal::getLinkNode(comp_node);
				type = TYPE_FILE;
			}
			if (type == TYPE_LINK_FOLDER) //���������ļ���
			{
				comp_node = DiskPathDeal::getLinkNode(comp_node);
				type = TYPE_FOLDER;
			}
			if (type == TYPE_FOLDER) //�ļ��� list<pair<pair<wstring, int>, char*> >file_list;//���<���֣���С>���׵�ַ
			{
				Folder* folder = static_cast<Folder*>(comp_node);
				bool flag = false;
				for (auto it : file_list)
				{

					if (folder->getNodeByName(it.first.first) != nullptr)
					{
						wstring str;
						if (flag == false)
						{
							do
							{
								wcout << L"���� " << it.first.first << L" �� (Yes/No/All): ";
								wcin >> str;
								transform(str.begin(), str.end(), str.begin(), ::tolower);
							} while (!(str == L"yes" || str == L"no" || str == L"all"));
						}
						if (str == L"all") flag = true;
						if (flag || str == L"yes" || str == L"all")
						{

							folder->Remove(it.first.first);
							folder->Add(it.first.first, TYPE_FILE);//����ļ�
							File* new_file_node = static_cast<File*>(folder->getNodeByName(it.first.first));
							new_file_node->setContentAgain(it.second, it.first.second);
						}
						else if (str == L"no")
						{
							continue;
						}
					}
					else
					{
						folder->Add(it.first.first, TYPE_FILE);//����ļ�
						File* new_file_node = static_cast<File*>(folder->getNodeByName(it.first.first));
						new_file_node->setContent(it.second, it.first.second);
					}
				}
			}
			else if (type == TYPE_FILE) //������ļ� //��Ӧcomp
			{
				File* file = static_cast<File*>(comp_node);
				wstring str;
				do
				{
					wcout << L"���� " << file->getName() << L" �� (Yes/No/All): ";
					wcin >> str;
					transform(str.begin(), str.end(), str.begin(), ::tolower);
				} while (!(str == L"yes" || str == L"no" || str == L"all"));
				if (str == L"yes" || str == L"all")
				{
					for (auto it : file_list) //list<pair<pair<wstring, int>, char*> >file_list;//���<���֣���С>���׵�ַ
					{
						file->setContentAgain(it.second, it.first.second);
					}
				}
			}
			else //�ж���Ŀ¼�Ƿ���ڣ����ڴ������ļ�
			{
				int type, size;
				Component* tmp_com_node = diskpath->containNode(folder_file.first, size, type);
				if (type != TYPE_FOLDER)
				{
					wcout << L"ϵͳ�Ҳ���ָ����·����" << endl;
					return false;
				}
				Folder* folder = static_cast<Folder*>(tmp_com_node);
				folder->Add(folder_file.second, TYPE_FILE);
				File* file = static_cast<File*>(folder->getNodeByName(folder_file.second));
				wstring str;
				for (auto it : file_list) //list<pair<pair<wstring, int>, char*> >file_list;//���<���֣���С>���׵�ַ
				{
					file->setContentAgain(it.second, it.first.second);
				}
			}
		}

	}
	for (auto it : file_list)
	{
		delete[]it.second;
	}
	return true;
}

CopyCommand::~CopyCommand()
{
}

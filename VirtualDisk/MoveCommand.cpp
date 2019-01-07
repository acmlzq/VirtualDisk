#include "MoveCommand.h"



MoveCommand::MoveCommand()
{
}

bool MoveCommand::executeCmd(DiskPath* diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
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

	if (path_list.size() != 2)
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}
	bool src_wildcard;
	list<Component*> src_comp_list;
	diskpath->getPathNode(path_list.front(), src_wildcard,src_comp_list);

	if (src_wildcard == true) //�������ͨ���
	{
		if (src_comp_list.size() == 0)
		{
			wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
			return false;
		}
	}
	else //�������ͨ���
	{
		if (src_comp_list.size() == 0)
		{
			wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
			return false;
		}
	}


	int dst_size, dst_type;
	Component* dst_comp = diskpath->containNode(path_list.back(), dst_size, dst_type);
	if (dst_type == TYPE_NOT_HAVE) //�����ļ���
	{
		pair<wstring, wstring>folder_and_file = DiskPathDeal::getPathFolderAndFile(path_list.back());
		int tmp_size, tmp_type;
		Component* tmp_comp = diskpath->containNode(folder_and_file.first, tmp_size, tmp_type);
		if (tmp_type != TYPE_FILE)
		{
			wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
		}
	}
	else if (dst_type == TYPE_FOLDER) //���ļ���dst_comp
	{
		bool do_not_ask = have_arg ? true : false;
		for (auto it : src_comp_list) //��Ӧ�ڵ�
		{
			Folder* folder_dst_comp = static_cast<Folder*>(dst_comp);
			if (folder_dst_comp->getNodeByName(it->getName())) //������ֳ�ͻ
			{
				wstring input;
				if (!do_not_ask)
				{
					do
					{
						wcout << L"���� " << it->getName() << L" �� (Yes/No/All):";
						wcin >> input;
					} while (!(input == L"yes" || input == L"no" || input == L"all"));
					if (input == L"all")
						do_not_ask = true;
				}
				if (input == L"no") continue;
				Component* comp = folder_dst_comp->getNodeByName(it->getName());
				if (comp->getType() == TYPE_FOLDER)//�ļ��У����ܽ��ļ��и����ļ��У�Ҳ���ܽ��ļ������ļ���
				{
					wcout << L"�ܾ����ʡ�" << endl;
					continue;
				}
			}
			Folder* father_folder = static_cast<Folder*>(it->getFatherNode());
			father_folder->RemoveNotDelete(it->getName());
			if (folder_dst_comp->getNodeByName(it->getName())!=nullptr) //���������
			{
				folder_dst_comp->Remove(it->getName());
			}
			folder_dst_comp->AddNotCreat(it);

		}
	}
	else if (dst_type == TYPE_FILE) //���ļ�
	{
		if (src_comp_list.size() != 1)
		{
			wcout << L"�ܾ����ʡ�" << endl;
			return false;
		}
		if (src_comp_list.front()->getType() != TYPE_FILE) //�������ļ�
		{
			wcout << L"�ܾ����ʡ�" << endl;
			return false;
		}
		Component* src_comp = src_comp_list.front();
		wstring input;
		bool do_not_ask = have_arg ? true : false;
		if (!do_not_ask)
		{
			do
			{
				wcout << L"���� " << dst_comp->getName() << L" �� (Yes/No/All):";
				wcin >> input;
			} while (!(input == L"yes" || input == L"no" || input == L"all"));
			if (input == L"all")
				do_not_ask = true;
		}
		if (input == L"no") return true;
		File* dst_file = static_cast<File*>(dst_comp);
		File* src_file = static_cast<File*>(src_comp);
		int dst_size;
		wchar_t* dst_content = src_file->getContent(dst_size);
		dst_file->setContent(dst_content, dst_size);
		Folder* father_src_file = static_cast<Folder*>(src_file->getFatherNode());
		father_src_file->Remove(src_file->getName());
	}
	return true;
}

MoveCommand::~MoveCommand()
{
}

#include "MklinkCommand.h"



MklinkCommand::MklinkCommand()
{

}

bool MklinkCommand::executeCmd(DiskPath*diskdeal, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	for (auto it : arg_list)
	{
		if (it == L"/d")
			have_arg = true;
		else
		{
			wcout << L"�����﷨����ȷ��" << endl;
			return false;
		}
	}
	if (path_list.size() != 2) //���жϸ�ʽ
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}

	int size, type, link_type;

	Component *comp_node = diskdeal->containNode(path_list.back(), size, link_type);
	if (size == -1) //�ж�Ŀ������·���Ƿ����(����·���������)
	{
		wcout << L"�ܾ����ʡ�" << endl;
		return false;
	}

	if (!DiskPathDeal::pathIsLegit(path_list.front())) //�ж�mklink�����ļ������Ƿ�Ϸ����ļ����Ʊ���Ϸ���
	{
		wcout << L"�ļ�����Ŀ¼�������﷨����ȷ��" << endl;
		return false;
	}

	auto complete_path = DiskPathDeal::getPathFolderAndFile(path_list.front());

	comp_node = diskdeal->containNode(complete_path.first, size, type);
	if (size == -1) //û���ҵ�
	{
		wcout << L"ϵͳ�Ҳ���ָ����·����" << endl;
		return false;
	}

	Folder* folder = static_cast<Folder*>(comp_node);
	if (folder->getNodeByName(complete_path.second) != nullptr) //�������
	{
		wcout << L"���ļ��Ѵ���ʱ���޷��������ļ���" << endl;
		return false;
	}
	if (link_type == TYPE_FOLDER || link_type == TYPE_LINK_FOLDER) type = TYPE_LINK_FOLDER;
	if (link_type == TYPE_FILE || link_type == TYPE_LINK_FILE) type = TYPE_LINK_FILE;
	folder->Add(complete_path.second, type); //���������ļ�
	SymLink* symlink_node = static_cast<SymLink*>(folder->getNodeByName(complete_path.second));
	symlink_node->setLink(path_list.back());
	return true;
}

MklinkCommand::~MklinkCommand()
{
}

/*
##mklink��������

* ��ʽ��mklink [/d] srcPath symbolLinkPath

* ��������
0. ���жϸ�ʽ
1. �ж�����·���Ƿ����
2. �ж������ļ������Ƿ�Ϸ�
3. �ж������ļ������Ƿ�����������
4. ���������ļ�
*/

#include "DirCommand.h"



DirCommand::DirCommand()
{

}

void DirCommand::getCurPathNode(DiskPath*diskpath,list<Component*>& ans_list, const wstring& path, bool &have_wildscard, const bool &have_arg_s, const bool &have_arg_ad)
{
	list<Component*> path_list;
	diskpath->getPathNode(path, have_wildscard,path_list);
	for (list<Component*>::iterator it = path_list.begin(); it != path_list.end();it++)
	{
		if (!(have_wildscard&&!have_arg_s)) //�����ͨ����Ļ���û��/s������������ļ�����ת����������Ҫת��
		{
			if ((*it)->getType() == TYPE_LINK_FOLDER) //Ϊ���������ļ���
			{
				path_list.push_back(DiskPathDeal::getLinkNode(*it));
				continue;
			}
		}
		if (have_arg_ad || have_arg_s) //�������ad,s����,����ֻ��ӡ�ļ�����Ϣ����ô��ɾ�����е��ļ�
		{
			if ((*it)->getType() != TYPE_FILE)
			{
				ans_list.push_back(*it);
			}
		}
		else
		{
			ans_list.push_back(*it);
		}
	}
}

bool DirCommand::executeCmd(DiskPath* diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg_s = false, have_arg_ad = false;
	list<wstring> arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	for (auto it : arg_list)
	{
		if (it == L"/s")
		{
			have_arg_s = true;
		}
		else if (it == L"/ad")
		{
			have_arg_ad = true;
		}
		else
		{
			wcout << L"������ʽ����ȷ" << endl;
			return false;
		}
	}
	if (path_list.size() == 0)
		path_list.push_back(L"");
	for (auto list_it : path_list)
	{
		bool have_wildscard;
		list<Component*> path;
		getCurPathNode(diskpath, path, list_it, have_wildscard, have_arg_s, have_arg_ad);
		for (auto path_it : path)
		{
			if (have_arg_s) //�������s����
			{
				list<pair<wstring, Component*>> list_pair;
				DiskPathDeal::getNodeByRecur(path_it, list_pair);
				for (auto it : list_pair)
				{
					Folder* folder = static_cast<Folder*>(it.second);
					wcout << "\n " << it.first << L" ��Ŀ¼" << endl;
					folder->displayAll(!have_arg_ad);
				}
			}
			else
			{
				if (!have_wildscard && path_it->getType() == TYPE_FOLDER) //���û��ͨ����������ļ���   
				{
					(static_cast<Folder*>(path_it))->displayAll(!have_arg_ad);
				}
				else
					path_it->Display();
			}
		}
	}

	return true;
}

DirCommand::~DirCommand()
{

}
#include "CdCommand.h"



CdCommand::CdCommand()
{
}

bool CdCommand::executeCmd(DiskPath*diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list,arg_list,path_list);
	if (arg_list.size()!=0||path_list.size() == 0)
	{
		wcout << diskpath->getCurPath() << endl;
		return true;
	}
	if (path_list.size() != 1)
	{
		wcout << L"ϵͳ�Ҳ���ָ����·����" << endl;
		return false;
	}
	int size, type;
	diskpath->containNode(path_list.front(), size, type);
	if (type == TYPE_NOT_HAVE)
	{
		wcout << L"ϵͳ�Ҳ���ָ����·����" << endl;
		return false;
	}
	else if (type == TYPE_FILE)
	{
		wcout << L"Ŀ¼������Ч��" << endl;
		return false;
	}
	if (diskpath->updateCurPath(path_list.front()) == false)
	{
		wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
		return false;
	}
	return true;
}

CdCommand::~CdCommand()
{
}

/*
* ��������
0. ���жϸ�ʽ
1. �ж�Դ�ļ��Ƿ����
2. �л���Ŀ��·��
*/

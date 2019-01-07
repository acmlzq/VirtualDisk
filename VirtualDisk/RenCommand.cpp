#include "RenCommand.h"



RenCommand::RenCommand()
{

}

bool RenCommand::executeCmd(DiskPath*diskdeal, list<wstring>&arg_path_list, FileDisk* filedisk) //�����path_list�ǳ���������������ַ�������
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
	if (arg_list.size()!=0||path_list.size() != 2) //�жϸ�ʽ��·��������������Ϊ2��û�����������
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}
	int size, type;
	auto comp_node = diskdeal->containNode(path_list.front(), size, type);
	if (type == 0) //�ж�Դ�ļ��Ƿ����(Դ�ļ��������)
	{
		wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
		return false;
	}
	if (!DiskPathDeal::pathIsLegit(path_list.back())) //�ж��������������Ƿ�Ϸ�������Ϸ���
	{
		wcout << L"�ļ�����Ŀ¼�������﷨����ȷ��" << endl;
		return false;
	}
	Folder* father_node = static_cast<Folder*>(comp_node->getFatherNode());//�ж��������������Ƿ�����������
	if (father_node->getNodeByName(path_list.back()) != nullptr)
	{
		wcout << L"����һ�������ļ��������Ҳ����ļ���" << endl;
		return false;
	}
	wstring old_name = comp_node->getName(); //�����֣�ͬʱ���¸��ڵ�����ӽڵ����ֺͽڵ��ӳ���ϵ
	comp_node->setName(path_list.back());
	father_node->Cmap.erase(old_name);
	father_node->Cmap.insert(make_pair(path_list.back(), comp_node));
	return true;
}

RenCommand::~RenCommand()
{

}

/*

* ��ʽ��ren src dst

* ��������
0. ���жϸ�ʽ
1. �ж�Դ�ļ��Ƿ����
2. ���ж��������������Ƿ�Ϸ�
3. �ж��������������Ƿ�����������
4. �����֣�ͬʱ���¸��ڵ�Ķ����ӽڵ����ֺͽڵ��ӳ���ϵ

*/

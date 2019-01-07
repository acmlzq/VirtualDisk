#include "MdCommand.h"

MdCommand::MdCommand()
{

}

/*
���ж�·���Ƿ�Ϸ���
���ж�·���Ƿ����
Ȼ�󴴽�

��δ��ɹ��ܣ�
  �������ڿո���ļ���
*/
bool MdCommand::executeCmd(DiskPath* diskpath, list<wstring> &arg_path_list, FileDisk* filedisk)//�����list����Ϊ�����������·��
{
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list,arg_list,path_list);
	if (arg_list.size()!=0||path_list.size() == 0)
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}
	for (auto path : path_list)
	{
		
		if (!DiskPathDeal::pathIsLegit(path)) // ·�����Ϸ������·�����Ϸ���ʾ
		{
			wcout << L"�ļ�����Ŀ¼�������﷨����ȷ��" << endl;
			continue;
		}
		int type, size;
		diskpath->containNode(path, size, type); //�ж�·���Ƿ����
		if (type != TYPE_NOT_HAVE) //˵����Ŀ¼�Ѿ����ڣ����·���Ѿ�������ʾ
		{
			wcout << L"��·���Ѿ�����" << endl;
			continue;
		}

		Component * comp_node;//ͨ���ж��Ǿ���·���������·��������ȡĿ¼�ڵ�
		list<wstring>list_single_path;
		DiskPathDeal::getSplitPath(list_single_path,path);
		if (DiskPathDeal::isAbsPath(path)) //����·��
		{
			comp_node = diskpath->getRootNode();
			list_single_path.pop_front();
		}
		else
		{
			comp_node = diskpath->getCurNode();
		}
		for (auto it : list_single_path) //���β��ң������ھʹ���
		{
			Folder* folder = static_cast<Folder*>(comp_node);
			if (it == L".")
				continue;
			if (it == L"..")
			{
				if (comp_node->getFatherNode() == nullptr)
				{
					wcout << L"�ļ�����Ŀ¼�������﷨����ȷ��" << endl;
					break;
				}
				else
					comp_node = comp_node->getFatherNode();
				continue;
			}
			if (folder->getNodeByName(it) == nullptr) //���û�оʹ���
			{
				bool flag = false;
				folder->Add(it, TYPE_FOLDER);
				comp_node = folder->getNodeByName(it);
			}
			else  //�Ѿ����ڵ�
			{
				comp_node = folder->getNodeByName(it);
				if (comp_node->getType() == TYPE_LINK_FOLDER)
					comp_node = DiskPathDeal::getLinkNode(comp_node);
			}
		}
	}
	return true;
}


MdCommand::~MdCommand()
{

}

/*
##md��������

* ��ʽ��md path

* ��������
1. ���ж�·�������Ƿ�Ϸ�
2. ���ж�·���Ƿ����
3. ��ȡĿ¼�ڵ�
4. �����жϲ�����
*/

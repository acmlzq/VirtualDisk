#include "RdCommand.h"

RdCommand::RdCommand()
{
}

bool RdCommand::executeCmd(DiskPath* diskpath, list<wstring>&arg_path_list, FileDisk* filedisk)
{
	bool have_arg = false;
	list<wstring>arg_list, path_list;
	getArgAndPath(arg_path_list, arg_list, path_list);
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
	if (path_list.size() == 0)
	{
		wcout << L"�����﷨����ȷ��" << endl;
		return false;
	}
	int size, type;
	for (auto it : path_list)
	{
		Component *comp = diskpath->containNode(it, size, type);
		if (diskpath->nodeIsInPathList(comp)) //·��״̬�еĽڵ㲻��ɾ��
		{
			wcout << L"��һ����������ʹ�ô��ļ��������޷����ʡ�" << endl;
			continue;
		}
		if (type == 0)
		{
			wcout << L"ϵͳ�Ҳ���ָ�����ļ���" << endl;
		}
		else if (type == TYPE_FILE || type == TYPE_LINK_FILE)
		{
			wcout << L"Ŀ¼������Ч��" << endl;
		}
		else if (type == TYPE_FOLDER || type == TYPE_LINK_FOLDER)
		{
			if (have_arg == true) //�в���
			{
				wstring answer;
				do
				{
					wcout << it << L",�Ƿ�ȷ��<Y/N>? ";
					wcin >> answer;
				} while (!(answer.size() == TYPE_FOLDER && ((char)toupper(answer[0]) == 'Y' || (char)toupper(answer[0]) == 'N')));
				if ((char)toupper(answer[0]) == 'Y')
				{
					Folder* father_node = static_cast<Folder*>(comp->getFatherNode());
					father_node->Cmap.erase(comp->getName());
					delete comp;  //ɾ����ͬʱ���ͷ��ڴ�
				}

			}
			else if (have_arg == false)//�޲���
			{
				if (type == TYPE_LINK_FOLDER) //���������ļ���
				{
					Folder * father_node = static_cast<Folder*>(comp->getFatherNode());
					father_node->Cmap.erase(comp->getName());
					delete comp;  //ɾ����ͬʱ���ͷ��ڴ�
				}
				else //�ļ���
				{
					Folder * folder = (Folder*)comp;
					if (folder->Cmap.size() == TYPE_FILE)
					{
						Folder* father_node = static_cast<Folder*>(comp->getFatherNode());
						father_node->Cmap.erase(comp->getName());
						delete comp;  //ɾ����ͬʱ���ͷ��ڴ�
					}
					else
					{
						wcout << L"Ŀ¼���ǿյġ�" << endl;
					}
				}
			}
		}
	}

	return true;
}

RdCommand::~RdCommand()
{
}

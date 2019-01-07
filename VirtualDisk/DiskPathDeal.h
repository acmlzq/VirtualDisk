#pragma once
#include <Windows.h>
#include <io.h>
#include <fstream>
#include <algorithm>
#include "Component.h"
#include "File.h"
#include "Folder.h"
#include "SymLink.h"


//����·��������Ҫ��һЩ�ʹ���·����صķ���(�ʹ��̵�ǰ·��λ���޹�)��
class DiskPathDeal
{
private:
	static Component* getRootNode(Component*);
public:
	DiskPathDeal();
	static wstring getAbsPath(Component*); //�õ��ýڵ�ľ���·��
	static void recurGetNode(Component*, list<pair<wstring, Component*>>&); //�ݹ��ȡĿ¼������Ŀ¼ʵ��
	static void getNodeByRecur(Component*, list<pair<wstring, Component*>>&); //�ݹ��ȡĿ¼������Ŀ¼
	static pair<wstring, wstring> getPathFolderAndFile(const wstring&); //��ȡ·����Ӧ���ļ����ļ���
	static void getSplitPath(list<wstring>&,const wstring &path, char split_char = '\\');//��·����ֳɶ��������	
	static Component* getLinkNode(Component*); //��ȡ�������ӽڵ������·���ڵ�
	static bool pathIsLegit(const wstring &path); //�ж�·�������Ƿ�Ϸ�
	static bool isAbsPath(const wstring& path);
	~DiskPathDeal();
};


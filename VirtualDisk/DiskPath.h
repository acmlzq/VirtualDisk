#pragma once
#include "File.h"
#include "Folder.h"
#include "SymLink.h"
#include "DiskPathDeal.h"
#include "Component.h"

//��¼���̸��ڵ㵽��ǰ�ڵ��·����ͬʱ����һЩ�ʹ�·��λ����صķ���
class DiskPath
{
private:
	list<pair<wstring, Component *> > path_list;
public:
	Component* containNode(const wstring&, int &, int &); //�ж�·���Ƿ����
	wstring getCurPath(); //��ȡ��ǰ·��
	void getPathNode(const wstring&, bool&, list<Component*>&); //��ȡ·���ڵ㣨���ܰ���*���������������ս�������list��
	void setListHead(Component*, const wstring&);//���øýڵ�Ϊ����ͷ��
	bool updateCurPath(const wstring&);//���µ�ǰ·��
	Component* getRootNode(); //��ȡ���ڵ�
	Component* getCurNode(); //��ȡ��ǰĿ¼�ڵ�
	Component * getCurFather(Component*); //��ȡ��ǰ�ڵ㸸�ڵ�
	void ClearPathList(); //���·������ʽ�����̵�ʱ����Ҫ�õ���
	bool nodeIsInPathList(Component*); //�жϽڵ��Ƿ���·����
	DiskPath();
	~DiskPath();
};

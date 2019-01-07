#pragma once
#include <cstdio>
#include <algorithm>
#include "DiskPath.h"
#include "FileDisk.h"
#include "Factory.h"
#include "Command.h"
#include "TruePathDeal.h"
//��������࣬���������нӿڵ�ʵ��
class VirtualDisk
{
private:
	Factory * factory;
	FileDisk* filedisk;
	DiskPath* diskpath;
public:
	VirtualDisk();
	bool containNode(string, int&, int&);//�ж�·���Ƿ����
	bool executeCmd(string);//ִ������
	bool formatDisk();//��ʽ������
	string getCurPath();//��ȡ��ǰ·��
	string getLinkNode(string);//��ȡ���ӽڵ�����·��
	void getCmdStr();//�õ�ִ������
	virtual ~VirtualDisk();
};


#pragma once
#include "Component.h"
#include "Folder.h"

//�����࣬�������ڵ㣬��ʽ�����̡�
class FileDisk
{
public:
	Component* root;
public:
	FileDisk();
	void formatDisk();//��ʽ������
	Component* getRootNode();//��ȡ���ڵ�
	void setRootNode(Component*);//���ø��ڵ�
	~FileDisk();
};


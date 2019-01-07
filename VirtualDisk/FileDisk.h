#pragma once
#include "Component.h"
#include "Folder.h"

//磁盘类，创建根节点，格式化磁盘。
class FileDisk
{
public:
	Component* root;
public:
	FileDisk();
	void formatDisk();//格式化磁盘
	Component* getRootNode();//获取根节点
	void setRootNode(Component*);//设置根节点
	~FileDisk();
};


#pragma once
#include <cstdio>
#include <algorithm>
#include "DiskPath.h"
#include "FileDisk.h"
#include "Factory.h"
#include "Command.h"
#include "TruePathDeal.h"
//虚拟磁盘类，满足需求中接口的实现
class VirtualDisk
{
private:
	Factory * factory;
	FileDisk* filedisk;
	DiskPath* diskpath;
public:
	VirtualDisk();
	bool containNode(string, int&, int&);//判断路径是否存在
	bool executeCmd(string);//执行命令
	bool formatDisk();//格式化磁盘
	string getCurPath();//获取当前路径
	string getLinkNode(string);//获取链接节点链接路径
	void getCmdStr();//得到执行命令
	virtual ~VirtualDisk();
};


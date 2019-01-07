#pragma once
#include "File.h"
#include "Folder.h"
#include "SymLink.h"
#include "DiskPathDeal.h"
#include "Component.h"

//记录磁盘根节点到当前节点的路径，同时处理一些和此路径位置相关的方法
class DiskPath
{
private:
	list<pair<wstring, Component *> > path_list;
public:
	Component* containNode(const wstring&, int &, int &); //判断路径是否存在
	wstring getCurPath(); //获取当前路径
	void getPathNode(const wstring&, bool&, list<Component*>&); //获取路径节点（可能包含*？操作符），最终结果存放在list中
	void setListHead(Component*, const wstring&);//设置该节点为链表头部
	bool updateCurPath(const wstring&);//更新当前路径
	Component* getRootNode(); //获取根节点
	Component* getCurNode(); //获取当前目录节点
	Component * getCurFather(Component*); //获取当前节点父节点
	void ClearPathList(); //清空路径（格式化磁盘的时候需要用到）
	bool nodeIsInPathList(Component*); //判断节点是否在路径中
	DiskPath();
	~DiskPath();
};

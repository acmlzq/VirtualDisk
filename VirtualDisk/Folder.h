#pragma once
#include "Component.h"
#include "File.h"
#include "SymLink.h"
#include<map>
#include<string>

//目录（文件夹）类，主要涉及文件夹添加、移除，查找文件，以及展示当前目录下列表信息
class Folder : public Component
{
public:
	map<wstring, Component*>Cmap;
public:
	Folder(Component *, const wstring&, int);
	void Add(const wstring&, int); //如果需要Add，记得先判断该文件名是否已经存在，这个函数不做判断
	void AddNotCreat(Component* comp); //只是添加进去，不创建
	void displayAll(bool);//展示当前目录信息
	Component* getNodeByName(const wstring&);//通过名字获取节点
	void Remove(const wstring&);//删除
	void RemoveNotDelete(const wstring&);//只是去掉，不删除
	~Folder();
};


#pragma once
#include <Windows.h>
#include <io.h>
#include <fstream>
#include <algorithm>
#include "Component.h"
#include "File.h"
#include "Folder.h"
#include "SymLink.h"


//磁盘路径处理，主要是一些和磁盘路径相关的方法(和磁盘当前路径位置无关)，
class DiskPathDeal
{
private:
	static Component* getRootNode(Component*);
public:
	DiskPathDeal();
	static wstring getAbsPath(Component*); //得到该节点的绝对路径
	static void recurGetNode(Component*, list<pair<wstring, Component*>>&); //递归获取目录所有子目录实现
	static void getNodeByRecur(Component*, list<pair<wstring, Component*>>&); //递归获取目录所有子目录
	static pair<wstring, wstring> getPathFolderAndFile(const wstring&); //获取路径对应的文件和文件夹
	static void getSplitPath(list<wstring>&,const wstring &path, char split_char = '\\');//将路径拆分成多个独立的	
	static Component* getLinkNode(Component*); //获取符号链接节点的链接路径节点
	static bool pathIsLegit(const wstring &path); //判断路径名称是否合法
	static bool isAbsPath(const wstring& path);
	~DiskPathDeal();
};


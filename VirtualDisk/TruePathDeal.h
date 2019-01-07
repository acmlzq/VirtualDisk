#pragma once
#include <windows.h>
#include <io.h>
#include <fstream>
#include <atlbase.h>
#include <atlconv.h>
#include "FileDisk.h"
#include "File.h"
#include "SymLink.h"

//对于真实磁盘中的操作进行处理
class TruePathDeal
{
private:
	static void writeIntToTrueDisk(const wstring&, int); //向真实磁盘中写一个int
	static void writeStringToTrueDisk(const wstring&, const wstring&);//向真实磁盘中写字符串
	static void writeCharToTrueDisk(const wstring&, int, const wchar_t*);//向真实磁盘中写字节流
	static Component* readComponent(ifstream&);//从真实磁盘中读取一个Component部件
	static int readInt(ifstream&); //从真实磁盘中读取一个Int
	static wstring readwstring(ifstream&);//从真实磁盘中读取一个字符串
	static wchar_t* readFile(int&, ifstream&);//从真实磁盘文件中读取数据
public:
	TruePathDeal();
	static void writeDataToTrueDisk(const wstring&, Component*);//写数据到真实磁盘
	static void readDataFromTrueDisk(const wstring&, FileDisk*);//从真实磁盘读数据
	static bool isTrueDiskPath(wstring&); //判断是不是真实磁盘路径，并去除掉@字符
	static void getTrueDiskFile(list<pair<pair<wstring, int>, wchar_t*> >&,wstring &); //通过路径获取其名称，大小，数据流
	static void getTrueDiskFiles(const wstring&, vector<wstring>&);//获取真实磁盘文件
	static void getTrueDiskFileInfo(pair<pair<wstring, int>, wchar_t*>&,const wstring &path); //由真实磁盘路径得到真实磁盘文件的名称，大小，byte指针
	static bool containTrueDiskNode(wstring path, int&size, int&type); //对应VirtualDisk中containNode判断磁盘节点是否存在及类型
	~TruePathDeal();
};


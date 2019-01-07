#pragma once
#include<string>
#include<iostream>
#include"windows.h"
#include<list>
#include<vector>
using namespace std;

//用于处理和路径无关的字符串
class StringDeal
{
public:
	StringDeal();
	static wstring intToString(int);//将int输出转换成字符串，并且每三位中间逗号隔开
	static wstring getWildcardStr(const wstring &); //获取通配符前几个字符串
	static bool matches(const wchar_t*, const wchar_t*); //判断文件名称是否符合通配符
	static void strReplace(wstring&, const wstring&, const wstring&);//置换名字，比如 copy 1* 11*，要将文件1开头的部分改成11
	static bool isHaveWildcard(const wstring&); //是否含有通配符
	static void getInput(wstring &, list<wstring>&); //初始化输入字符串，对双引号做转换
	static int getCmdArgSize(const list<wstring>&); //获取命令参数个数
	static wstring dealMultiByteToWideChar(const char *,int); //将多字节转换成宽字节
	static string dealWideCharToMultiByte(const wchar_t*,int);//将宽字节转换成多字节
	static char* dealsWideCharToMultiByte(const wchar_t *wstr, int,int&);//返回值为char*类型
	static wchar_t* dealsMultiByteToWideChar(const char* wstr, int &len);   // 返回值为wchar_t*类型
	~StringDeal();
};



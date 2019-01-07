#pragma once
#include <string>
#include <ctime>
#include <windows.h>
using namespace std;

//时间类，用于获取时间
class MyTime
{
public:
	MyTime();
	wstring getNowTime();//获取当前时间
	~MyTime();
};

#pragma once
#include <string>
#include <ctime>
#include <windows.h>
using namespace std;

//ʱ���࣬���ڻ�ȡʱ��
class MyTime
{
public:
	MyTime();
	wstring getNowTime();//��ȡ��ǰʱ��
	~MyTime();
};

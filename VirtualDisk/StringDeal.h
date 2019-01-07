#pragma once
#include<string>
#include<iostream>
#include"windows.h"
#include<list>
#include<vector>
using namespace std;

//���ڴ����·���޹ص��ַ���
class StringDeal
{
public:
	StringDeal();
	static wstring intToString(int);//��int���ת�����ַ���������ÿ��λ�м䶺�Ÿ���
	static wstring getWildcardStr(const wstring &); //��ȡͨ���ǰ�����ַ���
	static bool matches(const wchar_t*, const wchar_t*); //�ж��ļ������Ƿ����ͨ���
	static void strReplace(wstring&, const wstring&, const wstring&);//�û����֣����� copy 1* 11*��Ҫ���ļ�1��ͷ�Ĳ��ָĳ�11
	static bool isHaveWildcard(const wstring&); //�Ƿ���ͨ���
	static void getInput(wstring &, list<wstring>&); //��ʼ�������ַ�������˫������ת��
	static int getCmdArgSize(const list<wstring>&); //��ȡ�����������
	static wstring dealMultiByteToWideChar(const char *,int); //�����ֽ�ת���ɿ��ֽ�
	static string dealWideCharToMultiByte(const wchar_t*,int);//�����ֽ�ת���ɶ��ֽ�
	static char* dealsWideCharToMultiByte(const wchar_t *wstr, int,int&);//����ֵΪchar*����
	static wchar_t* dealsMultiByteToWideChar(const char* wstr, int &len);   // ����ֵΪwchar_t*����
	~StringDeal();
};



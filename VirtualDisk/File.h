#pragma once
#include "Component.h"

//�ļ��࣬��Ҫ�������ļ����á���ȡ����������
class File :public Component
{
private:
	wchar_t *content;
public:
	File(Component*, const wstring&, const int);
	bool setContent(const wchar_t *, const int);//������
	bool setContentAgain(const wchar_t*, const int);//�ٴζ�����ԭ�л������������
	wchar_t * getContent(int&) const;//��Ҫ��ȡ��С����
	~File();
};


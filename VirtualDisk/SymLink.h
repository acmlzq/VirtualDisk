#pragma once
#include "Component.h"
#include "Folder.h"
#include "DiskPath.h"

//���������ļ����У��࣬��Ҫ�漰��ȡ���ӣ���������
class SymLink : public Component
{
private:
	wstring link_path;
public:
	SymLink(Component *, const wstring&, int);
	const wstring& getLinkPath()const;//��ȡ����·��
	void setLink(const wstring&);//��������·��
	void Display() override; //��ӡ��Ϣ
	~SymLink();
};


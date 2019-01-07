#pragma once
#include "Component.h"
#include "File.h"
#include "SymLink.h"
#include<map>
#include<string>

//Ŀ¼���ļ��У��࣬��Ҫ�漰�ļ�����ӡ��Ƴ��������ļ����Լ�չʾ��ǰĿ¼���б���Ϣ
class Folder : public Component
{
public:
	map<wstring, Component*>Cmap;
public:
	Folder(Component *, const wstring&, int);
	void Add(const wstring&, int); //�����ҪAdd���ǵ����жϸ��ļ����Ƿ��Ѿ����ڣ�������������ж�
	void AddNotCreat(Component* comp); //ֻ����ӽ�ȥ��������
	void displayAll(bool);//չʾ��ǰĿ¼��Ϣ
	Component* getNodeByName(const wstring&);//ͨ�����ֻ�ȡ�ڵ�
	void Remove(const wstring&);//ɾ��
	void RemoveNotDelete(const wstring&);//ֻ��ȥ������ɾ��
	~Folder();
};


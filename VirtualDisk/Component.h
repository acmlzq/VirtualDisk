#pragma once

#include<iostream>
#include<iomanip>
#include<cstring>
#include<string>
#include<vector>
#include<list>
#include<map>
#include<stack>
#include<queue>
#include "MyTime.h"
#include "StringDeal.h"

#define TYPE_NOT_HAVE 0
#define TYPE_FOLDER 1
#define TYPE_FILE 2
#define TYPE_LINK_FOLDER -3
#define TYPE_LINK_FILE 3



using namespace std;
/*
  �����࣬�����ļ����ļ��У����������ļ��Ĺ������֡�
  ��Ҫ���ԣ����֣����ڣ���С�����ͣ����ڵ�
  ��Ҫ������չʾ�ļ���Ӧ��Ϣ
 */
class Component
{
private:
	MyTime now_time;
	wstring name;
	wstring date;
	int size;
	int type;
	Component *father_node;
public:
	Component(Component*, const wstring&, int);
	const wstring& getName() const;
	const wstring& getDate() const;
	wstring getTypeName() const;
	const wstring& getTime() const;
	int getType() const;
	int getSize() const;
	Component* getFatherNode() const;
	void setType(int);
	void setSize(int);
	void setName(const wstring&);
	void setTime(const wstring&);
	void setFatherNode(Component*);
	virtual void Display();
	virtual ~Component();
};


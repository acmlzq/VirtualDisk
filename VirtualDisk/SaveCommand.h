#pragma once
#include <cmath>
#include <cstdio>
#include <direct.h>
#include "Command.h"
#include "TruePathDeal.h"
#include "StringDeal.h"

//�������������Ϣ����ʵ���̵�����   ��ʽ��save dstPath
class SaveCommand :	public Command
{
public:
	SaveCommand();
	bool executeCmd(DiskPath* diskpath,list<wstring>&, FileDisk*) override;
	~SaveCommand();
};


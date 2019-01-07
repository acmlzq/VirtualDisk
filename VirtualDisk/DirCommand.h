#pragma once
#include "Command.h"

//��ӡ·���ļ�/�ļ�����Ϣ  ��ʽ�� /s /ad [path]...
class DirCommand :public Command
{
private:
	void getCurPathNode(DiskPath*, list<Component*>&,const wstring&,bool&,const bool&,const bool&);
public:
	DirCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~DirCommand();
};


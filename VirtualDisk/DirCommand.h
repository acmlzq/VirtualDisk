#pragma once
#include "Command.h"

//打印路径文件/文件夹信息  格式： /s /ad [path]...
class DirCommand :public Command
{
private:
	void getCurPathNode(DiskPath*, list<Component*>&,const wstring&,bool&,const bool&,const bool&);
public:
	DirCommand();
	bool executeCmd(DiskPath*, list<wstring>&, FileDisk*) override;
	~DirCommand();
};


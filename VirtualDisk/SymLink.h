#pragma once
#include "Component.h"
#include "Folder.h"
#include "DiskPath.h"

//符号链接文件（夹）类，主要涉及获取链接，设置链接
class SymLink : public Component
{
private:
	wstring link_path;
public:
	SymLink(Component *, const wstring&, int);
	const wstring& getLinkPath()const;//获取链接路径
	void setLink(const wstring&);//设置链接路径
	void Display() override; //打印信息
	~SymLink();
};


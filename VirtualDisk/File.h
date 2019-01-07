#pragma once
#include "Component.h"

//文件类，主要包含了文件设置、获取数据流方法
class File :public Component
{
private:
	wchar_t *content;
public:
	File(Component*, const wstring&, const int);
	bool setContent(const wchar_t *, const int);//读数据
	bool setContentAgain(const wchar_t*, const int);//再次读，在原有基础上添加数据
	wchar_t * getContent(int&) const;//需要获取大小问题
	~File();
};


#include "File.h"

File::File(Component* _father_node, const wstring &_name, int _type) :Component(_father_node, _name, _type)
{
	setSize(0);
	content = nullptr;
}

bool File::setContent(const wchar_t * ptr, int wchar_size) //先判断content是否为空，不为空则先释放原有内存，再申请新内存
{
	if (nullptr != content)
	{
		delete []content;
	}
	setSize(wchar_size);
	content = new wchar_t[wchar_size];
	wmemcpy(content, ptr, wchar_size);
	return true;
}

bool File::setContentAgain(const wchar_t * ptr, int wchar_size) //在原有的基础上追加
{
	wchar_t *tmp_content = new wchar_t[getSize() + wchar_size];
	wmemcpy(tmp_content, content, getSize());
	wmemcpy(tmp_content + getSize(), ptr, wchar_size);
	if (content != nullptr)
	{
		delete[]content;
	}
	content = nullptr;
	content = tmp_content;
	setSize(getSize() + wchar_size);
	return true;
}

wchar_t * File::getContent(int &wchar_size) const //引用传参，获取大小
{
	wchar_size = getSize();
	return content;
}

File::~File()
{
	if (nullptr != content)
	{
		delete []content;
	}
}


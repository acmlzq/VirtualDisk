#include "File.h"

File::File(Component* _father_node, const wstring &_name, int _type) :Component(_father_node, _name, _type)
{
	setSize(0);
	content = nullptr;
}

bool File::setContent(const wchar_t * ptr, int wchar_size) //���ж�content�Ƿ�Ϊ�գ���Ϊ�������ͷ�ԭ���ڴ棬���������ڴ�
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

bool File::setContentAgain(const wchar_t * ptr, int wchar_size) //��ԭ�еĻ�����׷��
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

wchar_t * File::getContent(int &wchar_size) const //���ô��Σ���ȡ��С
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


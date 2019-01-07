#include "SymLink.h"


SymLink::SymLink(Component* _father_node, const wstring &_name, int _type) :Component(_father_node, _name, _type)
{

}

void SymLink::Display()
{
	wcout << setiosflags(ios::left) << getDate() << L"    "<<left << setw(15) << getTypeName() << getName() << L" [" << link_path << L"]" << endl;
}

const wstring& SymLink::getLinkPath() const //�����·�����߾���·����õ���ڵ㣬��DiskDeal��ContainNodeʵ��
{
	return link_path;
}

void SymLink::setLink(const wstring& new_path)
{
	link_path = new_path;
}

SymLink::~SymLink()
{

}

#include "SymLink.h"


SymLink::SymLink(Component* _father_node, const wstring &_name, int _type) :Component(_father_node, _name, _type)
{

}

void SymLink::Display()
{
	wcout << setiosflags(ios::left) << getDate() << L"    "<<left << setw(15) << getTypeName() << getName() << L" [" << link_path << L"]" << endl;
}

const wstring& SymLink::getLinkPath() const //由相对路径或者绝对路径获得到其节点，由DiskDeal的ContainNode实现
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

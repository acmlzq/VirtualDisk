#include "Component.h"

wstring Component::getTypeName() const
{
	if (type == TYPE_FOLDER) return L"<DIR>";
	else if (type == TYPE_FILE) return L"";
	else if (type == TYPE_LINK_FILE) return L"<SYMLINK>";
	else if (type == -TYPE_LINK_FILE) return L"<SYMLINKD>";
	else return L"<ERROR>";
}
Component::Component(Component* _father_node, const wstring &_name, int _type)
{
	father_node = _father_node;
	date = now_time.getNowTime();
	name = _name;
	size = 0;
	type = _type;
}


const wstring& Component::getName() const
{
	return name;
}

const wstring& Component::getDate() const
{
	return date;
}
int Component::getType() const
{
	return type;
}
int Component::getSize() const
{
	return size;
}
const wstring& Component::getTime() const
{
	return date;
}

void Component::setTime(const wstring& new_time)
{
	date = new_time;
}

void Component::Display()
{
	if (type == TYPE_FILE) //文件
		wcout << date << L"    " << right << setw(14) << StringDeal::intToString(size) << L" " << getName() << endl;
	else //文件夹
		wcout << date << L"    " << left << setw(15) << getTypeName() << getName() << endl;
}

void Component::setSize(int new_size)
{
	size = new_size;
}
void Component::setType(int new_type)
{
	type = new_type;
}
void Component::setName(const wstring &new_name)
{
	name = new_name;
}

Component* Component::getFatherNode() const
{
	return father_node;
}

void Component::setFatherNode(Component*comp)
{
	father_node = comp;
}

Component::~Component()
{

}

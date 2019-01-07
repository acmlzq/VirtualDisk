#include "VirtualDisk.h"


VirtualDisk::VirtualDisk()
{
	filedisk = new FileDisk();
	diskpath = new DiskPath();
	factory = new Factory();
	diskpath->setListHead(filedisk->getRootNode(), filedisk->getRootNode()->getName());
}

bool VirtualDisk::containNode(string path, int&size, int&type)
{
	wstring w_path = StringDeal::dealMultiByteToWideChar(path.c_str(), path.size());
	if (TruePathDeal::isTrueDiskPath(w_path))
	{
		return TruePathDeal::containTrueDiskNode(w_path, size, type);
	}
	else
	{
		if (nullptr == diskpath->containNode(w_path, size, type))
		{
			return false;
		}
		else
		{
			type = abs(type);
			return true;
		}
	}
}

bool VirtualDisk::executeCmd(string cmd_str)
{
	wcin.imbue(locale(locale(), "", LC_CTYPE)); // 1
	wcout.imbue(locale(locale(), "", LC_CTYPE)); // 1
	wstring w_cmd_str = StringDeal::dealMultiByteToWideChar(cmd_str.c_str(), cmd_str.size());
	Folder* folder = static_cast<Folder*>(diskpath->getCurNode());
	list<wstring>split_str;
	StringDeal::getInput(w_cmd_str,split_str); //得到空格隔开的字符串列表
	wstring front_str = split_str.front();
	if (split_str.size() != 0) split_str.pop_front();
	transform(front_str.begin(), front_str.end(), front_str.begin(), ::tolower);//将cmd命令转换为小写
	Command* command = nullptr;
	command=factory->CreateCommand(front_str);
	if (command != nullptr)
	{
		command->executeCmd(diskpath, split_str, filedisk);
		delete command;
	}
	return true;
}

bool VirtualDisk::formatDisk()
{
	diskpath->ClearPathList();
	filedisk->formatDisk();
	diskpath->setListHead(filedisk->getRootNode(), filedisk->getRootNode()->getName());
	return true;
}

string VirtualDisk::getCurPath()
{
	wstring w_cur_path = diskpath->getCurPath();
	string cur_path = StringDeal::dealWideCharToMultiByte(w_cur_path.c_str(), w_cur_path.size());
	return cur_path;
}

string VirtualDisk::getLinkNode(string node_path)
{
	int size, type;
	wstring w_node_path = StringDeal::dealMultiByteToWideChar(node_path.c_str(),node_path.size());
	Component *symlink = diskpath->containNode(w_node_path, size, type);
	if (symlink->getType() == TYPE_LINK_FILE || symlink->getType() == TYPE_LINK_FOLDER) //如果是符号链接类型的话
	{
		symlink = DiskPathDeal::getLinkNode(static_cast<SymLink*>(symlink));
		if (symlink != nullptr)
		{
			wstring w_abs_path = DiskPathDeal::getAbsPath(symlink);
			string abs_path = StringDeal::dealWideCharToMultiByte(w_abs_path.c_str(), w_abs_path.size());
			return abs_path;
		}
		else
			return "";
	}
	return "";
}

void VirtualDisk::getCmdStr()
{
	cout << getCurPath() << ">";
	string input_str;
	while (getline(cin, input_str))
	{
		if (input_str.size() != 0)
		{
			if (input_str == "formatDisk")
			{
				formatDisk();
				cout << endl;
				cout << getCurPath() << ">";
				continue;
			}
			executeCmd(input_str);
		}
		cout << endl;
		cout << getCurPath() << ">";
	}
}

VirtualDisk::~VirtualDisk()
{
	delete filedisk;
	delete diskpath;
	delete factory;
}

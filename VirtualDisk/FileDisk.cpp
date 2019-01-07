#include "FileDisk.h"


FileDisk::FileDisk()
{
	root = new Folder(nullptr, L"C:", TYPE_FOLDER);
}

void FileDisk::formatDisk()
{
	if (nullptr != root)	delete root;
	root = new Folder(nullptr, L"C:", TYPE_FOLDER);
}

Component* FileDisk::getRootNode()
{
	return root;
}

void FileDisk::setRootNode(Component* _root)
{
	root = _root;
}

FileDisk::~FileDisk()
{
	delete root;
}

#include"File.h"
#include"Folder.h"
#include "SymLink.h"
#include "FileDisk.h"
#include "VirtualDisk.h"

int main()
{
	VirtualDisk * root = new VirtualDisk();
	root->getCmdStr();
	return 0;
}

#include "ClsCommand.h"



ClsCommand::ClsCommand()
{

}

bool ClsCommand::executeCmd(DiskPath*diskpath, list<wstring>&path_list, FileDisk* filedisk)
{
	system("cls");
	return true;
}

ClsCommand::~ClsCommand()
{
}

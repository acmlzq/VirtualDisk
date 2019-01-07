#include "Factory.h"

Factory::Factory()
{

}

Command* Factory::CreateCommand(const wstring &command_name) //这里command_name已经全部转换为了小写了
{
	if (command_name == L"md")
	{
		return new MdCommand();
	}
	else if (command_name == L"rd")
	{
		return new RdCommand();
	}
	else if (command_name == L"mklink")
	{
		return new MklinkCommand();
	}
	else if (command_name == L"dir")
	{
		return new DirCommand();
	}
	else if (command_name == L"ren")
	{
		return new RenCommand();
	}
	else if (command_name == L"del")
	{
		return new DelCommand();
	}
	else if (command_name == L"cd")
	{
		return new CdCommand();
	}
	else if (command_name == L"cls")
	{
		return new ClsCommand();
	}
	else if (command_name == L"copy")
	{
		return new CopyCommand();
	}
	else if (command_name == L"save")
	{
		return new SaveCommand();
	}
	else if (command_name == L"load")
	{
		return new LoadCommand();
	}
	else if (command_name == L"move")
	{
		return new MoveCommand();
	}
	return nullptr;
}

Factory::~Factory()
{
}

#include "Command.h"



Command::Command()
{

}

void Command::getArgAndPath(list<wstring>&arg_path_list, list<wstring>&arg_list, list<wstring>&path_list)
{
	arg_list.clear();
	path_list.clear();
	for (auto str : arg_path_list)
	{
		if (str[0] == '/'&&str.size() >= 2 && str.size() <= 3)
		{
			arg_list.push_back(str);
		}
		else
		{
			path_list.push_back(str);
		}
	}
}

Command::~Command()
{

}

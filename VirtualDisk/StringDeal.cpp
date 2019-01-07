#include "StringDeal.h"

StringDeal::StringDeal()
{
	//ctor
}

StringDeal::~StringDeal()
{
	//dtor
}

wstring StringDeal::intToString(int num)
{
	wstring str; 
	while (num)
	{
		str += ('0' + num % 10);
		num /= 10;
	}
	reverse(str.begin(), str.end());
	for (int index = (int)str.size() - 3; index > 0; index -= 3)
		str.insert(index, L",");
	return str;
}


wstring StringDeal::getWildcardStr(const wstring &path) //获取通配符前几个字符串
{
	wstring ans;
	for (auto it : path)
	{
		if (it == '?' || it == '*')
			break;
		else
			ans += it;
	}
	return ans;
}



bool StringDeal::matches(const wchar_t*wildcard, const wchar_t *str)
{
	if (*wildcard == '\0')
		return *str == '\0';
	if (*wildcard == '?')
		return matches(++wildcard, ++str);
	else if (*wildcard == '*')
	{
		for (++wildcard; *str; ++str)
		{
			if (matches(wildcard, str))
				return true;
		}
		return *wildcard == '\0';
	}
	else
		return *wildcard == *str&&matches(++wildcard, ++str);
}

void StringDeal::strReplace(wstring&path_name, const wstring&wild_str1, const wstring&wild_str2)//置换名字，比如 copy 1* 11*，要将文件1开头的部分改成11
{
	wstring new_name = wild_str2;
	int idx = 0;
	for (unsigned int i = 0; i <= path_name.size(); i++)
	{
		if (i == path_name.size() || path_name[i] != wild_str1[i])
		{
			idx = i;
			break;
		}
	}
	for (unsigned int i = idx; i < path_name.size(); i++)
	{
		new_name += path_name[i];
	}
	path_name = new_name;
}


bool StringDeal::isHaveWildcard(const wstring &name)
{
	for (auto c : name)
	{
		if (c == '*' || c == '?')
			return true;
	}
	return false;
}

//将整体路径拆分成单个路径
void StringDeal::getInput(wstring& path,list<wstring>& ans_path)
{
	wstring tmp_str;
	bool quote_flag = 0;
	for (unsigned int i = 0; i <= path.size(); i++)
	{
		if (i == path.size() || (path[i] == ' ' && !quote_flag))
		{
			quote_flag = false;
			if (tmp_str == L"cd..")
			{
				ans_path.push_back(L"cd");
				ans_path.push_back(L"..");
			}
			else if (tmp_str != L"")
			{
				if (tmp_str.size() == 2 && (tmp_str[0] == '\\' || tmp_str[0] == '/') && i)
				{
					auto it = ans_path.begin();
					it++;
					ans_path.insert(it, tmp_str);
				}
				else
					ans_path.push_back(tmp_str);
			}
			tmp_str = L"";
			continue;
		}
		if (path[i] == '"')
		{
			if (!quote_flag)
				quote_flag = true;
			else
				quote_flag = false;
			continue;
		}
		if (path[i] == '/' || path[i] == '\\')
		{
			quote_flag = false;
		}
		tmp_str += path[i];
	}
}

int StringDeal::getCmdArgSize(const list<wstring>&path_list)
{
	int arg_sum = 0;
	for (auto it:path_list)
	{
		if (it.size() >= 2 && it.size() <= 3 && (it[0] == '/'))
		{
			arg_sum++;
		}
		else
			break;
	}
	return arg_sum;
}


wstring StringDeal::dealMultiByteToWideChar(const char *str,int len) //A2W   测试没问题
{
	DWORD wchar_len = MultiByteToWideChar(CP_ACP, 0, str, len, NULL, 0);
	wstring wstr(wchar_len, 0);
	MultiByteToWideChar(CP_ACP, 0, str, len, &wstr[0], wchar_len);
	return wstr;
}

string StringDeal::dealWideCharToMultiByte(const wchar_t *wstr,int len)   // W2A
{
	DWORD str_len = WideCharToMultiByte(CP_ACP, 0, wstr, len, NULL, 0, NULL, NULL);
	string str(str_len, 0);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, &str[0], str_len, NULL, NULL);
	return str;
}
char* StringDeal::dealsWideCharToMultiByte(const wchar_t* wstr, int w_len,int &len)   // W2A
{
	len = WideCharToMultiByte(CP_ACP, 0, wstr, w_len, NULL, 0, NULL, NULL);
	char *str = new char[len];
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	return str;
}
wchar_t* StringDeal::dealsMultiByteToWideChar(const char* str,int& len)   // A2W
{
	DWORD wchar_len = MultiByteToWideChar(CP_ACP, 0, str, len, NULL, 0);
	wchar_t *wstr = new wchar_t[wchar_len];
	MultiByteToWideChar(CP_ACP, 0, str, len, wstr, wchar_len);
	len = wchar_len;
	return wstr;
}
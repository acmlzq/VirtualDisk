#include "TruePathDeal.h"



TruePathDeal::TruePathDeal()
{
}



bool TruePathDeal::isTrueDiskPath(wstring& path)
{
	if (path.size() != 0 && path[0] == '@')
	{
		path.erase(path.begin());
		return true;
	}
	return false;
}

void TruePathDeal::getTrueDiskFile(list<pair<pair<wstring, int>, wchar_t*> >&list_pair,wstring &path) //通过真实磁盘路径获取信息  //名字，大小，数据
{
	if (_access(StringDeal::dealWideCharToMultiByte(path.c_str(), path.size()).c_str(), 0) == -1) // 文件不存在返回-1
	{
		return ;
	}
	pair<wstring, wstring>folder_file = DiskPathDeal::getPathFolderAndFile(path);
	DWORD ftype = GetFileAttributesA(StringDeal::dealWideCharToMultiByte(folder_file.first.c_str(), folder_file.first.size()).c_str());
	if (!(ftype & FILE_ATTRIBUTE_DIRECTORY)) //如果不是目录
	{
		wcout << L"系统找不到指定的路径。" << endl;
		return ;
	}
	bool have_wildcards = StringDeal::isHaveWildcard(folder_file.second);
	if (have_wildcards) //含有通配符
	{
		vector<wstring>all_files;
		getTrueDiskFiles(folder_file.first, all_files); //获取目录下所有文件
		for (auto it : all_files)
		{
			if (StringDeal::matches(folder_file.second.c_str(), it.c_str()))
			{
				wstring full_name = folder_file.first + L"//" + it;
			}
		}
	}
	else //不含通配符
	{
		DWORD ftype = GetFileAttributesA(StringDeal::dealWideCharToMultiByte(path.c_str(), path.size()).c_str());
		if (ftype & FILE_ATTRIBUTE_DIRECTORY) //如果是目录
		{
			vector<wstring>all_files;
			getTrueDiskFiles(path, all_files); //获取目录下所有文件
			for (auto it : all_files)
			{
				if (StringDeal::matches(path.c_str(), it.c_str()))
				{
					wstring full_name = path + L"//" + it;
					pair<pair<wstring, int>, wchar_t*> tmp_pair;
					getTrueDiskFileInfo(tmp_pair, full_name);
					list_pair.push_back(tmp_pair);
				}
			}
		}
		else //如果不是目录,是文件
		{
			pair<pair<wstring, int>, wchar_t*> tmp_pair;
			getTrueDiskFileInfo(tmp_pair, path);
			list_pair.push_back(tmp_pair);
		}
	}
	return ;
}

bool TruePathDeal::containTrueDiskNode(wstring path, int&size, int&type)
{
	if (_access(StringDeal::dealWideCharToMultiByte(path.c_str(), path.size()).c_str(), 0) == -1) // 文件不存在返回-1
	{
		size = -1, type = TYPE_NOT_HAVE;
		return false;
	}
	DWORD ftype = GetFileAttributesA(StringDeal::dealWideCharToMultiByte(path.c_str(), path.size()).c_str());
	if (ftype & FILE_ATTRIBUTE_DIRECTORY) //如果是目录
	{
		size = 0, type = TYPE_FOLDER;
		return true;
	}
	else //如果是文件
	{
		ifstream infile(path.c_str(), ios::binary);
		infile.seekg(0, ios::end);
		size = (int)infile.tellg();
		infile.close();
		type = TYPE_FILE;
		return true;
	}
}

void TruePathDeal::getTrueDiskFileInfo(pair<pair<wstring, int>, wchar_t*>&tmp_pair,const wstring &path) //已经确保该path存在，且为文件
{
	pair<wstring, wstring>folder_file = DiskPathDeal::getPathFolderAndFile(path);
	ifstream infile(path.c_str(), ios::binary);
	infile.seekg(0, ios::end);
	int size = (int)infile.tellg();
	infile.seekg(0, ios::beg);
	wchar_t *str = readFile(size,infile); //这里记得释放内存
	tmp_pair = make_pair(make_pair(folder_file.second, size), str);
	infile.close();
	return;
}

void TruePathDeal::getTrueDiskFiles(const wstring &path, vector<wstring>& files) //files对应最终的文件数组
{
	//文件句柄
	long   hFile = 0;
	//文件信息
	struct _finddata_t fileinfo;
	wstring p; //wstring转char*
	wstring wstr_p = p.assign(path).append(L"\\*");
	if ((hFile = _findfirst(StringDeal::dealWideCharToMultiByte(wstr_p.c_str(), wstr_p.size()).c_str(), &fileinfo)) != -1) //需要将宽字节转换成多字节
	{
		do
		{
			if ((fileinfo.attrib &  _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getTrueDiskFiles(p.assign(path).append(L"\\").append(StringDeal::dealMultiByteToWideChar(fileinfo.name,strlen(fileinfo.name))), files); //需要char* 转 wstring
			}
			else
			{
				files.push_back(StringDeal::dealMultiByteToWideChar(fileinfo.name,strlen(fileinfo.name))); //需要char* 转 wstring
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}



void TruePathDeal::writeDataToTrueDisk(const wstring& path, Component* comp)
{
	writeIntToTrueDisk(path, comp->getType());
	writeStringToTrueDisk(path, comp->getTime());
	writeStringToTrueDisk(path, comp->getName());
	if (comp->getType() == TYPE_FOLDER) //文件夹
	{
		Folder* folder = static_cast<Folder*>(comp);
		writeIntToTrueDisk(path, static_cast<int>(folder->Cmap.size()));
	}
	else if (comp->getType() == TYPE_FILE) //文件
	{
		File *file = static_cast<File*>(comp);
		int wchar_size;
		wchar_t *content = file->getContent(wchar_size);
		writeCharToTrueDisk(path, wchar_size, content);
	}
	else if (abs(comp->getType()) == TYPE_LINK_FILE)  //符号链接
	{
		SymLink* symlink = static_cast<SymLink*>(comp);
		writeStringToTrueDisk(path, symlink->getLinkPath());
	}
}

void TruePathDeal::writeIntToTrueDisk(const wstring&path, int num)
{
	ofstream outfile(path.c_str(), ios::binary | ios::app);
	outfile.write((char*)&num, sizeof(num));
	outfile.close();
}
void TruePathDeal::writeStringToTrueDisk(const wstring&path, const wstring& wstr)
{
	ofstream outfile(path.c_str(), ios::binary | ios::app);
	string str = StringDeal::dealWideCharToMultiByte(wstr.c_str(),wstr.size());
	const char *p = str.c_str();
	int str_size = strlen(p);
	outfile.write((char*)&str_size, sizeof(str_size)); //先把长度新进去
	outfile.write((char *)p, str_size); //把数据写进去
	outfile.close();
}
void TruePathDeal::writeCharToTrueDisk(const wstring&path, int w_len, const wchar_t* content) //这个要留意
{
	ofstream outfile(path.c_str(), ios::binary | ios::app);
	int len; //表示的是非unicode的长度
	char *p = StringDeal::dealsWideCharToMultiByte(content,w_len,len);
	outfile.write((char *)&len, sizeof(p)); //先把长度加进去
	outfile.write((char *)p, len); //把数据写进去
	outfile.close();
	delete[]p;
}
void TruePathDeal::readDataFromTrueDisk(const wstring& path, FileDisk*filedisk)
{
	ifstream infile(path.c_str(), ios::binary);
	filedisk->formatDisk();
	Component *root = filedisk->getRootNode();
	Component *tmp_root = readComponent(infile);
	root->setSize(tmp_root->getSize());
	delete tmp_root;
	queue<Component*>comp_q;
	comp_q.push(root);
	while (!comp_q.empty())
	{
		Component * now_top = comp_q.front();
		comp_q.pop();
		if (now_top->getType() != TYPE_FOLDER)	continue; //文件夹，逐个添加
		Folder *folder = (Folder*)now_top;
		for (int i = 1; i <= folder->getSize(); i++)
		{
			Component* tmp_comp = readComponent(infile);
			folder->Cmap.insert(make_pair(tmp_comp->getName(),tmp_comp));
			tmp_comp->setFatherNode(now_top);
			if (tmp_comp->getType() == TYPE_FOLDER) //文件夹
				comp_q.push(tmp_comp);
		}
	}
	infile.close();
}
Component* TruePathDeal::readComponent(ifstream& infile)
{
	int type = readInt(infile);
	wstring the_time = readwstring(infile);
	wstring name = readwstring(infile);
	if (type == TYPE_FOLDER) //文件夹
	{
		int node_size = readInt(infile);
		Folder*folder = new Folder(nullptr, name, type); //父节点，名字，类型
		folder->setTime(the_time);
		folder->setSize(node_size);
		return (Component*)folder;
	}
	else if (type == TYPE_FILE) //文件
	{
		int size = readInt(infile);
		wchar_t *content = readFile(size, infile);
		File*file = new File(nullptr, name, type);
		file->setTime(the_time);
		file->setContent(content, size);
		delete []content;
		return (Component*)file;
	}
	else if (abs(type) == TYPE_LINK_FILE) //符号链接
	{
		wstring link_path = readwstring(infile);
		SymLink* symlink = new SymLink(nullptr, name, type);
		symlink->setTime(the_time);
		symlink->setLink(link_path);
		return (Component*)symlink;
	}
	return nullptr;
}
int TruePathDeal::readInt(ifstream& infile)
{
	int num;
	infile.read((char*)&num, sizeof(num));
	return num;
}
wstring TruePathDeal::readwstring(ifstream& infile)
{
	int len = readInt(infile), i = 0;
	string str;
	char c;
	while (i++ < len)
	{
		infile.read((char*)&c, sizeof(c));
		str += c;
	}
	wstring wstr = StringDeal::dealMultiByteToWideChar(str.c_str(),len);
	return wstr;
}
wchar_t* TruePathDeal::readFile(int &len, ifstream& infile) //这个要留意
{
	int i = 0;
	char *str = new char[len];
	infile.read((char*)str, len);
	wchar_t *wstr = StringDeal::dealsMultiByteToWideChar(str,len);
	delete[]str;
	return wstr;
}
TruePathDeal::~TruePathDeal()
{
}

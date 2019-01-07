#pragma once
#include <windows.h>
#include <io.h>
#include <fstream>
#include <atlbase.h>
#include <atlconv.h>
#include "FileDisk.h"
#include "File.h"
#include "SymLink.h"

//������ʵ�����еĲ������д���
class TruePathDeal
{
private:
	static void writeIntToTrueDisk(const wstring&, int); //����ʵ������дһ��int
	static void writeStringToTrueDisk(const wstring&, const wstring&);//����ʵ������д�ַ���
	static void writeCharToTrueDisk(const wstring&, int, const wchar_t*);//����ʵ������д�ֽ���
	static Component* readComponent(ifstream&);//����ʵ�����ж�ȡһ��Component����
	static int readInt(ifstream&); //����ʵ�����ж�ȡһ��Int
	static wstring readwstring(ifstream&);//����ʵ�����ж�ȡһ���ַ���
	static wchar_t* readFile(int&, ifstream&);//����ʵ�����ļ��ж�ȡ����
public:
	TruePathDeal();
	static void writeDataToTrueDisk(const wstring&, Component*);//д���ݵ���ʵ����
	static void readDataFromTrueDisk(const wstring&, FileDisk*);//����ʵ���̶�����
	static bool isTrueDiskPath(wstring&); //�ж��ǲ�����ʵ����·������ȥ����@�ַ�
	static void getTrueDiskFile(list<pair<pair<wstring, int>, wchar_t*> >&,wstring &); //ͨ��·����ȡ�����ƣ���С��������
	static void getTrueDiskFiles(const wstring&, vector<wstring>&);//��ȡ��ʵ�����ļ�
	static void getTrueDiskFileInfo(pair<pair<wstring, int>, wchar_t*>&,const wstring &path); //����ʵ����·���õ���ʵ�����ļ������ƣ���С��byteָ��
	static bool containTrueDiskNode(wstring path, int&size, int&type); //��ӦVirtualDisk��containNode�жϴ��̽ڵ��Ƿ���ڼ�����
	~TruePathDeal();
};


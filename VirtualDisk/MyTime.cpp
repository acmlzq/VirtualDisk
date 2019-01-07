#pragma warning(disable : 4996)
#include <atlbase.h>
#include <atlconv.h>
#include "MyTime.h"
#include "StringDeal.h"

MyTime::MyTime()
{

}

wstring MyTime::getNowTime()
{
	time_t t = time(0);
	char time_str[64] = {0};
	strftime(time_str, sizeof(time_str), "%Y/%m/%d  %H:%M", localtime(&t));
	wstring now_time = StringDeal::dealMultiByteToWideChar(time_str,strlen(time_str));
	return now_time;
}

MyTime::~MyTime()
{
}




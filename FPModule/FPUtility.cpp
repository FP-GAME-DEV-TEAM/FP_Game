/********************************
* FileName: FPUtility.cpp
* FileFunc: FP工程中常用的API实现
* Author: SQK
* Date: 2013-06-12
* Descript: 包含一些文件检查和校验操作
********************************/

#include "stdafx.h"
#include "FPDataType.h"

//检查目录
BOOL WINAPI IsFolderExist(tstring whichPath)
{
	WIN32_FIND_DATA ffd;
	BOOL exist = FALSE;
	HANDLE hFind = FindFirstFile(whichPath.c_str(), &ffd);
	if((hFind != INVALID_HANDLE_VALUE) && (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{
		exist = TRUE;
	}
	FindClose(hFind);
	return exist;
}

//获得一个路径字符串
LPTSTR WINAPI NewPathString(const tstring &str, const int size)
{
	LPTSTR tmp;
	int len = str.length();
	if(0==len || size<16 || len>size)
	{
		return NULL;
	}
	tmp = new TCHAR[size];
	ZeroMemory(tmp, size*sizeof(TCHAR));
	_tcscpy(tmp, str.c_str());
	return tmp;
}

//得到最上层窗口
HWND WINAPI GetRootWindow(HWND hWnd)
{
	HWND hTemp;
	if(NULL == hWnd)
	{
		return NULL;
	}
	hTemp = GetParent(hWnd);
	while(hTemp)
	{
		hWnd = hTemp;
		hTemp = GetParent(hTemp);
	}
	return hWnd;
}

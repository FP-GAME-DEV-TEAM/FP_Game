/********************************
* FileName: FPUtility.cpp
* FileFunc: FP�����г��õ�APIʵ��
* Author: SQK
* Date: 2013-06-12
* Descript: ����һЩ�ļ�����У�����
********************************/

#include "stdafx.h"
#include "FPModule.h"

//���Ŀ¼
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

//���һ��·���ַ���
LPTSTR WINAPI NewPath(const tstring &str, const int size)
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

//�õ����ϲ㴰��
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

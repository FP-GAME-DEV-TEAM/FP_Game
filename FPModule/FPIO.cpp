/********************************
* FileName: FPIO.cpp
* FileFunc: FP��ϷIO����ʵ��ģ��
* Author: SQK
* Date: 2013-07-23
* Descript: ��ģ����Ҫ����ʵ����Ϸ��Դ�Ͷ�Ӧ����ģ��ĶԽ�
********************************/

#include "stdafx.h"
#include "FPModule.h"


VOID CALLBACK ReadCompleted(DWORD dwError, DWORD dwCount, LPOVERLAPPED lpOver)
{
	return;
}


BOOL WINAPI FileReadData(HANDLE hFile, BOOL isAsync, DWORD dwOffset, LONG dwSize, LPVOID lpBuffer)
{
	BOOL bResult;
	DWORD dwCount = 0;
	DWORD dwError = 0;
	LPOVERLAPPED lpOver;
	if(isAsync)
	{
		lpOver = new OVERLAPPED();
		memset(lpOver, 0, sizeof(lpOver));
		lpOver->Offset = dwOffset;
		lpOver->hEvent = lpBuffer;
		ReadFileEx(hFile, lpBuffer, dwSize, lpOver, ReadCompleted);
	}
	else
	{
		bResult = ReadFile(hFile, lpBuffer, dwSize, &dwCount, NULL);
		if(bResult && dwCount == 0)
		{
			
		}
	}
	return FALSE;
}

BOOL WINAPI FileWriteData(HANDLE hFile, BOOL isAsync, DWORD dwOffset, LPVOID lpBuffer, LONG dwSize)
{
	return FALSE;
}
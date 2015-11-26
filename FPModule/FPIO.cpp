/********************************
* FileName: FPIO.cpp
* FileFunc: FP��ϷIO����ʵ��ģ��
* Author: SQK
* Date: 2013-07-23
* Descript: ��ģ����Ҫ����ʵ����Ϸ��Դ�Ͷ�Ӧ����ģ��ĶԽ�
********************************/

#include "stdafx.h"
#include "FPModule.h"
#include "FPFunction.h"

UINT CALLBACK BinProc(HANDLE param)
{
	HANDLE hStartEvent = param;
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	// Set thread start event
	if (!SetEvent(hStartEvent))
	{
		FP_DEBUG_MSG(_T("set start event failed, errno:%d\n"), GetLastError());
		return 1;
	}
	// Main bin thread loop
	while (true)
	{
		if (GetMessage(&msg, NULL, FPMSG_BASE, FPMSG_END))
		{
			switch (msg.message)
			{
			case FPMSG_THREAD_START:
				// Check IO file status
				break;
			default:
				break;
			}
		}
	}
	return 0;
}


BOOL WINAPI FileReadData(HANDLE hFile, BOOL isAsync, DWORD dwOffset, LONG dwSize, LPVOID lpBuffer)
{
	return FALSE;
}

BOOL WINAPI FileWriteData(HANDLE hFile, BOOL isAsync, DWORD dwOffset, LPVOID lpBuffer, LONG dwSize)
{
	return FALSE;
}

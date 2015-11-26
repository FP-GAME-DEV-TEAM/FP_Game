/********************************
* FileName: FPThread.cpp
* FileFunc: FP游戏多线程实现模块
* Author: SQK
* Date: 2015-11-23
* Descript: -
********************************/

#include "stdafx.h"
#include "FPModule.h"
#include "FPFunction.h"

typedef UINT(CALLBACK *FP_THREAD_ROUTINE)(LPVOID);

HANDLE hBinThread = NULL;
UINT dwBinThreadId = 0;
FP_THREAD_ROUTINE lpBinCallback;

HRESULT StartIOThread()
{
	//create thread start event
	HANDLE hStartEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (hStartEvent == 0)
	{
		FP_DEBUG_MSG(_T("Create start event failed, errno:%d\n"), GetLastError());
		return E_FAIL;
	}

	//start thread
	lpBinCallback = BinProc;
	hBinThread = (HANDLE)_beginthreadex(NULL, 0, lpBinCallback, hStartEvent, 0, &dwBinThreadId);
	if (NULL == hBinThread)
	{
		FP_DEBUG_MSG(_T("Start thread failed, errno:%d\n"), GetLastError());
		CloseHandle(hStartEvent);
		return E_FAIL;
	}

	//wait thread start event to avoid PostThreadMessage return errno:1444
	WaitForSingleObject(hStartEvent, INFINITE);
	CloseHandle(hStartEvent);
	return 0;
}

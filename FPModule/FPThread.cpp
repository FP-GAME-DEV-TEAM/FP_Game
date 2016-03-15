/********************************
* FileName: FPThread.cpp
* FileFunc: FP游戏多线程实现模块
* Author: SQK
* Date: 2015-11-23
* Descript: 线程实例为单例，每类线程只能启动一个。
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPFunction.h"

typedef UINT(CALLBACK *FP_THREAD_ROUTINE)(LPVOID);

/* Bin thread and the relevants */
HANDLE hBinThread = NULL;
UINT dwBinThreadId = 0;
FP_THREAD_ROUTINE lpBinCallback;

HRESULT StartIOThreads(PGameRes pRes)
{
	//check if the IO threads has been started.
	DWORD dwExitCode = 0;
	if (hBinThread != NULL)
	{
		if (GetExitCodeThread(hBinThread, &dwExitCode) && STILL_ACTIVE == dwExitCode)
		{
			return E_HANDLE;
		}
	}

	//create thread start event
	HANDLE hStartEvent = CreateEvent(0, FALSE, FALSE, 0);
	if (0 == hStartEvent)
	{
		FP_DEBUG_MSG(_T("Creating start event for IO thread failed, errno:%d\n"), GetLastError());
		return E_FAIL;
	}

	//start thread
	lpBinCallback = BinProc;
	hBinThread = (HANDLE)_beginthreadex(NULL, 0, lpBinCallback, hStartEvent, 0, &dwBinThreadId);
	if (NULL == hBinThread)
	{
		FP_DEBUG_MSG(_T("Starting IO thread failed, errno:%d\n"), GetLastError());
		CloseHandle(hStartEvent);
		return E_FAIL;
	}

	//wait thread start event to avoid PostThreadMessage return errno:1444
	WaitForSingleObject(hStartEvent, INFINITE);
	CloseHandle(hStartEvent);
	return S_OK;
}

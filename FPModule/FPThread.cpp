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

/* Bin thread and the relevants */
ThreadInfo mainIOThread = { 0 };

HRESULT WINAPI StartIOThreads()
{
	//check if the IO threads has been started.
	DWORD dwExitCode = 0;
	if (mainIOThread.hThread != NULL)
	{
		if (GetExitCodeThread(mainIOThread.hThread, &dwExitCode) && STILL_ACTIVE == dwExitCode)
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
	mainIOThread.lpCallback = BinProc;
	mainIOThread.hThread = (HANDLE)_beginthreadex(NULL, 0, mainIOThread.lpCallback, hStartEvent, 0, &mainIOThread.uThreadId);
	if (NULL == mainIOThread.hThread)
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

HRESULT WINAPI DestroyIOThreads()
{
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_THREAD_STOP, 0, 0);
	//wait thread end and then close handle
	WaitForSingleObject(mainIOThread.hThread, INFINITE);
	CloseHandle(mainIOThread.hThread);
	FP_DEBUG_MSG(_T("Game IO thread stoped.\n"));
	return S_OK;
}

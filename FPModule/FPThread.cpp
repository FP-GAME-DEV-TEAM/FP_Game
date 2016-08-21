/********************************
* FileName: FPThread.cpp
* FileFunc: FP��Ϸ���߳�ʵ��ģ��
* Author: SQK
* Date: 2015-11-23
* Descript: �߳�ʵ��Ϊ������ÿ���߳�ֻ������һ����
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
		GetExitCodeThread(mainIOThread.hThread, &dwExitCode);
		if (STILL_ACTIVE == dwExitCode)
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
	WaitForSingleObject(hStartEvent, FP_THREAD_TIMEOUT);
	CloseHandle(hStartEvent);
	return S_OK;
}

HRESULT WINAPI DestroyIOThreads()
{
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_THREAD_STOP, 0, 0);
	//wait thread end and then close handle
	WaitForSingleObject(mainIOThread.hThread, FP_THREAD_TIMEOUT);
	CloseHandle(mainIOThread.hThread);
	FP_DEBUG_MSG(_T("Game IO thread stoped.\n"));
	return S_OK;
}

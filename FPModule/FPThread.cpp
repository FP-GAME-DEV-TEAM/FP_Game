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

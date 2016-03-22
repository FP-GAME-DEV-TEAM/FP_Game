/********************************
* FileName: FPIO.cpp
* FileFunc: FP游戏IO操作实现模块
* Author: SQK
* Date: 2013-07-23
* Descript: 该模块主要用来实现游戏资源和对应功能模块的对接
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPFunction.h"

extern GameGraphics *mainGraphics;

HRESULT ReadBinData(HANDLE hFile, LPVOID pIn, LPVOID pOut);

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
			case FPMSG_IO_READ_ANIMEDATA:
				ReadBinData(mainGraphics->GetFileHandle(1), (LPVOID)msg.wParam, (LPVOID)msg.lParam);
				break;
			default:
				break;
			}
		}
	}
	return 0;
}

HRESULT ReadBinData(HANDLE hFile, LPVOID pIn, LPVOID pOut)
{
	return S_OK;
}


BOOL __fastcall FileReadData(HANDLE hFile, DWORD dwOffset, LONG dwSize, LPVOID lpBuffer)
{
	return FALSE;
}

BOOL __fastcall FileWriteData(HANDLE hFile, DWORD dwOffset, LPVOID lpBuffer, LONG dwSize)
{
	return FALSE;
}

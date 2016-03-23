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
#include "FPDump.h"

extern GameGraphics *mainGraphics;

static HRESULT FileReadData(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback);
static HRESULT FileWriteData(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback);
static HRESULT __fastcall ReadData(HANDLE hFile, LONG dwOffset, DWORD dwSize, LPVOID lpBuffer);
static HRESULT __fastcall WriteData(HANDLE hFile, LPVOID lpBuffer, LONG dwOffset, DWORD dwSize);

UINT CALLBACK BinProc(HANDLE param)
{
	HANDLE hParam = param;
	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	// Set thread start event
	if (!SetEvent(hParam))
	{
		FP_DEBUG_MSG(_T("Setting start event failed, errno:%d\n"), GetLastError());
		return 1;
	}
	FP_DEBUG_MSG(_T("IO Thread started.\n"));
	// Main bin thread loop
	hParam = NULL;
	while (true)
	{
		if (GetMessage(&msg, NULL, FPMSG_BASE, FPMSG_END))
		{
			switch (msg.message)
			{
			case FPMSG_THREAD_START:
				// Check IO file status
				break;
			case FPMSG_IO_READ_IMAGEDATA:
				hParam = mainGraphics->GetFileHandle(FP_HANDLE_GRAPHIC_DATA);
				break;
			case FPMSG_IO_READ_ANIMEDATA:
				hParam = mainGraphics->GetFileHandle(FP_HANDLE_ANIME_DATA);
				break;
			case FPMSG_IO_READ_IMAGEINFO:
				hParam = mainGraphics->GetFileHandle(FP_HANDLE_GRAPHIC_INFO);
				break;
			case FPMSG_IO_READ_ANIMEINFO:
				hParam = mainGraphics->GetFileHandle(FP_HANDLE_ANIME_INFO);
				break;
			default:
				break;
			}
			if (NULL != hParam)
			{
				FileReadData(hParam, (PIOList)msg.wParam, (FP_THREAD_ROUTINE)msg.lParam);
			}
		}
	}
	return 0;
}

static HRESULT FileReadData(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback)
{
	HRESULT hResult;
	BOOL bFlag = TRUE;
	PIOItem pItem = NULL;
	PTCHAR lpTempStr = NULL;
	int length = 0;
	if (NULL == hFile || INVALID_HANDLE_VALUE == hFile || NULL == pItemList)
	{
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_INVALIDARG;
	}
	if (!(pItemList->count > 0) || NULL == pItemList->pList)
	{
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_POINTER;
	}
	for (size_t i = 0; i < pItemList->count; i++)
	{
		pItem = pItemList->pList[i];
		if (NULL == pItem)
		{
			ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
			return E_FAIL;
		}
		if (!pItem->isCompleted)
		{
			if (pItem->maxSize > 0)
			{
				lpTempStr = new TCHAR[pItem->maxSize + 1];
				ZeroMemory(lpTempStr, sizeof(TCHAR)*(length + 1));
				hResult = ReadData(hFile, pItem->offset, sizeof(TCHAR)* pItem->maxSize, lpTempStr);
				if (FAILED(hResult))
				{
					delete[] lpTempStr;
					lpTempStr = NULL;
					bFlag = FALSE;
					continue;
				}
				length = _tcschr((PTCHAR)lpTempStr, pItem->end) - lpTempStr;
				if (length < 0)
				{
					length = pItem->maxSize;
				}
				pItem->size = length;
				pItem->pData = new TCHAR[length + 1];
				ZeroMemory(pItem->pData, sizeof(TCHAR)*(length + 1));
				CopyMemory(pItem->pData, lpTempStr, sizeof(TCHAR)*(length + 1));
				delete[] lpTempStr;
			}
			else
			{
				hResult = ReadData(hFile, pItem->offset, pItem->size, pItem->pData);
			}
			if (SUCCEEDED(hResult))
			{
				pItem->isCompleted = TRUE;
			}
		}
		if (pItem->isCompleted == FALSE)
		{
			bFlag = FALSE;
		}
	}
	if (NULL != lpCallback && bFlag)
	{
		lpCallback(pItemList);
	}
	return S_OK;
}

static HRESULT FileWriteData(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback)
{
	return S_OK;
}

static HRESULT __fastcall ReadData(HANDLE hFile, LONG dwOffset, DWORD dwSize, LPVOID lpBuffer)
{
	DWORD dwResult;
	BOOL bFlag;
	dwResult = SetFilePointer(hFile, dwOffset, NULL, FILE_BEGIN);
	if (INVALID_SET_FILE_POINTER == dwResult)
	{
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_FAIL;
	}
	bFlag = ReadFile(hFile, lpBuffer, dwSize, &dwResult, NULL);
	if (FALSE == bFlag)
	{
		FP_DEBUG_MSG(_T("Can't read file, errno:%d\n"), GetLastError());
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_FAIL;
	}
	return S_OK;
}

static HRESULT __fastcall WriteData(HANDLE hFile, LPVOID lpBuffer, LONG dwOffset, DWORD dwSize)
{
	return S_OK;
}

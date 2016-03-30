/********************************
* FileName: FPIO.cpp
* FileFunc: FP��ϷIO����ʵ��ģ��
* Author: SQK
* Date: 2013-07-23
* Descript: ��ģ����Ҫ����ʵ����Ϸ��Դ�Ͷ�Ӧ����ģ��ĶԽ�
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPFunction.h"
#include "FPDump.h"

static HRESULT FPFileRead(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback);
static HRESULT FPFileWrite(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback);
static HRESULT FPPalettePreload(const PPalLib pPal, PIOList pItemList, FP_THREAD_ROUTINE lpCallback);

static HRESULT __fastcall ReadData(HANDLE hFile, LONG dwOffset, DWORD dwSize, LPVOID lpBuffer);
static HRESULT __fastcall WriteData(HANDLE hFile, LPVOID lpBuffer, LONG dwOffset, DWORD dwSize);

UINT CALLBACK BinProc(HANDLE param)
{
	HANDLE hParam = param;
	MSG msg;
	HRESULT hResult;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	// Set thread start event
	if (!SetEvent(hParam))
	{
		FP_DEBUG_MSG(_T("Setting start event failed, errno:%d\n"), GetLastError());
		return 1;
	}
	FP_DEBUG_MSG(_T("Game IO thread started.\n"));
	mainIOThread.bRunning = TRUE;
	// Main bin thread loop
	hParam = NULL;
	while (mainIOThread.bRunning)
	{
		if (GetMessage(&msg, NULL, FPMSG_BASE, FPMSG_END))
		{
			switch (msg.message)
			{
			case FPMSG_THREAD_START:
				mainIOThread.bRunning = TRUE;
				break;
			case FPMSG_THREAD_STOP:
				mainIOThread.bRunning = FALSE;
				break;
			case FPMSG_IO_READ_IMAGEDATA:
			case FPMSG_IO_READ_ANIMEDATA:
			case FPMSG_IO_READ_IMAGEINFO:
			case FPMSG_IO_READ_ANIMEINFO:
				hParam = mainGraphics->GetFileHandle(msg.message);
				hResult = FPFileRead(hParam, (PIOList)msg.wParam, (FP_THREAD_ROUTINE)msg.lParam);
				if (FAILED(hResult))
				{
					ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
					FP_DEBUG_MSG(_T("An error occurred in IO proc, errno:0x%0x\n"), hResult);
				}
				break;
			case FPMSG_IO_READ_PALETTE:
				hParam = mainEnv->GetPaletteLib();
				hResult = FPPalettePreload((const PPalLib)hParam, (PIOList)msg.wParam, (FP_THREAD_ROUTINE)msg.lParam);
				if (FAILED(hResult))
				{
					ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
					FP_DEBUG_MSG(_T("An error occurred during Pal preload, errno:0x%0x\n"), hResult);
				}
				break;
			default:
				break;
			}
		}
	}
	_endthreadex(0);
	return 0;
}

static HRESULT FPFileRead(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback)
{
	HRESULT hResult;
	PIOItem pItem = NULL;
	PTCHAR lpTempStr = NULL;
	int length = 0;
	if (NULL == hFile || INVALID_HANDLE_VALUE == hFile || NULL == pItemList)
	{
		return E_INVALIDARG;
	}
	if (!(pItemList->count > 0) || NULL == pItemList->pList)
	{
		return E_POINTER;
	}
	for (size_t i = 0; i < pItemList->count; i++)
	{
		pItem = &pItemList->pList[i];
		if (NULL == pItem)
		{
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
	}
	if (NULL != lpCallback)
	{
		lpCallback(pItemList);
	}
	return S_OK;
}

static HRESULT FPFileWrite(HANDLE hFile, PIOList pItemList, FP_THREAD_ROUTINE lpCallback)
{
	return S_OK;
}

static HRESULT FPPalettePreload(const PPalLib pPal, PIOList pItemList, FP_THREAD_ROUTINE lpCallback)
{
	HRESULT hResult;
	HANDLE hFile;
	tstring strPath;
	PIOItem pItem;
	LPBYTE buffer;
	if (NULL == pPal || NULL == pItemList)
	{
		return E_INVALIDARG;
	}
	if (!(pItemList->count > 0) || NULL == pItemList->pList)
	{
		return E_POINTER;
	}
	if (pItemList->count != pPal->sum)
	{
		return E_FAIL;
	}
	buffer = new BYTE[FP_FILE_SIZE_PAL];
	for (size_t i = 0; i < pItemList->count; i++)
	{
		strPath = pPal->palPath;
		strPath += _T("\\");
		strPath += (pPal->fileList[i]);
		pItem = &pItemList->pList[i];
		hFile = CreateFile(strPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			delete[] buffer;
			return E_HANDLE;
		}
		hResult = ReadData(hFile, pItem->offset, pItem->size, buffer);
		CloseHandle(hFile);
		for (size_t j = 0; j < FP_STORE_PAL_OPTIONAL; j++)
		{
			CopyMemory((PPALETTEENTRY)(pItem->pData) + j, buffer + 3 * j, 3);
			((PPALETTEENTRY)(pItem->pData) + j)->peFlags = 0;
		}
		if (SUCCEEDED(hResult))
		{
			pItem->isCompleted = TRUE;
		}
	}
	delete[] buffer;
	if (NULL != lpCallback)
	{
		lpCallback(pItemList);
	}
	return S_OK;
}

static HRESULT __fastcall ReadData(HANDLE hFile, LONG dwOffset, DWORD dwSize, LPVOID lpBuffer)
{
	DWORD dwResult;
	BOOL bFlag;
	dwResult = SetFilePointer(hFile, dwOffset, NULL, FILE_BEGIN);
	if (INVALID_SET_FILE_POINTER == dwResult)
		return E_FAIL;
	bFlag = ReadFile(hFile, lpBuffer, dwSize, &dwResult, NULL);
	if (FALSE == bFlag)
		return E_FAIL;
	return S_OK;
}

static HRESULT __fastcall WriteData(HANDLE hFile, LPVOID lpBuffer, LONG dwOffset, DWORD dwSize)
{
	return S_OK;
}

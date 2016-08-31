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
#include "FPUtility.h"

static HRESULT FPFileRead(HANDLE hFile, PIOList pList, FP_THREAD_ROUTINE lpCallback);
static HRESULT FPFileWrite(HANDLE hFile, PIOList pList, FP_THREAD_ROUTINE lpCallback);
static HRESULT FPPalettePreload(const PPalLib pPal, PIOList pList, FP_THREAD_ROUTINE lpCallback);

HRESULT __fastcall ReadData(HANDLE hFile, LONG dwOffset, DWORD dwSize, LPVOID lpBuffer);
HRESULT __fastcall WriteData(HANDLE hFile, LPVOID lpBuffer, LONG dwOffset, DWORD dwSize);

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
	// Main IO thread loop
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
			case FPMSG_IO_READ_GRAPHICDATA:
			case FPMSG_IO_READ_GRAPHICINFO:
			case FPMSG_IO_READ_ANIMEDATA:
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
		if (FAILED(hResult) && NULL != msg.wParam)
		{
			IOCompleteDefault(((PIOList)msg.wParam)->hEvent);
		}
	}
	_endthreadex(0);
	return 0;
}

PIOList WINAPI IOList::CreateIOList(UINT type, HANDLE event, LONG count)
{
	PIOList list = new IOList();
	list->isCompleted = FALSE;
	list->uIOType = type;
	list->hEvent = event;
	list->nCount = count;
	PIOItem items = new IOItem[count];
	ZeroMemory(items, count * sizeof(IOItem));
	list->pItemList = items;
	return list;
}

PIOItem IOList::SetIOListItem(LONG index, LONG id, DWORD maxSize, DWORD offset, DWORD sizeEnd, LPVOID lpData)
{
	if (index < 0 || index >= this->nCount)
	{
		return NULL;
	}
	PIOItem item = &(this->pItemList)[index];
	if (maxSize > 0)
	{
		item->id = id;
		item->maxSize = maxSize;
		item->offset = offset;
		item->end = sizeEnd;
		item->pData = NULL;
	}
	else
	{
		item->id = id;
		item->maxSize = 0;
		item->offset = offset;
		item->size = sizeEnd;
		if (NULL == lpData)
		{
			lpData = new BYTE[sizeEnd];
		}
		item->pData = lpData;
	}
	return item;
}

IOList::~tagIOList()
{
	PIOItem item = this->pItemList;
	delete[] item;
}

UINT CALLBACK IOCompleteDefault(LPVOID pParam)
{
	if (NULL == pParam)
	{
		return E_INVALIDARG;
	}
	PIOList pList = (PIOList)pParam;
	if (NULL != pList->hEvent)
	{
		if (!SetEvent(pList->hEvent))
		{
			return E_HANDLE;
		}
	}
	delete pList;
	return S_OK;
}

static HRESULT FPFileRead(HANDLE hFile, PIOList pList, FP_THREAD_ROUTINE lpCallback)
{
	HRESULT hResult;
	PIOItem pItem = NULL;
	PTCHAR lpTempStr = NULL;
	int length = 0;
	if (NULL == hFile || INVALID_HANDLE_VALUE == hFile || NULL == pList)
	{
		return E_INVALIDARG;
	}
	if (!(pList->nCount > 0) || NULL == pList->pItemList)
	{
		return E_POINTER;
	}
	for (LONG i = 0; i < pList->nCount; i++)
	{
		pItem = &pList->pItemList[i];
		if (NULL == pItem)
		{
			return E_POINTER;
		}
		if (pItem->maxSize > 0)
		{
			lpTempStr = new TCHAR[pItem->maxSize + 1];
			ZeroMemory(lpTempStr, sizeof(TCHAR)*(pItem->maxSize + 1));
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
		if (FAILED(hResult))
		{
			break;
		}
	}
	if (SUCCEEDED(hResult))
	{
		pList->isCompleted = TRUE;
		if (NULL != lpCallback)
		{
			lpCallback(pList);
		}
	}
	return hResult;
}

static HRESULT FPFileWrite(HANDLE hFile, PIOList pList, FP_THREAD_ROUTINE lpCallback)
{
	return S_OK;
}

static HRESULT FPPalettePreload(const PPalLib pPal, PIOList pList, FP_THREAD_ROUTINE lpCallback)
{
	HRESULT hResult;
	HANDLE hFile;
	tstring strPath;
	PIOItem pItem;
	LPBYTE buffer;
	if (NULL == pPal || NULL == pList)
	{
		return E_INVALIDARG;
	}
	if (!(pList->nCount > 0) || NULL == pList->pItemList)
	{
		return E_POINTER;
	}
	if (pList->nCount != pPal->sum)
	{
		return E_POINTER;
	}
	buffer = new BYTE[FP_FILE_SIZE_PAL];
	for (LONG i = 0; i < pList->nCount; i++)
	{
		strPath = pPal->palPath;
		strPath += _T("\\");
		strPath += (pPal->fileList[i]);
		pItem = &pList->pItemList[i];
		hFile = CreateFile(strPath.c_str(), GENERIC_READ, FILE_SHARE_READ,
			NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
		if (INVALID_HANDLE_VALUE == hFile)
		{
			delete[] buffer;
			return E_HANDLE;
		}
		hResult = ReadData(hFile, pItem->offset, pItem->size, buffer);
		CloseHandle(hFile);
		for (LONG j = 0; j < FP_STORE_PAL_OPTIONAL; j++)
		{
			CopyMemory((PPALETTEENTRY)(pItem->pData) + j, buffer + 3 * j, 3);
			((PPALETTEENTRY)(pItem->pData) + j)->peFlags = 0;
		}
		if (FAILED(hResult))
		{
			break;
		}
	}
	delete[] buffer;
	if (SUCCEEDED(hResult))
	{
		pList->isCompleted = TRUE;
		if (NULL != lpCallback)
		{
			lpCallback(pList);
		}
	}
	return hResult;
}

HRESULT __fastcall ReadData(HANDLE hFile, LONG dwOffset, DWORD dwSize, LPVOID lpBuffer)
{
	DWORD dwResult;
	BOOL bFlag;
	dwResult = SetFilePointer(hFile, dwOffset, NULL, FILE_BEGIN);
	if (INVALID_SET_FILE_POINTER == dwResult)
		return E_HANDLE;
	bFlag = ReadFile(hFile, lpBuffer, dwSize, &dwResult, NULL);
	if (FALSE == bFlag)
		return E_FAIL;
	return S_OK;
}

HRESULT __fastcall WriteData(HANDLE hFile, LPVOID lpBuffer, LONG dwOffset, DWORD dwSize)
{
	return S_OK;
}

HRESULT __fastcall EncryptData()
{
	return S_OK;
}

HRESULT __fastcall DecryptData(LPBYTE lpData, DWORD dwLength, DWORD dwSize, LPBYTE lpBuffer)
{
	BOOL repeat, blank;
	DWORD count, i, m, n;
	BYTE step, key;
	m = 0;
	n = 0;
	while (n < dwLength)
	{
		key = lpData[n] & 0xF0;
		count = lpData[n] & 0x0F;
		repeat = (key & 0xC0) & 0x80 == 0 ? FALSE : TRUE;
		blank = (key & 0xC0) & 0x40 == 0 ? FALSE : TRUE;
		step = (key & 0x30) >> 4;
		++n;
		if (repeat)
		{
			if (blank)
				key = 0x00;
			else
				key = lpData[n++];
			for (i = 0; i < step; i++)
			{
				count <<= 8;
				count += lpData[n + i];
			}
			if (m + count > dwSize)
			{
				count = dwSize - m;
				memset(&lpBuffer[m], key, count);
				return S_FALSE;
			}
			memset(&lpBuffer[m], key, count);
			m += count;
			n += step;
		}
		else
		{
			for (i = 0; i < step; i++)
			{
				count <<= 8;
				count += lpData[n + i];
			}
			n += step;
			if (m + count > dwSize)
			{
				count = dwSize - m;
				memset(&lpBuffer[m], key, count);
				return S_FALSE;
			}
			memcpy(&lpBuffer[m], &lpData[n], count);
			m += count;
			n += count;
		}
	}
	return S_OK;
}

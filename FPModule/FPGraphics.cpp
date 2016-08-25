/********************************
* FileName: FPGraphics.cpp
* FileFunc: FP游戏图像资源实现模块
* Author: SQK
* Date: 2016-03-15
* Descript: 游戏图像资源的载入接口
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPFunction.h"
#include "FPDump.h"


GameGraphics *mainGraphics = NULL;

extern HWND hMainWnd;

//=====================================
// GameGraphics类的实现
//

GameGraphics *GameGraphics::pInstance = NULL;

GameGraphics::GameGraphics()
{
	this->hGraphicInfo = NULL;
	this->hGraphicData = NULL;
	this->hAnimeInfo = NULL;
	this->hAnimeData = NULL;
	ZeroMemory(this->mPalette, FP_STORE_PAL_COUNT);
	ZeroMemory(this->mPaletteDefault, FP_STORE_PAL_DEFAULT);
	ZeroMemory(this->mPaletteOptional, FP_FILE_COUNT_PAL*FP_STORE_PAL_OPTIONAL);
}

GameGraphics::~GameGraphics()
{
	mainGraphics = NULL;
}

HRESULT WINAPI GameGraphics::Create(const PBinLib pBin)
{
	tstring path;
	if (NULL != pInstance) //检查是否已经初始化
	{
		mainGraphics = pInstance;
		return E_HANDLE;
	}
	if (NULL == pBin) //检查BinLib是否有效
	{
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_FAIL;
	}
	pInstance = new GameGraphics();
	path = pBin->binPath;
	path += _T("\\");
	pInstance->hGraphicInfo = CreateFile((path + pBin->sGraphicInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	pInstance->hGraphicData = CreateFile((path + pBin->sGraphicData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	pInstance->hAnimeInfo = CreateFile((path + pBin->sAnimeInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	pInstance->hAnimeData = CreateFile((path + pBin->sAnimeData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_RANDOM_ACCESS, NULL);
	if (pInstance->hGraphicInfo == INVALID_HANDLE_VALUE
		|| pInstance->hGraphicData == INVALID_HANDLE_VALUE
		|| pInstance->hAnimeInfo == INVALID_HANDLE_VALUE
		|| pInstance->hAnimeData == INVALID_HANDLE_VALUE)
	{
		delete pInstance;
		pInstance = NULL;
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_FAIL;
	}
	mainGraphics = pInstance;
	return S_OK;
}

VOID WINAPI GameGraphics::Destroy()
{
	if (NULL == pInstance)
	{
		return;
	}
	//释放文件资源句柄
	if (pInstance->hGraphicInfo != NULL && pInstance->hGraphicInfo != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hGraphicInfo);
	if (pInstance->hGraphicData != NULL && pInstance->hGraphicData != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hGraphicData);
	if (pInstance->hAnimeInfo != NULL && pInstance->hAnimeInfo != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hAnimeInfo);
	if (pInstance->hAnimeData != NULL && pInstance->hAnimeData != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hAnimeData);
	// Set ptr to null
	pInstance->hGraphicInfo = NULL;
	pInstance->hGraphicData = NULL;
	pInstance->hAnimeInfo = NULL;
	pInstance->hAnimeData = NULL;
	// Free main environment
	SAFE_DELETE(pInstance);
}

UINT CALLBACK GameGraphics::GraphicsIOComplete(LPVOID pParam)
{
	if (NULL == pParam)
	{
		return E_INVALIDARG;
	}
	PIOList pItemList = (PIOList)pParam;
	if (pItemList->ioType == FPMSG_IO_READ_IMAGEDATA)
	{
		// Decompress ?
		PIOItem pItem;
		for (LONG i = 0; i < pItemList->count; i++)
		{
			pItem = &pItemList->pList[i];
			DecryptData((LPBYTE)pItem->pData, pItem->size, 0, NULL);
		}
	}
	return IOCompleteDefault(pItemList->hEvent);
}

HANDLE GameGraphics::GetFileHandle(const UINT type) const
{
	HANDLE handle = NULL;
	DWORD id = type & 0x000000ff;
	switch (id)
	{
	case FP_HANDLE_GRAPHIC_DATA:
		handle = this->hGraphicData;
		break;
	case FP_HANDLE_GRAPHIC_INFO:
		handle = this->hGraphicInfo;
		break;
	case FP_HANDLE_ANIME_DATA:
		handle = this->hAnimeData;
		break;
	case FP_HANDLE_ANIME_INFO:
		handle = this->hAnimeInfo;
		break;
	default:
		break;
	}
	return handle;
}

HRESULT GameGraphics::GetImage(LONG id, const FPImage **pData)
{
	if (id < 0 || id > mGraphicList.size() || NULL == pData)
	{
		return E_FAIL;
	}
	ImageMap::const_iterator it = mGraphicCache.find(id);
	if (it != mGraphicCache.end())
	{
		*pData = it->second;
		return S_OK;
	}
	mGraphicCache[id] = NULL;
	mImageReqQueue.push_back(id);
	*pData = NULL;
	return S_FALSE;
}

HRESULT GameGraphics::GetAction(LONG id, const FPAction **pData)
{
	if (id < 0 || id > mAnimeList.size() || NULL == pData)
	{
		return E_FAIL;
	}
	ActionMap::const_iterator it = mAnimeCache.find(id);
	if (it != mAnimeCache.end())
	{
		*pData = it->second;
		return S_OK;
	}
	// ?
	return NULL;
}

HRESULT GameGraphics::GetPalette(LONG id, const PALETTEENTRY **pData)
{
	DWORD i, n = 0;
	if (mPaletteIndex == id)
	{
		if (NULL != pData)
		{
			*pData = mPalette;
		}
		return S_FALSE;
	}
	for (i = 0; i < FP_STORE_PAL_FRONT; i++)
	{
		mPalette[n++] = mPaletteDefault[i];
	}
	for (i = 0; i < FP_STORE_PAL_OPTIONAL; i++)
	{
		mPalette[n++] = mPaletteOptional[id][i];
	}
	for (i = 0; i < FP_STORE_PAL_BACK; i++)
	{
		mPalette[n++] = mPaletteDefault[FP_STORE_PAL_FRONT + i];
	}
	mPaletteIndex = id;
	if (NULL != pData)
	{
		*pData = mPalette;
	}
	PostMessage(hMainWnd, FPMSG_WINDOW_DEBUG_MSG, (WPARAM)id, (LPARAM)&mPalette[16]);
	FP_DEBUG_MSG(_T("Palette switched to No.%d.\n"), id);
	return S_OK;
}

HRESULT GameGraphics::InitPalette(const PPalLib pPal)
{
	HRESULT hResult;
	DWORD n, count;
	tstring filePath = pPal->palPath;
	tstring tmpPath;
	HANDLE hFile;
	LPBYTE buffer;

	//固定调色板的载入
	n = 0;
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00000000);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00800000);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00008000);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00808000);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00000080);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00800080);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00008080);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00808080);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00c0dcc0);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00a6caf0);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00de0000);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00ff5f00);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00ffffa0);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00005fd2);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x0050d2ff);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x0028e128);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00f5c396);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x001ea05f);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00c37d46);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x009b551e);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00464137);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x0028231e);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00fffbf0);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x003a6ea5);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00808080);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00ff0000);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x0000ff00);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00ffff00);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x000000ff);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00ff80ff);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x0000ffff);
	FillMemory(&mPaletteDefault[n++], sizeof(PALETTEENTRY), 0x00ffffff);

	//可变调色板的载入
	HANDLE hDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	PIOList list = IOList::CreateIOList(FPMSG_IO_READ_PALETTE, FP_FILE_COUNT_PAL, hDoneEvent);
	for (n = 0; n < list->count; n++)
	{
		list->SetIOListItem(n, 0, 0, FP_FILE_SIZE_PAL, mPaletteOptional[n]);
	}
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_PALETTE, (WPARAM)list, (LPARAM)GraphicsIOComplete);
	if (WaitForSingleObject(hDoneEvent, FP_THREAD_TIMEOUT) == WAIT_OBJECT_0)
	{
		FP_DEBUG_MSG(_T("Palette all loaded.\n"));
		hResult = S_OK;
	}
	else
	{
		ErrorHandler(ERROR_RES_RequestTimeout, _T(__FUNCTION__));
		hResult = E_HANDLE;
	}
	delete list;
	CloseHandle(hDoneEvent);
	mPaletteIndex = -1;
	return hResult;
}

HRESULT GameGraphics::LoadGraphicInfo()
{
	HRESULT hResult;
	HANDLE hFile = GetFileHandle(FP_HANDLE_GRAPHIC_INFO);
	DWORD dwSize = GetFileSize(hFile, NULL);
	DWORD dwCount = 0;
	if (dwSize % sizeof(GraphicInfo) != 0)
	{
		ErrorHandler(ERROR_RES_DigitalMismatch, _T(__FUNCTION__));
		return E_FAIL;
	}
	dwCount = dwSize / sizeof(GraphicInfo);
	mGraphicList.resize(dwCount);
	HANDLE hDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	PIOList list = IOList::CreateIOList(FPMSG_IO_READ_IMAGEINFO, 1, hDoneEvent);
	list->SetIOListItem(0, 0, 0, dwSize, &mGraphicList[0]);
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_IMAGEINFO, (WPARAM)list, (LPARAM)GraphicsIOComplete);
	if (WaitForSingleObject(hDoneEvent, FP_THREAD_TIMEOUT) == WAIT_OBJECT_0)
	{
		FP_DEBUG_MSG(_T("Graphic info loaded.\n"));
		hResult = S_OK;
	}
	else
	{
		ErrorHandler(ERROR_RES_RequestTimeout, _T(__FUNCTION__));
		hResult = E_HANDLE;
	}
	delete list;
	CloseHandle(hDoneEvent);
	return hResult;
}

HRESULT GameGraphics::LoadAnimeInfo()
{
	HRESULT hResult;
	HANDLE hFile = GetFileHandle(FP_HANDLE_ANIME_INFO);
	DWORD dwSize = GetFileSize(hFile, NULL);
	DWORD dwCount = 0;
	if (dwSize % sizeof(AnimeInfo) != 0)
	{
		ErrorHandler(ERROR_RES_DigitalMismatch, _T(__FUNCTION__));
		return E_FAIL;
	}
	dwCount = dwSize / sizeof(AnimeInfo);
	mAnimeList.resize(dwCount);
	HANDLE hDoneEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	PIOList list = IOList::CreateIOList(FPMSG_IO_READ_ANIMEINFO, 1, hDoneEvent);
	list->SetIOListItem(0, 0, 0, dwSize, &mAnimeList[0]);
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_ANIMEINFO, (WPARAM)list, (LPARAM)GraphicsIOComplete);
	if (WaitForSingleObject(hDoneEvent, FP_THREAD_TIMEOUT) == WAIT_OBJECT_0)
	{
		FP_DEBUG_MSG(_T("Anime info loaded.\n"));
		hResult = S_OK;
	}
	else
	{
		ErrorHandler(ERROR_RES_RequestTimeout, _T(__FUNCTION__));
		hResult = E_HANDLE;
	}
	delete list;
	CloseHandle(hDoneEvent);
	return hResult;
}

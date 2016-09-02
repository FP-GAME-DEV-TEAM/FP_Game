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
	pInstance->hGraphicInfo = NULL;
	pInstance->hGraphicData = NULL;
	pInstance->hAnimeInfo = NULL;
	pInstance->hAnimeData = NULL;
	ZeroMemory(pInstance->mPalette, FP_STORE_PAL_COUNT);
	ZeroMemory(pInstance->mPaletteDefault, FP_STORE_PAL_DEFAULT);
	ZeroMemory(pInstance->mPaletteOptional, FP_FILE_COUNT_PAL*FP_STORE_PAL_OPTIONAL);
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
	PIOList pList = (PIOList)pParam;
	PIOItem pItem = NULL;
	if (pList->isCompleted && pList->uIOType == FPMSG_IO_READ_GRAPHICDATA)
	{
		PGraphicData pGD = NULL;
		PFPImage pImg = NULL;
		DWORD dwSize = 0;
		KeyValue *kvList = new KeyValue[pList->nCount];
		for (LONG n = 0; n < pList->nCount; n++)
		{
			pItem = &pList->pItemList[n];
			pGD = (PGraphicData)pItem->pData;
			dwSize = pGD->width * pGD->height;
			pImg = (PFPImage)new BYTE[(sizeof(FPImage)-sizeof(DWORD)) + dwSize];
			pImg->width = pGD->width;
			pImg->height = pGD->height;
			pImg->offsetX = pInstance->mGraphicList[pItem->id].offsetX;
			pImg->offsetY = pInstance->mGraphicList[pItem->id].offsetY;
			if (pGD->flag % 2 == 0)
			{
				CopyMemory(pImg->data, (pGD + 1), dwSize);
			}
			else
			{
				DecryptData((LPBYTE)(pGD + 1), pGD->size - sizeof(GraphicData), dwSize, (LPBYTE)(pImg->data));
			}
			kvList[n].first = pItem->id;
			kvList[n].second = pImg;
			delete[] pItem->pData;
		}
		PostMessage(hMainWnd, FPMSG_IO_READ_GRAPHICDATA, pList->nCount, (LPARAM)kvList);
	}
	if (pList->isCompleted && pList->uIOType == FPMSG_IO_READ_ANIMEDATA)
	{
		PAnimeHead pAH = NULL;
		PAnimeData pAD = NULL;
		PFPAction pAct = NULL;
		PFPFrame pFrm = NULL;
		KeyValue *kvList = new KeyValue[pList->nCount];
		for (LONG n = 0; n < pList->nCount; n++)
		{
			pItem = &pList->pItemList[n];
			pAct = new FPAction();
			ZeroMemory(pAct, sizeof(FPAction));
			LPBYTE lpPtr = (LPBYTE)pItem->pData;
			WORD wCount = pInstance->mAnimeList[pItem->id].count;
			for (WORD i = 0; i < wCount; i++)
			{
				pAH = (PAnimeHead)lpPtr;
				lpPtr += sizeof(AnimeHead);
				pFrm = (PFPFrame)new BYTE[(sizeof(FPFrame)-sizeof(LONG)) + (pAH->frames)*sizeof(AnimeData)];
				pFrm->duration = pAH->duration;
				pFrm->count = pAH->frames;
				pAD = (PAnimeData)lpPtr;
				CopyMemory(pFrm->index, pAD, (pAH->frames)*sizeof(AnimeData));
				pAct->frame[pAH->facing][pAH->action] = pFrm;
				lpPtr += (pAH->frames)*sizeof(AnimeData);
			}
			kvList[n].first = pItem->id;
			kvList[n].second = pAct;
			delete[] pItem->pData;
		}
		PostMessage(hMainWnd, FPMSG_IO_READ_ANIMEDATA, pList->nCount, (LPARAM)kvList);
	}
	return IOCompleteDefault(pParam);
}

HRESULT GameGraphics::IODataBack(const UINT type, const LONG count, const LPVOID data)
{
	if (FPMSG_IO_READ_GRAPHICDATA == type)
	{
		KeyValue *kvList = (PKeyValue)data;
		ImageMap::iterator it;
		for (LONG i = 0; i < count; i++)
		{
			it = mGraphicCache.find(kvList[i].first);
			if (it == mGraphicCache.end())
			{
				return E_FAIL;
			}
			it->second = (PFPImage)kvList[i].second;
			PostMessage(hMainWnd, FPMSG_WINDOW_DEBUG_MSG, kvList[i].first, (LPARAM)kvList[i].second);
		}
		delete[] kvList;
		return S_OK;
	}
	if (FPMSG_IO_READ_ANIMEDATA == type)
	{
		KeyValue *kvList = (PKeyValue)data;
		ActionMap::iterator it;
		for (LONG i = 0; i < count; i++)
		{
			it = mAnimeCache.find(kvList[i].first);
			if (it == mAnimeCache.end())
			{
				return E_FAIL;
			}
			it->second = (PFPAction)kvList[i].second;
			PostMessage(hMainWnd, FPMSG_WINDOW_DEBUG_MSG, kvList[i].first, (LPARAM)kvList[i].second);
		}
		delete[] kvList;
		return S_OK;
	}
	return E_FAIL;
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

HRESULT GameGraphics::InitPalette(const PPalLib pPal)
{
	HRESULT hResult;
	LONG n;
	tstring filePath = pPal->palPath;
	tstring tmpPath;

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
	PIOList pList = IOList::CreateIOList(FPMSG_IO_READ_PALETTE, hDoneEvent, FP_FILE_COUNT_PAL);
	for (n = 0; n < pList->nCount; n++)
	{
		pList->SetIOListItem(n, n, 0, 0, FP_FILE_SIZE_PAL, mPaletteOptional[n]);
	}
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_PALETTE, (WPARAM)pList, (LPARAM)GraphicsIOComplete);
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
	CloseHandle(hDoneEvent);
	mPaletteIndex = -1;
	return hResult;
}

HRESULT GameGraphics::InitGraphicInfo()
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
	PIOList pList = IOList::CreateIOList(FPMSG_IO_READ_GRAPHICINFO, hDoneEvent, 1);
	pList->SetIOListItem(0, 0, 0, 0, dwSize, &mGraphicList[0]);
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_GRAPHICINFO, (WPARAM)pList, (LPARAM)GraphicsIOComplete);
	if (WaitForSingleObject(hDoneEvent, FP_THREAD_TIMEOUT) == WAIT_OBJECT_0)
	{
		FP_DEBUG_MSG(_T("Graphic info loaded.\n"));
		hResult = S_OK;
	}
	else
	{
		ErrorHandler(ERROR_RES_RequestTimeout, _T(__FUNCTION__));
		FP_DEBUG_MSG(_T("Failed to load graphic info.\n"));
		hResult = E_HANDLE;
	}
	CloseHandle(hDoneEvent);
	return hResult;
}

HRESULT GameGraphics::InitAnimeInfo()
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
	PIOList pList = IOList::CreateIOList(FPMSG_IO_READ_ANIMEINFO, hDoneEvent, 1);
	pList->SetIOListItem(0, 0, 0, 0, dwSize, &mAnimeList[0]);
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_ANIMEINFO, (WPARAM)pList, (LPARAM)GraphicsIOComplete);
	if (WaitForSingleObject(hDoneEvent, FP_THREAD_TIMEOUT) == WAIT_OBJECT_0)
	{
		FP_DEBUG_MSG(_T("Anime info loaded.\n"));
		hResult = S_OK;
	}
	else
	{
		ErrorHandler(ERROR_RES_RequestTimeout, _T(__FUNCTION__));
		FP_DEBUG_MSG(_T("Failed to load anime info.\n"));
		hResult = E_HANDLE;
	}
	CloseHandle(hDoneEvent);
	return hResult;
}

HRESULT GameGraphics::GetGraphicInfo(const LONG id, const GraphicInfo **pInfo)
{
	if (id < 0 || id >= mGraphicList.size())
	{
		return E_FAIL;
	}
	if (NULL != pInfo)
	{
		*pInfo = &mGraphicList[id];
	}
	return S_OK;
}

HRESULT GameGraphics::GetAnimeInfo(const LONG id, const AnimeInfo **pInfo)
{
	if (id < 0 || id >= mAnimeList.size())
	{
		return E_FAIL;
	}
	if (NULL != pInfo)
	{
		*pInfo = &mAnimeList[id];
	}
	return S_OK;
}

LONG GameGraphics::GetCurrentPaletteIndex()
{
	return this->mPaletteIndex;
}

HRESULT GameGraphics::ChangePalette(const LONG id, const PALETTEENTRY **pData)
{
	DWORD i, n = 0;
	if (id<0 || id >= FP_FILE_COUNT_PAL)
	{
		return E_FAIL;
	}
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

HRESULT GameGraphics::GetImage(const LONG id, const FPImage **pData)
{
	if (id < 0 || id >= mGraphicList.size() || NULL == pData)
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
	mImageReqQueue.insert(id);
	*pData = NULL;
	return S_FALSE;
}

HRESULT GameGraphics::GetAction(const LONG id, const FPAction **pData)
{
	if (id < 0 || id >= mAnimeList.size() || NULL == pData)
	{
		return E_FAIL;
	}
	ActionMap::const_iterator it = mAnimeCache.find(id);
	if (it != mAnimeCache.end())
	{
		*pData = it->second;
		return S_OK;
	}
	mAnimeCache[id] = NULL;
	mActionReqQueue.insert(id);
	*pData = NULL;
	return NULL;
}

HRESULT GameGraphics::LoopIORequest(const DWORD dwTick)
{
	LONG n, count;
	BinReqSet::const_iterator it;
	PIOList pList = NULL;
	// Loop GraphicData IO Requests
	n = 0;
	count = mImageReqQueue.size();
	if (count > 0)
	{
		pList = IOList::CreateIOList(FPMSG_IO_READ_GRAPHICDATA, NULL, count);
		for (it = mImageReqQueue.cbegin(); it != mImageReqQueue.cend(); it++)
		{
			const GraphicInfo *pGraphic;
			if (FAILED(GetGraphicInfo((*it), &pGraphic)))
			{
				continue;
			}
			if (NULL != pGraphic)
			{
				pList->SetIOListItem(n, (*it), 0, pGraphic->addr, pGraphic->size, new BYTE[pGraphic->size]);
			}
			n++;
		}
		PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_GRAPHICDATA, (WPARAM)pList, (LPARAM)GraphicsIOComplete);
		mImageReqQueue.clear();
	}
	// Loop AnimeData IO Requests
	n = 0;
	count = mActionReqQueue.size();
	if (count > 0)
	{
		pList = IOList::CreateIOList(FPMSG_IO_READ_ANIMEDATA, NULL, count);
		for (it = mActionReqQueue.cbegin(); it != mActionReqQueue.cend(); it++)
		{
			const AnimeInfo *pAnime = NULL;
			if (FAILED(GetAnimeInfo((*it), &pAnime)))
			{
				continue;
			}
			if (NULL != pAnime)
			{
				pList->SetIOListItem(n, (*it), 0, pAnime->addr, pAnime->size, new BYTE[pAnime->size]);
			}
			n++;
		}
		PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_ANIMEDATA, (WPARAM)pList, (LPARAM)GraphicsIOComplete);
		mActionReqQueue.clear();
	}
	return S_OK;
}

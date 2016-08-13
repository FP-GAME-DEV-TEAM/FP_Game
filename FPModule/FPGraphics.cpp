/********************************
* FileName: FPGraphics.cpp
* FileFunc: FP��Ϸͼ����Դʵ��ģ��
* Author: SQK
* Date: 2016-03-15
* Descript: ��Ϸͼ����Դ������ӿ�
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPDump.h"


GameGraphics *mainGraphics = NULL;

extern HWND hMainWnd;

//=====================================
// GameGraphics���ʵ��
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
	if (NULL != pInstance) //����Ƿ��Ѿ���ʼ��
	{
		mainGraphics = pInstance;
		return E_HANDLE;
	}
	if (NULL == pBin) //���BinLib�Ƿ���Ч
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
	//�ͷ��ļ���Դ���
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

HANDLE GameGraphics::GetFileHandle(const UINT type) const
{
	HANDLE handle = NULL;
	switch (type)
	{
	case FP_HANDLE_GRAPHIC_DATA:
		handle = this->hGraphicData;
		break;
	case FP_HANDLE_ANIME_DATA:
		handle = this->hAnimeData;
		break;
	case FP_HANDLE_GRAPHIC_INFO:
		handle = this->hGraphicInfo;
		break;
	case FP_HANDLE_ANIME_INFO:
		handle = this->hAnimeInfo;
		break;
	default:
		break;
	}
	return handle;
}

HRESULT GameGraphics::GetImageById(LONG id, LPVOID pData)
{
	return NULL;
}

HRESULT GameGraphics::GetAnimeById(LONG id, LPVOID pData)
{
	return NULL;
}

HRESULT GameGraphics::SwitchPalette(LONG id)
{
	DWORD i, n=0;
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
	PostMessage(hMainWnd, FPMSG_WINDOW_DEBUG_MSG, (WPARAM)id, (LPARAM)&mPalette[16]);
	FP_DEBUG_MSG(_T("Palette switched to No.%d.\n"), id);
	return S_OK;
}

static UINT CALLBACK FreeIOList(LPVOID pParam)
{
	PIOList list = (PIOList)pParam;
	PIOItem item = list->pList;
	delete[] item;
	delete list;
	return 0;
}

HRESULT GameGraphics::InitPalette(const PPalLib pPal)
{
	DWORD n, count;
	tstring filePath = pPal->palPath;
	tstring tmpPath;
	HANDLE hFile;
	LPBYTE buffer;

	//�̶���ɫ�������
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

	//�ɱ��ɫ�������
	PIOList list = new IOList();
	list->count = FP_FILE_COUNT_PAL;
	PIOItem item = new IOItem[FP_FILE_COUNT_PAL];
	for (n = 0; n < FP_FILE_COUNT_PAL; n++)
	{
		ZeroMemory(&item[n], sizeof(IOItem));
		item[n].isCompleted = FALSE;
		item[n].offset = 0;
		item[n].size = FP_FILE_SIZE_PAL;
		item[n].pData = mPaletteOptional[n];
	}
	list->pList = item;
	PostThreadMessage(mainIOThread.uThreadId, FPMSG_IO_READ_PALETTE, (WPARAM)list, (LPARAM)FreeIOList);
	return S_OK;
}

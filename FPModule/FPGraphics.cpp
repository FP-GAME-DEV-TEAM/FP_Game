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

extern GameEnv *mainEnv;

//=====================================
//初始化图形模块
//

FP_MODULE_API VOID WINAPI InitGraphics()
{
	GameGraphics::Create(mainEnv->GetBinLib());
}

GameGraphics *GameGraphics::pInstance = NULL;

GameGraphics::GameGraphics()
{
	this->hGraphicInfo = NULL;
	this->hGraphicData = NULL;
	this->hAnimeInfo = NULL;
	this->hAnimeData = NULL;
	ZeroMemory(this->pPalette, FP_STORE_PAL_COUNT);
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

HANDLE GameGraphics::GetFileHandle(const int type) const
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
	return S_OK;
}

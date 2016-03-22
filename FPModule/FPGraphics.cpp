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

//=====================================
//��ʼ��ͼ��ģ��
//

FP_MODULE_API BOOL WINAPI InitGraphics(const IGameEnv &IEnv)
{
	LPCTSTR s = IEnv.GetRootPath();
	return TRUE;
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
	if (NULL != pInstance) //����Ƿ��Ѿ���ʼ��
	{
		mainGraphics = pInstance;
		return E_HANDLE;
	}
	if (NULL != pBin) //���BinLib�Ƿ���Ч
	{
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_FAIL;
	}
	pInstance = new GameGraphics();
	path = pBin->binPath;
	pInstance->hGraphicInfo = CreateFile((path + pBin->sGraphicInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	pInstance->hGraphicData = CreateFile((path + pBin->sGraphicData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	pInstance->hAnimeInfo = CreateFile((path + pBin->sAnimeInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	pInstance->hAnimeData = CreateFile((path + pBin->sAnimeData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
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
	//�ͷ�Ӱ����Դ���
	if (pInstance->hGraphicInfo != NULL && pInstance->hGraphicInfo != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hGraphicInfo);
	if (pInstance->hGraphicData != NULL && pInstance->hGraphicData != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hGraphicData);
	if (pInstance->hAnimeInfo != NULL && pInstance->hAnimeInfo != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hAnimeInfo);
	if (pInstance->hAnimeData != NULL && pInstance->hAnimeData != INVALID_HANDLE_VALUE)
		CloseHandle(pInstance->hAnimeData);
	// Free main environment
	SAFE_DELETE(pInstance);
}

HANDLE GameGraphics::GetFileHandle(int type) const
{
	return NULL;
}


//ͨ��ID�õ�ͼƬ
HRESULT GameGraphics::GetImageById(LONG id, LPVOID pData)
{
	return NULL;
}

//ͨ��ID�õ�����
HRESULT GameGraphics::GetAnimeById(LONG id, LPVOID pData)
{
	return NULL;
}

//������ɫ��
HRESULT GameGraphics::SwitchPalette(LONG id)
{
	return S_OK;
}

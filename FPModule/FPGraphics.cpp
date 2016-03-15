/********************************
* FileName: FPGraphics.cpp
* FileFunc: FP��Ϸͼ����Դʵ��ģ��
* Author: SQK
* Date: 2016-03-15
* Descript: ��Ϸͼ����Դ������ӿ�
********************************/

#include "stdafx.h"
#include "FPDataType.h"


//=====================================
//��ʼ��ͼ��ģ��
//

FP_MODULE_API BOOL WINAPI InitGraphics(const IGameEnv &IEnv)
{
	LPCTSTR s = IEnv.GetRootPath();
	return TRUE;
}

//���캯��
GameGraphics::GameGraphics(const PBinLib pBin)
{
	tstring path;
	path = pBin->binPath;
	hGraphicInfo = CreateFile((path + pBin->sGraphicInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	hGraphicData = CreateFile((path + pBin->sGraphicData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	hAnimeInfo = CreateFile((path + pBin->sAnimeInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	hAnimeData = CreateFile((path + pBin->sAnimeData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
}

//��������
GameGraphics::~GameGraphics()
{
	//�ͷ�Ӱ����Դ���
	if (hGraphicInfo != NULL && hGraphicInfo != INVALID_HANDLE_VALUE)
		CloseHandle(hGraphicInfo);
	if (hGraphicData != NULL && hGraphicData != INVALID_HANDLE_VALUE)
		CloseHandle(hGraphicData);
	if (hAnimeInfo != NULL && hAnimeInfo != INVALID_HANDLE_VALUE)
		CloseHandle(hAnimeInfo);
	if (hAnimeData != NULL && hAnimeData != INVALID_HANDLE_VALUE)
		CloseHandle(hAnimeData);
}

//ͨ��ID�õ�ͼƬ
LPCVOID GameGraphics::GetImageById(int id) const
{
	return NULL;
}

//ͨ��ID�õ�����
LPCVOID GameGraphics::GetAnimeById(int id) const
{
	return NULL;
}

//������ɫ��
HRESULT GameGraphics::SwitchPalette(int id) const
{
	return S_OK;
}

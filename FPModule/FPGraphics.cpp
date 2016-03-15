/********************************
* FileName: FPGraphics.cpp
* FileFunc: FP游戏图像资源实现模块
* Author: SQK
* Date: 2016-03-15
* Descript: 游戏图像资源的载入接口
********************************/

#include "stdafx.h"
#include "FPDataType.h"


//=====================================
//初始化图形模块
//

FP_MODULE_API BOOL WINAPI InitGraphics(const IGameEnv &IEnv)
{
	LPCTSTR s = IEnv.GetRootPath();
	return TRUE;
}

//构造函数
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

//析构函数
GameGraphics::~GameGraphics()
{
	//释放影像资源句柄
	if (hGraphicInfo != NULL && hGraphicInfo != INVALID_HANDLE_VALUE)
		CloseHandle(hGraphicInfo);
	if (hGraphicData != NULL && hGraphicData != INVALID_HANDLE_VALUE)
		CloseHandle(hGraphicData);
	if (hAnimeInfo != NULL && hAnimeInfo != INVALID_HANDLE_VALUE)
		CloseHandle(hAnimeInfo);
	if (hAnimeData != NULL && hAnimeData != INVALID_HANDLE_VALUE)
		CloseHandle(hAnimeData);
}

//通过ID得到图片
LPCVOID GameGraphics::GetImageById(int id) const
{
	return NULL;
}

//通过ID得到动画
LPCVOID GameGraphics::GetAnimeById(int id) const
{
	return NULL;
}

//更换调色板
HRESULT GameGraphics::SwitchPalette(int id) const
{
	return S_OK;
}

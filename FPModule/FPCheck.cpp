/********************************
* FileName: FPCheck.cpp
* FileFunc: FP游戏资源类型实现模块
* Author: SQK
* Date: 2013-07-27
* Descript: 游戏全部资源的准备和完整性检查
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPDump.h"

BOOL WINAPI CheckPalLib(const tstring whichPath, PPalLib &pal)
{
	HANDLE hFile;
	WIN32_FIND_DATA ffd;
	tstring filePath, tmpPath; //临时字符串
	tstring::size_type pos, size; //查找位置
	int n = 0; //临时数组索引
	BOOL failed = FALSE; //是否检查失败
	//检查目录
	if (!IsFolderExist(whichPath))
	{
		ErrorHandler(ERROR_RES_MissingPath, _T(__FUNCTION__));
		return FALSE;
	}
	pal->fileList = new LPCTSTR[FP_FILE_COUNT_PAL];
	ZeroMemory(pal->fileList, FP_FILE_COUNT_PAL * sizeof(LPCTSTR));

	//检查调色板文件
	filePath = whichPath + _T("\\*");
	filePath += FP_FILE_SUFFIX_PAL;
	hFile = FindFirstFile(filePath.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE == hFile)
	{
		ErrorHandler(ERROR_RES_MissingFile, _T(__FUNCTION__));
		return FALSE;
	}
	do
	{
		tmpPath = ffd.cFileName;
		pos = tmpPath.find(_T("_")) + 1;
		size = tmpPath.rfind(_T(".")) - pos;
		tmpPath = tmpPath.substr(pos, size);
		if (2 == tmpPath.length() && isdigit(tmpPath[0]) && isdigit(tmpPath[1]))
		{
			n = _ttoi(tmpPath.c_str());
			if (n < 0 || n > FP_FILE_COUNT_PAL || NULL != pal->fileList[n])
			{
				ErrorHandler(ERROR_RES_DuplicatedFile, _T(__FUNCTION__));
				failed = TRUE;
				break;
			}
			else
			{
				pal->fileList[n] = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
				pal->sum++;
				continue;
			}
		}
		else
		{
			ErrorHandler(ERROR_RES_MissingFile, _T(__FUNCTION__));
			failed = TRUE;
			break;
		}
	} while (FindNextFile(hFile, &ffd));
	FindClose(hFile);
	//数量检查出问题
	if (failed || FP_FILE_COUNT_PAL != pal->sum)
	{
		ErrorHandler(ERROR_RES_FilesSumMismatch, _T(__FUNCTION__));
		SAFE_DELETE_ARRAY(pal->fileList);
		return FALSE;
	}
	pal->palPath = NewPathString(whichPath, MAX_PATH);

	return TRUE;
}

BOOL WINAPI CheckBinLib(const tstring whichPath, PBinLib &bin)
{
	HANDLE hFile;
	WIN32_FIND_DATA ffd;
	DWORD nVerGraph, nVerAnime, nTmp; //临时版本号
	tstring filePath, tmpPath, sVerGraph, sVerAnime; //临时字符串
	tstring::size_type pos, size; //查找位置
	//检查目录
	if (!IsFolderExist(whichPath))
	{
		ErrorHandler(ERROR_RES_MissingPath, _T(__FUNCTION__));
		return FALSE;
	}

	//图像目录文件
	filePath = whichPath + _T("\\") + FP_FILE_GRAPHIC_INFO;
	filePath += _T("*");
	filePath += FP_FILE_SUFFIX_BIN;
	hFile = FindFirstFile(filePath.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		tmpPath = ffd.cFileName;
		pos = tmpPath.rfind(_T("_")) + 1;
		size = tmpPath.rfind(_T(".")) - pos;
		sVerGraph = tmpPath.substr(pos, size);
		nVerGraph = _ttoi(sVerGraph.c_str());
		if (FindNextFile(hFile, &ffd)) //重复文件处理
		{
			ErrorHandler(ERROR_RES_DuplicatedFile, _T(__FUNCTION__));
			FindClose(hFile);
			return FALSE;
		}
		bin->sGraphicInfo = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
		FindClose(hFile);
	}
	else
	{
		ErrorHandler(ERROR_RES_MissingFile, _T(__FUNCTION__));
		return FALSE;
	}
	//图像数据文件
	filePath = whichPath + _T("\\") + FP_FILE_GRAPHIC_DATA;
	filePath += _T("*");
	filePath += FP_FILE_SUFFIX_BIN;
	hFile = FindFirstFile(filePath.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		tmpPath = ffd.cFileName;
		pos = tmpPath.rfind(_T("_")) + 1;
		size = tmpPath.rfind(_T(".")) - pos;
		sVerGraph = tmpPath.substr(pos, size);
		nTmp = _ttoi(sVerGraph.c_str());
		if (FindNextFile(hFile, &ffd)) //重复文件处理
		{
			ErrorHandler(ERROR_RES_DuplicatedFile, _T(__FUNCTION__));
			FindClose(hFile);
			return FALSE;
		}
		bin->sGraphicData = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
		FindClose(hFile);
	}
	else
	{
		ErrorHandler(ERROR_RES_MissingFile, _T(__FUNCTION__));
		return FALSE;
	}
	//判断图像版本是否一致
	if (nTmp != nVerGraph)
	{
		ErrorHandler(ERROR_RES_VersionMismatch, _T(__FUNCTION__));
		return FALSE;
	}

	//动画目录文件
	filePath = whichPath + _T("\\") + FP_FILE_ANIME_INFO;
	filePath += _T("*");
	filePath += FP_FILE_SUFFIX_BIN;
	hFile = FindFirstFile(filePath.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		tmpPath = ffd.cFileName;
		pos = tmpPath.rfind(_T("_")) + 1;
		size = tmpPath.rfind(_T(".")) - pos;
		sVerAnime = tmpPath.substr(pos, size);
		nVerAnime = _ttoi(sVerAnime.c_str());
		if (FindNextFile(hFile, &ffd)) //重复文件处理
		{
			ErrorHandler(ERROR_RES_DuplicatedFile, _T(__FUNCTION__));
			FindClose(hFile);
			return FALSE;
		}
		bin->sAnimeInfo = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
		FindClose(hFile);
	}
	else
	{
		ErrorHandler(ERROR_RES_MissingFile, _T(__FUNCTION__));
		return FALSE;
	}
	//动画数据文件
	filePath = whichPath + _T("\\") + FP_FILE_ANIME_DATA;
	filePath += _T("*");
	filePath += FP_FILE_SUFFIX_BIN;
	hFile = FindFirstFile(filePath.c_str(), &ffd);
	if (INVALID_HANDLE_VALUE != hFile)
	{
		tmpPath = ffd.cFileName;
		pos = tmpPath.rfind(_T("_")) + 1;
		size = tmpPath.rfind(_T(".")) - pos;
		sVerAnime = tmpPath.substr(pos, size);
		nTmp = _ttoi(sVerAnime.c_str());
		if (FindNextFile(hFile, &ffd)) //重复文件处理
		{
			ErrorHandler(ERROR_RES_DuplicatedFile, _T(__FUNCTION__));
			FindClose(hFile);
			return FALSE;
		}
		bin->sAnimeData = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
		FindClose(hFile);
	}
	else
	{
		ErrorHandler(ERROR_RES_MissingFile, _T(__FUNCTION__));
		return FALSE;
	}
	//判断动画版本是否一致
	if (nTmp != nVerAnime)
	{
		ErrorHandler(ERROR_RES_VersionMismatch, _T(__FUNCTION__));
		return FALSE;
	}
	bin->binPath = NewPathString(whichPath, MAX_PATH);
	bin->verGraphic = nVerGraph;
	bin->verAnime = nVerAnime;

	//检查调色板
	bin->pLibPal = new PalLib();
	ZeroMemory(bin->pLibPal, sizeof(PalLib));
	if (!CheckPalLib(whichPath + FP_PATH_PAL, bin->pLibPal))
	{
		SAFE_DELETE(bin->pLibPal);
		return FALSE;
	}

	//音效，BGM

	return TRUE;
}

BOOL WINAPI CheckDataSet(const tstring whichPath, PDataSet &data)
{
	return TRUE;
}

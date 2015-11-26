/********************************
* FileName: FPCheck.cpp
* FileFunc: FP游戏资源类型实现模块
* Author: SQK
* Date: 2013-07-27
* Descript: 游戏全部资源的准备和完整性检查
********************************/

#include "stdafx.h"
#include "FPModule.h"
#include "FPDataType.h"
#include "FPDump.h"

//资源检查函数声明
static BOOL WINAPI CheckPalLib(const tstring whichPath, PPalLib &pal);
static BOOL WINAPI CheckBinLib(const tstring whichPath, PBinLib &pLib);
static BOOL WINAPI CheckDataSet(const tstring whichPath, PDataSet &pLib);

extern GameEnv *mainEnv;

//=====================================
// GameEnv类中，资源检查部分的实现
//

GameEnv *GameEnv::pEnv = NULL;

GameEnv::GameEnv(const PGameRes pRes)
{
	tstring path;
	pGameRes = pRes;
	//生成二进制资源库
	pGraphic = new GraphicLink();
	path = pRes->pBinLib->binPath;
	pGraphic->hGraphicInfo = CreateFile((path + pRes->pBinLib->sGraphicInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	pGraphic->hGraphicData = CreateFile((path + pRes->pBinLib->sGraphicData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	pGraphic->hAnimeInfo = CreateFile((path + pRes->pBinLib->sAnimeInfo).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	pGraphic->hAnimeData = CreateFile((path + pRes->pBinLib->sAnimeData).c_str(), GENERIC_READ, FILE_SHARE_READ,
		NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
}

GameEnv::~GameEnv()
{
	//释放影像资源句柄
	if (this->pGraphic)
	{
		if (this->pGraphic->hGraphicInfo != NULL && this->pGraphic->hGraphicInfo != INVALID_HANDLE_VALUE)
			CloseHandle(this->pGraphic->hGraphicInfo);
		if (this->pGraphic->hGraphicData != NULL && this->pGraphic->hGraphicData != INVALID_HANDLE_VALUE)
			CloseHandle(this->pGraphic->hGraphicData);
		if (this->pGraphic->hAnimeInfo != NULL && this->pGraphic->hAnimeInfo != INVALID_HANDLE_VALUE)
			CloseHandle(this->pGraphic->hAnimeInfo);
		if (this->pGraphic->hAnimeData != NULL && this->pGraphic->hAnimeData != INVALID_HANDLE_VALUE)
			CloseHandle(this->pGraphic->hAnimeData);
	}
}

PGameRes WINAPI GameEnv::OpenResFiles(const tstring whichPath)
{
	tstring subPath;
	PGameRes pRes = new GameRes();
	ZeroMemory(pRes, sizeof(GameRes));
	pRes->rootPath = NewPath(whichPath, MAX_PATH);
	//检查二进制文件
	subPath = whichPath + FP_PATH_BIN;
	pRes->pBinLib = new BinLib();
	ZeroMemory(pRes->pBinLib, sizeof(BinLib));
	if (!CheckBinLib(subPath, pRes->pBinLib))
	{
		SAFE_DELETE(pRes->pBinLib);
		SAFE_DELETE(pRes);
		return NULL;
	}
	//检查配置文件
	subPath = whichPath + FP_PATH_DAT;
	pRes->pDataSet = new DataSet();
	ZeroMemory(pRes->pDataSet, sizeof(DataSet));
	if (!CheckDataSet(subPath, pRes->pDataSet))
	{
		SAFE_DELETE(pRes);
		return NULL;
	}
	return pRes;
}

HRESULT WINAPI GameEnv::ValidateFile(HANDLE hFile)
{
	return FALSE;
}

HRESULT WINAPI GameEnv::InitEnv(const tstring whichPath)
{
	if (NULL != pEnv) //检查是否已经初始化
	{
		mainEnv = pEnv;
		return E_HANDLE;
	}
	if (_T('\\') == whichPath[whichPath.length() - 1]) //检查结尾反斜杠符
	{
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_FAIL;
	}
	if (!IsFolderExist(whichPath)) //检查路径是否存在
	{
		ErrorHandler(ERROR_RES_MissingPath, _T(__FUNCTION__));
		return E_FAIL;
	}
	PGameRes pRes = OpenResFiles(whichPath);
	if (NULL != pRes) //检查资源文件完整性
	{
		pEnv = new GameEnv(pRes);
		mainEnv = pEnv;
		return S_OK;
	}
	return E_FAIL;
}

void WINAPI GameEnv::ReleaseEnv()
{
	// Free graphics data
	SAFE_DELETE(pEnv->pGraphic);
	// Free user data
	SAFE_DELETE(pEnv->pGameRes->pDataSet);
	// Free palette data
	SAFE_DELETE(pEnv->pGameRes->pBinLib->pLibPal->palPath);
	SAFE_DELETE_ARRAY(pEnv->pGameRes->pBinLib->pLibPal->fileList);
	SAFE_DELETE(pEnv->pGameRes->pBinLib->pLibPal);
	// Free bin data
	SAFE_DELETE(pEnv->pGameRes->pBinLib->binPath);
	SAFE_DELETE(pEnv->pGameRes->pBinLib->sAnimeData);
	SAFE_DELETE(pEnv->pGameRes->pBinLib->sAnimeInfo);
	SAFE_DELETE(pEnv->pGameRes->pBinLib->sGraphicData);
	SAFE_DELETE(pEnv->pGameRes->pBinLib->sGraphicInfo);
	SAFE_DELETE(pEnv->pGameRes->pBinLib);
	// Free game resource
	SAFE_DELETE(pEnv->pGameRes);
	// Free main environment
	SAFE_DELETE(pEnv);
}

LPCTSTR GameEnv::GetRootPath() const
{
	return this->pGameRes->rootPath;
}

LPCTSTR GameEnv::GetBinPath() const
{
	return this->pGameRes->pBinLib->binPath;
}

LPCTSTR GameEnv::GetDataPath() const
{
	return this->pGameRes->pDataSet->dataPath;
}

LPCTSTR GameEnv::GetMapPath() const
{
	return this->pGameRes->pMapPack->mapPath;
}


BinLib &GameEnv::GetBinLib() const
{
	return *(this->pGameRes->pBinLib);
}

PalLib &GameEnv::GetPaletteLib() const
{
	return *(this->pGameRes->pBinLib->pLibPal);
}

SndLib &GameEnv::GetSoundLib() const
{
	return *(this->pGameRes->pBinLib->pLibSnd);
}

BgmLib &GameEnv::GetBGMLib() const
{
	return *(this->pGameRes->pBinLib->pLibBgm);
}


//=====================================
//内部检查函数
//
static BOOL WINAPI CheckPalLib(const tstring whichPath, PPalLib &pal)
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
				pal->fileList[n] = NewPath(ffd.cFileName, FP_FILE_NAME_SIZE);
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
	pal->palPath = NewPath(whichPath, MAX_PATH);

	return TRUE;
}

static BOOL WINAPI CheckBinLib(const tstring whichPath, PBinLib &bin)
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
		bin->sGraphicInfo = NewPath(ffd.cFileName, FP_FILE_NAME_SIZE);
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
		bin->sGraphicData = NewPath(ffd.cFileName, FP_FILE_NAME_SIZE);
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
		bin->sAnimeInfo = NewPath(ffd.cFileName, FP_FILE_NAME_SIZE);
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
		bin->sAnimeData = NewPath(ffd.cFileName, FP_FILE_NAME_SIZE);
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
	bin->binPath = NewPath(whichPath, MAX_PATH);
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

static BOOL WINAPI CheckDataSet(const tstring whichPath, PDataSet &data)
{
	return TRUE;
}


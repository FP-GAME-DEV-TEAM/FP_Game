/********************************
* FileName: FPCheck.cpp
* FileFunc: FP��Ϸ��Դ����ʵ��ģ��
* Author: SQK
* Date: 2013-07-27
* Descript: ��Ϸȫ����Դ��׼���������Լ��
********************************/

#include "stdafx.h"
#include "FPModule.h"
#include "FPDataType.h"
#include "FPFunction.h"

//��Դ��麯������
static BOOL WINAPI CheckPalLib(const tstring whichPath, PPalLib &pal);
static BOOL WINAPI CheckBinLib(const tstring whichPath, PBinLib &pLib);
static BOOL WINAPI CheckDataSet(const tstring whichPath, PDataSet &pLib);


//================================
// GameEnv���У���Դ��鲿�ֵ�ʵ��
//

GameEnv *GameEnv::pEnv = NULL;

GameEnv::GameEnv(const PGameRes pRes)
{
	tstring path;
	pGameRes = pRes;
	//���ɶ�������Դ��
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
	//�ͷ�Ӱ����Դ���
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
	//���������ļ�
	subPath = whichPath + FP_PATH_BIN;
	pRes->pBinLib = new BinLib();
	ZeroMemory(pRes->pBinLib, sizeof(BinLib));
	if (!CheckBinLib(subPath, pRes->pBinLib))
	{
		SAFE_DELETE(pRes->pBinLib);
		SAFE_DELETE(pRes);
		return NULL;
	}
	//��������ļ�
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
	if (NULL != pEnv) //����Ƿ��Ѿ���ʼ��
	{
		return E_FAIL;
	}
	if (_T('\\') == whichPath[whichPath.length() - 1]) //����β��б�ܷ�
	{
		ErrorHandler(ERROR_RES_Unknown, _T(__FUNCTION__));
		return E_FAIL;
	}
	if (!IsFolderExist(whichPath)) //���·���Ƿ����
	{
		ErrorHandler(ERROR_RES_MissingPath, _T(__FUNCTION__));
		return E_FAIL;
	}
	PGameRes pRes = OpenResFiles(whichPath);
	if (NULL != pRes) //�����Դ�ļ�������
	{
		pEnv = new GameEnv(pRes);
		return S_OK;
	}
	return E_FAIL;
}

GameEnv* WINAPI GameEnv::GetEnv()
{
	return pEnv;
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


//================================
//�ڲ���麯��
//
static BOOL WINAPI CheckPalLib(const tstring whichPath, PPalLib &pal)
{
	HANDLE hFile;
	WIN32_FIND_DATA ffd;
	tstring filePath, tmpPath; //��ʱ�ַ���
	tstring::size_type pos, size; //����λ��
	int n = 0; //��ʱ��������
	BOOL failed = FALSE; //�Ƿ���ʧ��
	//���Ŀ¼
	if (!IsFolderExist(whichPath))
	{
		ErrorHandler(ERROR_RES_MissingPath, _T(__FUNCTION__));
		return FALSE;
	}
	pal->fileList = new LPCTSTR[FP_FILE_COUNT_PAL];
	ZeroMemory(pal->fileList, FP_FILE_COUNT_PAL * sizeof(LPCTSTR));

	//����ɫ���ļ�
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
	//������������
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
	DWORD nVerGraph, nVerAnime, nTmp; //��ʱ�汾��
	tstring filePath, tmpPath, sVerGraph, sVerAnime; //��ʱ�ַ���
	tstring::size_type pos, size; //����λ��
	//���Ŀ¼
	if (!IsFolderExist(whichPath))
	{
		ErrorHandler(ERROR_RES_MissingPath, _T(__FUNCTION__));
		return FALSE;
	}

	//ͼ��Ŀ¼�ļ�
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
		if (FindNextFile(hFile, &ffd)) //�ظ��ļ�����
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
	//ͼ�������ļ�
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
		if (FindNextFile(hFile, &ffd)) //�ظ��ļ�����
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
	//�ж�ͼ��汾�Ƿ�һ��
	if (nTmp != nVerGraph)
	{
		ErrorHandler(ERROR_RES_VersionMismatch, _T(__FUNCTION__));
		return FALSE;
	}

	//����Ŀ¼�ļ�
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
		if (FindNextFile(hFile, &ffd)) //�ظ��ļ�����
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
	//���������ļ�
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
		if (FindNextFile(hFile, &ffd)) //�ظ��ļ�����
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
	//�ж϶����汾�Ƿ�һ��
	if (nTmp != nVerAnime)
	{
		ErrorHandler(ERROR_RES_VersionMismatch, _T(__FUNCTION__));
		return FALSE;
	}
	bin->binPath = NewPath(whichPath, MAX_PATH);
	bin->verGraphic = nVerGraph;
	bin->verAnime = nVerAnime;

	//����ɫ��
	bin->pLibPal = new PalLib();
	ZeroMemory(bin->pLibPal, sizeof(PalLib));
	if (!CheckPalLib(whichPath + FP_PATH_PAL, bin->pLibPal))
	{
		SAFE_DELETE(bin->pLibPal);
		return FALSE;
	}

	//��Ч��BGM

	return TRUE;
}

static BOOL WINAPI CheckDataSet(const tstring whichPath, PDataSet &data)
{
	return TRUE;
}


/********************************
* FileName: FPCheck.cpp
* FileFunc: FP��Ϸ��Դ��ʵ��ģ��
* Author: SQK
* Date: 2016-03-16
* Descript: ��Ϸȫ����Դ��Ϣ��ʵ��
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPDump.h"

//��Դ��麯������
extern BOOL WINAPI CheckPalLib(const tstring whichPath, PPalLib &pal);
extern BOOL WINAPI CheckBinLib(const tstring whichPath, PBinLib &pLib);
extern BOOL WINAPI CheckDataSet(const tstring whichPath, PDataSet &pLib);

GameEnv *mainEnv = NULL;

//=====================================
// GameEnv���У���Դ��鲿�ֵ�ʵ��
//

GameEnv *GameEnv::pEnv = NULL;

GameEnv::GameEnv(const PGameRes pRes)
{
	pGameRes = pRes;
	//����ͼ����Դ��
	pGraphics = new GameGraphics(pRes->pBinLib);
}

GameEnv::~GameEnv()
{
	// Free graphics interface
	SAFE_DELETE(pEnv->pGraphics);
}

PGameRes WINAPI GameEnv::OpenResFiles(const tstring whichPath)
{
	tstring subPath;
	PGameRes pRes = new GameRes();
	ZeroMemory(pRes, sizeof(GameRes));
	pRes->rootPath = NewPathString(whichPath, MAX_PATH);
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

HRESULT WINAPI GameEnv::OpenEnv(const tstring whichPath)
{
	if (NULL != pEnv) //����Ƿ��Ѿ���ʼ��
	{
		mainEnv = pEnv;
		return E_HANDLE;
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
		mainEnv = pEnv;
		return S_OK;
	}
	return E_FAIL;
}

void WINAPI GameEnv::CloseEnv()
{
	if (NULL == pEnv)
	{
		return;
	}
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


const BinLib &GameEnv::GetBinLib() const
{
	return *(this->pGameRes->pBinLib);
}

const PalLib &GameEnv::GetPaletteLib() const
{
	return *(this->pGameRes->pBinLib->pLibPal);
}

const SndLib &GameEnv::GetSoundLib() const
{
	return *(this->pGameRes->pBinLib->pLibSnd);
}

const BgmLib &GameEnv::GetBGMLib() const
{
	return *(this->pGameRes->pBinLib->pLibBgm);
}

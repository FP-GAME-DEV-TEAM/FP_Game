/********************************
* FileName: FPCheck.cpp
* FileFunc: FP��Ϸ��Դ����ʵ��ģ��
* Author: SQK
* Date: 2013-07-27
* Descript: ��Ϸȫ����Դ��׼���������Լ��
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPDump.h"

BOOL WINAPI CheckPalLib(const tstring whichPath, PPalLib &pal)
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
	//������������
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
		bin->sGraphicInfo = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
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
		bin->sGraphicData = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
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
		bin->sAnimeInfo = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
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
		bin->sAnimeData = NewPathString(ffd.cFileName, FP_FILE_NAME_SIZE);
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
	bin->binPath = NewPathString(whichPath, MAX_PATH);
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

BOOL WINAPI CheckDataSet(const tstring whichPath, PDataSet &data)
{
	return TRUE;
}

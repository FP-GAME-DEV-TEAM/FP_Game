// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "FPDataType.h"
#include "FPFunction.h"

extern GameEnv *mainEnv;

TCHAR exeFilePath[MAX_PATH + 1] = { 0 };
HWND hMainWnd = NULL; // Main window handle

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);

BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  dwReason,
	LPVOID lpReserved
	)
{
	DWORD dwCurrentProcessId = GetCurrentProcessId();
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hModule);
#ifdef FP_MODULE_DEBUG
		_tcscpy(exeFilePath, FP_DEBUG_PATH); //ʹ�õ���·��
#else
		GetModuleFileName(NULL, exeFilePath, MAX_PATH); //�õ�ģ������·��
#endif
		(_tcsrchr(exeFilePath, _T('\\')))[0] = 0; //ɾ���ļ�����ֻ���·������������β��б�ܣ�
		if (!EnumWindows(EnumWindowsProc, (LPARAM)&dwCurrentProcessId))
		{
			hMainWnd = (HWND)dwCurrentProcessId;
		}
		FP_DEBUG_MSG(_T("FP DLL module attached.\n"));
		break;

	case DLL_PROCESS_DETACH:
		FP_DEBUG_MSG(_T("FP DLL module detached.\n"));
		break;

	default:
		break;
	}
	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwCurProcessId = *((DWORD*)lParam);
	DWORD dwProcessId = 0;
	GetWindowThreadProcessId(hwnd, &dwProcessId);
	if (dwProcessId == dwCurProcessId && GetParent(hwnd) == NULL)
	{
		*((HWND *)lParam) = hwnd;
		return FALSE;
	}
	return TRUE;
}

//��ʼ����Ϸ��Դ�ӿ�
FP_MODULE_API HRESULT WINAPI InitGameEnv(IGameEnv **IEnv)
{
	*IEnv = NULL;
	if (SUCCEEDED(GameEnv::OpenEnv(exeFilePath)))
	{
		if (SUCCEEDED(StartIOThreads()))
		{
			*IEnv = mainEnv;
			FP_DEBUG_MSG(_T("Game Env has been opened.\n"));
			return S_OK;
		}
	}
	return E_FAIL;
}

//�ͷ���Ϸ��Դ�ӿ�
FP_MODULE_API HRESULT WINAPI ReleaseGameEnv()
{
	if (FAILED(DestroyIOThreads()))
	{
		FP_DEBUG_MSG(_T("Failed to stop game IO thread.\n"));
	}
	GameEnv::CloseEnv();
	FP_DEBUG_MSG(_T("Game Env has been closed.\n"));
	return S_OK;
}

//��ʼ����Ϸͼ�νӿ�
FP_MODULE_API HRESULT WINAPI InitGraphics(IGameGraphics **IGraphics)
{
	*IGraphics = NULL;
	if (SUCCEEDED(GameGraphics::Create(mainEnv->GetBinLib())))
	{
		*IGraphics = mainGraphics;
		mainGraphics->InitPalette(mainEnv->GetBinLib()->pLibPal);
		mainGraphics->InitGraphicInfo();
		mainGraphics->InitAnimeInfo();
		FP_DEBUG_MSG(_T("Game Graphics has been initialized.\n"));
		return S_OK;
	}
	return E_FAIL;
}

//�ͷ���Ϸͼ�νӿ�
FP_MODULE_API HRESULT WINAPI ReleaseGraphics()
{
	GameGraphics::Destroy();
	FP_DEBUG_MSG(_T("Game Graphics has been released.\n"));
	return S_OK;
}

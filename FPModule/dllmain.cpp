// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "FPDataType.h"
#include "FPFunction.h"

extern GameEnv *mainEnv;

TCHAR exeFilePath[MAX_PATH + 1] = { 0 };

BOOL APIENTRY DllMain(
	HMODULE hModule,
	DWORD  dwReason,
	LPVOID lpReserved
	)
{
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

//��ʼ����Ϸ��Դ����
FP_MODULE_API VOID WINAPI InitGameEnv(IGameEnv **IEnv)
{
	if (SUCCEEDED(GameEnv::OpenEnv(exeFilePath)))
	{
		FP_DEBUG_MSG(_T("Game Env has been opened.\n"));
		StartIOThreads();
	}
	*IEnv = mainEnv;
}

//�ͷ���Ϸ��Դ����
FP_MODULE_API VOID WINAPI ReleaseGameEnv()
{
	GameEnv::CloseEnv();
	FP_DEBUG_MSG(_T("Game Env has been closed.\n"));
}

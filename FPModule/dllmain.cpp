// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "FPModule.h"
#include "FPDataType.h"

HMODULE dllModule = NULL;
GameEnv *mainEnv = NULL;

BOOL APIENTRY DllMain(
	HMODULE hModule, 
	DWORD  dwReason, 
	LPVOID lpReserved
)
{
	TCHAR exeFilePath[MAX_PATH + 1] = {0};
	switch(dwReason)
	{

	case DLL_PROCESS_ATTACH:
		dllModule = hModule;
#ifdef FP_MODULE_DEBUG
		_tcscpy(exeFilePath, FP_DEBUG_PATH); //ʹ�õ���·��
#else
		GetModuleFileName(NULL, exeFilePath, MAX_PATH); //�õ�ģ������·��
#endif
		(_tcsrchr(exeFilePath, _T('\\')))[0] = 0; //ɾ���ļ�����ֻ���·���ִ���������β��б�ܣ�
		if (S_OK == GameEnv::InitEnv(exeFilePath)) //��ʼ����Ϸ��Դ����
		{
			FP_DEBUG_MSG(_T("Game Env has been initialized.\n"));
			mainEnv = GameEnv::GetEnv();
		}
		break;

	case DLL_PROCESS_DETACH:
		mainEnv->ReleaseEnv();
		mainEnv = NULL;
		FP_DEBUG_MSG(_T("Game Env has been released.\n"));
		break;

	default:
		break;

	}

	return TRUE;
}

FP_MODULE_API VOID WINAPI LoadGameEnv(IGameEnv **IEnv)
{
	*IEnv = mainEnv;
}

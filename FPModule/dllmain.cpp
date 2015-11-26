// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "FPModule.h"
#include "FPDataType.h"

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
		DisableThreadLibraryCalls(hModule);
#ifdef FP_MODULE_DEBUG
		_tcscpy(exeFilePath, FP_DEBUG_PATH); //使用调试路径
#else
		GetModuleFileName(NULL, exeFilePath, MAX_PATH); //得到模块完整路径
#endif
		(_tcsrchr(exeFilePath, _T('\\')))[0] = 0; //删除文件名，只获得路径字串（不含结尾反斜杠）
		if (S_OK == GameEnv::InitEnv(exeFilePath)) //初始化游戏资源环境
		{
			FP_DEBUG_MSG(_T("Game Env has been initialized.\n"));
		}
		break;

	case DLL_PROCESS_DETACH:
		GameEnv::ReleaseEnv();
		mainEnv = NULL; // Must set null
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

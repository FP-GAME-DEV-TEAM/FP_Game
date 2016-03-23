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
		_tcscpy(exeFilePath, FP_DEBUG_PATH); //使用调试路径
#else
		GetModuleFileName(NULL, exeFilePath, MAX_PATH); //得到模块完整路径
#endif
		(_tcsrchr(exeFilePath, _T('\\')))[0] = 0; //删除文件名，只获得路径（不包含结尾反斜杠）
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

//初始化游戏资源环境
FP_MODULE_API VOID WINAPI InitGameEnv(IGameEnv **IEnv)
{
	if (SUCCEEDED(GameEnv::OpenEnv(exeFilePath)))
	{
		FP_DEBUG_MSG(_T("Game Env has been opened.\n"));
		StartIOThreads();
	}
	*IEnv = mainEnv;
}

//释放游戏资源环境
FP_MODULE_API VOID WINAPI ReleaseGameEnv()
{
	GameEnv::CloseEnv();
	FP_DEBUG_MSG(_T("Game Env has been closed.\n"));
}

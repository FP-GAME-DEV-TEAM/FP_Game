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

//初始化游戏资源接口
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

//释放游戏资源接口
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

//初始化游戏图形接口
FP_MODULE_API HRESULT WINAPI InitGraphics(IGameGraphics **IGraphics)
{
	*IGraphics = NULL;
	if (SUCCEEDED(GameGraphics::Create(mainEnv->GetBinLib())))
	{
		*IGraphics = mainGraphics;
		FP_DEBUG_MSG(_T("Game Graphics has been initialized.\n"));
		mainGraphics->InitPalette(mainEnv->GetBinLib()->pLibPal);
		return S_OK;
	}
	return E_FAIL;
}

//释放游戏图形接口
FP_MODULE_API HRESULT WINAPI ReleaseGraphics()
{
	GameGraphics::Destroy();
	FP_DEBUG_MSG(_T("Game Graphics has been released.\n"));
	return S_OK;
}

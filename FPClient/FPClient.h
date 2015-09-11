/********************************
* FileName: FPClient.h
* FileFunc: 客户端模块定义汇总
* Author: SQK
* Date: 2015-08-26
* Descript: 包含FP工程全部宏和私有模块定义
********************************/

#pragma once

#include "resource.h"

/* Game resources lib info */
#define GAME_RESOURCE_DLL _T("FPModule.dll")

/* Game main message loop setting */
#define GAME_FRAME_RATE 60
#define GAME_LOOP_TIMER 0
#define GAME_LOOP_SYSMSG 1

/* Game window size */
#define STAGE_DEFAULT_WIDTH			800
#define STAGE_DEFAULT_HEIGHT		600

/* Color format - Bits per pixel */
#define STAGE_BPP_INDEX				8
#define STAGE_BPP_LOW				16
#define STAGE_BBP_HIGH				32

/* Application display mode */
#define STAGE_MODE_FULLSCREEN		FALSE
#define STAGE_MODE_WINDOWED			TRUE


HRESULT Game_Init();
HRESULT Game_Shutdown();
HRESULT CreateWindowedDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight);
HRESULT CreateFullScreenDisplay(HWND hWnd, DWORD dwWidth, DWORD dwHeight, DWORD dwBPP);
VOID DestroyGameDisplay();

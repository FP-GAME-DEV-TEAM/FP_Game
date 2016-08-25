/********************************
* FileName: FPClient.h
* FileFunc: 客户端模块定义汇总
* Author: SQK
* Date: 2015-08-26
* Descript: 包含FP工程全部宏和私有模块定义
********************************/

#pragma once

#include "resource.h"

#include "..\\FPModule\\FPModule.h"

#ifdef FP_CLIENT_DEBUG
#if (defined UNICODE) || (defined _UNICODE)
#pragma comment(linker, "/subsystem:\"console\" /entry:\"wWinMainCRTStartup\"")
#else
#pragma comment(linker, "/subsystem:\"console\" /entry:\"WinMainCRTStartup\"")
#endif
#endif

/* Game resources lib info */
#define GAME_RESOURCE_DLL _T("FPModule.dll")

/* Game main message loop setting */
#define GAME_FRAME_RATE 60
#define GAME_LOOP_TIMER 0
#define GAME_LOOP_SYSMSG 1

/* Game window size */
#define STAGE_DEFAULT_WIDTH			800
#define STAGE_DEFAULT_HEIGHT		600

#define STAGE_WINDOW_WIDTH			768
#define STAGE_WINDOW_HEIGHT			576

#define STAGE_BUFFER_WIDTH			1024
#define STAGE_BUFFER_HEIGHT			768

/* Color format - Bits per pixel */
#define STAGE_BPP_INDEX				8
#define STAGE_BPP_LOW				16
#define STAGE_BBP_HIGH				32

/* Application display mode */
#define STAGE_MODE_FULLSCREEN		FALSE
#define STAGE_MODE_WINDOWED			TRUE

// Declarations of external globals
extern HINSTANCE hInst;
extern HWND hMainWnd;
extern BOOL fWindowed;
extern BOOL fActive;
extern LPDIRECTDRAW7 lpdd;

extern IGameEnv *gameEnv;
extern IGameGraphics *gameGraphics;

// Declarations of external functions
extern HRESULT InitGameDisplay(BOOL flag);
extern VOID DestroyGameDisplay();

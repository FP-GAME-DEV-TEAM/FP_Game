/********************************
* FileName: FPFunction.h
* FileFunc: FP游戏资源类型定义模块
* Author: SQK
* Date: 2013-07-26
* Descript: 包括FP工程内所有公共函数的声明
********************************/


#pragma once


//=====================================
// Game Environment API
//

FP_MODULE_API HRESULT WINAPI InitGameEnv(IGameEnv **IEnv); //初始化资源模块
FP_MODULE_API HRESULT WINAPI ReleaseGameEnv(); //释放资源模块

FP_MODULE_API HRESULT WINAPI InitGraphics(IGameGraphics **IGraphics); //初始化图像模块
FP_MODULE_API HRESULT WINAPI DGraphics(); //初始化图像模块


//=====================================
// IO API
//

UINT CALLBACK BinProc(HANDLE param);
UINT CALLBACK IOCompleteDefault(LPVOID pParam);
HRESULT WINAPI StartIOThreads();
HRESULT WINAPI DestroyIOThreads();

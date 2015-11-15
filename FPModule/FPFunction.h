/********************************
* FileName: FPFunction.h
* FileFunc: FP游戏资源类型定义模块
* Author: SQK
* Date: 2013-07-26
* Descript: 包括FP工程内所有公共函数的声明
********************************/


#pragma once


//================================
// Game Environment API
//

FP_MODULE_API VOID WINAPI LoadGameEnv(IGameEnv **IEnv); //初始化图像模块
FP_MODULE_API BOOL WINAPI InitGraphic(const IGameEnv &IEnv); //初始化图像模块


//================================
// Error Handle API
//

ResError WINAPI ErrorHandler(ErrorRes type, const LPTSTR funcName); //错误处理器
ResError WINAPI GetResError(); //获得上一个错误

FP_MODULE_API void WINAPI PrintError(ErrorRes type); //打印出某个类型的错误


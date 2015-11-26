/********************************
* FileName: FPDump.cpp
* FileFunc: FP游戏资源类型实现模块
* Author: SQK
* Date: 2013-07-25
* Descript: 主要用于错误处理的实现
********************************/

#include "stdafx.h"
#include "FPModule.h"
#include "FPDump.h"

//资源错误链表
static ResError gErrorList = {ERROR_RES_Unknown, NULL, NULL};
static PResError gpLastErr = &gErrorList;

//=====================================
//错误处理函数的实现
//
ResError WINAPI ErrorHandler(ErrorRes type, const LPTSTR funcName)
{
	gpLastErr->next = new ResError();
	gpLastErr = gpLastErr->next;
	gpLastErr->error = type;
	gpLastErr->func = funcName;
	gpLastErr->next = NULL;
	return *gpLastErr;
}

ResError WINAPI GetResError()
{
	return *gpLastErr;
}

FP_MODULE_API void WINAPI PrintError(ErrorRes type)
{

}
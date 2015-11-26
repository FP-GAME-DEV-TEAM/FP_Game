/********************************
* FileName: FPDump.cpp
* FileFunc: FP��Ϸ��Դ����ʵ��ģ��
* Author: SQK
* Date: 2013-07-25
* Descript: ��Ҫ���ڴ������ʵ��
********************************/

#include "stdafx.h"
#include "FPModule.h"
#include "FPDump.h"

//��Դ��������
static ResError gErrorList = {ERROR_RES_Unknown, NULL, NULL};
static PResError gpLastErr = &gErrorList;

//=====================================
//����������ʵ��
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
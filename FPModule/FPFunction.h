/********************************
* FileName: FPFunction.h
* FileFunc: FP��Ϸ��Դ���Ͷ���ģ��
* Author: SQK
* Date: 2013-07-26
* Descript: ����FP���������й�������������
********************************/


#pragma once


//================================
// Error Handle API
//

ResError WINAPI ErrorHandler(ErrorRes type, const LPTSTR funcName); //��������
ResError WINAPI GetResError(); //�����һ������

FP_MODULE_API void WINAPI PrintError(ErrorRes type); //��ӡ��ĳ�����͵Ĵ���


//================================
// Game Environment API
//
FP_MODULE_API VOID WINAPI LoadGameEnv(IGameEnv **IEnv); //��ʼ��ͼ��ģ��
FP_MODULE_API BOOL WINAPI InitGraphic(const IGameEnv &IEnv); //��ʼ��ͼ��ģ��


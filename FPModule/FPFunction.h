/********************************
* FileName: FPFunction.h
* FileFunc: FP��Ϸ��Դ���Ͷ���ģ��
* Author: SQK
* Date: 2013-07-26
* Descript: ����FP���������й�������������
********************************/


#pragma once


//=====================================
// Game Environment API
//

FP_MODULE_API VOID WINAPI LoadGameEnv(IGameEnv **IEnv); //��ʼ��ͼ��ģ��
FP_MODULE_API BOOL WINAPI InitGraphic(const IGameEnv &IEnv); //��ʼ��ͼ��ģ��


//=====================================
// IO API
//

UINT CALLBACK BinProc(HANDLE param);

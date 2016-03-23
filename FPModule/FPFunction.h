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

FP_MODULE_API VOID WINAPI InitGameEnv(IGameEnv **IEnv); //��ʼ����Դģ��
FP_MODULE_API VOID WINAPI ReleaseGameEnv(); //�ͷ���Դģ��

FP_MODULE_API VOID WINAPI InitGraphics(); //��ʼ��ͼ��ģ��


//=====================================
// IO API
//

UINT CALLBACK BinProc(HANDLE param);
HRESULT WINAPI StartIOThreads();
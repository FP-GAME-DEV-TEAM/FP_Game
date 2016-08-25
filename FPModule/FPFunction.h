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

FP_MODULE_API HRESULT WINAPI InitGameEnv(IGameEnv **IEnv); //��ʼ����Դģ��
FP_MODULE_API HRESULT WINAPI ReleaseGameEnv(); //�ͷ���Դģ��

FP_MODULE_API HRESULT WINAPI InitGraphics(IGameGraphics **IGraphics); //��ʼ��ͼ��ģ��
FP_MODULE_API HRESULT WINAPI DGraphics(); //��ʼ��ͼ��ģ��


//=====================================
// IO API
//

UINT CALLBACK BinProc(HANDLE param);
UINT CALLBACK IOCompleteDefault(LPVOID pParam);
HRESULT WINAPI StartIOThreads();
HRESULT WINAPI DestroyIOThreads();

HRESULT __fastcall EncryptData();
HRESULT __fastcall DecryptData(LPBYTE lpData, DWORD dwLength, DWORD dwSize, LPBYTE *lppBuffer);

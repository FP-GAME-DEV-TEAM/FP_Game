/********************************
* FileName: FPDump.h
* FileFunc: FP��Ϸ����׷��ģ��
* Author: SQK
* Date: 2015-11-27
* Descript: -
********************************/


#pragma once


//=====================================
// Error Data Types
//

//��Դ��������
typedef enum tagErrorRes
{
	ERROR_RES_Unknown = 0, //δ֪����
	ERROR_RES_MissingPath = 1, //·���쳣
	ERROR_RES_MissingFile = 2, //�ļ���ʧ
	ERROR_RES_DuplicatedFile = 3, //�������ļ�������
	ERROR_RES_VersionMismatch = 4, //�汾ƥ�䲻ͨ��
	ERROR_RES_FilesSumMismatch = 5, //�ļ�������ƥ��
	ERROR_RES_DateTimeMismatch = 6, //�޸�����ƥ�䲻ͨ��
	ERROR_RES_DigitalMismatch = 7, //�ļ�У��ʧ��
} ErrorRes;

//��Դ������Ϣ
typedef struct tagResError
{
	ErrorRes error; //��Դ��������
	LPTSTR func; //������
	tagResError *next; //��һ������
} ResError, *PResError;

//IO��������
typedef enum tagErrorIO
{
	ERROR_IO_Unknown = 0, //δ֪����
	ERROR_IO_EOF = 1, //�����ļ�β
	ERROR_IO_Limited = 2, //Ȩ�����ƴ���
	ERROR_IO_Timeout = 3, //�첽��ʱ
} ErrorIO;

//IO������Ϣ
typedef struct tagIOError
{
	ErrorIO error; //IO��������
	LPTSTR func; //������
	tagIOError *next; //��һ������
} IOError, *PIOError;


//=====================================
// Error Handle API
//

ResError WINAPI ErrorHandler(ErrorRes type, const LPTSTR funcName); //��������
ResError WINAPI GetResError(); //�����һ������

FP_MODULE_API void WINAPI PrintErrors(ErrorRes type); //��ӡ��ĳ�����͵�ȫ������
FP_MODULE_API tstring WINAPI GetLastErrorInfo(); //�õ����һ�δ�����Ϣ

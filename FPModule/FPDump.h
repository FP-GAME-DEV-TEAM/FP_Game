/********************************
* FileName: FPDump.h
* FileFunc: FP游戏错误追踪模块
* Author: SQK
* Date: 2015-11-27
* Descript: -
********************************/


#pragma once


//=====================================
// Error Data Types
//

//资源错误类型
typedef enum tagErrorRes
{
	ERROR_RES_Unknown = 0, //未知错误
	ERROR_RES_MissingPath = 1, //路径异常
	ERROR_RES_MissingFile = 2, //文件丢失
	ERROR_RES_DuplicatedFile = 3, //有类似文件名存在
	ERROR_RES_VersionMismatch = 4, //版本匹配不通过
	ERROR_RES_FilesSumMismatch = 5, //文件个数不匹配
	ERROR_RES_DateTimeMismatch = 6, //修改日期匹配不通过
	ERROR_RES_DigitalMismatch = 7, //文件校验失败
} ErrorRes;

//资源错误信息
typedef struct tagResError
{
	ErrorRes error; //资源错误类型
	LPTSTR func; //错误函数
	tagResError *next; //下一个错误
} ResError, *PResError;

//IO错误类型
typedef enum tagErrorIO
{
	ERROR_IO_Unknown = 0, //未知错误
	ERROR_IO_EOF = 1, //读到文件尾
	ERROR_IO_Limited = 2, //权限限制错误
	ERROR_IO_Timeout = 3, //异步超时
} ErrorIO;

//IO错误信息
typedef struct tagIOError
{
	ErrorIO error; //IO错误类型
	LPTSTR func; //错误函数
	tagIOError *next; //下一个错误
} IOError, *PIOError;


//=====================================
// Error Handle API
//

ResError WINAPI ErrorHandler(ErrorRes type, const LPTSTR funcName); //错误处理器
ResError WINAPI GetResError(); //获得上一个错误

FP_MODULE_API void WINAPI PrintErrors(ErrorRes type); //打印出某个类型的全部错误
FP_MODULE_API tstring WINAPI GetLastErrorInfo(); //得到最近一次错误信息

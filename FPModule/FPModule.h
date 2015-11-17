/********************************
* FileName: FPModule.h
* FileFunc: dll模块定义汇总
* Author: SQK
* Date: 2013-06-01
* Descript: 包含FP工程全部宏和私有模块定义
********************************/


#pragma once


/* This macro is for dll debugging */
#ifdef FP_MODULE_COMPILE
#define FP_MODULE_API __declspec(dllexport)
#else
#define FP_MODULE_API __declspec(dllimport)
#endif

/* This macro is for FP project debugging */
#ifdef FP_PROJECT_DEBUG
#include <stdarg.h>
#define FP_DEBUG_MSG(format, ...) _tprintf(format, ##__VA_ARGS__)
#else
#define FP_DEBUG_MSG(format, ...)
#endif

/* tstring for different char-set encodings */
#if (defined UNICODE) || (defined _UNICODE)
#define tstring std::wstring
#else
#define tstring std::string
#endif

/* These macros below can convert a macro to its varible name. */
#define _S(x) __S(x)
#define __S(x) _T(#x)


//资源数量宏定义
#define FP_FILE_NAME_SIZE 32
#define FP_FILE_COUNT_BGM 43
#define FP_FILE_COUNT_PAL 18
#define FP_FILE_COUNT_SND 266
#define FP_FILE_SIZE_PAL 672

#define FP_STORE_PAL_COUNT 256
#define FP_STORE_PAL_DEFAULT 32
#define FP_STORE_PAL_SELECTED 224

//相对路径和文件名宏定义
#define FP_PATH_BIN _T("\\bin") //资源文件相对路径
#define FP_PATH_PAL _T("\\pal") //调色板文件相对路径
#define FP_PATH_SND _T("\\sound") //音效文件相对路径
#define FP_PATH_BGM _T("\\bgm") //背景音乐文件相对路径

#define FP_PATH_DAT _T("\\data") //数据文件相对路径
#define FP_PATH_SET _T("\\setting") //设置文件相对路径
#define FP_PATH_LOG _T("\\log") //日志文件相对路径
#define FP_PATH_EML _T("\\email") //邮件文件相对路径

#define FP_PATH_MAP _T("\\map") //地图文件相对路径
#define FP_PATH_STC _T("\\static") //固定地图数据相对路径
#define FP_PATH_RDM _T("\\random") //随机地图数据相对路径

#define FP_FILE_GRAPHIC_INFO _T("GraphicInfo_") //图像字典文件名
#define FP_FILE_GRAPHIC_DATA _T("GraphicData_") //图像数据文件名
#define FP_FILE_ANIME_INFO _T("AnimeInfo_") //动画字典文件名
#define FP_FILE_ANIME_DATA _T("AnimeData_") //动画数据文件名

//调色板文件宏定义
#define FP_PALETTE_DAY		palet_00
#define FP_PALETTE_DUSK		palet_01
#define FP_PALETTE_NIGHT	palet_02
#define FP_PALETTE_DAWN		palet_03
#define FP_PALETTE_04		palet_04
#define FP_PALETTE_05		palet_05
#define FP_PALETTE_06		palet_06
#define FP_PALETTE_07		palet_07
#define FP_PALETTE_08		palet_08
#define FP_PALETTE_09		palet_09
#define FP_PALETTE_10		palet_10
#define FP_PALETTE_11		palet_11
#define FP_PALETTE_12		palet_12
#define FP_PALETTE_13		palet_13
#define FP_PALETTE_14		palet_14
#define FP_PALETTE_15		palet_15
#define FP_PALETTE_BLACK	palet_16
#define FP_PALETTE_WHITE	palet_17
//调色板枚举
typedef enum tagPalette
{
	palet_00 = 0,
	palet_01 = 1,
	palet_02 = 2,
	palet_03 = 3,
	palet_04 = 4,
	palet_05 = 5,
	palet_06 = 6,
	palet_07 = 7,
	palet_08 = 8,
	palet_09 = 9,
	palet_10 = 10,
	palet_11 = 11,
	palet_12 = 12,
	palet_13 = 13,
	palet_14 = 14,
	palet_15 = 15,
	palet_16 = 16,
	palet_17 = 17,
} Palette;

//文件后缀宏定义
#define FP_FILE_SUFFIX_BIN _T(".bin") //图像资源文件后缀
#define FP_FILE_SUFFIX_DAT _T(".dat") //数据配置文件后缀
#define FP_FILE_SUFFIX_PAL _T(".pal") //调色板文件后缀
#define FP_FILE_SUFFIX_LOG _T(".log") //日志文件后缀
#define FP_FILE_SUFFIX_MAP _T(".map") //地图文件后缀
#define FP_FILE_SUFFIX_MP3 _T(".mp3") //背景音乐文件后缀
#define FP_FILE_SUFFIX_WAV _T(".wav") //音效文件后缀


//================================
// Resource Data Types
//

//背景音乐库
typedef struct tagBgmLib
{
	LPCTSTR bgmPath; //背景音乐目录，以斜杠结束
	DWORD sum; //实际音乐文件总数
	LPCTSTR *fileList; //文件列表
	LPDWORD crc32; //数据校验
} BgmLib, *PBgmLib;

//游戏音效库
typedef struct tagSndLib
{
	LPCTSTR sndPath; //游戏音效目录，以斜杠结束
	DWORD sum; //实际音效文件总数
	LPCTSTR *fileList; //文件列表
	LPDWORD crc32; //数据校验
} SndLib, *PSndLib;

//调色板库
typedef struct tagPalLib
{
	LPCTSTR palPath; //调色板目录，以斜杠结束
	DWORD sum; //调色板总数
	LPCTSTR *fileList; //文件列表
	LPDWORD crc32; //数据校验
} PalLib, *PPalLib;

//二进制资源库
typedef struct tagBinLib
{
	LPCTSTR binPath; //二进制数据目录，以斜杠结束
	DWORD verGraphic; //图像资源版本号
	DWORD verAnime; //动画资源版本号
	LPCTSTR sGraphicInfo; //图像目录文件
	LPCTSTR sGraphicData; //图像数据文件
	LPCTSTR sAnimeInfo; //动画目录文件
	LPCTSTR sAnimeData; //动画数据文件
	PPalLib pLibPal; //调色板信息
	PBgmLib pLibBgm; //背景音乐信息
	PSndLib pLibSnd; //游戏音效信息
} BinLib, *PBinLib;


//用户数据
typedef struct tagLocalData
{
	LPCTSTR userID; //用来指示当前用户
} LocalData, *PLocalData;

//数据配置资源库
typedef struct tagDataSet
{
	LPCTSTR dataPath; //配置数据目录，以斜杠结束
	LPCTSTR userID; //用来指示当前用户
	PLocalData userData; //指向当前用户的本地数据
} DataSet, *PDataSet;

//地图资源库
typedef struct tagMapPack
{
	LPCTSTR mapPath; //地图目录，以斜杠结束
	//公共地图
	//私有地图
	//随机地图
} MapPack, *PMapPack;

//游戏资源结构
typedef struct tagGameRes
{
	LPCTSTR rootPath; //游戏根目录，以斜杠结束
	WORD mainVersion; //游戏主版本
	WORD subVersion; //游戏子版本
	PBinLib pBinLib; //二进制资源
	PDataSet pDataSet; //配置数据资源
	PMapPack pMapPack; //地图资源
} GameRes, *PGameRes;


//影像链接库
typedef struct tagGraphicLink
{
	HANDLE hGraphicInfo; //图像目录对象
	HANDLE hGraphicData; //图像数据对象
	HANDLE hAnimeInfo; //动画目录对象
	HANDLE hAnimeData; //动画数据对象
	LPBYTE table_Selected; //可选调色板
	LPBYTE table_Default; //固定调色板
} GraphicLink, *PGraphicLink;


//================================
// Game Environment Interface
//

class FP_MODULE_API IGameEnv
{
public:
	virtual LPCTSTR GetRootPath() const = 0; //得到游戏根目录
	virtual LPCTSTR GetBinPath() const = 0; //得到游戏数据目录
	virtual LPCTSTR GetDataPath() const = 0; //得到用户数据目录
	virtual LPCTSTR GetMapPath() const = 0; //得到地图目录
	virtual BinLib &GetBinLib() const = 0; //得到二进制库引用
	virtual PalLib &GetPaletteLib() const = 0; //得到调色板库引用
	virtual SndLib &GetSoundLib() const = 0; //得到音效库引用
	virtual BgmLib &GetBGMLib() const = 0; //得到背景音乐库引用
	virtual BOOL LoadPalette(PalLib& pal) = 0; //加载调色板
};


//================================
// Utility API
//

// tstring转大写
inline tstring WINAPI StrToUpper(const tstring &str)
{
	tstring tmp = str;
	transform(tmp.begin(),tmp.end(),tmp.begin(),toupper);
	return tmp;
}
// tstring转小写
inline tstring WINAPI StrToLower(const tstring &str)
{
	tstring tmp = str;
	transform(tmp.begin(),tmp.end(),tmp.begin(),tolower);
	return tmp;
}


//-----------------------------------------------------------------------------
// Misc functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct, 0, sizeof(ddstruct)); ddstruct.dwSize = sizeof(ddstruct);}

LPTSTR WINAPI NewPath(const tstring &str, const int size); //利用不可更改的字符串，得到用户自定长度的字符串
BOOL WINAPI IsFolderExist(tstring whichPath); //判读目录是否存在，不存在返回FALSE
HWND WINAPI GetRootWindow(HWND hWnd); //带入任意控件句柄，获得最上层父窗口的句柄


//================================
// IO API
//

BOOL WINAPI FileReadData(HANDLE hFile, BOOL isAsync, DWORD dwOffset, LONG dwSize, LPVOID lpBuffer);
BOOL WINAPI FileWriteData(HANDLE hFile, BOOL isAsync, DWORD dwOffset, LPVOID lpBuffer, LONG dwSize);


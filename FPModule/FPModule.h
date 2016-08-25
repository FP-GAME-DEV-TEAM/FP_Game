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


//-----------------------------------------------------------------------------
// Misc Functions
//-----------------------------------------------------------------------------
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct, 0, sizeof(ddstruct)); ddstruct.dwSize = sizeof(ddstruct);}


//-----------------------------------------------------------------------------
// Game Message Defines
//-----------------------------------------------------------------------------

// Start at 0x0400, AKA WM_USER. end with 0x7fff.
// FP project own message base value
#define FPMSG_BASE						(WM_USER)

// Window message
#define FPMSG_WINDOW_NOP				(FPMSG_BASE + 0x0000)

#define FPMSG_WINDOW_DEBUG_MSG			(FPMSG_BASE + 0x0010)
#define FPMSG_WINDOW_DEBUG_DLG			(FPMSG_BASE + 0x0011)

// Thread message
#define FPMSG_THREAD_START				(FPMSG_BASE + 0x0100)
#define FPMSG_THREAD_STOP				(FPMSG_BASE + 0x0101)
#define FPMSG_THREAD_PAUSE				(FPMSG_BASE + 0x0102)

#define FPMSG_IO_READ_IMAGEDATA			(FPMSG_BASE + 0x0200)
#define FPMSG_IO_READ_IMAGEINFO			(FPMSG_BASE + 0x0201)
#define FPMSG_IO_READ_ANIMEDATA			(FPMSG_BASE + 0x0202)
#define FPMSG_IO_READ_ANIMEINFO			(FPMSG_BASE + 0x0203)
#define FPMSG_IO_READ_PALETTE			(FPMSG_BASE + 0x0205)

#define FPMSG_IO_READ_SOUND				(FPMSG_BASE + 0x0210)
#define FPMSG_IO_READ_MUSIC				(FPMSG_BASE + 0x0211)
#define FPMSG_IO_READ_MIDI				(FPMSG_BASE + 0x0212)

#define FPMSG_IO_READ_TEXT				(FPMSG_BASE + 0x0220)
#define FPMSG_IO_READ_USERLOG			(FPMSG_BASE + 0x0221)

#define FPMSG_IO_READ_MAP_PUBLIC		(FPMSG_BASE + 0x0230)
#define FPMSG_IO_READ_MAP_PRIVATE		(FPMSG_BASE + 0x0231)
#define FPMSG_IO_READ_MAP_RANDOM		(FPMSG_BASE + 0x0232)

#define FPMSG_END						(WM_USER + 0x0c00 - 1)


//=====================================
// Game Constant Defines
//

//资源数量宏定义
#define FP_FILE_NAME_SIZE 32
#define FP_FILE_COUNT_BGM 43
#define FP_FILE_COUNT_PAL 18
#define FP_FILE_COUNT_SND 266
#define FP_FILE_SIZE_PAL 672

#define FP_STORE_PAL_COUNT 256
#define FP_STORE_PAL_DEFAULT 32
#define FP_STORE_PAL_OPTIONAL 224
#define FP_STORE_PAL_FRONT 16
#define FP_STORE_PAL_BACK 16

#ifdef FP_PROJECT_DEBUG
#define FP_THREAD_TIMEOUT INFINITE
#else
#define FP_THREAD_TIMEOUT 10000L
#endif

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

#define FP_FILE_GAME_CONFIG _T("Config.dat") //动画数据文件名
#define FP_FILE_GRAPHIC_INFO _T("GraphicInfo_") //图像字典文件名
#define FP_FILE_GRAPHIC_DATA _T("GraphicData_") //图像数据文件名
#define FP_FILE_ANIME_INFO _T("AnimeInfo_") //动画字典文件名
#define FP_FILE_ANIME_DATA _T("AnimeData_") //动画数据文件名

//文件后缀宏定义
#define FP_FILE_SUFFIX_BIN _T(".bin") //图像资源文件后缀
#define FP_FILE_SUFFIX_DAT _T(".dat") //数据配置文件后缀
#define FP_FILE_SUFFIX_PAL _T(".fgp") //调色板文件后缀
#define FP_FILE_SUFFIX_LOG _T(".log") //日志文件后缀
#define FP_FILE_SUFFIX_MAP _T(".map") //地图文件后缀
#define FP_FILE_SUFFIX_MP3 _T(".mp3") //背景音乐文件后缀
#define FP_FILE_SUFFIX_WAV _T(".wav") //音效文件后缀

//文件对应的HANDLE掩码
#define FP_HANDLE_GRAPHIC_DATA 0x0000 //图像数据句柄编号
#define FP_HANDLE_GRAPHIC_INFO 0x0001 //图像字典句柄编号
#define FP_HANDLE_ANIME_DATA 0x0002 //动画数据句柄编号
#define FP_HANDLE_ANIME_INFO 0x0003 //动画字典句柄编号


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


//可以直接显示的图片
typedef struct tagFPImage
{
	LONG width; //图片的宽度
	LONG height; //图片的高度
	LONG offsetX; //图片显示时的X轴偏移
	LONG offsetY; //图片显示时的Y轴偏移
	DWORD lentgh; //图片像素数据长度
	DWORD data[1]; //图片像素填充数据
} FPImage, *PFPImage;

//可以直接播放的动画帧
typedef struct tagFPFrame
{
	DWORD duration; //所有动画帧播放一遍耗时
	DWORD count; //动画帧总数
	LONG index[1]; //帧数据开始
} FPFrame, *PFPFrame;

//可以直接播放的动画
typedef struct tagFPAction
{
	BOOL isLoop[20]; //某个具体动作十分循环播放
	PFPFrame frame[8][20]; //8个方向和20个动作所对应的帧数据
} FPAction, *PFPAction;


//=====================================
// Game Graphics Interface
//

class FP_MODULE_API IGameGraphics
{
public:
	virtual HANDLE GetFileHandle(const UINT type) const = 0; //得到文件操作句柄
	virtual HRESULT GetImage(LONG id, const FPImage **pData) = 0; //通过ID得到图片
	virtual HRESULT GetAction(LONG id, const FPAction **pData) = 0; //通过ID得到动画
	virtual HRESULT GetPalette(LONG id, const PALETTEENTRY **pData) = 0; //更换调色板
};


//=====================================
// Game Audio Interface
//

class FP_MODULE_API IGameAudio
{
public:
	virtual LPCVOID GetWavDataById(LPCTSTR name) const = 0; //通过名称得到.wav文件数据
	virtual LPCVOID GetMp3DataById(LPCTSTR name) const = 0; //通过名称得到.mp3文件数据
	
};


//=====================================
// Game Text Interface
//

class FP_MODULE_API IGameText
{
public:
	virtual LPCTSTR GetTextById(int id) const = 0; //通过ID得到文本
	virtual LPCTSTR GetUserLogById(int id) const = 0; //通过ID得到用户日志
};


//=====================================
// Game Text Interface
//

class FP_MODULE_API IGameMaps
{
public:
	virtual LPCVOID GetLocalMapById(int id) const = 0; //通过ID得到本地地图
	virtual LPCVOID GetRemoteMapById(int id) const = 0; //通过ID得到远程地图
};


//=====================================
// Game Network Interface
//

class FP_MODULE_API IGameNetwork
{
	//unimplemented
};


//=====================================
// Game Environment Interface
//

class FP_MODULE_API IGameEnv
{
public:
	virtual LPCTSTR GetRootPath() const = 0; //得到游戏根目录
	virtual LPCTSTR GetBinPath() const = 0; //得到游戏数据目录
	virtual LPCTSTR GetDataPath() const = 0; //得到用户数据目录
	virtual LPCTSTR GetMapPath() const = 0; //得到地图目录
};


//=====================================
// Utility API
//

class MemoryStream;

// tstring转大写
inline tstring StrToUpper(const tstring &str)
{
	tstring tmp = str;
	transform(tmp.begin(),tmp.end(),tmp.begin(),toupper);
	return tmp;
}
// tstring转小写
inline tstring StrToLower(const tstring &str)
{
	tstring tmp = str;
	transform(tmp.begin(),tmp.end(),tmp.begin(),tolower);
	return tmp;
}

LPTSTR WINAPI NewPathString(const tstring &str, const int size); //利用不可更改的字符串，得到用户自定长度的字符串
BOOL WINAPI IsFolderExist(tstring whichPath); //判读目录是否存在，不存在返回FALSE
HWND WINAPI GetRootWindow(HWND hWnd); //带入任意控件句柄，获得最上层父窗口的句柄

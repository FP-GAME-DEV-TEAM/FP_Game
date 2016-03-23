/********************************
* FileName: FPDataType.h
* FileFunc: FP游戏资源类型定义模块
* Author: SQK
* Date: 2013-07-26
* Descript: 包括FP工程内所有最基本数据类型的定义
********************************/


#pragma once

#include "FPModule.h"

//=====================================
// Graphics Data Types
//

//图像目录结构(40BYTE)
typedef struct tagGraphicInfo
{
	LONG id; //图像的唯一标识ID
	DWORD addr; //图像在数据文件中的位置
	DWORD size; //图像数据的实际大小
	LONG offsetX; //图像显示时的X轴偏移
	LONG offsetY; //图像显示时的Y轴偏移
	LONG width; //图像的宽度
	LONG height; //图像的高度
	BYTE east; //图像向东方的占地
	BYTE south; //图像向南方的占地
	BYTE path; //用于地图，0表示障碍物，1表示可以走上去
	BYTE reserve[5]; //保留位，作用未知
	LONG index; //用于反查的地图编号
} GraphicInfo;

//图像数据头结构(16BYTE)
typedef struct tagGraphicData
{
	BYTE mod[2]; //魔数，固定为“RD”，意为real data
	BYTE flag; //版本编号，用于判断压缩方式
	BYTE check; //校验位，用途未知
	LONG width; //图片的宽度
	LONG height; //图片的高度
	DWORD size; //图片数据大小（包括这个头）
} GraphicData;

//动画目录结构(16BYTE)
typedef struct tagAnimeInfo
{
	LONG id; //动画的唯一标识ID
	DWORD addr; //动画在数据文件中的位置
	DWORD size; //一个完整动画的大小
	WORD count; //一个完整动画包含的动作数
	WORD check; //版本校验位，用途未知
} AnimeInfo;

//动画数据的头结构(12BYTE)
typedef struct tagAnimeHead
{
	WORD facing; //动作的朝向0-7
	WORD action; //动作的类型（有些动作不是所有动画都有）
	DWORD duration; //这个动作循环所需的时间（单位为毫秒）
	DWORD frames; //这个动作包含的图片帧数
} AnimeHead;

//动画数据单元(10BYTE)
typedef struct tagAnimeData
{
	LONG index; //图片帧对应的图像物理ID
	BYTE reserve[6]; //保留位，可能与游戏逻辑判定和音乐播放有关
} AnimeData;



//=====================================
// Resource Data Types
//

//背景音乐库
typedef struct tagBgmLib
{
	LPCTSTR bgmPath; //背景音乐目录
	DWORD sum; //实际音乐文件总数
	LPCTSTR *fileList; //文件列表
	LPDWORD crc32; //数据校验
} BgmLib, *PBgmLib;

//游戏音效库
typedef struct tagSndLib
{
	LPCTSTR sndPath; //游戏音效目录
	DWORD sum; //实际音效文件总数
	LPCTSTR *fileList; //文件列表
	LPDWORD crc32; //数据校验
} SndLib, *PSndLib;

//调色板库
typedef struct tagPalLib
{
	LPCTSTR palPath; //调色板目录
	DWORD sum; //调色板总数
	LPCTSTR *fileList; //文件列表
	LPDWORD crc32; //数据校验
} PalLib, *PPalLib;

//二进制资源库
typedef struct tagBinLib
{
	LPCTSTR binPath; //二进制数据目录
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
	LPCTSTR dataPath; //配置数据目录
	LPCTSTR userID; //用来指示当前用户
	PLocalData userData; //指向当前用户的本地数据
} DataSet, *PDataSet;

//地图资源库
typedef struct tagMapPack
{
	LPCTSTR mapPath; //地图目录
	//公共地图
	//私有地图
	//随机地图
} MapPack, *PMapPack;

//游戏资源结构
typedef struct tagGameRes
{
	LPCTSTR rootPath; //游戏根目录
	WORD mainVersion; //游戏主版本
	WORD subVersion; //游戏子版本
	PBinLib pBinLib; //二进制资源
	PDataSet pDataSet; //配置数据资源
	PMapPack pMapPack; //地图资源
} GameRes, *PGameRes;



//=====================================
// IO Data Types
// Sample: PostThreadMessage(ThreadId, IOType, IOReqList, CallbackFunc)
// CallbackFunc: void (*func)(void)
//

typedef struct tagIOItem
{
	BOOL isCompleted;
	DWORD offset;
	union
	{
		DWORD size;
		TCHAR end;
	};
	DWORD maxSize;
	LPVOID pData;
} IOItem, *PIOItem;

typedef struct tagIOList
{
	LONG count;
	PIOItem *pList;
} IOList, *PIOList;

/* Definition of FP thread routine */
typedef UINT(CALLBACK *FP_THREAD_ROUTINE)(LPVOID);



//=====================================
// Interface Implements Data Types
//

//游戏图像类
class GameGraphics : public IGameGraphics
{
private:
	static GameGraphics *pInstance; //单例对象

	HANDLE hGraphicInfo; //图像目录对象
	HANDLE hGraphicData; //图像数据对象
	HANDLE hAnimeInfo; //动画目录对象
	HANDLE hAnimeData; //动画数据对象
	PALETTEENTRY pPalette[FP_STORE_PAL_COUNT]; //当前调色板

	std::map<LONG, LPVOID> graphicCache;

	GameGraphics(); //构造函数
	virtual ~GameGraphics(void); //析构函数

public:
	static HRESULT WINAPI Create(const PBinLib pBin);
	static VOID WINAPI Destroy();

	HANDLE GetFileHandle(const int type) const;
	HRESULT GetImageById(LONG id, LPVOID pData); //通过ID得到图片
	HRESULT GetAnimeById(LONG id, LPVOID pData); //通过ID得到动画
	HRESULT SwitchPalette(LONG id); //更换调色板
};


//游戏声音类
class GameAudio : public IGameAudio
{
public:
	LPCVOID GetWavDataById(LPCTSTR name) const; //通过名称得到.wav文件数据
	LPCVOID GetMp3DataById(LPCTSTR name) const; //通过名称得到.mp3文件数据
};


//游戏文本类
class GameText : public IGameText
{
public:
	LPCTSTR GetTextById(LONG id) const; //通过ID得到文本
	LPCTSTR GetUserLogById(LONG id) const; //通过ID得到用户日志
};


//游戏地图类
class GameMaps : public IGameMaps
{
public:
	LPCVOID GetLocalMapById(LONG id) const; //通过ID得到本地地图
	LPCVOID GetRemoteMapById(LONG id) const; //通过ID得到远程地图
};


//游戏网络类
class GameNetwork : public IGameNetwork
{

};


//游戏资源环境类
class GameEnv : public IGameEnv
{
private:
	static GameEnv *pInstance; //单例游戏资源对象

	PGameRes pGameRes; //游戏基本信息

	GameEnv(const PGameRes pRes); //私有构造方法
	virtual ~GameEnv(void); //析构函数

protected:
	static PGameRes WINAPI OpenResFiles(const tstring whichPath); //判断目录是否为游戏资源的根目录
	static HRESULT WINAPI ValidateFile(HANDLE hFile); //校验指定文件

public:
	static HRESULT WINAPI OpenEnv(const tstring whichPath); //载入全部资源引用并打开对象
	static VOID WINAPI CloseEnv(); //卸载全部资源引用并关闭对象

	const PBinLib GetBinLib() const; //得到二进制库引用
	const PPalLib &GetPaletteLib() const; //得到调色板库引用
	const PSndLib &GetSoundLib() const; //得到音效库引用
	const PBgmLib &GetBGMLib() const; //得到背景音乐库引用

public:
	LPCTSTR GetRootPath() const; //得到游戏根目录
	LPCTSTR GetBinPath() const; //得到游戏数据目录
	LPCTSTR GetDataPath() const; //得到用户数据目录
	LPCTSTR GetMapPath() const; //得到地图目录

	HRESULT InitPalette(); //加载调色板
};

/********************************
* FileName: FPDataType.h
* FileFunc: FP游戏资源类型定义模块
* Author: SQK
* Date: 2013-07-26
* Descript: 包括FP工程内所有最基本数据类型的定义
********************************/


#pragma once


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


//游戏资源环境管理类
class GameEnv : public IGameEnv
{
private:
	static GameEnv *pEnv; //单例对象
	PGameRes pGameRes; //游戏基本信息
	PGraphicLink pGraphic; //影像资源集合

	GameEnv(PGameRes pRes); //私有构造方法
	~GameEnv(); //析构函数

	static PGameRes WINAPI OpenResFiles(const tstring whichPath); //判断目录是否为游戏资源的根目录
	static HRESULT WINAPI ValidateFile(HANDLE hFile); //校验指定文件

public:
	static HRESULT WINAPI InitEnv(const tstring whichPath); //初始化对象
	static void WINAPI ReleaseEnv(); //销毁游戏全部资源对象

	LPCTSTR GetRootPath() const; //得到游戏根目录
	LPCTSTR GetBinPath() const; //得到游戏数据目录
	LPCTSTR GetDataPath() const; //得到用户数据目录
	LPCTSTR GetMapPath() const; //得到地图目录

	BinLib &GetBinLib() const; //得到二进制库引用
	PalLib &GetPaletteLib() const; //得到调色板库引用
	SndLib &GetSoundLib() const; //得到音效库引用
	BgmLib &GetBGMLib() const; //得到背景音乐库引用

	BOOL LoadPalette(PalLib& pal); //加载调色板
};


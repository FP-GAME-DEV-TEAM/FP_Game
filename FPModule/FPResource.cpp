/********************************
* FileName: FPResource.cpp
* FileFunc: FP游戏资源类型实现模块
* Author: SQK
* Date: 2013-06-12
* Descript: 主要用于游戏全部资源的准备和完整性检查
********************************/

#include "stdafx.h"
#include "FPDataType.h"
#include "FPFunction.h"

//图像库数据缓存
static GraphicInfo *gListGraphic; //图像目录数据
static AnimeInfo *gListAnime; //动画目录数据

static DWORD gPaletteDefault[FP_STORE_PAL_DEFAULT]; //固定调色板数组表
static DWORD gPaletteSelected[FP_FILE_COUNT_PAL][FP_STORE_PAL_OPTIONAL]; //可变调色板数组表

extern UINT dwBinThreadId;

BOOL GameEnv::LoadPalette(PalLib& pal)
{
	DWORD n,count;
	tstring filePath = pal.palPath;
	tstring tmpPath;
	HANDLE hFile;
	LPBYTE buffer;

	//固定调色板的载入
	n = 0;
	gPaletteDefault[n++] = 0x00000000;
	gPaletteDefault[n++] = 0x00800000;
	gPaletteDefault[n++] = 0x00008000;
	gPaletteDefault[n++] = 0x00808000;
	gPaletteDefault[n++] = 0x00000080;
	gPaletteDefault[n++] = 0x00800080;
	gPaletteDefault[n++] = 0x00008080;
	gPaletteDefault[n++] = 0x00808080;
	gPaletteDefault[n++] = 0x00c0dcc0;
	gPaletteDefault[n++] = 0x00a6caf0;
	gPaletteDefault[n++] = 0x00de0000;
	gPaletteDefault[n++] = 0x00ff5f00;
	gPaletteDefault[n++] = 0x00ffffa0;
	gPaletteDefault[n++] = 0x00005fd2;
	gPaletteDefault[n++] = 0x0050d2ff;
	gPaletteDefault[n++] = 0x0028e128;
	gPaletteDefault[n++] = 0x00f5c396;
	gPaletteDefault[n++] = 0x001ea05f;
	gPaletteDefault[n++] = 0x00c37d46;
	gPaletteDefault[n++] = 0x009b551e;
	gPaletteDefault[n++] = 0x00464137;
	gPaletteDefault[n++] = 0x0028231e;
	gPaletteDefault[n++] = 0x00fffbf0;
	gPaletteDefault[n++] = 0x003a6ea5;
	gPaletteDefault[n++] = 0x00808080;
	gPaletteDefault[n++] = 0x00ff0000;
	gPaletteDefault[n++] = 0x0000ff00;
	gPaletteDefault[n++] = 0x00ffff00;
	gPaletteDefault[n++] = 0x000000ff;
	gPaletteDefault[n++] = 0x00ff80ff;
	gPaletteDefault[n++] = 0x0000ffff;
	gPaletteDefault[n++] = 0x00ffffff;

	//可变调色板的载入
	buffer = new BYTE[FP_FILE_SIZE_PAL];
	for(n=0; n<FP_FILE_COUNT_PAL; n++)
	{
		PostThreadMessage(dwBinThreadId, FPMSG_IO_READ_PALETTE, 0, 0);
	}
	
	delete buffer;
	return TRUE;
}
